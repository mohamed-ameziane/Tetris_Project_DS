#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

using namespace std;

class Piece {
public:
    enum Shape { SQUARE, DIAMOND, CIRCLE, TRIANGLE };
    enum Color { RED, YELLOW, BLUE, GREEN };
    Shape shape;
    Color color;
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
    Piece upcomingPiece;

    MainPiecesChain();
    void addPieceLeft(Piece piece);
    void addPieceRight(Piece piece);
    void startgame(sf::RenderWindow& window);
    void deleteThreeConsecutive();
    void deleteLastThreeConsecutive();
    void removeFirstNode();
    void removeLastNode();
    int getSize();
    void drawPieces(sf::RenderWindow& window);
    void drawUpcomingPiece(sf::RenderWindow& window);
};

Piece generateRandomPiece();

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "SFML Pieces Game");
    MainPiecesChain chain;
    chain.startgame(window);
    return 0;
}

Piece::Piece(Shape shape, Color color) : shape(shape), color(color) {}
PiecesChainNode::PiecesChainNode(Piece piece) : piece(piece), next(nullptr) {}

MainPiecesChain::MainPiecesChain() : head(nullptr), tail(nullptr), upcomingPiece(Piece(Piece::SQUARE, Piece::RED)) {
    srand(time(nullptr));
    for (int i = 0; i < 5; i++) {
        Piece randomPiece = generateRandomPiece();
        addPieceRight(randomPiece);
    }
    tail = head;
    while (tail->next != head) {
        tail = tail->next;
    }
    tail->next = head;

    // Initialize upcomingPiece with a random piece
    Piece::Shape randomShape = static_cast<Piece::Shape>(rand() % 4);
    Piece::Color randomColor = static_cast<Piece::Color>(rand() % 4);
    upcomingPiece = Piece(randomShape, randomColor);
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

void MainPiecesChain::startgame(sf::RenderWindow& window) {
    int score = 0;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error loading font file." << endl;
        return;
    }
    sf::Text titleText("TITRES DZEB", font, 30);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(20.0f, 20.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                Piece randomPiece = generateRandomPiece();
                upcomingPiece = randomPiece;
                if (event.key.code == sf::Keyboard::A) {
                    addPieceLeft(randomPiece);
                } else if (event.key.code == sf::Keyboard::D) {
                    addPieceRight(randomPiece);
                }
            }
        }

        deleteThreeConsecutive();
        deleteLastThreeConsecutive();

        window.clear(sf::Color::Black);

        // Draw title text
        window.draw(titleText);

        drawUpcomingPiece(window);

        drawPieces(window);

        if (getSize() == 12) {
            sf::Text gameOverText("GAME OVER", font, 50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(300.0f, 250.0f);
            window.draw(gameOverText);
            window.display();
            sf::sleep(sf::seconds(2)); // Display "GAME OVER" for 2 seconds before clearing the window
            window.close();
            return;
        } else if (getSize() == 0) {
            sf::Text winText("YOU WIN", font, 50);
            winText.setFillColor(sf::Color::Green);
            winText.setPosition(320.0f, 250.0f);
            window.draw(winText);
            window.display();
            sf::sleep(sf::seconds(2)); // Display "YOU WIN" for 2 seconds before clearing the window
            window.close();
            return;
        }

        window.display();

        // Implement your scoring and game end conditions here
    }
}



void MainPiecesChain::drawUpcomingPiece(sf::RenderWindow& window) {
    sf::CircleShape shape;
    switch (upcomingPiece.shape) {
        case Piece::SQUARE: shape = sf::CircleShape(25.0f, 4); break;
        case Piece::DIAMOND: shape = sf::CircleShape(25.0f, 4); break;
        case Piece::CIRCLE: shape = sf::CircleShape(25.0f); break;
        case Piece::TRIANGLE: shape = sf::CircleShape(25.0f, 3); break;
    }
    switch (upcomingPiece.color) {
        case Piece::RED: shape.setFillColor(sf::Color::Red); break;
        case Piece::YELLOW: shape.setFillColor(sf::Color::Yellow); break;
        case Piece::BLUE: shape.setFillColor(sf::Color::Blue); break;
        case Piece::GREEN: shape.setFillColor(sf::Color::Green); break;
    }
    shape.setPosition(50.0f, 50.0f);
    window.draw(shape);
}

void MainPiecesChain::drawPieces(sf::RenderWindow& window) {
    if (head == nullptr) {
        cout << "The circular linked list is empty." << endl;
        return;
    }
    PiecesChainNode* current = head;
    float startX = 100.0f;
    float startY = 300.0f;
    float spacing = 50.0f;
    do {
        sf::CircleShape shape;
        switch (current->piece.shape) {
            case Piece::SQUARE: shape = sf::CircleShape(25.0f, 4); break;
            case Piece::DIAMOND: shape = sf::CircleShape(25.0f, 4); break;
            case Piece::CIRCLE: shape = sf::CircleShape(25.0f); break;
            case Piece::TRIANGLE: shape = sf::CircleShape(25.0f, 3); break;
        }
        switch (current->piece.color) {
            case Piece::RED: shape.setFillColor(sf::Color::Red); break;
            case Piece::YELLOW: shape.setFillColor(sf::Color::Yellow); break;
            case Piece::BLUE: shape.setFillColor(sf::Color::Blue); break;
            case Piece::GREEN: shape.setFillColor(sf::Color::Green); break;
        }
        shape.setPosition(startX, startY);
        startX += spacing;
        window.draw(shape);
        current = current->next;
    } while (current != head);
}
