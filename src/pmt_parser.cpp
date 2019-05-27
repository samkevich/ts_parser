#include <iostream>
#include "pmt_parser.hpp"

PMTParser::PMTParser(const PMTCallback& callback)
    : m_callback(callback) {
}

bool PMTParser::parse(const Packet& packet, const char* data, size_t offset) const {
    if (packet.payload_start_indicator) {
        // only included when the ts package is a PSI.
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
    uint32_t pmt = uint32Reader::get(data, offset);
    auto program_info_length = pmt_data::program_info_length::get(pmt);
    offset += program_info_length + 4;
    const size_t table_data_size = section_length - (9 + 4 + program_info_length);
    if (table_data_size >= TS_SIZE - TS_HEADER_SIZE) {
        std::cerr << "parse table_data_size error, table_data_size = " << table_data_size << std::endl;
        return false;
    }
    
    for (size_t end = offset+table_data_size; offset < end;) {
        auto stream_header = uint40Reader::get(data, offset);
        auto stream_type = pes_data::stream_type::get(stream_header);
        auto stream_pid = pes_data::elementary_pid::get(stream_header);
        auto info_length = pes_data::es_info_length::get(stream_header);
        m_callback(stream_pid, stream_type);
        offset += 5 + info_length;
    }
    return true;
}
