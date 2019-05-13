
#include <iostream>
#include <fstream>
#include <memory>
#include "buffer.hpp"
#include "parser.hpp"
#include "pes_parser.hpp"
#include "pmt_parser.hpp"
#include "pat_parser.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Use " << argv[0] << " file.ts" << std::endl;
        return 1;
    }
    
    std::ifstream file_ts(argv[1], std::ifstream::binary);
    if (file_ts.fail()) {
        std::cout << "Could not open " << argv[1] << std::endl;
        return 1;
    }
    
    std::ofstream file_v("./video.mpv", std::ifstream::binary);
    std::ofstream file_a("./audio.mpa", std::ifstream::binary);

    TSParser parser;

    auto video_parser = std::make_shared<PESParser>([&file_v](const char* data, size_t size) {
        file_v.write(data, size);
    });
    
    auto audio_parser = std::make_shared<PESParser>([&file_a](const char* data, size_t size) {
        file_a.write(data, size);
    });
    
    auto pmt_parser = std::make_shared<PMTParser>([&](uint16_t pes_pid, uint8_t stream_type) {
        if (stream_type::is_audio(stream_type)) {
            parser.add_payload_parser(pes_pid, audio_parser);
        } else if (stream_type::is_video(stream_type)) {
            parser.add_payload_parser(pes_pid, video_parser);
        }
    });
    
    auto pat_parser = std::make_shared<PATParser>([&parser, &pmt_parser](uint16_t pmt_pid){
        parser.add_payload_parser(pmt_pid, pmt_parser);
    });
    parser.add_payload_parser(0, pat_parser);
    
    Buffer buffer(TS_SIZE * 1000);
    
    while (file_ts.good()) {

        if (buffer.size() < TS_SIZE) {
            buffer.read_from(file_ts);
        }

        while(buffer.size() >= TS_SIZE) {
            if (parser.parse(buffer.data())) {
                buffer.consume(TS_SIZE);
            } else {
                buffer.consume(1);
            }
        }
	}
	return 0;
}

