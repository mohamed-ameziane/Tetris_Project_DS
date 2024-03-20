#include <iostream>
#include <unistd.h>

class Piece {
public:
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

class PiecesChainNode {
public:
    Piece piece;
    PiecesChainNode* next;

    PiecesChainNode(Piece piece);
};

class MainPiecesChain {
public:
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
    void removeFirstNode();
    void removeLastNode();
    int getSize();
};

class sameColorChainNode : public PiecesChainNode {
public:
    sameColorChainNode* next;
    sameColorChainNode* prev;

    sameColorChainNode(Piece piece);
};

class sameColorChain : public MainPiecesChain {
public:
    sameColorChainNode* head;
    sameColorChainNode* tail;

    sameColorChain();

    void moveLeft();
    int getSize();
};

class sameShapeChainNode : public PiecesChainNode {
public:
    sameShapeChainNode* next;
    sameShapeChainNode* prev;

    sameShapeChainNode(Piece piece);
};

class sameShapeChain : public MainPiecesChain {
public:
    sameShapeChainNode* head;
    sameShapeChainNode* tail;

    sameShapeChain();

    void moveLeft();
    int getSize();
};

class Game {
    public:
        void startgame(){}
};

Piece generateRandomPiece();
void menu();

// Piece class methods
Piece::Piece(Shape shape, Color color) : shape(shape), color(color) {}

// PiecesChainNode class methods
PiecesChainNode::PiecesChainNode(Piece piece) : piece(piece), next(nullptr) {}

// MainPiecesChain class methods
MainPiecesChain::MainPiecesChain() : head(nullptr), tail(nullptr) {
    srand(time(nullptr));
    for (int i = 0; i < 10; i++) {
        Piece randomPiece = generateRandomPiece();
        addPieceRight(randomPiece);
    }
    tail = head;
    while (tail->next != head) {
        tail = tail->next;
    }
    tail->next = head;
}

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

void MainPiecesChain::deleteThreeConsecutive() {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
        return;
    }

    if (head->piece.shape == head->next->piece.shape &&
        head->piece.shape == head->next->next->piece.shape ||
        head->piece.color == head->next->piece.color &&
        head->piece.color == head->next->next->piece.color) {

        removeFirstNode();
        removeFirstNode();
        removeFirstNode();
    }
}

void MainPiecesChain::removeFirstNode() {
    if (head == nullptr) {
        return;
    }

    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }
    PiecesChainNode* temp = head;
    head = head->next;
    tail->next = head;
    delete temp;
}

int MainPiecesChain::getSize() {
    if (head == nullptr)
        return 0;

    int count = 0;
    PiecesChainNode* current = head;

    do {
        count++;
        current = current->next;
    } while (current != head);

    return count;
}

void MainPiecesChain::deleteLastThreeConsecutive() {
    int size = getSize();
    if (size < 3) {
        return;
    }

    PiecesChainNode* thirdToLast = head;
    for (int i = 0; i < size - 3; ++i) {
        thirdToLast = thirdToLast->next;
    }

    if ((tail->piece.shape == thirdToLast->piece.shape &&
         tail->piece.shape == thirdToLast->next->piece.shape) ||
        (tail->piece.color == thirdToLast->piece.color &&
         tail->piece.color == thirdToLast->next->piece.color)) {

        removeLastNode();
        removeLastNode();
        removeLastNode();
    }
}

void MainPiecesChain::removeLastNode() {
    if (head == nullptr) {
        return;
    }

    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }

    PiecesChainNode* temp = head;
    while (temp->next != tail) {
        temp = temp->next;
    }
    delete tail;
    tail = temp;
    tail->next = head;
}

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

void MainPiecesChain::printPiece(Piece piece) {
    switch (piece.shape) {
        case Piece::SQUARE: std::cout << "S"; break;
        case Piece::DIAMOND: std::cout << "D"; break;
        case Piece::CIRCLE: std::cout << "C"; break;
        case Piece::TRIANGLE: std::cout << "T"; break;
    }

    std::cout << " | ";

    switch (piece.color) {
        case Piece::RED: std::cout << "\033[31mR\033[0m"; break; // Red
        case Piece::YELLOW: std::cout << "\033[33mY\033[0m"; break; // Yellow
        case Piece::BLUE: std::cout << "\033[34mB\033[0m"; break; // Blue
        case Piece::GREEN: std::cout << "\033[32mG\033[0m"; break; // Green
    }
}

// sameColorChainNode class methods
sameColorChainNode::sameColorChainNode(Piece piece) : PiecesChainNode(piece), next(nullptr), prev(nullptr) {}

// sameColorChain class methods
sameColorChain::sameColorChain() : head(nullptr), tail(nullptr) {}

void sameColorChain::moveLeft() {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    sameColorChainNode* temp = head;
    head = head->next;
    head->prev = nullptr;
    delete temp;
}

int sameColorChain::getSize() {
    if (head == nullptr) {
        return 0;
    }

    int count = 0;
    sameColorChainNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }

    return count;
}

// sameShapeChainNode class methods
sameShapeChainNode::sameShapeChainNode(Piece piece) : PiecesChainNode(piece), next(nullptr), prev(nullptr) {}

// sameShapeChain class methods
sameShapeChain::sameShapeChain() : head(nullptr), tail(nullptr) {}

void sameShapeChain::moveLeft() {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    sameShapeChainNode* temp = head;
    head = head->next;
    head->prev = nullptr;
    delete temp;
}

void menu() {
    int score = 0;
    bool gameStarted = false;
    MainPiecesChain chain;

    while (true) {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. View High Score" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;

        switch(choice) {
            case 1:
                if (!gameStarted) {
                    chain.startgame();
                    gameStarted = true;
                } else {
                    std::cout << "Game has already started." << std::endl;
                }
                break;
            case 2:
                std::cout << "High Score: " << score << std::endl;
                break;
            case 3:
                std::cout << "Exiting..." << std::endl;
                return;
            default:
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
        }
    }
}

int main() {
    menu();
    return 0;
}
