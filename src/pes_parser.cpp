#include <iostream>
#include "pes_parser.hpp"

PESParser::PESParser(const PESCallback& callback)
    : m_callback(callback) {
}

bool PESParser::parse(const Packet& packet, const char* data, size_t offset) const {
    uint8_t pes_headerlength = 0;
    if (packet.payload_start_indicator) {
        pes_headerlength = uint8Reader::get(data, offset+8);
        offset += PES_HEADER_SIZE + PES_HEADER_OPTIONAL_SIZE + pes_headerlength;
    }
    size_t payload_size = TS_SIZE - offset;
    if (payload_size > TS_SIZE - TS_HEADER_SIZE)
    {
        std::cerr << "parse payload_size error, payload_size = " << payload_size << std::endl;
        payload_size = 0;
        return false;
    }
    m_callback(data+offset, payload_size);
    return true;
}
