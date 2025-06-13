#pragma once

#include <revolution.h>
#include <Game/NameObj/NameObjFactory.hpp>

typedef NameObjFactory::Name2CreateFunc Name2CreateFunc;

namespace {
    /// @brief Contains the new NameObjFactory entries,
    const Name2CreateFunc cCreateTableExt[] = {

    };

    /// @brief The end address of the cCreateTableExt table.
    const Name2CreateFunc* cCreateTableExtEnd = (Name2CreateFunc*)((u32)cCreateTableExt + sizeof(cCreateTableExt));
}