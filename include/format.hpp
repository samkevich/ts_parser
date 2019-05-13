#pragma once

#include "reader.hpp"

// https://en.wikipedia.org/wiki/MPEG_transport_stream
// https://en.wikipedia.org/wiki/Program-specific_information
// https://en.wikipedia.org/wiki/Packetized_elementary_stream

static constexpr uint32_t TS_SIZE = 188;
static constexpr uint32_t TS_HEADER_SIZE = 4;
static constexpr uint32_t PES_HEADER_SIZE = 6;
static constexpr uint32_t PES_HEADER_OPTIONAL_SIZE = 3;
static constexpr char SYNC_BYTE_VAL = 0x47;

namespace transport_packet {
    using sync_byte = FieldReader<uint32_t, uint8_t, 0xff000000, 24>;
    using transport_error_indicator = FieldReader<uint32_t, bool, 0x800000, 23>;
    using payload_unit_start_indicator  = FieldReader<uint32_t, bool, 0x400000, 22>;
    using transport_priority = FieldReader<uint32_t, bool, 0x200000, 21>;
    using pid = FieldReader<uint32_t, uint16_t, 0x1fff00, 8>;
    using adaptation_field_control = FieldReader<uint32_t, uint8_t, 0x30, 4>;
    using continuity_counter = FieldReader<uint32_t, uint8_t, 0xf, 0>;
}

namespace adaptation_field {
    using adaptation_field_length = FieldReader<uint16_t, uint8_t, 0xFF00, 8>;
    using discontinuity_indicator = FieldReader<uint16_t, bool, 0x80, 7>;
    using random_access_indicator = FieldReader<uint16_t, bool, 0x40, 6>;
    using elementary_stream_priority_indicator = FieldReader<uint16_t, bool, 0x20, 5>;
    using pcr_flag = FieldReader<uint16_t, bool, 0x10, 4>;
    using opcr_flag = FieldReader<uint16_t, bool, 0x08, 3>;
    using splicing_point_flag = FieldReader<uint16_t, bool, 0x04, 2>;
    using transport_private_data_flag = FieldReader<uint16_t, bool, 0x02, 1>;
    using adaptation_field_extension_flag = FieldReader<uint16_t, bool, 0x01, 0>;
}

namespace table_header {
    using section_length = FieldReader<uint32_t, uint16_t, 0x3FF, 0>;
}

namespace pat_data {
    using program_num = FieldReader<uint32_t, uint16_t, 0xFFFF0000, 16>;
    using program_map_pid = FieldReader<uint32_t, uint16_t, 0x1FFF, 0>;
}

namespace pmt_data {
    using pcr_pid = FieldReader<uint32_t, uint16_t, 0x1FFF0000, 16>;
    using program_info_length = FieldReader<uint32_t, uint16_t, 0x3FF, 0>;
}

namespace pes_data {
    using stream_type = FieldReader<uint64_t, uint8_t, 0xFF00000000, 32>;
    using elementary_pid = FieldReader<uint64_t, uint16_t, 0x1FFF0000, 16>;
    using es_info_length = FieldReader<uint64_t, uint16_t, 0x3FF, 0>;
}

namespace stream_type {
    inline bool is_audio(uint8_t stream_type) {
        return stream_type == 0x03 || stream_type == 0x04 ||
        stream_type == 0x0f || stream_type == 0x11 ||
        stream_type == 0x80 || stream_type == 0x81 ||
        stream_type == 0x82 || stream_type == 0x83 ||
        stream_type == 0x84 || stream_type == 0x85 ||
        stream_type == 0x86 || stream_type == 0x8a ||
        stream_type == 0xa1 || stream_type == 0xa2 ||
        stream_type == 0x90;
    }
    
    inline bool is_video(uint8_t stream_type) {
        return stream_type == 0x01 || stream_type == 0x02 ||
        stream_type == 0x1b || stream_type == 0x20 ||
        stream_type == 0x10 || stream_type == 0x24 ||
        stream_type == 0x42 || stream_type == 0xd1 ||
        stream_type == 0xea;
    }
}
