#pragma once

class Piece {
    public :
        enum Shape { SQUARE, DIAMOND, CIRCLE, TRIANGLE };
        enum Color { RED, YELLOW, BLUE, GREEN };

        Shape shape;
        Color color;


        Piece* nextSameShape;
        Piece* nextSameColor;

        Piece* prevSameShape;
        Piece* prevSameColor;


        Piece(Shape shape, Color color);
};

class PiecesChainNode{
    public :
        Piece piece;
        PiecesChainNode* next;

        PiecesChainNode(Piece piece);
};

class MainPiecesChain{
    public :
        PiecesChainNode* head;
        PiecesChainNode* tail;

        MainPiecesChain();

        void addPieceLeft(Piece piece);
        void addPieceRight(Piece piece);
        void printChain();
        void printPiece(Piece); 
        void removePieces();
};

class Board {
    public :
        int width;
        int height;
    
        Board(int width, int height);
};

Piece generateRandomPiece();
