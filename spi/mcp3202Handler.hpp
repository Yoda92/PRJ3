#include "spiHandler.hpp"

class mcp3202Handler {
  public: 
    static void init(void);
    static uint16_t getValue(int channel);
  private:
    static uint16_t combineBytes(uint8_t, uint8_t);
    static const uint8_t commandByte0 = 0b00011010;
    static const uint8_t commandByte1 = 0b00011110;
};