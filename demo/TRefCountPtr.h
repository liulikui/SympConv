#ifndef TREF_COUNT_PTR_H
#define TREF_COUNT_PTR_H

#include <algorithm>  // for std::swap
#include <utility>    // for std::forward

// TRefCountPtr - 支持引用计数的智能指针模板类
// 要求模板参数T必须提供AddRef()和Release()方法

template <typename T>
class TRefCountPtr
{
public:
    // 默认构造函数
    TRefCountPtr() : mPtr(nullptr) {}
    
    // 从原始指针构造
    explicit TRefCountPtr(T* ptr) : mPtr(ptr)
    {
        if (mPtr) 
        {
            mPtr->AddRef();
        }
    }
    
    // 拷贝构造函数
    TRefCountPtr(const TRefCountPtr<T>& other) : mPtr(other.mPtr)
    {
        if (mPtr) 
        {
            mPtr->AddRef();
        }
    }
    
    // 移动构造函数
    TRefCountPtr(TRefCountPtr<T>&& other) noexcept : mPtr(other.mPtr)
    {
            other.mPtr = nullptr;
        }
    
        // 析构函数
    ~TRefCountPtr()
    {
        if (mPtr) 
        {
            mPtr->Release();
        }
    }
    
    // 拷贝赋值运算符
    TRefCountPtr<T>& operator=(const TRefCountPtr<T>& other)
    {
        if (this != &other) 
        {
            // 先增加新指针的引用计数，避免自赋值导致对象被提前释放
            T* temp = other.mPtr;
            if (temp) 
            {
                temp->AddRef();
            }
            
            // 释放旧指针
            if (mPtr) 
            {
                mPtr->Release();
            }
            
            mPtr = temp;
        }
        return *this;
    }
    
    // 移动赋值运算符
    TRefCountPtr<T>& operator=(TRefCountPtr<T>&& other) noexcept
    {
        if (this != &other) 
        {
            // 释放旧指针
            if (mPtr) 
            {
                mPtr->Release();
            }
            
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        return *this;
    }
    
    // 原始指针赋值运算符
    TRefCountPtr<T>& operator=(T* ptr)
    {
        if (mPtr != ptr) 
        {
            // 先增加新指针的引用计数
            if (ptr)
            {
                ptr->AddRef();
            }
            
            // 释放旧指针
            if (mPtr)
            {
                mPtr->Release();
            }
            
            mPtr = ptr;
        }
        return *this;
    }
    
    // 解引用运算符
    T& operator*() const
    {
        return *mPtr;
    }
    
    // 箭头运算符
    T* operator->() const
    {
        return mPtr;
    }
    
    // 获取原始指针
    T* Get() const
    {
        return mPtr;
    }
    
    // 检查是否为空
    bool IsNull() const
    {
        return mPtr == nullptr;
    }
    
    // 显式转换为bool
    explicit operator bool() const
    {
        return mPtr != nullptr;
    }
    
    // 重置指针
    void Reset()
    {
        if (mPtr)
        {
            mPtr->Release();
            mPtr = nullptr;
        }
    }
    
    // 交换两个智能指针
    void Swap(TRefCountPtr<T>& other)
    {
        std::swap(mPtr, other.mPtr);
    }
    
private:
    T* mPtr; // 原始指针
};

// 比较运算符重载
template <typename T>
bool operator==(const TRefCountPtr<T>& lhs, const TRefCountPtr<T>& rhs)
{
    return lhs.Get() == rhs.Get();
}

template <typename T>
bool operator!=(const TRefCountPtr<T>& lhs, const TRefCountPtr<T>& rhs)
{
    return lhs.Get() != rhs.Get();
}

template <typename T>
bool operator<(const TRefCountPtr<T>& lhs, const TRefCountPtr<T>& rhs)
{
    return lhs.Get() < rhs.Get();
}

#endif // TREF_COUNT_PTR_H
