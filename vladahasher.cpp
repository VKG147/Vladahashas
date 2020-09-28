#include "vladahasher.h"
#include <iostream>

std::string Vladahasher::getHash(const std::string & input) {
    std::deque<std::bitset<kBlockSize>> bitBlocks = getBitBlocks(input);

    for (std::deque<std::bitset<kBlockSize>>::iterator it = bitBlocks.begin(); it != bitBlocks.end(); ++it) {
        shuffleBitBlock(*it);  // May not change, look out
    }


}

std::string Vladahasher::getHash(const std::stringstream & inputStream) {
    return getHash(inputStream.str());
}

std::deque<std::bitset<Vladahasher::kBlockSize>> Vladahasher::getBitBlocks(const std::string & input) {
    std::deque<std::bitset<kBlockSize>> bitBlocks;

    int i = 0, blockCount = 0;
    do {
        std::bitset<kBlockSize> bitBlock;

        for (; i+blockCount*(kBlockSize/8) < input.length() && i < kBlockSize/8; i++) {
            std::bitset<8> character(input[i+blockCount*(kBlockSize/8)]);

            for (int j = 0; j < 8; ++j) {
                bitBlock[i*8 + j] = character[j];
            }
        }

        bitBlocks.push_back(bitBlock);
        blockCount++;
        i = 0;
    } while (i + blockCount*(kBlockSize/8) < input.length());

    return bitBlocks;
}

void Vladahasher::shuffleBitBlock(std::bitset<kBlockSize> &bitBlock) {

}