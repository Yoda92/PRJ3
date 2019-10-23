#pragma once

// Child class of spiHandler
#include "spiHandler.hpp"

class mcp3202Handler : public spiHandler {
  public: 
    static void init(void);
    static uint16_t getValue(int channel);
  private:
    static uint16_t combineBytes(uint8_t, uint8_t);
    static uint8_t commandByte0;
    static uint8_t commandByte1;
};