#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

const int GRID_SIZE = 9;
const int SUBGRID_SIZE = 3;
const int EMPTY_CELL = 0;

// Function to display the Sudoku grid
void displayGrid(int grid[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == EMPTY_CELL) {
                std::cout << " -";
            } else {
                std::cout << " " << grid[i][j];
            }
        }
        std::cout << std::endl;
    }
}

// Function to check if a value already exists in a row
bool isValueInRow(int grid[GRID_SIZE][GRID_SIZE], int row, int value) {
    for (int col = 0; col < GRID_SIZE; col++) {
        if (grid[row][col] == value) {
            return true;
        }
    }
    return false;
}

// Function to check if a value already exists in a column
bool isValueInColumn(int grid[GRID_SIZE][GRID_SIZE], int col, int value) {
    for (int row = 0; row < GRID_SIZE; row++) {
        if (grid[row][col] == value) {
            return true;
        }
    }
    return false;
}

// Function to check if a value already exists in a subgrid
bool isValueInSubgrid(int grid[GRID_SIZE][GRID_SIZE], int startRow, int startCol, int value) {
    for (int row = 0; row < SUBGRID_SIZE; row++) {
        for (int col = 0; col < SUBGRID_SIZE; col++) {
            if (grid[row + startRow][col + startCol] == value) {
                return true;
            }
        }
    }
    return false;
}

// Function to check if a value can be placed in a cell
bool isValueValid(int grid[GRID_SIZE][GRID_SIZE], int row, int col, int value) {
    return !isValueInRow(grid, row, value) &&
           !isValueInColumn(grid, col, value) &&
           !isValueInSubgrid(grid, row - row % SUBGRID_SIZE, col - col % SUBGRID_SIZE, value);
}

// Function to find the next empty cell in the Sudoku puzzle
bool findEmptyCell(int grid[GRID_SIZE][GRID_SIZE], int& row, int& col) {
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] == EMPTY_CELL) {
                return true;
            }
        }
    }
    return false;
}

// Function to solve the Sudoku puzzle using backtracking
bool solvePuzzle(int grid[GRID_SIZE][GRID_SIZE]) {
    int row, col;

    // Find an empty cell
    if (!findEmptyCell(grid, row, col)) {
        return true;  // Puzzle solved
    }

    // Try placing values 1-9 in the empty cell
    for (int value = 1; value <= GRID_SIZE; value++) {
        if (isValueValid(grid, row, col, value)) {
            grid[row][col] = value;

            if (solvePuzzle(grid)) {
                return true;  // Puzzle solved
            }

            grid[row][col] = EMPTY_CELL;  // Undo the placement
        }
    }

    return false;  // Puzzle cannot be solved
}

// Function to generate a random Sudoku puzzle
void generatePuzzle(int grid[GRID_SIZE][GRID_SIZE], int difficulty) {
    // Clear the grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = EMPTY_CELL;
        }
    }

    // Seed the random number generator
    std::srand(std::time(0));

    // Fill the diagonal subgrids
    for (int i = 0; i < GRID_SIZE; i += SUBGRID_SIZE) {
        for (int j = 0; j < GRID_SIZE; j += SUBGRID_SIZE) {
            int value = 1 + std::rand() % GRID_SIZE;
            while (!isValueValid(grid, i, j, value)) {
                value = 1 + std::rand() % GRID_SIZE;
            }
            grid[i][j] = value;
        }
    }

    // Solve the puzzle
    // (Note: This is a basic implementation that solves the puzzle by brute force)
    // (A more optimized solving algorithm can be implemented for better performance)
    // (However, since the focus is on the game and not the solver, a basic approach is used here)
    solvePuzzle(grid);

    // Remove cells based on difficulty level
    int numCellsToRemove = 0;
    if (difficulty == 1) {
        numCellsToRemove = 35;  // Easy difficulty
    } else if (difficulty == 2) {
        numCellsToRemove = 45;  // Medium difficulty
    } else if (difficulty == 3) {
        numCellsToRemove = 55;  // Hard difficulty
    }

    // Remove random cells
    while (numCellsToRemove > 0) {
        int row = std::rand() % GRID_SIZE;
        int col = std::rand() % GRID_SIZE;
        if (grid[row][col] != EMPTY_CELL) {
            grid[row][col] = EMPTY_CELL;
            numCellsToRemove--;
        }
    }
}





int main() {
    int grid[GRID_SIZE][GRID_SIZE];
    int difficulty;

    std::cout << "Welcome to the Sudoku Game!" << std::endl;

    // Get the difficulty level from the user
    do {
        std::cout << "Select the difficulty level (1-Easy, 2-Medium, 3-Hard): ";
        std::cin >> difficulty;
    } while (difficulty < 1 || difficulty > 3);

    // Generate and display the puzzle
    generatePuzzle(grid, difficulty);
    std::cout << "Generated Sudoku Puzzle:" << std::endl;
    displayGrid(grid);

    // Play the game
    bool gameOver = false;
    char action;
    int row, col, value;


    while (!gameOver) {
        std::cout << "Enter your action (A-Answer, Q-Quit): ";
        std::cin >> action;

        switch (action) {
            case 'A':  // Answer
            case 'a':
                std::cout << "Sudoku Puzzle Answer:" << std::endl;
                solvePuzzle(grid);
                displayGrid(grid);
                gameOver = true;
                break;

            case 'Q':  // Quit
            case 'q':
                gameOver = true;
                break;

            default:
                std::cout << "Invalid action. Please try again." << std::endl;
        }
    }

    std::cout << "Thank you for playing Sudoku!" << std::endl;
    return 0;
}
