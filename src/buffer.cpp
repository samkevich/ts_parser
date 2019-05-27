#include <istream>
#include <iterator>
#include "buffer.hpp"

Buffer::Buffer(size_t capacity)
    : m_buffer(capacity), m_current(0), m_last(0) {
}

size_t Buffer::size() const {
    return m_last - m_current;
}

const char* Buffer::data() const {
    return &m_buffer[m_current];
}

void Buffer::read_from(std::istream& input) {
    if (m_last - m_current > 0) {
        std::move(std::next(m_buffer.begin(), m_current),
                  std::next(m_buffer.begin(), m_last),
                  m_buffer.begin());
    }
    m_last = m_last - m_current;
    m_current = 0;
    input.read(&m_buffer[m_last], m_buffer.size()-m_last);
    m_last += input.gcount();
}

void Buffer::consume(size_t amount) {
    if (amount > size())
        amount = size();
    m_current += amount;
}
