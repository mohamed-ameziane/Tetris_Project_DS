#include "tetris.h"
#include <iostream>
#include <ctime>

int generer();

int main() {
    MainPiecesChain chain;
    // chain.addPieceRight(Piece(Piece::SQUARE, Piece::RED));
    // chain.addPieceRight(Piece(Piece::DIAMOND, Piece::YELLOW));
    // chain.addPieceRight(Piece(Piece::CIRCLE, Piece::BLUE));
    // chain.addPieceRight(Piece(Piece::TRIANGLE, Piece::GREEN));
    // chain.addPieceRight(Piece(Piece::SQUARE, Piece::RED));

    chain.addPieceRight(Piece(Piece::DIAMOND, Piece::RED));

    chain.printChain();

    

    return 0;
}
