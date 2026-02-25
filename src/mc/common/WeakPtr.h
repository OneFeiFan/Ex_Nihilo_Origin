#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/common/SharedCounter.h"

template <typename T>
class SharedPtr;

template <typename T>
class WeakPtr {
public:
    [[nodiscard]] WeakPtr() noexcept : counter(nullptr) {}
    [[nodiscard]] WeakPtr(std::nullptr_t) noexcept : counter(nullptr) {}

    // 1. 标准拷贝构造函数 (复制时弱引用计数 +1)
    WeakPtr(const WeakPtr& other) {
        counter = other.counter;
        if (counter) {
            counter->addWeakCount();
        }
    }

    // 2. 标准移动构造函数 (转移所有权，必须把原指针置空！)
    WeakPtr(WeakPtr&& other) noexcept {
        counter = other.counter;
        other.counter = nullptr;
    }

    // 3. 标准拷贝赋值操作符
    WeakPtr& operator=(const WeakPtr& other) {
        if (this != &other) {
            // 注意：WeakPtr 析构时调用的是 releaseWeak
            if (counter) { counter->releaseWeak(); }
            counter = other.counter;
            if (counter) { counter->addWeakCount(); }
        }
        return *this;
    }

    // 4. 标准移动赋值操作符
    WeakPtr& operator=(WeakPtr&& other) noexcept {
        if (this != &other) {
            if (counter) { counter->releaseWeak(); }
            counter = other.counter;
            other.counter = nullptr;
        }
        return *this;
    }

    template <class Y>
    [[nodiscard]] explicit WeakPtr(SharedPtr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    {
        counter = (SharedCounter<T>*)other.counter;
        if (counter) {
            counter->addWeakCount();
        }
    }

    template <class Y>
    [[nodiscard]] explicit WeakPtr(WeakPtr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    {
        counter = (SharedCounter<T>*)other.counter;
        if (counter) {
            counter->addWeakCount();
        }
    }

    template <class Y>
    [[nodiscard]] explicit WeakPtr(WeakPtr<Y>&& other)
        requires(std::convertible_to<Y*, T*>)
    {
        counter       = (SharedCounter<T>*)other.counter;
        other.counter = nullptr;
    }

    ~WeakPtr() {
        if (counter) {
            counter->releaseWeak();
        }
    }

    template <class Y>
    WeakPtr<T>& operator=(SharedPtr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    {
        if (counter != (SharedCounter<T>*)other.counter) {
            counter = (SharedCounter<T>*)other.counter;
            if (counter) {
                counter->addWeakCount();
            }
        }
        return *this;
    }

    template <class Y>
    WeakPtr<T>& operator=(WeakPtr<Y> const& other)
        requires(std::convertible_to<Y*, T*>)
    {
        if (counter != (SharedCounter<T>*)other.counter) {
            counter = (SharedCounter<T>*)other.counter;
            if (counter) {
                counter->addWeakCount();
            }
        }
        return *this;
    }

    template <class Y>
    WeakPtr<T>& operator=(WeakPtr<Y>&& other)
        requires(std::convertible_to<Y*, T*>)
    {
        if (counter != (SharedCounter<T>*)other.counter) {
            counter       = (SharedCounter<T>*)other.counter;
            other.counter = nullptr;
        }
        return *this;
    }

    [[nodiscard]] int use_count() const { return counter ? counter->getShareCount() : 0; }

    [[nodiscard]] bool expired() const { return use_count() == 0; }

    [[nodiscard]] SharedPtr<T> lock() const { return expired() ? SharedPtr<T>() : SharedPtr<T>(*this); }

    [[nodiscard]] T* get() const { return counter ? counter->get() : nullptr; }

    [[nodiscard]] T* operator->() const { return get(); }

    [[nodiscard]] T& operator*() const { return *get(); }

    [[nodiscard]] explicit operator bool() const { return get() != nullptr; }

    SharedCounter<T>* counter;
};
