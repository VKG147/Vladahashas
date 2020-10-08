#include <iostream>
#include "Vladahasher.h"

int main(int argc, char** argv) {
    Vladahasher vladahasher;

    if (argc >= 2) {
        std::cout << vladahasher.getHashFromFile(argv[1]);
    }
    else {
        while (true) {
            std::string input;
            std::cout << "Please type a string: ";
            std::cin >> input;
            std::cout << "Hash: " << vladahasher.getHash(input) << "\n\n";
        }
    }
    return 0;
}