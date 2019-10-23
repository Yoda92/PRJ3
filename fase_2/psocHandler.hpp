// Child class of spiHandler

class psocHandler : public spiHandler {
  public: 
    static void init(void);
    static void sendByte(uint8_t&);
  private:
};