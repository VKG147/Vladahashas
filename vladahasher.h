#pragma once

#include <string>
#include <sstream>
#include <deque>
#include <bitset>

class Vladahasher {
private:
    static constexpr uint32_t kBlockSize = 256;
    static constexpr uint32_t kHashSize = 128;
    static constexpr uint32_t kShuffleCount = 4;

    std::bitset<kHashSize> getHashSum(const std::bitset<kHashSize> & hash1, const std::bitset<kHashSize> & hash2);
    std::string hashToString(const std::bitset<kHashSize> & hash);

    std::deque<std::bitset<kHashSize>> getHashBlocks(const std::bitset<kBlockSize> & bitBlock);
    std::deque<std::bitset<kBlockSize>> getBitBlocks(const std::string &);

    std::bitset<kHashSize> hashStringToBitset(const std::string & hashString);

    void shuffleBitBlock(std::bitset<kBlockSize>& bitBlock);
    void shuffleBitBlockHelper(std::bitset<kBlockSize>& bitBlock);
    void invertBitsInRange(std::bitset<kBlockSize>& bitBlock, const int & start, const int & end);
    void reverseBitsInRange(std::bitset<kBlockSize>& bitBlock, const int & start, const int & end);
public:
    std::string getHash(const std::string & input);
    std::string getHash(const std::stringstream & inputStream);
    std::string getHashFromFile(const std::string & filePath);

    double getHashPercentageDifference(const std::string & h1, const std::string & h2);
};
