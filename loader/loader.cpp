#include "Loader.hpp"

namespace {
    inline u32 resolveAddress(u32 text, u32 address) {
        return address & 0x80000000 ? address : (text + address);
    }

    inline u8* cmdAddr32(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u32 *)address = target;
        return pInput + 4;
    }

    inline u8* cmdAddr16Lo(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u16 *)address = target & 0xFFFF;
        return pInput + 4;
    }

    inline u8* cmdAddr16Hi(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u16 *)address = target & 0xFFFF;
        return pInput + 4;
    }

    inline u8* cmdAddr16Ha(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u16 *)address = target >> 16;
        if (target & 0x8000)
            *(u16 *)address += 1;
        return pInput + 4;
    }

    inline u8* cmdRel24(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        u32 delta = target - address;
        *(u32 *)address &= 0xFC000003;
        *(u32 *)address |= (delta & 0x3FFFFFC);
        return pInput + 4;
    }

    inline u8* cmdWrite32(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        *(u32 *)address = value;
        return pInput + 4;
    }

    inline u8* cmdWrite16(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        *(u16 *)address = value & 0xFFFF;
        return pInput + 4;
    }

    inline u8* cmdWrite8(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        *(u8 *)address = value & 0xFF;
        return pInput + 4;
    }

    inline u8* cmdCondWritePointer(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        u32 original = ((const u32 *)pInput)[1];
        if (*(u32 *)address == original)
            *(u32 *)address = target;
        return pInput + 8;
    }

    inline u8* cmdCondWrite32(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        u32 original = ((const u32 *)pInput)[1];
        if (*(u32 *)address == original)
            *(u32 *)address = value;
        return pInput + 8;
    }

    inline u8* cmdCondWrite16(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        u32 original = ((const u32 *)pInput)[1];
        if (*(u16 *)address == (original & 0xFFFF))
            *(u16 *)address = value & 0xFFFF;
        return pInput + 8;
    }

    inline u8* cmdCondWrite8(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        u32 original = ((const u32 *)pInput)[1];
        if (*(u8 *)address == (original & 0xFF))
            *(u8 *)address = value & 0xFF;
        return pInput + 8;
    }

    inline u8* cmdBranch(u8 *pInput, u32 text, u32 address) {
        *(u32 *)address = 0x48000000;
        return cmdRel24(pInput, text, address);
    }

    inline u8* cmdBranchLink(u8 *pInput, u32 text, u32 address) {
        *(u32 *)address = 0x48000001;
        return cmdRel24(pInput, text, address);
    }
}

namespace TicoLoader {
    static u32 sCodeSize;
    static u32 sCodeAddr;

    void init() {
        if (sCodeSize == 0) {
            OSReport("TicoLoader : No binary found\n");
            return;
        }

        DVDFileInfo fileHandle;
        int id = DVDConvertPathToEntrynum(KAMEK_BINARY_NAME);

        if (!DVDFastOpen(id, &fileHandle)) {
            error("TicoLoader : Cannot create file handle\n");
            return;
        }

        u8 *pBinary = (u8 *)ROUND_UP((u32)SingletonHolder<HeapMemoryWatcher>::get()->mGameHeapNapa + 0x200, 32);
        u32 binarySize = fileHandle.length;

        KamekHeader *pHeader = (KamekHeader*)pBinary;
        DVDReadPrio(&fileHandle, pBinary, binarySize, 0, 2);
        DVDClose(&fileHandle);

        if (pHeader->magic1 != 'Kame' || pHeader->magic2 != 'k\0') {
            error("TicoLoader : Invalid binary header\n");
            return;
        }

        u32 additionalSize = 0u;
        if (pHeader->version == 2) {
            additionalSize = sizeof(KamekHeaderV2);
        }

        u32 maxAllocatable = JKRHeap::sSystemHeap->getMaxAllocatableSize(32);
        u8 *pCodeLinked = new (JKRHeap::sSystemHeap, 32) u8[sCodeSize];
        u32 codeSize = pHeader->codeSize;
        u32 bssSize = pHeader->bssSize;
        u8 *pCodeStart = pCodeLinked;
        u8 *pBssStart = pCodeLinked + codeSize;
        u8 *pSrcCode = pBinary + sizeof(KamekHeader) + additionalSize;
        sCodeAddr = (u32)pCodeLinked;

        memcpy(pCodeStart, pSrcCode, codeSize);
        memset(pBssStart, 0, bssSize);

        OSReport("TicoLoader : Address: 0x%X | Size: 0x%X/0x%X\n", pCodeLinked, sCodeSize, maxAllocatable);

        u8 *pSrcLink = pSrcCode + codeSize;
        u32 linkSize = binarySize - sizeof(KamekHeader) - additionalSize - codeSize;
        link(pCodeStart, sCodeSize, pSrcLink, linkSize);

        memset(pBinary, 0, binarySize);
    }

    void initHeaps(HeapMemoryWatcher *pWatcher) {
        TicoLoader::initBinarySize();
        pWatcher->createHeaps();
    }

    void initBinarySize() {
        DVDFileInfo fileHandle;
        int id = DVDConvertPathToEntrynum(KAMEK_BINARY_NAME);

        if (id < 0) {
            sCodeSize = 0;
            return;
        }

        if (!DVDFastOpen(id, &fileHandle)) {
            error("TicoLoader : Cannot create file handle\n");
            return;
        }

        KamekHeader header ATTRIBUTE_ALIGN(32);
        DVDReadPrio(&fileHandle, &header, sizeof(header), 0, 2);
        DVDClose(&fileHandle);

        sCodeSize = ROUND_UP(header.codeSize + header.bssSize, 32);
    }

    void link(u8 *pCodeLinked, u32 codeSize, u8 *pSrcLink, u32 linkSize) {
        u32 text = (u32)pCodeLinked;

        u8 *pInput = pSrcLink;
        u8 *pEnd = pSrcLink + linkSize;

        while (pInput < pEnd)  {
            const KamekCommand *pCmdHeader = ((const KamekCommand *)pInput)++;

            u32 address;
            if (pCmdHeader->address == 0xFFFFFE) {
                address = *(((u32 *)pInput)++);
            }
            else {
                address = pCmdHeader->address + text;
            }

            #define kDispatchCommand(name) \
            case k##name: pInput = ::cmd##name(pInput, text, address); break

            switch (pCmdHeader->cmdType) {
                kDispatchCommand(Addr32);
                kDispatchCommand(Addr16Lo);
                kDispatchCommand(Addr16Hi);
                kDispatchCommand(Addr16Ha);
                kDispatchCommand(Rel24);
                kDispatchCommand(Write32);
                kDispatchCommand(Write16);
                kDispatchCommand(Write8);
                kDispatchCommand(CondWritePointer);
                kDispatchCommand(CondWrite32);
                kDispatchCommand(CondWrite16);
                kDispatchCommand(CondWrite8);
                kDispatchCommand(Branch);
                kDispatchCommand(BranchLink);
            default:
                OSReport("TicoLoader : Unknown command %d\n", pCmdHeader->cmdType);
            }

            register u32 cacheAddr = address;
#ifndef INTELLISENSE_FIX
            __asm {
                dcbst r0, cacheAddr
                sync
                icbi  r0, cacheAddr
            }
#endif
        }
    }

    void error(const char *pMessage) {
        GXColor fg = {0xFF, 0xFF, 0xFF, 0xFF};
        GXColor bg = {0};
        OSFatal(fg, bg, pMessage);
    }

    void printExceptionInfo(JUTConsole *pConsole, const char *pSeperator) {
        pConsole->print("-------------------------------- TK\n");
        pConsole->print_f("Region:         %s\n", BUILD_REGION_NAME);
        pConsole->print_f("Binary Address: 0x%08X\n", sCodeAddr);
        pConsole->print_f("Binary Size:    0x%08X\n", sCodeSize);
        pConsole->print(pSeperator);
    }
}

// Custom code loading
extern kmSymbol init__19GameSystemExceptionFv;
kmBranch(&init__19GameSystemExceptionFv + 0xB8, TicoLoader::init);

// Custom code size initializing
extern kmSymbol __ct__17HeapMemoryWatcherFv;
kmCall(&__ct__17HeapMemoryWatcherFv + 0x50, TicoLoader::initHeaps);

// Enable crash debugger
extern kmSymbol handleException__19GameSystemExceptionFUsP9OSContextUlUl;
kmWrite32(&handleException__19GameSystemExceptionFUsP9OSContextUlUl + 0x38, PPC_NOP);
kmWrite32(&handleException__19GameSystemExceptionFUsP9OSContextUlUl + 0xFC, PPC_NOP);
kmSymWrite32(OSPanic, 0x108, PPC_NOP);
kmSymWrite32(__OSUnhandledException, 0x54, PPC_NOP);

// Extra crash log information
extern kmSymbol printContext__12JUTExceptionFUsP9OSContextUlUl;
kmCall(&printContext__12JUTExceptionFUsP9OSContextUlUl + 0x208, TicoLoader::printExceptionInfo);

// Disable audio during exceptions
extern "C" void __OSStopAudioSystem();
extern kmSymbol handleException__19GameSystemExceptionFUsP9OSContextUlUl;
kmBranch(&handleException__19GameSystemExceptionFUsP9OSContextUlUl + 0x114, __OSStopAudioSystem);

// Display file name in file not found crashes
extern kmSymbol loadToMainRAM__10FileRipperFPCcPUcbP7JKRHeapQ210FileRipper14AllocDirection;
kmWrite32(&loadToMainRAM__10FileRipperFPCcPUcbP7JKRHeapQ210FileRipper14AllocDirection + 0x4C, PPC_MR(3, 26)); // mr r3, r26

// Expand system heap
extern kmSymbol createHeaps__17HeapMemoryWatcherFv;
kmWrite16(&createHeaps__17HeapMemoryWatcherFv + 0x2A, 7);

// Shrink home button heap
kmWrite16(&createHeaps__17HeapMemoryWatcherFv + 0x82, 5);

// Expand audio heap
extern kmSymbol init__10GameSystemFv;
kmWrite16(&init__10GameSystemFv + 0x26, 0xE8);

extern kmSymbol startDMA__12JKRAramPieceFP12JKRAMCommand;
kmWrite16(&startDMA__12JKRAramPieceFP12JKRAMCommand + 0x4A, 0xE8);

extern kmSymbol aramSize__9AudParams;
kmWrite32(&aramSize__9AudParams, 0xE80000);

extern kmSymbol createRootHeap__17HeapMemoryWatcherFv;
kmWrite16(&createRootHeap__17HeapMemoryWatcherFv + 0x2E, 0xE8);