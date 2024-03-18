#pragma once

class Piece {
    public :
        enum Shape { SQUARE, DIAMOND, CIRCLE, TRIANGLE };
        enum Color { RED, YELLOW, BLUE, GREEN };

        Shape shape;
        Color color;



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
        void startgame(); 
        void removePieces();
};

class ShiftingPiecesChainNode : public PiecesChainNode {
    public :
        Piece* nextSameShape;
        Piece* nextSameColor;

        Piece* prevSameShape;
        Piece* prevSameColor;

        ShiftingPiecesChainNode(Piece piece);
};

class ShiftingPiecesChain : public MainPiecesChain {
    public :
        ShiftingPiecesChainNode* head;
        ShiftingPiecesChainNode* tail;

        ShiftingPiecesChain();
        void moveLeft();
};

Piece generateRandomPiece();
