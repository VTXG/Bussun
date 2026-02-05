#pragma once

#include "TicoKit.h"

#include "JSystem/JUtility/JUTConsole.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"

#if defined(USA)
#define BUILD_REGION_NAME "USA"
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_USA.bin"
#elif defined(PAL)
#define BUILD_REGION_NAME "PAL"
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_PAL.bin"
#elif defined(JPN)
#define BUILD_REGION_NAME "JPN"
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_JPN.bin"
#elif defined(KOR)
#define BUILD_REGION_NAME "KOR"
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_KOR.bin"
#endif

#define kAddr32 1
#define kAddr16Lo 4
#define kAddr16Hi 5
#define kAddr16Ha 6
#define kRel24 10
#define kWrite32 32
#define kWrite16 33
#define kWrite8 34
#define kCondWritePointer 35
#define kCondWrite32 36
#define kCondWrite16 37
#define kCondWrite8 38
#define kBranch 64
#define kBranchLink 65

struct KamekHeader {
    /* 0x00 */ u32 magic1;
    /* 0x04 */ u16 magic2;
    /* 0x06 */ u16 version;
    /* 0x08 */ u32 bssSize;
    /* 0x0C */ u32 codeSize;
};

struct KamekHeaderV2 {
    /* 0x00 */ u32 ctorStart;
    /* 0x04 */ u32 ctorEnd;
    /* 0x08 */ u32 padding[2];
};

struct KamekCommand {
    /* 0x00 */ unsigned cmdType : 8;
    /* 0x01 */ unsigned address : 24;
};

namespace TicoLoader {
    void init();
    void initHeaps(HeapMemoryWatcher *pWatcher);
    void initBinarySize();
    void link(u8 *pCodeLinked, u32 codeSize, u8 *pSrcLink, u32 linkSize);
    void error(const char *pMessage);
    void printExceptionInfo(JUTConsole *pConsole, const char *pSeperator);
}