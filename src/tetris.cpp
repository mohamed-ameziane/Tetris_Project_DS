#include "tetris.h"
#include <iostream>
#include <ctime>
using namespace std;

// Board class methods
Board::Board(int width, int height) : width(width), height(height) {}

// Piece class methods
Piece::Piece(Shape shape, Color color) : shape(shape), color(color) {};

// PiecesChainNode class methods
PiecesChainNode::PiecesChainNode(Piece piece) : piece(piece), next(nullptr) {};

// MainPiecesChain class methods
// MainPiecesChain::MainPiecesChain() : head(nullptr), tail(nullptr){
//     // Generate random pieces and add them to the chain
//     for (int i = 0; i < 5; i++) {
//         Piece randomPiece = generateRandomPiece();
//         addPieceLeft(randomPiece);
//     }
// }

// made by salah
MainPiecesChain::MainPiecesChain() : head(nullptr), tail(nullptr) {
    srand(time(nullptr));
    for (int i = 0; i < 5; i++) {
        Piece randomPiece = generateRandomPiece();
        addPieceRight(randomPiece);
    }
    tail = head;
    while (tail->next != head) {
        tail = tail->next;
    }
}



// void MainPiecesChain::addPieceLeft(Piece piece) {
//     PiecesChainNode* newNode = new PiecesChainNode(piece);
//     newNode->next = head;
//     head = newNode;
//     tail->next = newNode;
// }

void MainPiecesChain::addPieceLeft(Piece piece) {
    PiecesChainNode* newNode = new PiecesChainNode(piece);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = head;
        tail->next = newNode;
        head = newNode; 
    }
}

void MainPiecesChain::addPieceRight(Piece piece) {
    PiecesChainNode* newNode = new PiecesChainNode(piece);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = head;
        tail->next = newNode; 
        tail = newNode; 
    }
}

Piece generateRandomPiece() {
    int randomShapeIndex = rand() % 4;
    int randomColorIndex = rand() % 4;
    
    Piece::Shape randomShape;
    Piece::Color randomColor;

    switch (randomShapeIndex) {
        case 0: randomShape = Piece::SQUARE; break;
        case 1: randomShape = Piece::DIAMOND; break;
        case 2: randomShape = Piece::CIRCLE; break;
        case 3: randomShape = Piece::TRIANGLE; break;
    }

    switch (randomColorIndex) {
        case 0: randomColor = Piece::RED; break;
        case 1: randomColor = Piece::YELLOW; break;
        case 2: randomColor = Piece::BLUE; break;
        case 3: randomColor = Piece::GREEN; break;
    }

    return Piece(randomShape, randomColor);
}

// Implement the printChain() method
void MainPiecesChain::printChain() {
    if (head == nullptr) {
        std::cout << "The circular linked list is empty." << std::endl;
        return;
    }
    PiecesChainNode* current = head;
    do {
        char shapeSymbol;
        switch (current->piece.shape) {
            case Piece::SQUARE: shapeSymbol = 'S'; break;
            case Piece::DIAMOND: shapeSymbol = 'D'; break;
            case Piece::CIRCLE: shapeSymbol = 'C'; break;
            case Piece::TRIANGLE: shapeSymbol = 'T'; break;
        }
        
        std::string colorCode;
        switch (current->piece.color) {
            case Piece::RED: colorCode = "\033[31m"; break; // Red
            case Piece::YELLOW: colorCode = "\033[33m"; break; // Yellow
            case Piece::BLUE: colorCode = "\033[34m"; break; // Blue
            case Piece::GREEN: colorCode = "\033[32m"; break; // Green
        }

        std::cout << colorCode << shapeSymbol << "\033[0m" << " | "; // Reset color

        current = current->next;
    } while (current != head);

    std::cout << std::endl;
}
