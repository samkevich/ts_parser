#pragma once

#include "parser.hpp"

typedef std::function<void(const char* data, size_t size)> PESCallback;

class PESParser: public PayloadParser  {
public:
    PESParser(const PESCallback& callback);
    bool parse(const Packet& packet, const char* data, size_t offset) const override;
    
private:
    PESCallback m_callback;
};
