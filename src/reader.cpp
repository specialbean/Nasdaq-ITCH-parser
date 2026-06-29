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
    explicit Reader(const std::string& file_name) { file.open(file_name, std::ios::binary); }

    auto bufferMessage() -> std::vector<char> {
        file.seekg(0, std::ios::end);
        auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buf(static_cast<size_t>(size));
        file.read(buf.data(), size);
        return buf;
    }
};
