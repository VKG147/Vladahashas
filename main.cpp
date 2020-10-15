#include <iostream>
#include "Vladahasher.h"

int main(int argc, char** argv) {
    Vladahasher vladahasher;

    std::string h1 = vladahasher.getHash("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    std::string h2 = vladahasher.getHash("baaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    std::cout << h1 << "\n" << h2 << std::endl;

    std::cout << vladahasher.getHashPercentageDifference(h1, h2) << std::endl;

    if (argc >= 2) {
        std::cout << vladahasher.getHashFromFile(argv[1]);
    }
    else {
        std::cout << "Welcome to Vladahashas. Enter text surrounded by \" to get a hash. Type \"quit\" without quotes to quit\n";
        while (true) {
            std::string input;
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
    return 0;
}