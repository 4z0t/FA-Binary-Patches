#include <type_traits>

template <typename T>
struct ObjectRef;

template <typename T>
struct ObjectRefChain
{
    using NodeT = typename ObjectRef<T>;
    friend class NodeT;
private:
    NodeT* _start;

public:
    ~ObjectRefChain()
    {
        for (NodeT* node = _start; node; node = _start)
        {
            _start = node->_next;
            node->_chain = nullptr;
            node->_next = nullptr;
        }
    }
};

template <typename T>
struct ObjectRef
{
    static_assert(std::is_base_of_v<ObjectRefChain<T>, T>);

    using ChainT = typename T::ObjectRefChain;
    friend ChainT;
private:
    ChainT* _chain = nullptr;
    ObjectRef* _next = nullptr;

public:
    T* GetObject()
    {
        return _chain ? static_cast<T*>(_chain) : nullptr;
    }

    const T* GetObject() const
    {
        return _chain ? static_cast<T*>(_chain) : nullptr;
    }

    ObjectRef(T* obj)
    {

        ChainT* chain = obj ? static_cast<ChainT*>(obj) : nullptr;
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

    ~ObjectRef()
    {
        ObjectRef<T>** i = &_chain->_start;
        if (i)
        {
            while (*i != this)
            {
                i = &(*i)->_next;
            }
            *i = _next;
        }
    }
};
