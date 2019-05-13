#include "parser.hpp"

bool TSParser::parse(const char* data) const {
    char sync_byte = uint8Reader::get(data);
    // skip if it is not a sync byte.
    if (sync_byte != SYNC_BYTE_VAL)
        return false;
    uint32_t header = uint32Reader::get(data);
    namespace tp = transport_packet;
    auto transport_error = tp::transport_error_indicator::get(header);
    if (transport_error)
        return true;
    
    Packet packet;
    packet.payload_start_indicator = tp::payload_unit_start_indicator::get(header);
    packet.pid = tp::pid::get(header);
    auto adaptation_field_control = tp::adaptation_field_control::get(header);
    packet.has_payload = adaptation_field_control & 0x1;
    packet.has_adaptation = adaptation_field_control & 0x2;
    
    if (!packet.has_payload)
        return true;
    
    //skip private packets
    if (packet.pid >= 16 && packet.pid <= 31)
        return true;
    
    size_t payload_offset = 4;
    if (packet.has_adaptation) {
        payload_offset += uint8Reader::get(data, payload_offset) + 1;
    }
    
    auto it = m_parsers.find(packet.pid);
    if (it != m_parsers.end()) {
        return it->second->parse(packet, data, payload_offset);
    }
    
    return true;
}

void TSParser::add_payload_parser(uint16_t pid, const std::shared_ptr<PayloadParser>& parser) {
    m_parsers[pid] = parser;
}
