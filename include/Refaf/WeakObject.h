
#include <type_traits>

// From MohoEngine.dll: 0x10005430 - 0x1000548E
namespace Refaf
{
    // Moho::WeakPtr
    template <typename T>
    class WeakPtr;

    // Moho::WeakObject
    template <typename T>
    class WeakObject
    {
        using WeakPtrT = typename WeakPtr<T>;
        friend class WeakPtrT;

    private:
        WeakPtrT *_head;

    public:
        WeakObject() : _head{nullptr}
        {
        }

        WeakObject(const WeakObject &) = delete;
        WeakObject(WeakObject &&) = delete;
        WeakObject &operator=(const WeakObject &) = delete;
        WeakObject &operator=(WeakObject &&) = delete;

        /* Applications:
            Moho::UserEntity : 0x008B8892
            Moho::Unit : 0x006A736E
        */
        ~WeakObject()
        {
            for (WeakPtrT *node = _head, *next = nullptr; node != nullptr; node = next)
            {
                next = node->_next;
                node->_object = nullptr;
                node->_next = nullptr;
            }
            _head = nullptr;
        }
    };

    template <typename T>
    class WeakPtr
    {
        static_assert(std::is_base_of_v<WeakObject<T>, T>);

        using WeakObjectT = WeakObject<T>;
        friend WeakObjectT;

    private:
        WeakObjectT *_object = nullptr;
        WeakPtr *_next = nullptr;

    public:
        T *GetObject() const
        {
            return _object != nullptr
                       ? static_cast<T *>(_object)
                       : nullptr;
        }

        WeakPtr() : _object{nullptr}, _next{nullptr}
        {
        }

        WeakPtr(T *obj) : WeakPtr()
        {
            Link(obj);
        }

        WeakPtr(T &obj) : WeakPtr(&obj)
        {
        }

        WeakPtr(const WeakPtr &other) : WeakPtr(other.GetObject())
        {
        }

        WeakPtr(WeakPtr &&other) = delete;

        WeakPtr &operator=(const WeakPtr &other)
        {
            Unlink();
            Link(other.GetObject());
            return *this;
        }

        WeakPtr &operator=(WeakPtr &&other) = delete;

        WeakPtr &operator=(T &object)
        {
            Unlink();
            Link(&object);
            return *this;
        }

        WeakPtr &operator=(std::nullptr_t)
        {
            Unlink();
            return *this;
        }

        ~WeakPtr()
        {
            Unlink();
        }

    private:
        // MohoEngine.dll: 0x10005450
        void Link(T *obj)
        {
            WeakObjectT *weak_object = obj ? static_cast<WeakObjectT *>(obj) : nullptr;
            _object = weak_object;
            if (_object)
            {
                _next = _object->_head;
                _object->_head = this;
            }
            else
            {
                _next = nullptr;
            }
        }

        // MohoEngine.dll: 0x10005470
        void Unlink()
        {
            if (_object)
            {
                if (_object->_head == this)
                {
                    _object->_head = _next;
                }
                else
                    for (WeakPtr *node = _object->_head; node != nullptr; node = node->_next)
                    {
                        if (node->_next == this)
                        {
                            node->_next = _next;
                            break;
                        }
                    }
            }
            _next = nullptr;
            _object = nullptr;
        }
    };
} // namespace Refaf