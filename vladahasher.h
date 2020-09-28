#pragma once

#include <string>
#include <sstream>
#include <deque>
#include <bitset>

class Vladahasher {
private:
    static constexpr uint32_t kBlockSize = 512;

    std::deque<std::bitset<kBlockSize>> getBitBlocks(const std::string &);
    void shuffleBitBlock(std::bitset<kBlockSize>& bitBlock);
public:
    std::string getHash(const std::string &);
    std::string getHash(const std::stringstream &);
};
