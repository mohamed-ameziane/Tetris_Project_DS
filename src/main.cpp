#include "tetris.h"
#include <iostream>

int main() {
    // Create a sample board
    Board board(10, 20);

    // Create a main chain of pieces
    MainPiecesChain mainChain;

    // Print the main chain of pieces
    std::cout << "Main Chain of Pieces:" << std::endl;
    mainChain.printChain();

    return 0;
}
