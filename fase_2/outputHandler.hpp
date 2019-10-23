#pragma once

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "spiHandler.hpp"

class outputHandler : public spiHandler {
  public: 
    static void sendOutput(uint8_t&);
    static void init(void);
  private:

};