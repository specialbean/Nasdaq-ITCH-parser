#pragma once
#include "common/utils.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <vector>

class Reader {
    std::ifstream file;

    char *returnHexMessage(int length) {
        char buf[length];
        file.read(buf, length);
        return;
    }

  public:
    Reader(std::string file_name) {
        file.setf(std::ios_base::hex);
        file.open(file_name);
    }

    int getNextHexMessage() {
        char buf[2];
        file.read(buf, 2);
        hexPairToByte(buf[0], buf[1]);
    }
};
