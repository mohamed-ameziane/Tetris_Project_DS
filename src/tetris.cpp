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
MainPiecesChain::MainPiecesChain() : head(nullptr), tail(nullptr){
    // Generate random pieces and add them to the chain
    for (int i = 0; i < 5; i++) {
        Piece randomPiece = generateRandomPiece();
        addPieceLeft(randomPiece);
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
    srand(time(nullptr));
    int randomShapeIndex = rand() % 4;
    int randomColorIndex = rand() % 4;

    Piece::Shape randomShape;
    Piece::Color randomColor;

    switch (randomShapeIndex) {
        case 0: randomShape = Piece::SQUARE; break;
        case 1: randomShape = Piece::RECTANGLE; break;
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
    PiecesChainNode* current = head;
    while (current != nullptr) {
        cout << "Shape: ";
        switch (current->piece.shape) {
            case Piece::SQUARE: cout << "Square"; break;
            case Piece::RECTANGLE: cout << "Rectangle"; break;
            case Piece::CIRCLE: cout << "Circle"; break;
            case Piece::TRIANGLE: cout << "Triangle"; break;
        }
        cout << ", Color: ";
        switch (current->piece.color) {
            case Piece::RED: cout << "Red"; break;
            case Piece::YELLOW: cout << "Yellow"; break;
            case Piece::BLUE: cout << "Blue"; break;
            case Piece::GREEN: cout << "Green"; break;
        }
        cout << endl;
        current = current->next;
    }
}