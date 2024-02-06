#include <iostream>
#include <vector>
#include <conio.h>  // For keyboard input
#include <cstdlib>  // For std::rand and std::srand
#include <ctime>    // For std::time
#include <chrono>   // For time functions
#include <thread>   // For std::this_thread::sleep_for

using namespace std;

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;

vector<vector<int>> grid(GRID_WIDTH, vector<int>(GRID_HEIGHT, 0));

pair<int, int> snakeHead = {GRID_WIDTH / 2, GRID_HEIGHT / 2};  // Initial position of the snake

vector<pair<int, int>> snakeBody;  // Snake's body

bool justAte = false; // Flag indicating if the snake just ate

void placeFood() {
    int x, y;
    do {
        x = rand() % GRID_WIDTH;
        y = rand() % GRID_HEIGHT;
    } while (grid[x][y] != 0); // Keep looping until an empty cell is found

    grid[x][y] = 2; // Place the food
}

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir = STOP;

void moveSnake() {
    if (justAte) {
        snakeBody.insert(snakeBody.begin(), snakeHead);
        justAte = false; // Indicates that the snake has grown and no longer just ate
    } else if (!snakeBody.empty()) {
        snakeBody.insert(snakeBody.begin(), snakeHead);
        snakeBody.pop_back();
    }

    switch (dir) {
        case LEFT:
            snakeHead.first--;
            break;
        case RIGHT:
            snakeHead.first++;
            break;
        case UP:
            snakeHead.second--;
            break;
        case DOWN:
            snakeHead.second++;
            break;
        default:
            break;
    }

    // Check for eating the food
    if (grid[snakeHead.first][snakeHead.second] == 2) {
        grid[snakeHead.first][snakeHead.second] = 0; // Remove the food from the grid
        snakeBody.push_back({0, 0}); // Extend the snake's body
        justAte = true; // Snake just ate
    }
}

bool gameOver() {
    if (snakeHead.first >= GRID_WIDTH || snakeHead.first < 0 ||
        snakeHead.second >= GRID_HEIGHT || snakeHead.second < 0) {
        return true;
    }

    for (auto segment : snakeBody) {
        if (segment == snakeHead) {
            return true;
        }
    }
    return false;
}

void printGrid() {
    system("cls");  // Clear the screen
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (x == snakeHead.first && y == snakeHead.second)
                cout << "O";  // Represents the head of the snake
            else if (grid[x][y] == 2)
                cout << "*";  // Represents the food
            else {
                bool isBodyPart = false;
                for (auto segment : snakeBody) {
                    if (x == segment.first && y == segment.second) {
                        cout << "+";  // Represents the snake's body
                        isBodyPart = true;
                        break;
                    }
                }
                if (!isBodyPart) cout << " ";  // Empty space
            }
        }
        cout << endl;
    }
}

int main() {
    srand(time(nullptr));  // Initial point for the random number generator
    placeFood();  // Place the initial food

    while (!gameOver()) {
        if (_kbhit()) {  // Is there a keyboard input?
            switch (_getch()) {  // Which key was pressed?
                case 'a':
                    dir = LEFT;
                    break;
                case 'd':
                    dir = RIGHT;
                    break;
                case 'w':
                    dir = UP;
                    break;
                case 's':
                    dir = DOWN;
                    break;
            }
        }

        moveSnake();
        if (justAte) {
            placeFood();
            justAte = false;
        }
        printGrid();  // Print the grid

        // Add a delay to control the game speed
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    cout << "Game Over!" << endl;
    return 0;
}
