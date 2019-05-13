#pragma once

#include <vector>

class Buffer {
public:
    Buffer(size_t capacity);
    size_t size() const;
    const char* data() const;
    void read_from(std::istream& input);
    void consume(size_t amount);
    
private:
    std::vector<char> m_buffer;
    size_t m_current;
    size_t m_last;
};
