#pragma once

#include <cstdint>
#include <cstddef>

template<class T = unsigned char>
class ByteReader {
public:
    static T get(const char* data, size_t offset = 0) {
        return static_cast<T>(*(data+offset));
    }
};

template<class T, class reader = ByteReader<>, size_t size = sizeof(T)>
class TypeReader {
public:
    static T get(const char* data, size_t offset = 0) {
        T res{};
        for (size_t i = 0; i < size; ++i) {
            res = res << 8;
            res |= static_cast<T>(reader::get(data, offset+i));
        }
        return res;
    }
};

using uint8Reader = TypeReader<uint8_t, ByteReader<>, 1>;
using uint16Reader = TypeReader<uint16_t, ByteReader<>, 2>;
using uint24Reader = TypeReader<uint32_t, ByteReader<>, 3>;
using uint32Reader = TypeReader<uint32_t, ByteReader<>, 4>;
using uint40Reader = TypeReader<uint64_t, ByteReader<>, 5>;

template<class In, class Out, uint64_t mask, uint64_t shift>
class FieldReader {
public:
    static Out get(In value) {
        return static_cast<Out>((value & static_cast<In>(mask)) >> shift);
    }
};
