#ifndef UTILS_BYTEARRAY_H
#define UTILS_BYTEARRAY_H
#include <memory>
#include <string>
using namespace std::string_literals;

struct bytearray {
    typedef unsigned char value_type;
    typedef unsigned char* pointer_type;
    typedef const unsigned char* const_pointer_type;

    bytearray()
        : m_size(0)
    {
    }

    explicit bytearray(const std::size_t size)
        : m_ptr(static_cast<pointer_type>(::malloc(size)), ::free)
        , m_size(size)
    {
    }

    bytearray(const const_pointer_type ptr, const std::size_t size)
        : m_ptr(static_cast<pointer_type>(::malloc(size)), ::free)
        , m_size(size)
    {
        ::memcpy(m_ptr.get(), ptr, size);
    }

    bytearray(const void* ptr, const std::size_t size)
        : m_ptr(static_cast<pointer_type>(::malloc(size)), ::free)
        , m_size(size)
    {
        ::memcpy(m_ptr.get(), ptr, size);
    }

    bytearray(const bytearray& copy)
        = default;

    bytearray& operator=(const bytearray& oth)
        = default;

    explicit operator void*() const { return cast<void>(); }

    template <typename T>
    [[nodiscard]] T* cast() const
    {
        return reinterpret_cast<T*>(get());
    }

    [[nodiscard]] bool is_empty() const { return !m_ptr || !m_size; }

    [[nodiscard]] unsigned char* get() const { return m_ptr.get(); }

    [[nodiscard]] std::size_t size() const { return m_size; }

    explicit operator bool() const { return !is_empty(); }

    explicit operator std::string() const { return std::string(reinterpret_cast<const char*>(get()), size()); }

    explicit operator std::u16string() const { return std::u16string(reinterpret_cast<const char16_t*>(get()), size() / 2); };

    explicit operator std::u8string() const { return std::u8string(reinterpret_cast<const char8_t*>(get()), size()); };

private:
    std::shared_ptr<value_type> m_ptr;
    std::size_t m_size;
};

#endif //UTILS_BYTEARRAY_H