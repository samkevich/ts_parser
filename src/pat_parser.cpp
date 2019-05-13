#include <iostream>
#include "pat_parser.hpp"

PATParser::PATParser(const PATCallback& callback)
    : m_callback(callback) {
}

bool PATParser::parse(const Packet& packet, const char* data, size_t offset) const {
    if (packet.payload_start_indicator) {
        //only included when the ts package is a PSI.
        uint8_t pointer_field = uint8Reader::get(data, offset);
        if (pointer_field > 0)
            offset += pointer_field;
        offset += 1;
    }
    
    uint32_t table_header = uint24Reader::get(data, offset);
    auto section_length = table_header::section_length::get(table_header);
    if (section_length >= TS_SIZE - TS_HEADER_SIZE) {
        std::cerr << "parse section_length error, section_length = " << section_length << std::endl;
        return false;
    }
    
    offset += 8;
    const size_t table_data_size = section_length-9;
    for (size_t end = offset+table_data_size; offset < end; offset += 4) {
        uint32_t pat = uint32Reader::get(data, offset);
        auto program_map_pid = pat_data::program_map_pid::get(pat);
        m_callback(program_map_pid);
    }
    return true;
}
