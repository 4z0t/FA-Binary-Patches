#include <type_traits>

template <typename T>
class ObjectRef;

template <typename T>
class ObjectRefChain
{
    using NodeT = typename ObjectRef<T>;
    friend class NodeT;

private:
    NodeT *_start;

public:
    ObjectRefChain() : _start{nullptr}
    {
    }

    ObjectRefChain(const ObjectRefChain &) = delete;
    ObjectRefChain(ObjectRefChain &&) = delete;
    ObjectRefChain &operator=(const ObjectRefChain &) = delete;
    ObjectRefChain &operator=(ObjectRefChain &&) = delete;

    ~ObjectRefChain()
    {
        for (NodeT *node = _start; node; node = _start)
        {
            _start = node->_next;
            node->_chain = nullptr;
            node->_next = nullptr;
        }
    }
};

template <typename T>
class ObjectRef
{
    static_assert(std::is_base_of_v<ObjectRefChain<T>, T>);

    using ChainT = typename T::ObjectRefChain;
    friend ChainT;

private:
    ChainT *_chain = nullptr;
    ObjectRef *_next = nullptr;

public:
    T *GetObject() const
    {
        return _chain ? static_cast<T *>(_chain) : nullptr;
    }

    ObjectRef() : _chain{nullptr}, _next{nullptr}
    {
    }

    ObjectRef(T *obj)
    {
        Set(obj);
    }

    ObjectRef(const ObjectRef &other) : ObjectRef(other.GetObject())
    {
    }

    ObjectRef &operator=(const ObjectRef &other)
    {
        Release();
        Set(other.GetObject());
        return *this;
    }

    ObjectRef &operator=(ObjectRef &&other)
    {
        Release();
        Set(other.GetObject());
        return *this;
    }

    void Set(T *obj)
    {
        ChainT *chain = obj ? static_cast<ChainT *>(obj) : nullptr;
        _chain = chain;
        if (chain)
        {
            _next = chain->_start;
            chain->_start = this;
        }
        else
        {
            _next = nullptr;
        }
    }

    void Release()
    {
        if (_chain)
        {
            ObjectRef<T> **i = &_chain->_start;
            if (i)
            {
                while (*i != this)
                {
                    i = &(*i)->_next;
                }
                *i = _next;
            }
        }
        _next = nullptr;
        _chain = nullptr;
    }

    ~ObjectRef()
    {
        Release();
    }
};
