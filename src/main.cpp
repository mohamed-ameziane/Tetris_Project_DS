#include <iostream>
#include <unistd.h> // For usleep function
#include "tetris.h" // Include necessary headers

using namespace std;

int main() {
    MainPiecesChain chain;
    Board board(10, 20); // Assuming 10x20 board size

    // Loop for the game
    while (true) {
        Piece randomPiece = generateRandomPiece();
        chain.printChain(); // Print the ready pieces
        cout << "Next piece to add: ";
        chain.printPiece(randomPiece); // Print the next piece to add
        cout << " (Press 'A' to add to the left, 'D' to add to the right)" << endl;
        
        char input;
        cin >> input;
        if (input == 'A' || input == 'a') {
            chain.addPieceLeft(randomPiece);
        } else if (input == 'D' || input == 'd') {
            chain.addPieceRight(randomPiece);
        }
        
        // Clear the screen
        cout << "\033[2J\033[1;1H"; // ANSI escape codes to clear the screen
        
        // Print the updated linked list
        cout << "Main Chain of Pieces:" << endl;
        chain.printChain();
        
        // Simulate delay for demonstration
        usleep(1000000); // 1 second delay
    }

    return 0;
}
