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
        void startgame(); 
        void deleteThreeConsecutive();
        void deleteLastThreeConsecutive();
        //void deleteConsecutive();
        void removeFirstNode();
        void removeLastNode();
        int getSize();
};

class sameColorChainNode : public PiecesChainNode{
    public :
        sameColorChainNode* next;
        sameColorChainNode* prev;

        sameColorChainNode(Piece piece);
};

class sameColorChain : public MainPiecesChain{
    public :
        sameColorChainNode* head;
        sameColorChainNode* tail;

        sameColorChain();

        void moveLeft();
        int getSize();
};

class sameShapeChainNode : public PiecesChainNode{
    public :
        sameShapeChainNode* next;
        sameShapeChainNode* prev;

        sameShapeChainNode(Piece piece);
};

class sameShapeChain : public MainPiecesChain{
    public :
        sameShapeChainNode* head;
        sameShapeChainNode* tail;

        sameShapeChain();

        void moveLeft();
        int getSize();
};

class Game {
    public :
        

};

Piece generateRandomPiece();
void menu();