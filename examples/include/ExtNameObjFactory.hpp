#pragma once

#include <revolution.h>
#include <Game/NameObj/NameObj.hpp>

class NameObjFactory {
public:
    struct Name2CreateFunc {
        const char* mName;                          // 0x0
        NameObj* (*mCreationFunc)(const char *);    // 0x4
        const char* mArchiveName;                   // 0x8
    };

    static const Name2CreateFunc* getName2CreateFunc(const char *, const Name2CreateFunc *);
};

typedef NameObj* (*CreationFuncPtr)(const char *);
typedef NameObjFactory::Name2CreateFunc Name2CreateFunc;

namespace {
    template<typename T>
    NameObj* createNameObj(const char *pName) {
        return new T(pName);
    }

    /// @brief Contains the new NameObjFactory entries,
    const Name2CreateFunc cCreateTableExt[] = {
        
    };

    /// @brief The end address of the cCreateTableExt table.
    const Name2CreateFunc* cCreateTableExtEnd = (Name2CreateFunc*)((u32)cCreateTableExt + sizeof(cCreateTableExt));
}
