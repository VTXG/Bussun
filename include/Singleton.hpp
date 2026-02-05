#pragma once

#include "Game/SingletonHolder.hpp"

template<typename T>
class TKSingletonHolder {
public:
    static void init() {
        if (sInstance == nullptr) {
            sInstance = new T();
        }
    }

    inline static T* get() {
        return sInstance;
    }

public:
    static T* sInstance;
};

template<typename T>
T* TKSingletonHolder<T>::sInstance;

namespace TK {
    template<typename T>
    inline void initSingleton() {
        TKSingletonHolder<T>::init();
    };
    
    template<typename T>
    inline T* getSingleton() {
        return TKSingletonHolder<T>::get();
    };
}