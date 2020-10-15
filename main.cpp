#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include "Vladahasher.h"

Vladahasher vladahasher;

const std::vector<std::string> testingPaths = {
        "test1_1.txt",
        "test1_2.txt",
        "test2_1.txt",
        "test2_2.txt",
        "test3_1.txt",
        "test3_2.txt",
        "test4.txt"
};

void runAnalysis();
void inputAnySize();
void inputFixedSize();
void outputIsDeterministic();
void hashFunctionEffectiveness();
void outputNotSimilar();
void testForCollisions();
void bruteForce(const std::string & hashToFind, int max_len, const std::string & cur);

int main(int argc, char** argv) {
    if (argc >= 2) {
        std::cout << vladahasher.getHashFromFile(argv[1]);
    }
    else {
        std::string input;
        std::cout << "Run analysis? (y/n)\n";
        std::cin >> input;
        if (input == "y" || input == "Y")
            runAnalysis();
        else {
            std::cout << "Welcome to Vladahashas. Enter text surrounded by \" to get a hash. Type \"quit\" without quotes to quit\n";
            while (true) {
                std::cout << "Please enter a string: ";
                std::cin >> input;

                if (input.length() >= 1 && input[0] == '\"' && input[input.length()-1] == '\"') {
                    input = input.substr(1, input.length()-2); // Remove quotations
                    std::cout << "Hash: " << vladahasher.getHash(input) << "\n\n";
                }
                else if (input == "quit") {
                    break;
                }
                else {
                    std::cout << "Did you mean to surround that with quotation marks? :)\n";
                }
            }
        }
    }
    return 0;
}

void runAnalysis() {
    std::cout << "===INPUT ANY SIZE===\n";
    inputAnySize();
    std::cout << "\n===OUTPUT FIXED SIZE===\n";
    inputFixedSize();
    std::cout << "\n===OUTPUT IS DETERMINISTIC===\n";
    outputIsDeterministic();
    std::cout << "\n===HASH FUNCTION EFFECTIVENESS===\n";
    hashFunctionEffectiveness();
    std::cout << "\n===OUTPUT INDISCERNIBLE===\n";
    outputNotSimilar();
//    std::cout << "\n===COLLISION TESTING===\n";
//    testForCollisions();
}

void inputAnySize() {
    for (auto it = testingPaths.begin(); it != testingPaths.end(); ++it) {
        std::cout << "Hash of " << *it << ": " << vladahasher.getHashFromFile("input/" + *it) << "\n";
    }
}

void inputFixedSize() {
    for (auto it = testingPaths.begin(); it != testingPaths.end(); ++it) {
        std::cout << "Hash size (in bits) of " << *it << ": " << vladahasher.getHashFromFile("input/" + *it).length()*8 << "\n";
    }
}

void outputIsDeterministic() {
    for (auto it = testingPaths.begin(); it != testingPaths.end(); ++it) {
        std::string hash = vladahasher.getHashFromFile("input/" + *it);

        bool det = true;

        for (int i = 0; i < 5; ++i) {
            std::string new_hash = vladahasher.getHashFromFile("input/" + *it);
            if (hash != new_hash) {
                det = false;
                break;
            }
        }

        if (det) std::cout << "Hashed "  << *it << " 5 times and the hash was the same throughout\n";
        else std::cout << "Hashing not deterministic!\n";
    }
}

void hashFunctionEffectiveness() {
    std::vector<std::string> lines;
    std::string line;

    std::ifstream sr("input/konstitucija.txt");
    while (std::getline(sr, line)) {
        lines.push_back(line);
    } sr.close();

    auto begin = std::chrono::steady_clock::now();
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        vladahasher.getHash(*it);
    }
    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cout << "Total time to hash each line of konstitucija.txt: " << static_cast<double>(diff)/1000 << "s\n";
}

void outputNotSimilar() {
    std::string str1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::string str2 = "aaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaa";

    std::string hash1 = vladahasher.getHash(str1);
    std::string hash2 = vladahasher.getHash(str2);

    double diffPerc = 100*vladahasher.getHashDifference(hash1, hash2);

    std::cout << "Hash of str1 differs from hash of str2 by " << diffPerc << "%\n";
}

std::string challenger = "";
std::string strToAttack = "IamC";
bool hashMatchFound = false;
unsigned long long total_attempted = 0;
void testForCollisions() {
    std::string hashToCollideWith = vladahasher.getHash(strToAttack);

    std::cout << "=" << hashToCollideWith << "\n";

    bruteForce(hashToCollideWith, 4, "");

    if (hashMatchFound) {
        std::cout << "Hash collision found after " << total_attempted << " attempts\n";
        std::cout << strToAttack << " collides with " << challenger << "\n";
    }
    else {
        std::cout << "No collisions found after " << total_attempted << " attempts\n";
    }
}

void bruteForce(const std::string & hashToFind, int max_len, const std::string & cur) {
    if (vladahasher.getHash(cur) == hashToFind && cur != strToAttack) {
        hashMatchFound = true;
        challenger = cur;
        return;
    }
    if(cur.length() == max_len) {
        return;
    }
    for(int c = 1; c < 256; ++c) {
        std::string next = cur + (char)c;
        total_attempted++;
        bruteForce(hashToFind, max_len, next);
        if (hashMatchFound) return;
    }
}