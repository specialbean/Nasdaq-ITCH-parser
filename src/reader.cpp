#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Reader {
    std::ifstream file;

    /* will be made functional after
    std::vector<char> getHexMessage(int length) {
        char buf[length];
        file.read(buf, length);
        std::vector<char> res(buf, buf + length);
        return res;
    }
    */

   public:
    explicit Reader(const std::string& file_name) {
        file.open(file_name, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "failed to open: " << file_name << '\n';
        }
    }

    auto bufferMessage() -> std::vector<char> {
        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        std::cout << "File end pointer: " << size << '\n';
        file.seekg(0, std::ios::beg);

        std::vector<char> buf(static_cast<size_t>(size));
        file.read(buf.data(), size);
        return buf;
    }
};
