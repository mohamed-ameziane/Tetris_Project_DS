#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>


using namespace std;

static int score = 0;

/* CLASSES */
class Piece {
public:
    enum Shape { SQUARE, DIAMOND, CIRCLE, TRIANGLE };
    enum Color { RED, YELLOW, BLUE, GREEN };

    Shape shape;
    Color color;

    Piece(Shape shape, Color color);
    Piece();
    void printPiece();
};

class Node {
public:
    Piece piece;
    Node* next;

    Node(Piece piece) : piece(piece), next(nullptr) {}
};

class MainList {
public:
    Node* head;
    Node* tail;

    MainList();

    void addPieceLeft(Piece piece);
    void addPieceRight(Piece piece);
    void printChain();
    void deleteThreeConsecutive();
    void deleteLastThreeConsecutive();
    int deleteLast();
    int deleteFirst();
    void removeFirstNode();
    void removeLastNode();
    int getSize();
};

class Game {
public:
    static const int MAX_SCORE = 10;
    int scoreHistory[100];
    int scoreHistorySize = 0;

    MainList chain;
    Piece upcomingPiece;
    bool pieceAdded = false;

    void startGame(sf::RenderWindow& window);
    void addScore(int score);
    void printScoreHistory();
    void loadScoreHistory();
    void saveScoreHistory();
    void sortScoreHistory();

    void drawPieces(sf::RenderWindow& window);
    void drawUpcomingPiece(sf::RenderWindow& window);
};

class Menu {
private:
    sf::Text titleText;
    sf::Text startGameText;
    sf::Text viewScoreText;
    sf::Text exitText;

public:
    Menu(sf::Font& font) {
        titleText.setFont(font);
        titleText.setCharacterSize(40);
        titleText.setFillColor(sf::Color::White);
        titleText.setString("Tetris Game");
        titleText.setPosition(360.0f, 20.0f);

        startGameText.setFont(font);
        startGameText.setCharacterSize(24);
        startGameText.setFillColor(sf::Color::White);
        startGameText.setString("Start Game");
        startGameText.setPosition(400.0f, 150.0f);

        viewScoreText.setFont(font);
        viewScoreText.setCharacterSize(24);
        viewScoreText.setFillColor(sf::Color::White);
        viewScoreText.setString("View Score History");
        viewScoreText.setPosition(370.0f, 200.0f);

        exitText.setFont(font);
        exitText.setCharacterSize(24);
        exitText.setFillColor(sf::Color::White);
        exitText.setString("Exit");
        exitText.setPosition(440.0f, 250.0f);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(titleText);
        window.draw(startGameText);
        window.draw(viewScoreText);
        window.draw(exitText);
    }

    int checkClick(sf::RenderWindow& window) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (startGameText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return 1;
                }

                if (viewScoreText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return 2;
                }

                if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    return 3;
                }
            }
        }
        return 0;
    }
};



/* MAIN FUNCTION */
int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 500), "TETRIS");
    sf::Font font;
    if (!font.loadFromFile("Pacifico.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return 1;
    }

    Menu menu(font);

    while (window.isOpen()) {
        window.clear(sf::Color::Black);

        menu.draw(window);
        window.display();

        int choice = menu.checkClick(window);

        if (choice == 1) {
            Game game;
            game.startGame(window);
        } else if (choice == 2) {
            Game game;
            game.loadScoreHistory();
            game.printScoreHistory();
        } else if (choice == 3) {
            window.close();
        }
    }

    return 0;
}



/* FUNCTIONS */
Piece::Piece(Shape shape, Color color) : shape(shape), color(color) {};
Piece::Piece(){}

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


MainList::MainList() : head(nullptr), tail(nullptr) {
    srand(time(nullptr));
        
    for (int i = 0; i < 7; i++) {
        Piece randomPiece = generateRandomPiece();
        addPieceRight(randomPiece);
    }
        
    tail = head;
        
    while (tail->next != head) {
        tail = tail->next;
    }
        
    tail->next = head;
}

void MainList::addPieceLeft(Piece piece) {
    Node* newNode = new Node(piece);
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

void MainList::addPieceRight(Piece piece) {
    Node* newNode = new Node(piece);
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

void MainList::deleteThreeConsecutive() {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
        return;
    }

    if (head->piece.shape == head->next->piece.shape &&
        head->piece.shape == head->next->next->piece.shape &&
        head->piece.color == head->next->piece.color &&
        head->piece.color == head->next->next->piece.color) {

        for (int i = 0; i < 3; i++)
        {
            removeFirstNode();
        }

        score += 20;
    }

    if (head->piece.shape == head->next->piece.shape &&
        head->piece.shape == head->next->next->piece.shape ||
        head->piece.color == head->next->piece.color &&
        head->piece.color == head->next->next->piece.color) {

        for (int i = 0; i < 3; i++)
        {
            removeFirstNode();
        }

        score += 10;
    }
}

void MainList::deleteLastThreeConsecutive() {
    int size = getSize();
    if (size < 3) {
        return;
    }

    Node* thirdToLast = head;
    for (int i = 0; i < size - 3; ++i) {
        thirdToLast = thirdToLast->next;
    }

    if ((tail->piece.shape == thirdToLast->piece.shape &&
         tail->piece.shape == thirdToLast->next->piece.shape) &&
        (tail->piece.color == thirdToLast->piece.color &&
         tail->piece.color == thirdToLast->next->piece.color)) {

        for (int i = 0; i < 3; i++)
        {
            removeLastNode();
        }

        score += 20;
    }

    else if ((tail->piece.shape == thirdToLast->piece.shape &&
         tail->piece.shape == thirdToLast->next->piece.shape) ||
        (tail->piece.color == thirdToLast->piece.color &&
         tail->piece.color == thirdToLast->next->piece.color)) {

        for (int i = 0; i < 3; i++)
        {
            removeLastNode();
        }
        

        score += 10;
    }
}

void MainList::removeFirstNode() {
    if (head == nullptr) {
        return;
    }

    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }
    Node* temp = head;
    head = head->next;
    tail->next = head;
    delete temp;
}



void MainList::removeLastNode() {
    if (head == nullptr) {
        return;
    }

    if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return;
    }

    Node* temp = head;
    while (temp->next != tail) {
        temp = temp->next;
    }
    delete tail;
    tail = temp;
    tail->next = head;
}

int MainList::getSize() {
    if (head == nullptr)
        return 0;

    int count = 0;
    Node* current = head;

    do {
        count++;
        current = current->next;
    } while (current != head);

    return count;
}


void Game::drawPieces(sf::RenderWindow& window) {
    if (chain.head == nullptr) {
        cout << "The circular linked list is empty." << endl;
        return;
    }
    Node* current = chain.head;
    float startX = 100.0f;
    float startY = 300.0f;
    float spacing = 50.0f;
    float shapeSize = 40.0f;

    do {
        sf::Shape* shape;

        switch (current->piece.shape) {
            case Piece::SQUARE:
                shape = new sf::RectangleShape(sf::Vector2f(shapeSize, shapeSize));
                break;
            case Piece::DIAMOND:
                shape = new sf::CircleShape(shapeSize / 2.0f, 4);
                break;
            case Piece::CIRCLE:
                shape = new sf::CircleShape(shapeSize / 2.0f);
                break;
            case Piece::TRIANGLE:
                shape = new sf::CircleShape(shapeSize / 2.0f, 3);
                break;
        }

        switch (current->piece.color) {
            case Piece::RED: shape->setFillColor(sf::Color::Red); break;
            case Piece::YELLOW: shape->setFillColor(sf::Color::Yellow); break;
            case Piece::BLUE: shape->setFillColor(sf::Color::Blue); break;
            case Piece::GREEN: shape->setFillColor(sf::Color::Green); break;
        }

        shape->setPosition(startX, startY);
        window.draw(*shape);
        delete shape;

        startX += spacing;
        current = current->next;
    } while (current != chain.head);
}

void Game::drawUpcomingPiece(sf::RenderWindow& window) {
    sf::Shape* shape;

    switch (upcomingPiece.shape) {
        case Piece::SQUARE:
            shape = new sf::RectangleShape(sf::Vector2f(40, 40));
            break;
        case Piece::DIAMOND:
            shape = new sf::CircleShape(20, 4);
            break;
        case Piece::CIRCLE:
            shape = new sf::CircleShape(20);
            break;
        case Piece::TRIANGLE:
            shape = new sf::CircleShape(20, 3);
            break;
    }

    switch (upcomingPiece.color) {
        case Piece::RED: shape->setFillColor(sf::Color::Red); break;
        case Piece::YELLOW: shape->setFillColor(sf::Color::Yellow); break;
        case Piece::BLUE: shape->setFillColor(sf::Color::Blue); break;
        case Piece::GREEN: shape->setFillColor(sf::Color::Green); break;
    }

    shape->setPosition(50.0f, 50.0f);
    window.draw(*shape);
    delete shape;
}

void Game::addScore(int score) {
    if (scoreHistorySize < MAX_SCORE) {
        scoreHistory[scoreHistorySize++] = score;
    } else {
        cout << "Score history is full" << endl;
    }
}

void Game::saveScoreHistory() {
    std::ofstream file("score_history.txt", std::ios::app);  
    if (!file.is_open()) {
        std::cout << "Error opening file" << std::endl;
    } else {
        file << scoreHistory[scoreHistorySize - 1] << std::endl;
        
        file.close();
        std::cout << "Score saved successfully." << std::endl;
    }
}


void Game::loadScoreHistory() {
    std::ifstream inFile("score_history.txt");  
    if (!inFile.is_open()) {
        std::cout << "Unable to open score history file." << std::endl;
    } else {
        scoreHistorySize = 0;
        int score;
        while (inFile >> score) {
            scoreHistory[scoreHistorySize++] = score;
        }
        sortScoreHistory();
        inFile.close();
        std::cout << "Score history loaded successfully." << std::endl;
    }
}


void Game::printScoreHistory() {
    cout << "--------Score History--------" << endl;
    for (int i = 0; i < scoreHistorySize; i++) {
        cout << i + 1 << ". " << scoreHistory[i] << endl;
    }
    cout << "----------------------------" << endl;
}

void Game::sortScoreHistory() {
    for (int i = 0; i < scoreHistorySize - 1; i++) {
        for (int j = 0; j < scoreHistorySize - i - 1; j++) {
            if (scoreHistory[j] < scoreHistory[j + 1]) {
                int temp = scoreHistory[j];
                scoreHistory[j] = scoreHistory[j + 1];
                scoreHistory[j + 1] = temp;
            }
        }
    }
}

void Game::startGame(sf::RenderWindow& window) {
    sf::Font font1, font2, font3;

    if (!font1.loadFromFile("Pacifico.ttf")) {
        std::cerr << "Error loading font1!" << std::endl;
        return;
    }

    if (!font2.loadFromFile("FFF_Tusj.ttf")) {
        std::cerr << "Error loading font2!" << std::endl;
        return;
    }
    if (!font3.loadFromFile("SEASRN__.ttf")) {
        std::cerr << "Error loading font2!" << std::endl;
        return;
    }

    sf::Text scoreText;
    scoreText.setFont(font2);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setPosition(50, 100);

    sf::Text titleText("Tetris Game", font1, 40);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(360.0f, 20.0f);


    Piece randomPiece = generateRandomPiece();
    upcomingPiece = randomPiece;

    bool pieceAdded = false; 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!pieceAdded && event.type == sf::Event::KeyPressed && 
                (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)) {
                
                window.clear(sf::Color::Black);
                window.draw(titleText);
                drawUpcomingPiece(window);
                window.display();

                if (event.key.code == sf::Keyboard::A) {
                    chain.addPieceLeft(upcomingPiece);
                } else {
                    chain.addPieceRight(upcomingPiece);
                }

                randomPiece = generateRandomPiece();
                upcomingPiece = randomPiece;

                pieceAdded = true; 
            }
        }

        chain.deleteThreeConsecutive(); 
        chain.deleteLastThreeConsecutive();

        std::stringstream scoreStr;
        scoreStr << "Score: " << score;
        scoreText.setString(scoreStr.str());

        window.clear(sf::Color::Black);

        window.draw(titleText);

        window.draw(scoreText);

        drawUpcomingPiece(window);
        drawPieces(window);

        if (chain.getSize() == 17) {
            window.clear();
            sf::Text gameOverText("GAME OVER", font3, 60);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(300.0f, 200.0f);
            window.draw(gameOverText);

            sf::Text scoreText("Score: " + std::to_string(score), font3, 40);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(370.0f, 300.0f);
            window.draw(scoreText);

            this->addScore(score);
            this->saveScoreHistory();


            window.display();
            sf::sleep(sf::seconds(5));
            window.close();
            return;
        } else if (chain.getSize() == 0) {
            window.clear();
            sf::Text winText("YOU WIN", font3, 60);
            winText.setFillColor(sf::Color::Green);
            winText.setPosition(320.0f, 300.0f);
            window.draw(winText);


            sf::Text scoreText("Score: " + std::to_string(score), font3, 40);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(370.0f, 250.0f);
            window.draw(scoreText);

            this->addScore(score);
            this->saveScoreHistory();

            window.display();
            sf::sleep(sf::seconds(5)); 
            window.close();
            return;
        }

        window.display();
        pieceAdded = false;
    }
}