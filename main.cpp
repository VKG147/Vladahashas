#include <iostream>
#include "Vladahasher.h"

int main(int argc, char** argv) {
    Vladahasher vladahasher;

    if (argc >= 1) {
        std::cout << vladahasher.getHashFromFile(argv[0]);
    }
    else {
        while (true) {
            std::string input;
            std::cout << "Please type a string: ";
            std::cin >> input;
            std::cout << "\nHash: " << vladahasher.getHash(input) << "\n";
        }
    }
    return 0;
}