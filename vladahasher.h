#pragma once

#include <string>
#include <sstream>
#include <deque>
#include <bitset>

class Vladahasher {
private:
    static constexpr uint32_t kBlockSize = 256;
    static constexpr uint32_t kHashSize = 64;

    std::string hashToString(const std::bitset<kHashSize> & hash);
    std::deque<std::bitset<kHashSize>> getHashBlocks(const std::bitset<kBlockSize> & bitBlock);
    std::bitset<kHashSize> getHashSum(const std::bitset<kHashSize> & hash1, const std::bitset<kHashSize> & hash2);
    std::deque<std::bitset<kBlockSize>> getBitBlocks(const std::string &);
    void shuffleBitBlock(std::bitset<kBlockSize>& bitBlock);
    void invertBitsInRange(std::bitset<kBlockSize>& bitBlock, const int & start, const int & end);
    void reverseBitsInRange(std::bitset<kBlockSize>& bitBlock, const int & start, const int & end);
public:
    std::string getHash(const std::string &);
    std::string getHash(const std::stringstream &);
};
