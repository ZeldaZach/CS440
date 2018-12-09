#ifndef ASSIGNMENT3_SHAREDPTR_HPP
#define ASSIGNMENT3_SHAREDPTR_HPP

#include <atomic>
#include <cstddef>
#include <memory>
#include <iostream>
#include <mutex>

namespace cs540
{
    // Mutex for the references count
    static pthread_mutex_t reference_mutex = PTHREAD_MUTEX_INITIALIZER;

    template <typename T>
    class SharedPtr;

    template<class T>
    void DefaultDeleter(const void *p)
    {
        delete static_cast<const T*>(p);
        p = nullptr;
    }

    // https://www.codeproject.com/Articles/15351/Implementing-a-simple-smart-pointer-in-c
    struct ReferenceCounter
    {
    public:
        std::atomic_int references;
        const void *p;
        void (*deleter)(const void *);

    public:
        ReferenceCounter() : references(0) {}
        ReferenceCounter(const void *ptr) : references(0), p(ptr) {}
        ReferenceCounter(const ReferenceCounter &old) = default;
        ~ReferenceCounter() { if (p && deleter) (*deleter)(p); }

        int refCount() const
        {
            return references;
        }

        /*
         * THREAD-SAFE FUNCTION
         * Increments reference count
         */
        void AddRef()
        {
            references++;
        }

        /*
         * THREAD-SAFE FUNCTION
         * Decrements ref count and returns new value
         */
        int DelRef()
        {
            references--;

            return references;
        }
    };

    // https://stackoverflow.com/questions/2582161/c-function-pointer-as-parameter
    typedef void (*destructor_function)(const void *); // type for conciseness
    destructor_function destructorFunc; // variable to store function pointer type
    struct DestructorSaver
    {
        const void *p;
        void (*deleter)(const void *);

        DestructorSaver() : p(nullptr), deleter(nullptr) {}
        DestructorSaver(const void *ptr, destructor_function fp) : p(ptr), deleter(fp) {}
        DestructorSaver(const DestructorSaver &old) : p(old.p), deleter(old.deleter) {}
        ~DestructorSaver() { (*deleter)(p); }
    };

    template <typename T1, typename T2>
    bool operator==(const SharedPtr<T1> &sp1, const SharedPtr<T2> &sp2)
    {
        return sp1.get() == sp2.get();
    }

    template <typename T>
    bool operator==(const SharedPtr<T> &sp, std::nullptr_t nptr)
    {
        return (sp.get() == nptr);
    }

    template <typename T>
    bool operator==(std::nullptr_t nptr, const SharedPtr<T> &sp)
    {
        return (sp == nptr);
    }

    template <typename T1, typename T2>
    bool operator!=(const SharedPtr<T1> &sp1, const SharedPtr<T2> &sp2)
    {
        return (sp1.get() != sp2.get());
    }

    template <typename T>
    bool operator!=(const SharedPtr<T> &sp, std::nullptr_t nptr)
    {
        return (sp.get() != nptr);
    }

    template <typename T>
    bool operator!=(std::nullptr_t nptr, const SharedPtr<T> &sp)
    {
        return (sp != nptr);
    }

    template <typename T, typename U>
    SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp)
    {
        auto p = static_cast<T*>(sp.get());
        SharedPtr<T> SPT = SharedPtr<T>();

        SPT.setPtr(p);
        SPT.setRC(sp.getRC());

        pthread_mutex_lock(&reference_mutex);
        SPT.rc->AddRef();
        pthread_mutex_unlock(&reference_mutex);

        return SPT;
    }

    template <typename T, typename U>
    SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp)
    {
        auto p = dynamic_cast<T*>(sp.get());
        SharedPtr<T> SPT = SharedPtr<T>();

        SPT.setPtr(p);
        SPT.setRC(sp.getRC());

        pthread_mutex_lock(&reference_mutex);
        SPT.rc->AddRef();
        pthread_mutex_unlock(&reference_mutex);

        return SPT;
    }

    template <typename T>
    class SharedPtr
    {
    public:
        T                   *value_ptr;
        ReferenceCounter    *rc;

        /*
         *  Constructs a smart pointer that points to null.
         */
        SharedPtr() : value_ptr(nullptr), rc(nullptr)
        {
        }

        /*
         *  Constructs a smart pointer that points to the given object.
         *  The reference count is initialized to one.
         */
        template <typename U>
        SharedPtr(U *u_ptr) : value_ptr(u_ptr), rc(new ReferenceCounter(value_ptr))
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc)
            {
                rc->deleter = &DefaultDeleter<U>;
                rc->AddRef();
            }
            pthread_mutex_unlock(&reference_mutex);
        }

        /*
         *  If p is not null, then reference count of the managed object is incremented.
         */
        SharedPtr(const SharedPtr &p) : value_ptr(p.value_ptr), rc(p.rc)
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc)
            {
                rc->AddRef();
            }
            pthread_mutex_unlock(&reference_mutex);
        }

        /*
         *  If p is not null, then reference count of the managed object is incremented.
         */
        template <typename U>
        SharedPtr(const SharedPtr<U> &p) : value_ptr(p.value_ptr), rc(p.rc)
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc)
            {
                rc->deleter = &DefaultDeleter<U>;
                rc->AddRef();
            }
            pthread_mutex_unlock(&reference_mutex);
        }

        /*
         *  Move the managed object from the given smart pointer.
         *  The reference count must remain unchanged.
         *  After this function, p must be null.
         */
        SharedPtr(SharedPtr &&p) : value_ptr(std::move(p.value_ptr)), rc(p.rc)
        {
            p.value_ptr = nullptr;
            p.rc = nullptr;
        }

        /*
         *  Move the managed object from the given smart pointer.
         *  The reference count must remain unchanged.
         *  After this function, p must be null.
         */
        template <typename U>
        SharedPtr(SharedPtr<U> &&p) : value_ptr(std::move(p.value_ptr)), rc(p.rc)
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc)
            {
                rc->deleter = &DefaultDeleter<U>;
            }
            pthread_mutex_unlock(&reference_mutex);

            p.value_ptr = nullptr;
            p.rc = nullptr;
        }

        /*
         * Copy assignment.
         * Must handle self assignment.
         * Decrement reference count of current object,
         * if any, and increment reference count of the given object.
         */
        SharedPtr &operator=(const SharedPtr &copy)
        {
            if (this != &copy)
            {
                pthread_mutex_lock(&reference_mutex);
                if (rc && rc->refCount() > 0)
                {
                    rc->DelRef();
                }
                if (rc && rc->refCount() == 0)
                {
                    deleteContents();
                }

                value_ptr = copy.value_ptr;
                rc = copy.rc;
                if (rc)
                {
                    rc->AddRef();
                }
                pthread_mutex_unlock(&reference_mutex);
            }

            return *this;
        }

        /*
         * Copy assignment.
         * Must handle self assignment.
         * Decrement reference count of current object,
         * if any, and increment reference count of the given object.
         */
        template <typename U>
        SharedPtr<T> &operator=(const SharedPtr<U> &copy)
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc && rc->refCount() > 0)
            {
                rc->DelRef();
            }
            if (rc && rc->refCount() == 0)
            {
                deleteContents();
            }

            value_ptr = copy.value_ptr;
            rc = copy.rc;
            if (rc)
            {
                rc->deleter = &DefaultDeleter<U>;
                rc->AddRef();
            }
            pthread_mutex_unlock(&reference_mutex);

            return *this;
        }

        /*
         * Move assignment.
         * After this operation, p must be null.
         * The reference count associated with the object
         * (if p is not null before the operation)
         * will remain the same after this operation.
         */
        SharedPtr &operator=(SharedPtr &&p)
        {

            pthread_mutex_lock(&reference_mutex);
            if (rc && rc->refCount() > 0)
            {
                rc->DelRef();
            }
            if (rc && rc->refCount() == 0)
            {
                deleteContents();
            }

            value_ptr = std::move(p.value_ptr);
            rc = p.rc;
            pthread_mutex_unlock(&reference_mutex);

            p.value_ptr = nullptr;
            p.rc = nullptr;

            return *this;
        }

        /*
         * Move assignment.
         * After this operation, p must be null.
         * The reference count associated with the object
         * (if p is not null before the operation)
         * will remain the same after this operation.
         */
        template <typename U>
        SharedPtr &operator=(SharedPtr<U> &&p)
        {
            if (this->value_ptr != p.value_ptr)
            {
                pthread_mutex_lock(&reference_mutex);
                if (rc && rc->refCount() > 0)
                {
                    rc->DelRef();
                }
                if (rc && rc->refCount() == 0)
                {
                    deleteContents();
                }

                value_ptr = std::move(p.value_ptr);
                rc = p.rc;
                if (rc)
                {
                    rc->deleter = &DefaultDeleter<U>;
                }
                pthread_mutex_unlock(&reference_mutex);
            }

            p.value_ptr = nullptr;
            p.rc = nullptr;

            return *this;
        }

        /*
         * Decrement reference count of managed object.
         * If the reference count is zero, delete the object.
         */
        ~SharedPtr()
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc != nullptr && rc->refCount() > 0)
            {
                rc->DelRef();
            }
            if (rc != nullptr && rc->refCount() == 0)
            {
                deleteContents();
            }
            pthread_mutex_unlock(&reference_mutex);
        }

        /*
         *  The smart pointer is set to point to the null pointer.
         *  The reference count for the currently pointed to object, if any, is decremented.
         */
        void reset()
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc && rc->refCount() > 0)
            {
                rc->DelRef();
            }
            if (rc && rc->refCount() == 0)
            {
                deleteContents();
            }

            rc = nullptr;
            pthread_mutex_unlock(&reference_mutex);

            value_ptr = nullptr;
        }

        /*
         * Replace owned resource with another pointer.
         * If the owned resource has no other references, it is deleted.
         * If p has been associated with some other smart pointer, the behavior is undefined.
         */
        template <typename U>
        void reset(U *p)
        {
            pthread_mutex_lock(&reference_mutex);
            if (rc && rc->refCount() > 0)
            {
                rc->DelRef();
            }
            if (rc && rc->refCount() == 0)
            {
                deleteContents();
            }

            rc = new ReferenceCounter(p);
            if (rc)
            {
                rc->deleter = &DefaultDeleter<U>;
                rc->AddRef();
            }
            pthread_mutex_unlock(&reference_mutex);

            value_ptr = p;
        }

        T *get() const
        {
            return value_ptr;
        }

        ReferenceCounter* getRC() const
        {
            return rc;
        }

        void setPtr(T *ptr)
        {
            value_ptr = ptr;
        }

        void setRC(ReferenceCounter *rctmp)
        {
            rc = rctmp;
        }

        T& operator*() const
        {
            return *(value_ptr);
        }

        T* operator->() const
        {
            return (value_ptr);
        }

        explicit operator bool() const
        {
            return (value_ptr != nullptr);
        }

    protected:
        void deleteContents()
        {
            if (rc != nullptr)
            {
                delete rc;
                rc = nullptr;
            }
        }
    };
}

#endif // ASSIGNMENT3_SHAREDPTR_HPP
