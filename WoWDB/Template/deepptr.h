#ifndef DEEPPTR_H
#define DEEPPTR_H

template<class T>
class DeepPtr {
private:
    T* ptr;

public:
    // Constructors/destructors
    DeepPtr(T* p = nullptr);
    DeepPtr(const DeepPtr& dp);
    ~DeepPtr();

    // Operators overloading
    DeepPtr& operator=(const DeepPtr& dp);
    bool operator==(const DeepPtr& dp) const;
    bool operator!=(const DeepPtr& dp) const;
    bool operator<(const DeepPtr& dp) const;
    T* operator->() const;
    T& operator*() const;
};

/******************** CONSTRUCTORS/DESTRUCTORS ********************/
template<class T>
DeepPtr<T>::DeepPtr(T* p): ptr(p) {}

template<class T>
DeepPtr<T>::DeepPtr(const DeepPtr& dp) : ptr(dp.ptr != nullptr ? dp.ptr->clone() : nullptr) {}

template<class T>
DeepPtr<T>::~DeepPtr<T>() { if (ptr) delete ptr; }


/******************** OPERATORS OVERLOADING ********************/
template<class T>
DeepPtr<T>& DeepPtr<T>::operator=(const DeepPtr& dp) {
    if (this != &dp) {
        if (ptr) delete ptr;
        ptr = dp.ptr != nullptr ? dp.ptr->clone() : nullptr;
    }
    return *this;
}

template<class T>
bool DeepPtr<T>::operator==(const DeepPtr& dp) const { return *ptr == *dp; }

template<class T>
bool DeepPtr<T>::operator!=(const DeepPtr& dp) const { return *ptr != *dp; }

template<class T>
bool DeepPtr<T>::operator<(const DeepPtr &dp) const { return *ptr < *dp; }

template<class T>
T* DeepPtr<T>::operator->() const { return ptr; }

template<class T>
T& DeepPtr<T>::operator*() const { return *ptr; }

#endif // DEEPPTR_H
