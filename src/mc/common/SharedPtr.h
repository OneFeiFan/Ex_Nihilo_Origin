// 这个是AI写的，具体为什么原版会闪退我不知道，这个版本有什么其它问题我也不确定


#pragma once
#include <atomic>
#include <iostream>
#include <utility>

// 1. 实现引用计数控制块
template <class T>
class SharedPtrImpl {
public:
    T* ptr;                          // 原始指针
    std::atomic_uint referenceCount; // 强引用计数
    std::atomic_uint weakCount;      // 弱引用计数（用于管理 impl 本身的生命周期）

    SharedPtrImpl(T* p) : ptr(p), referenceCount(1), weakCount(0) {}
    
    ~SharedPtrImpl() {
        if (ptr) {
            delete ptr;
            ptr = nullptr;
        }
    }
};

// 预定义 WeakPtr 供 SharedPtr 使用
template <class T> class WeakPtr;

// 2. 实现 SharedPtr
template <class T>
class SharedPtr {
public:
    SharedPtrImpl<T>* impl;

    // 默认构造
    SharedPtr() : impl(nullptr) {}
    explicit SharedPtr(SharedPtrImpl<T>* i) : impl(i) {}

    // 拷贝构造：增加引用计数
    SharedPtr(const SharedPtr& therapeutic) : impl(therapeutic.impl) {
        if (impl) impl->referenceCount++;
    }

    // --- 核心功能：支持 SharedPtr<Derived> 到 SharedPtr<Base> 的转换 ---
    template <class U>
    SharedPtr(const SharedPtr<U>& other) : impl(reinterpret_cast<SharedPtrImpl<T>*>(other.impl)) {
        if (impl) {
            impl->referenceCount++;
        }
    }

    // 析构函数：处理引用计数减持
    ~SharedPtr() {
        release();
    }

    void release() {
        if (impl) {
            if (--impl->referenceCount == 0) {
                // 强引用归零，销毁对象
                delete impl->ptr;
                impl->ptr = nullptr;

                // 如果没有弱引用了，销毁控制块本身
                if (impl->weakCount == 0) {
                    delete impl;
                }
            }
            impl = nullptr;
        }
    }

    T* get() const {
        return impl ? impl->ptr : nullptr;
    }

    T* operator->() const { return get(); }

    // 静态 Make 方法
    template <class... Args>
    static SharedPtr<T> make(Args&&... args) {
        T* raw = new T(std::forward<Args>(args)...);
        return SharedPtr<T>(new SharedPtrImpl<T>(raw));
    }
};

// 3. 实现 WeakPtr
template <class T>
class WeakPtr {
public:
    SharedPtrImpl<T>* impl;

    WeakPtr() : impl(nullptr) {}

    // 从 SharedPtr 构造
    WeakPtr(const SharedPtr<T>& s) : impl(s.impl) {
        if (impl) impl->weakCount++;
    }

    // 支持协变构造
    template <class U>
    WeakPtr(const SharedPtr<U>& s) : impl(reinterpret_cast<SharedPtrImpl<T>*>(s.impl)) {
        if (impl) impl->weakCount++;
    }

    ~WeakPtr() {
        if (impl) {
            if (--impl->weakCount == 0 && impl->referenceCount == 0) {
                delete impl;
            }
        }
    }

    // 弱指针通常不直接 get，而是 lock() 变成强指针，这里按你要求保留 get
    T* get() const {
        if (impl && impl->referenceCount > 0)
            return impl->ptr;
        return nullptr;
    }
};