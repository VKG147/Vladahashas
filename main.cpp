#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <random>
#include <functional>
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
void testDifference();

void genFile() {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(33,126);
    auto getRand = std::bind(distribution, generator);

    std::ofstream sw("collisionTest.txt");

    for (int i = 0; i < 25000; ++i) {
        std::string p1 = "", p2 = "";

        for (int j = 0; j < 10; ++j) {
            p1 += (char)getRand();
            p2 += (char)getRand();
        }
        sw << p1 << " " << p2 << "\n";
        p1 = ""; p2 = "";

        for (int j = 0; j < 100; ++j) {
            p1 += (char)getRand();
            p2 += (char)getRand();
        }
        sw << p1 << " " << p2 << "\n";
        p1 = ""; p2 = "";

        for (int j = 0; j < 500; ++j) {
            p1 += (char)getRand();
            p2 += (char)getRand();
        }
        sw << p1 << " " << p2 << "\n";
        p1 = ""; p2 = "";

        for (int j = 0; j < 1000; ++j) {
            p1 += (char)getRand();
            p2 += (char)getRand();
        }
        sw << p1 << " " << p2 << "\n";
        p1 = ""; p2 = "";
    }

    sw.close();
}

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
            input = "";
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
    std::cout << "\n===OUTPUT IS NOT SIMILAR===\n";
    outputNotSimilar();
    std::cout << "\n===TEST DIFFERENCE===\n";
    testDifference();
    std::cout << "\n===COLLISION TESTING===\n";
    testForCollisions();
}

void inputAnySize() {
    for (auto it = testingPaths.begin(); it != testingPaths.end(); ++it) {
        std::cout << "Hash of " << *it << ": " << vladahasher.getHashFromFile("input/" + *it) << "\n";
    }
}

void inputFixedSize() {
    for (auto it = testingPaths.begin(); it != testingPaths.end(); ++it) {
        std::cout << "Hash size (in bits) of " << *it << ": " << vladahasher.getHashFromFile("input/" + *it).length()*4 << "\n";
    }
}

void outputIsDeterministic() {
    for (auto it = testingPaths.begin(); it != testingPaths.end(); ++it) {
        std::string hash = vladahasher.getHashFromFile("input/" + *it);

        bool det = true;

        for (int i = 0; i < 100; ++i) {
            std::string new_hash = vladahasher.getHashFromFile("input/" + *it);
            if (hash != new_hash) {
                det = false;
                break;
            }
        }

        if (det) std::cout << "Hashed "  << *it << " 100 times and the hash was the same throughout\n";
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
    std::string hash1 = vladahasher.getHashFromFile("input/test3_1.txt");
    std::string hash2 = vladahasher.getHashFromFile("input/test3_2.txt");

    double diffPerc = 100*vladahasher.getHashDifference(hash1, hash2);

    std::cout << "Hash of test3_1.txt differs from hash of test3_2.txt by " << diffPerc << "%\n";
}

void testForCollisions() {
    std::ifstream sr("collisionTest.txt");

    int colls = 0;

    while (!sr.eof()) {
        std::string a, b;
        sr >> a >> b;

        if (vladahasher.getHash(a) == vladahasher.getHash(b) && a != "" && b != "") {
            colls++;
        }
    }

    std::cout << colls << " collisions found out of 100,000 pairs" << std::endl;

    sr.close();
}

void testDifference() {
    std::ifstream sr("diffTest.txt");

    double diffBitSum = 0, diffHexSum = 0;
    int diffCount = 0;

    while (!sr.eof()) {
        std::string a, b;
        sr >> a >> b;

        if (a != "" && b != "") {
            std::string h1 = vladahasher.getHash(a);
            std::string h2 = vladahasher.getHash(b);

            diffBitSum += vladahasher.getHashDifference(h1, h2);
            diffHexSum += vladahasher.getHashDifferenceHex(h1, h2);
            diffCount++;
        }
    }

    std::cout << "Average hash difference out of 100,000 pairs was:\n";
    std::cout << 100*(diffBitSum/diffCount) << "% in bit level\n";
    std::cout << 100*(diffHexSum/diffCount) << "% in hex level\n";

    sr.close();
}