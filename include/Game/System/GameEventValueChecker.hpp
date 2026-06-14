#pragma once

#include <revolution/types.h>
#include "Game/System/BinaryDataChunkHolder.hpp"

class GameEventValueChecker : public BinaryDataChunkBase {
public:
    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual s32 deserialize(const u8*, u32);
    virtual void initializeData();

    void getValue(const char *pValueName) const;
    void setValue(const char *pValueName, u16 value);
    
    int findIndex(const char *pValueName) const;
    int findIndexFromHashCode(u16 hash) const;

    /* 0x04 */ u16* mValueTable;
    /* 0x08 */ u32 mValueNum;
};