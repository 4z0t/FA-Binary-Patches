
#include <type_traits>

namespace STD
{

    template <typename T>
    struct BasicString;
    using String = BasicString<char>;
    using WString = BasicString<wchar_t>;

    WString *__cdecl wstring_copy_ctor(WString *ws, const char *s) asm("0x938720");
    String *__thiscall InitString(String *this_, const char *str) asm("0x405550");
    String *__thiscall AssignString(String *this_, const char *str,
                                    size_t size) asm("0x4059E0");

    int __thiscall wstring_dtor(WString *ws) asm("0x00431390");
    void __thiscall string_dtor(String *ws) asm("0x00402370");

#define SSO_bytes 0x10ul

    template <typename T>
    class BasicString
    {
    public:
        static constexpr uint32_t sso_size = SSO_bytes / sizeof(T);

    private:
        void *dead; // ?
        union
        {
            T str[sso_size]; // data
            T *_data;        // pointer to data
        };
        uint32_t len;
        uint32_t capacity; // capacity?

    public:
        BasicString()
        {
            str[0] = T(0);
            len = 0;
            capacity = sso_size - 1;
        }

        BasicString(const char *s)
        {
            if constexpr (std::is_same_v<char, T>)
                InitString(this, s);
            else if constexpr (std::is_same_v<wchar_t, T>)
                wstring_copy_ctor(this, s);
            else
                static_assert(false, "Unknown type T.");
        }

        inline const T *data() const { return capacity < sso_size ? static_cast<const T *>(str) : _data; }
        inline T *data() { return capacity < sso_size ? static_cast<T *>(str) : _data; }

        inline void clear()
        {
            if (capacity >= sso_size)
            {
                free(data());
            }
            str[0] = T(0);
            len = 0;
            capacity = sso_size - 1;
        }

        ~BasicString() { clear(); }
    };

    static_assert(sizeof(String) == 0x1C);
    static_assert(String::sso_size == 0x10);
    static_assert(sizeof(WString) == 0x1C);
    static_assert(WString::sso_size == 0x8);

} // namespace STD
