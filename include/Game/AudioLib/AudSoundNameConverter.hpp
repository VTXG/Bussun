#pragma once

#include <JSystem/JAudio2/JAISound.hpp>
#include <revolution/types.h>

class AudSoundNameConverter {
public:
    JAISoundID getSoundID(const char*) const;
    JAISoundID getSoundID(const char*, u32) const;
};
