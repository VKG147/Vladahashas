#include "vladahasher.h"
#include <iostream>
#include <fstream>

std::string Vladahasher::hashToString(const std::bitset<kHashSize> &hash) {
    std::stringstream res;

    for (int i = 0; i < kHashSize; i += 8) {
        std::bitset<4> hexChar;
        for (int j = 0; j < 4; ++j) {
            hexChar[j] = hash[i+j];
        }

        res << std::hex << std::uppercase << hexChar.to_ulong();
    }

    return res.str();
}

std::string Vladahasher::getHash(const std::string & input) {
    std::deque<std::bitset<kBlockSize>> bitBlocks = getBitBlocks(input);

    for (int i = 0; i < bitBlocks.size(); ++i)
        shuffleBitBlock(bitBlocks[i]);

    std::bitset<kHashSize> hash(0);
    for (int i = 0; i < bitBlocks.size(); ++i) {
        auto hashBlocks = getHashBlocks(bitBlocks[i]);

        for (auto j = 0; j < hashBlocks.size(); ++j) {
            hash = getHashSum(hash, hashBlocks[j]);
        }
    }

    return hashToString(hash);
}

std::string Vladahasher::getHash(const std::stringstream & inputStream) {
    return getHash(inputStream.str());
}

std::string Vladahasher::getHashFromFile(const std::string & filePath) {
    std::ifstream inputFile(filePath);
    std::string inputString = "";

    if (inputFile.is_open()) {
        std::ostringstream ss;
        ss << inputFile.rdbuf();
        inputFile.close();
        inputString = ss.str(); 
    }

    return getHash(inputString);
}

std::deque<std::bitset<Vladahasher::kHashSize>> Vladahasher::getHashBlocks(const std::bitset<kBlockSize> & bitBlock) {
    std::deque<std::bitset<kHashSize>> hashBlocks;

    for (int i = 0; i < kBlockSize; i += kHashSize) {
        std::bitset<kHashSize> hashBlock;
        for (int j = i; j < i + kHashSize; ++j) {
            hashBlock[j-i] = bitBlock[j];
        }
        hashBlocks.push_back(hashBlock);
    }

    return hashBlocks;
}

std::bitset<Vladahasher::kHashSize> Vladahasher::getHashSum(const std::bitset<kHashSize> & hash1, const std::bitset<kHashSize> & hash2) {
    std::bitset<kHashSize> hashSum(0);

    bool carry = false;
    for (int i = 0; i < kHashSize; ++i) {
        hashSum[i] = (hash1[i]^hash2[i])^carry;
        carry = (hash1[i]&hash2[i])|(hash1[i]&carry)|(hash2[i]&carry);
    }

    return hashSum;
}

std::deque<std::bitset<Vladahasher::kBlockSize>> Vladahasher::getBitBlocks(const std::string & input) {
    std::deque<std::bitset<kBlockSize>> bitBlocks;

    int i = 0, blockCount = 0;
    do {
        std::bitset<kBlockSize> bitBlock;

        for (; i + blockCount*(kBlockSize/8) < input.length() && i < kBlockSize/8; i++) {
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
    for (int i = 0; i < kShuffleCount; ++i) {
        shuffleBitBlockHelper(bitBlock);
    }
}

void Vladahasher::shuffleBitBlockHelper(std::bitset<kBlockSize> &bitBlock) {
    int sWidth = kBlockSize/2;
    while (sWidth > 1) {
        for (int i = sWidth; i < kBlockSize; i += sWidth) {
            int left = i - 1, right = i;

            invertBitsInRange(bitBlock, left-sWidth+1, left+1);
            if (bitBlock[left]) {
                reverseBitsInRange(bitBlock, left-sWidth+1, left+1);
            }
            invertBitsInRange(bitBlock, right, right+sWidth);
            if (bitBlock[right]) {
                reverseBitsInRange(bitBlock, right, right+sWidth);
            }
        }
        sWidth /= 2;
    }

    for (int i = 0; i < kBlockSize; ++i) {
        if (bitBlock[i]) {
            invertBitsInRange(bitBlock, 0, i);
            reverseBitsInRange(bitBlock, i + 1, kBlockSize);
        }
        else {
            reverseBitsInRange(bitBlock, 0, i);
            invertBitsInRange(bitBlock, i + 1, kBlockSize);
        }
    }
}

void Vladahasher::invertBitsInRange(std::bitset<kBlockSize> &bitBlock, const int &start, const int &end) {
    if (start < 0 || start >= kBlockSize || end < 0 || end > kBlockSize) return;
    for (int i = start; i < end; ++i) {
        bitBlock[i] = ~bitBlock[i];
    }
}

void Vladahasher::reverseBitsInRange(std::bitset<kBlockSize> &bitBlock, const int &start, const int &end) {
    std::bitset<kBlockSize> tmp(bitBlock);
    if (start < 0 || start >= kBlockSize || end < 0 || end > kBlockSize) return;
    for (int i = start; i < end; ++i) {
        bitBlock[i] = tmp[end+start-i-1];
    }
}

std::bitset<Vladahasher::kHashSize> Vladahasher::hashStringToBitset(const std::string & hashString) {
    if (hashString.length()*8 != kHashSize) return {};

    std::bitset<kHashSize> hashBits(0);
    for (int i = 0; i < hashString.length(); ++i) {
        std::bitset<8> charBits(hashString[i]);

        for (int j = 0; j < 8; ++j) {
            hashBits[i*8+j] = charBits[j];
        }
    }

    return hashBits;
}

double Vladahasher::getHashDifference(const std::string & h1, const std::string & h2) {
    int bitDiff = 0;

    std::bitset<kHashSize> h1Bits = hashStringToBitset(h1);
    std::bitset<kHashSize> h2Bits = hashStringToBitset(h2);

    for (int i = 0; i < kHashSize; ++i) {
        if (!(h1Bits[i]&h2Bits[i])) bitDiff++;
    }

    return static_cast<double>(bitDiff) / kHashSize;
}