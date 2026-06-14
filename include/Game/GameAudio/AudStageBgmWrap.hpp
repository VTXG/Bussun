#pragma once

#include <revolution/types.h>

class AudStageBgmWrap {
public:
    s32 changeStageNameToSoundID(const char *, const char *, long);
    void getCometEventBgm(const char *);
    void getKoopaFortressAppearBgm(const char *);

    u32 mSoundId;
};