#include <revolution.h>
#include "kamek/hooks.h"

typedef void (*Func)();
extern Func __ctor_loc;
extern Func __ctor_end;

namespace {
    /// @brief Calls all static initializers after MR::initAcosTable. This is necessary to initialize static variables and
    /// constants in the BSS segment. Custom Nerve instances will be stored there, for example.
    void init() {
        for (Func* f = &__ctor_loc; f < &__ctor_end; f++) {
            (*f)();
        }

        OSReport("BUSSUN::init\n");
    }

    /// @brief Called at the end of GameSystem::frameLoop.
    void loop() {

    }
}

extern kmSymbol initAfterStationedResourceLoaded__10GameSystemFv;
extern kmSymbol frameLoop__10GameSystemFv;

kmBranch(&initAfterStationedResourceLoaded__10GameSystemFv + 0x5C, init);
kmBranch(&frameLoop__10GameSystemFv + 0x64, loop);