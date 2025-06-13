#include "ExtNameObjFactory.hpp"
#include "kamek/hooks.h"
#include <Game/Util/StringUtil.hpp>

namespace {
    const Name2CreateFunc* getName2CreateFuncExt(const char* pName, const Name2CreateFunc* pTable) {
        const Name2CreateFunc* pFunc = NameObjFactory::getName2CreateFunc(pName, pTable);

        if (!pFunc) {
            for (const Name2CreateFunc* i = cCreateTableExt; i < cCreateTableExtEnd; i += sizeof(Name2CreateFunc)) {
                if (i->mName && MR::isEqualStringCase(i->mName, pName)) {
                    // OSReport("ExtNameObjFactory: Created %s\n", i->mName);
                    return i;
                }
            }
        }

        return pFunc;
    }
}

extern kmSymbol getCreator__14NameObjFactoryFPCc;
extern kmSymbol getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter;

kmCall(&getCreator__14NameObjFactoryFPCc + 0x34, getName2CreateFuncExt);
kmCall(&getMountObjectArchiveList__14NameObjFactoryFP27NameObjArchiveListCollectorPCcRC12JMapInfoIter + 0x4C, getName2CreateFuncExt);