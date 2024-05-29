#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>

#define RED 'r'
#define BLUE 'b'

#define Q_SIZE "BOARD_SIZE"
#define Q_NUMBER "PAWNS_NUMBER"
#define Q_CORRECT "IS_BOARD_CORRECT"
#define Q_GAME_OVER "IS_GAME_OVER"
#define Q_POSSIBLE "IS_BOARD_POSSIBLE"
#define Q_RED_WIN_1 "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT"
#define Q_RED_WIN_2 "CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT"
#define Q_BLUE_WIN_1 "CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT"
#define Q_BLUE_WIN_2 "CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT"

bool checkerPawn(char c) {
    if (c == BLUE || c == RED)
        return true;
    else
        return false;
}

bool isBoard(char c) {
    switch (c) {
    case '-':
    case '<':
    case '>':
    case 'b':
    case 'r':
    case ' ':
        return true;
    default:
        return false;
    }
}

bool isLegal(int red, int blue) {
    int delta = red - blue;
    if (delta == 0 || delta == 1)
        return true;
    else
        return false;
}

bool dfs(char** boardMatrix, bool** visitedBoard, int x, int y, char player, const int size) {
    std::stack<int> previousX, previousY;
    previousX.push(x);
    previousY.push(y);
    const int dirX[] = { 1, 1, 0, 0, -1, -1 }; //right, right-down(if 1st half of board),right - down(if 2nd half of board)
    const int dirY[] = { 0, 1, 1, -1, 0, -1 }; //  left-down, vertically down(twooptions) ,left, right-up, left-up
    if (player == BLUE && previousX.top() == size - 1) {
        return true;
    }
    else if (player == RED && previousY.top() == size - 1) {
        return true;
    }

    while (!previousX.empty()) {
        int currentX = previousX.top();
        int currentY = previousY.top();
        previousX.pop();
        previousY.pop();
        visitedBoard[currentX][currentY] = true; // starting cell marked as visited
        for (int dir = 0; dir < 6; dir++) {
            int newX = currentX + dirX[dir];
            int newY = currentY + dirY[dir];
            if (newX >= 0 && newY >= 0 && newX < size && newY < size) {
                if (boardMatrix[newX][newY] == player && !visitedBoard[newX][newY]) {
                    if (dfs(boardMatrix, visitedBoard, newX, newY, player, size)) {
                        return true;
                    }
                }
            }
        }
    }
    return false; //if we don t find path: return false->game_over->no
}

char** createBoardMatrix(const std::vector<std::vector<char>>& board, int size) {
    char** boardMatrix = new char* [size];
    for (int i = 0; i < size; ++i) {
        boardMatrix[i] = new char[size];
    }
    for (int p = 0; p < size; p++) {
        for (int q = 0; q < size; q++) {
            boardMatrix[p][q] = '_';
        }
    }
    int column = 0;
    for (int p = 0; p < size; p++) {
        column = size - p - 1;
        for (int q = 0; q < size; q++) {
            if (q < column) {
                boardMatrix[p][q] = board[p + q][q];
            }
            else {
                boardMatrix[p][q] = board[p + q][column];
            }
        }
    }
    return boardMatrix;
}

void deleteBoardChar(char** boardMatrix, int size) {
    for (int i = 0; i < size; ++i) {
        delete[] boardMatrix[i];
    }
    delete[] boardMatrix;
}

bool** createBoardVisited(int size) {
    bool** visitedBoard = new bool* [size];
    for (int i = 0; i < size; i++) {
        visitedBoard[i] = new bool[size];
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            visitedBoard[i][j] = false;
        }
    }
    return visitedBoard;
}

void deleteVisitedBoard(bool** visitedBoard, int size) {
    for (int i = 0; i < size; i++) {
        delete[] visitedBoard[i];
    }
    delete[] visitedBoard;
}

int countRedPawns(char** boardMatrix, int size) {
    int counter = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (boardMatrix[i][j] == RED)
                counter++;
        }
    }
    return counter;
}

int countBluePawns(char** boardMatrix, int size) {
    int counter = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (boardMatrix[i][j] == BLUE)
                counter++;
        }
    }
    return counter;
}

int countFreeSpace(char** boardMatrix, int size) {
    int counter = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (boardMatrix[i][j] == 'f')
                counter++;
        }
    }
    return counter;
}

bool isGameOver(char** boardMatrix, bool** visitedBoard, int size, int counterRed, int counterBlue, char player, bool avoidChecking) {
    if (isLegal(counterRed, counterBlue) || avoidChecking) {
        if (player == RED) {
            for (int k = 0; k < size; k++) {
                if (boardMatrix[k][0] == RED && dfs(boardMatrix, visitedBoard, k, 0, RED, size)) {
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            visitedBoard[i][j] = false;
                        }
                    }
                    return true;
                }
            }
        }
        else {
            for (int k = 0; k < size; k++) {
                if (boardMatrix[0][k] == BLUE && dfs(boardMatrix, visitedBoard, 0, k, BLUE, size)) {
                    for (int i = 0; i < size; i++) {
                        for (int j = 0; j < size; j++) {
                            visitedBoard[i][j] = false;
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void is_board_possible(char** board, bool** visitedBoard, int size, int counterBlue, int counterRed) {
    char pawn = 'k';
    if (isGameOver(board, visitedBoard, size, counterRed, counterBlue, RED, false)) {
        pawn = RED; // winner
    }
    else if (isGameOver(board, visitedBoard, size, counterRed, counterBlue, BLUE, false)) {
        pawn = BLUE; //winner
    }

    if (counterBlue != counterRed - 1 && pawn == RED) // red must have 1 more pawn than blue to win
        std::cout << "NO" << std::endl;
    else if (pawn == BLUE && counterBlue != counterRed) // if blue won - number of pawns must be equal else illegal position
        std::cout << "NO" << std::endl;
    else if (pawn != 'k') //if there is game_over and numbers of pawns are correct we have to check if there was skipped win
    {
        bool isPossible = false;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (board[i][j] == pawn) {
                    int countRed = countRedPawns(board, size);
                    int countBlue = countBluePawns(board, size);
                    board[i][j] = 'k';
                    if (!isGameOver(board, visitedBoard, size, countRed, countBlue, pawn, false)) {
                        isPossible = true;       // if there is at least one situation when changed pawn change the result of game, board is possible because that mean win has not been skipped
                    }
                    board[i][j] = pawn;
                }
            }
        }
        if (isPossible)
            std::cout << "YES" << std::endl;
        else
            std::cout << "NO" << std::endl;
    }
    else {
        std::cout << "YES" << std::endl; // if there was no missed win and number of pawns are correct board is possible
    }
}

void redWin1(char** boardMatrix, bool** visitedBoard, int size, int counterRed, int counterBlue) {
    int freeSpace = countFreeSpace(boardMatrix, size);
    int deltaPawns = counterRed - counterBlue;
    bool win = false;
    int moveNumber = 0, numberOfMoves;
    if (deltaPawns == 0) {
        numberOfMoves = 1;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = RED;
                        int redPawns = countRedPawns(boardMatrix, size);
                        int bluePawns = countBluePawns(boardMatrix, size);
                        if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, RED, false)) {
                            win = true;
                        }
                        moveNumber++;
                        if (moveNumber == numberOfMoves && win)
                            break;
                        else if (moveNumber == numberOfMoves) {
                            boardMatrix[j][k] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
    else {
        numberOfMoves = 2;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = RED;
                        moveNumber++;
                        /*int x = 0, y = 0;
                        bool ifPawnPlaced = false;
                        for (int l = 0; l < size && !ifPawnPlaced; l++) {
                                for (int m = 0; m < size; m++) {
                                        if (boardMatrix[l][m] == 'f'){
                                                boardMatrix[l][m] = BLUE;
                                                moveNumber++;
                                                x = l;
                                                y = m;
                                                break;
                                        }
                                }
                        }*/
                        int redPawns = countRedPawns(boardMatrix, size);
                        int bluePawns = countBluePawns(boardMatrix, size);
                        if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, RED, true)) {
                            win = true;
                        }
                        if (moveNumber == numberOfMoves - 1 && win)
                            break;
                        else if (moveNumber == numberOfMoves - 1) {
                            boardMatrix[j][k] = 'f';
                            //boardMatrix[x][y] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
}

void blueWin1(char** boardMatrix, bool** visitedBoard, int size, int counterRed, int counterBlue) {
    int freeSpace = countFreeSpace(boardMatrix, size);
    int deltaPawns = counterRed - counterBlue;
    bool win = false;
    int moveNumber = 0, numberOfMoves;
    if (deltaPawns == 0) {
        numberOfMoves = 2;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = BLUE;
                        moveNumber++;
                        /*int x = 0, y = 0;
                        bool ifPawnPlaced = false;
                        for (int l = 0; l < size && !ifPawnPlaced; l++) {
                                for (int m = 0; m < size; m++) {
                                        if (boardMatrix[l][m] == 'f') {
                                                boardMatrix[l][m] = RED;
                                                moveNumber++;
                                                x = l;
                                                y = m;
                                                break;
                                        }
                                }
                        }*/
                        int redPawns = countRedPawns(boardMatrix, size);
                        int bluePawns = countBluePawns(boardMatrix, size);
                        if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, BLUE, true)) {
                            win = true;
                        }
                        if (moveNumber == numberOfMoves - 1 && win)
                            break;
                        else if (moveNumber == numberOfMoves - 1) {
                            boardMatrix[j][k] = 'f';
                            //boardMatrix[x][y] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
    else {
        numberOfMoves = 1;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = BLUE;
                        int redPawns = countRedPawns(boardMatrix, size);
                        int bluePawns = countBluePawns(boardMatrix, size);
                        if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, BLUE, false)) {
                            win = true;
                        }
                        moveNumber++;
                        if (moveNumber == numberOfMoves && win)
                            break;
                        else if (moveNumber == numberOfMoves) {
                            boardMatrix[j][k] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
}

void blueWin2(char** boardMatrix, bool** visitedBoard, int size, int counterRed, int counterBlue) {
    int freeSpace = countFreeSpace(boardMatrix, size);
    int deltaPawns = counterRed - counterBlue;
    bool win = false;
    int moveNumber = 0, numberOfMoves;
    if (deltaPawns == 0) {
        numberOfMoves = 4;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = BLUE;
                        moveNumber++;
                        /*int x = 0, y = 0;
                        bool ifPawnPlaced = false;
                        for (int l = 0; l < size && !ifPawnPlaced; l++) {
                                for (int m = 0; m < size; m++) {
                                        if (boardMatrix[l][m] == 'f') {
                                                boardMatrix[l][m] = RED;
                                                moveNumber++;
                                                x = l;
                                                y = m;
                                                break;
                                        }
                                }
                        }*/
                        if (moveNumber == numberOfMoves - 2 && !win) {
                            int redPawns = countRedPawns(boardMatrix, size);
                            int bluePawns = countBluePawns(boardMatrix, size);
                            if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, BLUE, true)) {
                                win = true;
                                break;
                            }
                        }
                        if (moveNumber == numberOfMoves - 2 && !win) {
                            boardMatrix[j][k] = 'f';
                            //boardMatrix[x][y] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
    else {
        numberOfMoves = 3;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = BLUE;
                        moveNumber++;
                        /*int x = 0, y = 0;
                        bool ifPawnPalced = false;
                        if(moveNumber != numberOfMoves){
                                for (int l = 0; l < size && !ifPawnPalced; l++) {
                                        for (int m = 0; m < size; m++) {
                                                if (boardMatrix[l][m] == 'f') {
                                                        boardMatrix[l][m] = RED;
                                                        moveNumber++;
                                                        x = l;
                                                        y = m;
                                                        ifPawnPalced = true;
                                                        break;
                                                }
                                        }
                                }
                        }*/
                        if (moveNumber == numberOfMoves - 1 && !win) {
                            int redPawns = countRedPawns(boardMatrix, size);
                            int bluePawns = countBluePawns(boardMatrix, size);
                            if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, BLUE, true)) {
                                win = true;
                                break;
                            }
                        }
                        if (moveNumber == numberOfMoves - 1 && !win) {
                            boardMatrix[j][k] = 'f';
                            //boardMatrix[x][y] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
}

void redWin2(char** boardMatrix, bool** visitedBoard, int size, int counterRed, int counterBlue) {
    int freeSpace = countFreeSpace(boardMatrix, size);
    int deltaPawns = counterRed - counterBlue;
    bool win = false;
    int moveNumber = 0, numberOfMoves;
    if (deltaPawns == 0) {
        numberOfMoves = 3;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = RED;
                        moveNumber++;
                        /*int x = 0, y = 0;
                        bool ifPawnPlaced = false;
                        if(moveNumber != numberOfMoves){
                                for (int l = 0; l < size && !ifPawnPlaced; l++) {
                                        for (int m = 0; m < size; m++) {
                                                if (boardMatrix[l][m] == 'f') {
                                                        boardMatrix[l][m] = BLUE;
                                                        moveNumber++;
                                                        x = l;
                                                        y = m;
                                                        ifPawnPlaced = true;
                                                        break;
                                                }
                                        }
                                }
                        }*/
                        if (moveNumber == numberOfMoves - 1 && !win) {
                            int redPawns = countRedPawns(boardMatrix, size);
                            int bluePawns = countBluePawns(boardMatrix, size);
                            if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, RED, true)) {
                                win = true;
                                break;
                            }
                        }
                        if (moveNumber == numberOfMoves - 1 && !win) {
                            boardMatrix[j][k] = 'f';
                            //boardMatrix[x][y] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
    else {
        numberOfMoves = 4;
        if (numberOfMoves <= freeSpace) {
            for (int j = 0; j < size && !win; j++) {
                for (int k = 0; k < size; k++) {
                    if (boardMatrix[j][k] == 'f') {
                        boardMatrix[j][k] = BLUE;
                        moveNumber++;
                        /*int x = 0, y = 0;
                        bool ifPawnPlaced = false;
                        for (int l = 0; l < size && ifPawnPlaced; l++) {
                                for (int m = 0; m < size; m++) {
                                        if (boardMatrix[l][m] == 'f') {
                                                boardMatrix[l][m] = RED;
                                                moveNumber++;
                                                x = l;
                                                y = m;
                                                ifPawnPlaced = true;
                                                break;
                                        }
                                }
                        }*/
                        if (moveNumber == numberOfMoves - 2 && !win) {
                            int redPawns = countRedPawns(boardMatrix, size);
                            int bluePawns = countBluePawns(boardMatrix, size);
                            if (isGameOver(boardMatrix, visitedBoard, size, redPawns, bluePawns, BLUE, true)) {
                                win = true;
                                break;
                            }
                        }
                        if (moveNumber == numberOfMoves - 2 && !win) {
                            boardMatrix[j][k] = 'f';
                            //boardMatrix[x][y] = 'f';
                            moveNumber--;
                        }
                    }
                }
            }
            if (win)
                std::cout << "YES" << std::endl;
            else
                std::cout << "NO" << std::endl;
        }
        else {
            std::cout << "NO" << std::endl;
        }
    }
}



int main() {
    std::string input;
    std::stack<char> stackCounter;
    std::vector<std::vector<char>>board;
    int counterBlue, counterRed, counterSize, row, column, tempRow, counterMinuses;
    counterBlue = counterRed = counterSize = column = 0;
    row = 1;
    tempRow = 0;
    while (std::getline(std::cin, input)) {
        bool procesingBoard = true;
        if (procesingBoard) {
            counterMinuses = 0;
            for (char c : input) {
                if (isBoard(c)) {
                    if (c == '-')
                        counterMinuses++;
                    if ((checkerPawn(c) || c == '<') && c != ' ') {
                        stackCounter.push(c);
                        counterMinuses = 0;
                        if (row == 0)
                            row++;
                    }

                    else if (c == '>') {
                        counterSize++;
                        column++;
                        if (row > board.size()) {
                            board.push_back(std::vector<char>(column, ' ')); //creating new row of board
                        }
                        else if (tempRow == row) {
                            board[row - 1].push_back(' '); // adding column to existing row
                        }
                        while (!stackCounter.empty()) {

                            char temp = stackCounter.top();
                            stackCounter.pop();
                            if (temp == BLUE) {
                                counterBlue++;
                                board[row - 1][column - 1] = temp;
                                break;
                            }
                            else if (temp == RED) {
                                counterRed++;
                                board[row - 1][column - 1] = temp;
                                break;
                            }
                            else if (temp == '<') {
                                board[row - 1][column - 1] = 'f';
                            }
                        }
                    }
                }
                if (counterMinuses == 3) {
                    counterMinuses = 0;
                    procesingBoard = false;
                    break;
                }
            }
            if (procesingBoard) {
                row++;
                tempRow = row;
                column = 0;
            }
        }
        if (input == Q_SIZE) {
            int ans = sqrt(counterSize);
            std::cout << ans << std::endl;
            counterBlue = counterRed = 0;
            counterSize = 0;
            tempRow = row = 0;
            board.clear();
        }
        else if (input == Q_NUMBER) {
            int ans = counterBlue + counterRed;
            std::cout << ans << std::endl;
            counterBlue = counterRed = 0;
            counterSize = 0;
            tempRow = row = 0;
            board.clear();
        }
        else if (input == Q_CORRECT) {
            bool checker = isLegal(counterRed, counterBlue);
            if (checker) {
                std::cout << "YES" << std::endl;
            }
            else {
                std::cout << "NO" << std::endl;
            }
            counterBlue = counterRed = 0;
            counterSize = 0;
            tempRow = row = 0;
            board.clear();
        }
        else if (input == Q_GAME_OVER) {
            int size = sqrt(counterSize);
            char** boardMatrix = createBoardMatrix(board, size);
            bool** visitedBoard = createBoardVisited(size);
            if (isGameOver(boardMatrix, visitedBoard, size, counterRed, counterBlue, RED, false)) {
                std::cout << "YES RED" << std::endl;
            }
            else if (isGameOver(boardMatrix, visitedBoard, size, counterRed, counterBlue, BLUE, false)) {
                std::cout << "YES BLUE" << std::endl;
            }
            else {
                std::cout << "NO" << std::endl;
            }
            deleteBoardChar(boardMatrix, size);
            deleteVisitedBoard(visitedBoard, size);
            counterBlue = counterRed = 0;
            counterSize = 0;
            tempRow = row = 0;
            board.clear();
        }
        else if (input == Q_POSSIBLE) {
            if (isLegal(counterRed, counterBlue)) {
                int size = sqrt(counterSize);
                char** boardMatrix = createBoardMatrix(board, size);
                bool** visitedBoard = createBoardVisited(size);
                is_board_possible(boardMatrix, visitedBoard, size, counterBlue, counterRed);
                deleteBoardChar(boardMatrix, size);
                deleteVisitedBoard(visitedBoard, size);
            }
            else {
                std::cout << "NO" << std::endl;
            }
            counterBlue = counterRed = 0;
            counterSize = 0;
            tempRow = row = 0;
            board.clear();
        }
        else if (input == Q_RED_WIN_1) {
            if (isLegal(counterRed, counterBlue)) {
                int size = sqrt(counterSize);
                char** boardMatrix = createBoardMatrix(board, size);
                bool** visitedBoard = createBoardVisited(size);
                if (isGameOver(boardMatrix, visitedBoard, size, counterRed, counterBlue, RED, false)) {
                    std::cout << "NO" << std::endl;
                }
                else {
                    redWin1(boardMatrix, visitedBoard, size, counterRed, counterBlue);
                }
                deleteBoardChar(boardMatrix, size);
                deleteVisitedBoard(visitedBoard, size);
            }
            else {
                std::cout << "NO" << std::endl;
            }
        }
        else if (input == Q_BLUE_WIN_1) {
            if (isLegal(counterRed, counterBlue)) {
                int size = sqrt(counterSize);
                char** boardMatrix = createBoardMatrix(board, size);
                bool** visitedBoard = createBoardVisited(size);
                if (isGameOver(boardMatrix, visitedBoard, size, counterRed, counterBlue, BLUE, false)) {
                    std::cout << "NO" << std::endl;
                }
                else {
                    blueWin1(boardMatrix, visitedBoard, size, counterRed, counterBlue);
                }
                deleteBoardChar(boardMatrix, size);
                deleteVisitedBoard(visitedBoard, size);
            }
            else {
                std::cout << "NO" << std::endl;
            }
        }
        else if (input == Q_RED_WIN_2) {
            if (isLegal(counterRed, counterBlue)) {
                int size = sqrt(counterSize);
                char** boardMatrix = createBoardMatrix(board, size);
                bool** visitedBoard = createBoardVisited(size);
                if (isGameOver(boardMatrix, visitedBoard, size, counterRed, counterBlue, RED, false)) {
                    std::cout << "NO" << std::endl;
                }
                else {
                    redWin2(boardMatrix, visitedBoard, size, counterRed, counterBlue);
                }
                deleteBoardChar(boardMatrix, size);
                deleteVisitedBoard(visitedBoard, size);
            }
            else {
                std::cout << "NO" << std::endl;
            }
        }
        else if (input == Q_BLUE_WIN_2) {
            if (isLegal(counterRed, counterBlue)) {
                int size = sqrt(counterSize);
                char** boardMatrix = createBoardMatrix(board, size);
                bool** visitedBoard = createBoardVisited(size);
                if (isGameOver(boardMatrix, visitedBoard, size, counterRed, counterBlue, BLUE, false)) {
                    std::cout << "NO" << std::endl;
                }
                else {
                    blueWin2(boardMatrix, visitedBoard, size, counterRed, counterBlue);
                }
                deleteBoardChar(boardMatrix, size);
                deleteVisitedBoard(visitedBoard, size);
            }
            else {
                std::cout << "NO" << std::endl;
            }
            counterBlue = counterRed = 0;
            counterSize = 0;
            tempRow = row = 0;
            board.clear();
        }
    }
    return 0;
}