#include <iostream>
#include "Vladahasher.h"

int main(int argc, char** argv) {
    Vladahasher vladahasher;
    std::cout << vladahasher.getHash("As einu miegotj");
    return 0;
}