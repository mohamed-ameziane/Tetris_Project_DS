#include <iostream>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <ncurses.h> // Alternative to conio.h for getch() function
using namespace std;

static int score = 0;

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

    void startGame();
    void addScore(int score);
    void printScoreHistory();
    void loadScoreHistory();
    void saveScoreHistory();
    void sortScoreHistory();

};

Piece generateRandomPiece();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    Game game;
    while (true) {
        system("clear");
        cout << "\033[38;5;50m-------- \033[3m\033[1mMenu\e[0m \033[38;5;50m---------\033[0m\n" << endl;
        cout << "1. Start Game" << endl;
        cout << "2. View Top Scores" << endl;
        cout << "3. Exit" << endl;
        cout << "\n\033[38;5;50m-----------------------\033[0m" << endl;
        cout << "Enter your choice: \033[32m";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                game.startGame();
                break;
            case 2:
                game.loadScoreHistory();
                game.printScoreHistory();
                break;
            case 3:
                cout << "\033[38;5;247mExiting..." << endl;
                return;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
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

void Game::startGame() {
    system("clear");
    MainList chain;

    cout << "\033[97m\033[1mMain Chain of Pieces:\033[0m\n" << endl;
    cout << "| ";
    chain.printChain();
    cout << "\n\033[38;5;115mScore: \033[0m" << score << endl;

    while (true) {
        Piece randomPiece = generateRandomPiece();
        cout << "\nNext piece: { ";
        randomPiece.printPiece();
        cout << " }" << endl;

        cout << "\nEnter your choice: \033[32m";
        char input;
        cin >> input;
        if (input == 'A' || input == 'a') {
            chain.addPieceLeft(randomPiece);
        } else if (input == 'D' || input == 'd') {
            chain.addPieceRight(randomPiece);
        } else if (input == 'X' || input == 'x') {
            cout << "Exiting..." << endl;
            this->addScore(score);
            this->saveScoreHistory();
            break;
        }

        chain.deleteThreeConsecutive();
        chain.deleteLastThreeConsecutive();

        cout << "\033[2J\033[1;1H"; 
        cout << "\033[97m\033[1mMain Chain of Pieces:\033[0m\n" << endl;
        cout << "| ";
        chain.printChain();

        // score += 1; //Fix the Score! it only increases by 1 for each piece added?!?

        cout << "\n\033[38;5;115mScore: \033[0m" << score << endl;

        if (chain.getSize() == 20) {
            cout << "\n\033[91mGame Over!\033[0m" << endl;
            cout << "\nYour Score: " << score << endl;
            this->addScore(score);
            this->saveScoreHistory();
            cout << "\n\n\033[38;5;245mPress x and Enter to continue..." << endl;
            char input;
            cin >> input;
            break;
        } else if (chain.getSize() == 0) {
            cout << "You Win!" << endl;
            cout << "Your Score: " << score << endl;
            this->addScore(score);
            this->saveScoreHistory();
            cout << "\n\n\033[38;5;245mPress x and Enter to continue..." << endl;
            char input;
            cin >> input;
            break;
        }
        usleep(9000);
    }
}

void Game::addScore(int score) {
    if (scoreHistorySize < MAX_SCORE) {
        scoreHistory[scoreHistorySize++] = score;
    } else {
        cout << "Score history is full" << endl;
    }
}

void Game::saveScoreHistory() {
    ofstream file("score_history.txt", ios::app); // Open file in append mode
    if (!file.is_open()) {
        cout << "Error opening file" << endl;
    } else {
        for (int i = 0; i < scoreHistorySize; i++) {
            file << scoreHistory[i] << endl;
        }
        file.close();
        cout << "Score saved successfully." << endl;
    }
}

void Game::loadScoreHistory() {
    ifstream inFile("score_history.txt");
    if (!inFile.is_open()) {
        cout << "Unable to open score history file." << endl;
    } else {
        scoreHistorySize = 0; // Reset score history size
        int score;
        while (inFile >> score) {
            scoreHistory[scoreHistorySize++] = score;
        }
        sortScoreHistory();
        inFile.close();
        cout << "Score history loaded successfully." << endl;
    }
}

void Game::printScoreHistory() {
    initscr();      // Initialize ncurses
    clear();        // Clear the screen

    printw("--------Score History--------\n");
    for (int i = 0; i < 10; i++) {
        printw("%d. %d\n", i + 1, scoreHistory[i]);
    }
    
    printw("\n\nPress any key to continue...");
    refresh();      // Refresh the screen to show the prompt

    getch();        // Wait for a key press
    endwin();       // End ncurses mode
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


MainList::MainList() : head(nullptr), tail(nullptr) {
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
        head->piece.shape == head->next->next->piece.shape ||
        head->piece.color == head->next->piece.color &&
        head->piece.color == head->next->next->piece.color) {

        removeFirstNode();
        removeFirstNode();
        removeFirstNode();

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
         tail->piece.shape == thirdToLast->next->piece.shape) ||
        (tail->piece.color == thirdToLast->piece.color &&
         tail->piece.color == thirdToLast->next->piece.color)) {

        removeLastNode();
        removeLastNode();
        removeLastNode();

        score += 10;
    }
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

void MainList :: printChain() {
    if (head == nullptr) {
        std::cout << "Empty" << endl;
        return;
    }

    Node* current = head;
    do {
        char shapeSymbol;
        switch (current->piece.shape) {
            case Piece::SQUARE: shapeSymbol = 'S'; break;
            case Piece::DIAMOND: shapeSymbol = 'D'; break;
            case Piece::CIRCLE: shapeSymbol = 'C'; break;
            case Piece::TRIANGLE: shapeSymbol = 'T'; break;
        }

        string colorCode;
        switch (current->piece.color) {
            case Piece::RED: colorCode = "\033[31m"; break; // Red
            case Piece::YELLOW: colorCode = "\033[33m"; break; // Yellow
            case Piece::BLUE: colorCode = "\033[34m"; break; // Blue
            case Piece::GREEN: colorCode = "\033[32m"; break; // Green
        }

        std::cout << colorCode << shapeSymbol << "\033[0m" << " | "; // Reset color

        current = current->next;
    } while (current != head);

    std::cout << endl;
}

Piece::Piece(Shape shape, Color color) : shape(shape), color(color) {};

Piece::Piece()
{
}

void Piece::printPiece()
{
    switch (this->shape) {
        case Piece::SQUARE: std::cout << "S"; break;
        case Piece::DIAMOND: std::cout << "D"; break;
        case Piece::CIRCLE: std::cout << "C"; break;
        case Piece::TRIANGLE: std::cout << "T"; break;
    }

    std::cout << " | ";

    switch (this->color) {
        case Piece::RED: std::cout << "\033[31mR\033[0m"; break; // Red
        case Piece::YELLOW: std::cout << "\033[33mY\033[0m"; break; // Yellow
        case Piece::BLUE: std::cout << "\033[34mB\033[0m"; break; // Blue
        case Piece::GREEN: std::cout << "\033[32mG\033[0m"; break; // Green
    }
}