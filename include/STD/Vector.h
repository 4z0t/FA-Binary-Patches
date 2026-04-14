#include <cstdint>

namespace STD
{
    template <typename T>
    class Vector
    {
        void *dead;
        T *_begin;
        T *_end;
        T *_capacity_end;

    public:
        struct _Iterator
        {
            _Iterator(T *it) : it(it) {}

            bool operator==(const _Iterator &other) { return other.it == it; };
            bool operator!=(const _Iterator &other) { return !(*this == other); };

            _Iterator &operator++()
            {
                ++it;
                return *this;
            }

            T &operator*() { return *it; }

        private:
            T *it;
        };

        using Iterator = _Iterator;

        T &operator[](int index) { return this->_begin[index]; }

        Iterator begin() { return Iterator(this->_begin); }
        Iterator end() { return Iterator(this->_end); }

        size_t size() { return this->_begin ? this->_end - this->_begin : 0; }
    };
} // namespace STD
