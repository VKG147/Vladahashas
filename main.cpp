#include <iostream>
#include <chrono>
#include "Vladahasher.h"

Vladahasher vladahasher;

void runAnalysis();
void inputAnySize();
void inputFixedSize();
void outputIsDeterministic();
void hashFunctionEffectiveness();
void outputIndiscernible();
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
    std::cout << "\nOUTPUT FIXED SIZE===\n";
    inputFixedSize();
    std::cout << "\n===OUTPUT IS DETERMINISTIC===\n";
    outputIsDeterministic();
    std::cout << "\n===HASH FUNCTION EFFECTIVENESS===\n";
    hashFunctionEffectiveness();
    std::cout << "\n===OUTPUT INDISCERNIBLE===\n";
    outputIndiscernible();
//    std::cout << "\n===COLLISION TESTING===\n";
//    testForCollisions();
}

void inputAnySize() {
    std::string str1('a', 1);
    std::string str2('a', 10);
    std::string str3('a', 1000);
    std::string str4('a', 10000);
    std::string str5('a', 100000);

    std::cout << "Hash of str1: " << vladahasher.getHash(str1) << "\n";
    std::cout << "Hash of str2: " << vladahasher.getHash(str2) << "\n";
    std::cout << "Hash of str3: " << vladahasher.getHash(str3) << "\n";
    std::cout << "Hash of str4: " << vladahasher.getHash(str4) << "\n";
    std::cout << "Hash of str5: " << vladahasher.getHash(str5) << "\n";
}

void inputFixedSize() {
    std::string str1 = "sndasod asdaoida diaydbasidsadpnoa";
    std::string str2 = "dasjknasnaksd dasad!!!@#@";

    std::cout << "Hash length of str1: " << vladahasher.getHash(str1).length() << "\n";
    std::cout << "Hash length of str2: " << vladahasher.getHash(str2).length() << "\n";
}

void outputIsDeterministic() {
    std::string str = "aaaaaaaaaa string";
    std::string hash = vladahasher.getHash(str);
    bool det = true;

    for (int i = 0; i < 5; ++i) {
        std::string new_hash = vladahasher.getHash(str);
        if (hash != new_hash) {
            det = false;
            break;
        }
    }

    if (det) std::cout << "Hash function is (most likely) deterministic\n";
    else std::cout << "Hash function is definitely not deterministic\n";
}

void hashFunctionEffectiveness() {
    const unsigned int totalHashes = 100;

    auto begin = std::chrono::steady_clock::now();

    std::string str = "a";
    for (int i = 0; i < totalHashes; ++i) {
        vladahasher.getHash(str);
        str += "a";
    }
    std::string hash = vladahasher.getHash(str);

    auto end = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cout << "Average time of single hash: " << static_cast<double>(diff)/totalHashes << "ms\n";
}

void outputIndiscernible() {
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