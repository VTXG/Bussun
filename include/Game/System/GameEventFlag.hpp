#pragma once

#include <revolution/types.h>

struct GameEventFlag {
    /* 0x00 */ const char* mName;
    /* 0x04 */ u8 mType;
    /* 0x05 */ u8 mSaveFlag;
    /* 0x06 */ u8 mCondition1;
    /* 0x07 */ u8 mCondition2;
    u32 _8;
    /* 0x0C */ const char* mCondition3;
    /* 0x10 */ const char* mCondition4;
};

class GameEventFlagAccessor {
public:
    GameEventFlagAccessor(const GameEventFlag*);

    const char* getName() const;
    bool isTypeGalaxyOpenStar() const;
    bool isTypeSpecialStar() const;
    bool isTypeStarPiece() const;
    bool isTypeEventValueIsZero() const;
    const char* getGalaxyName() const;
    s32 getStarId() const;
    s32 getStarPieceIndex() const;
    const char* getGalaxyNameWithStarPiece() const;
    s32 getNeedStarPieceNum() const;
    const char* getEventValueName() const;
    const char* getRequirement() const;

    /* 0x00 */ const GameEventFlag* mFlag;
};
