#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

const int INF = INT_MAX;

void printBoard(int rows, int cols, const vector<vector<int>>& board);

vector<vector<int>> makeTarget(int rows, int cols, int type);

int solve(int rows, int cols, vector<vector<int>> board, const vector<vector<int>>& target, bool show);


int main() {
    int rows, cols;
    while(1){
    cin >> rows >> cols;
    if(rows % 2 != 0){
        cout << "Количество строк должно быть четным! Попробуйте еще раз!"<< endl;
    }else{break;}}
    
    vector<vector<int>> board(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cin >> board[i][j];}}
    cout << "\n";
    

    vector<vector<int>> target0 = makeTarget(rows, cols, 1);
    vector<vector<int>> target1 = makeTarget(rows, cols, 0);
    
    int firstTargetRes = solve(rows, cols, board, target0, false);
    int secondTargetRes = solve(rows, cols, board, target1, false);
    int best = min(firstTargetRes, secondTargetRes);
    
    if (best >= INF) {
        cout << "Невозможно" << endl;
    } else {
        if (firstTargetRes <= secondTargetRes) solve(rows, cols, board, target0, true);
        else solve(rows, cols, board, target1, true);
        cout << "Результат: " << best << endl;}
    return 0;}


void printBoard(int rows, int cols, const vector<vector<int>>& board) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << board[i][j] << " ";}
        cout << endl;}
    cout << endl;}


vector<vector<int>> makeTarget(int rows, int cols, int type) {
    vector<vector<int>> target(rows, vector<int>(cols));
    int half = rows / 2;
    int topValue = (type == 0) ? 0 : 1;
    int bottomValue = (type == 0) ? 1 : 0;

    for (int i = 0; i < rows; ++i) {
        int val = (i < half) ? topValue : bottomValue;
        fill(target[i].begin(), target[i].end(), val);
    }
    return target;
}



int solve(int rows, int cols, vector<vector<int>> board, const vector<vector<int>>& target, bool show) {
    int diff = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] != target[i][j]) diff++;}}
    if (diff % 2 != 0) return INF;

    int moves = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] != target[i][j]) {
                if (j + 1 < cols) {
                    board[i][j] ^= 1;
                    board[i][j + 1] ^= 1;
                    moves++;
                    if (show) {
                        cout << "Шаг номер " <<  moves << endl;
                        printBoard(rows, cols, board);}
                } else if (i + 1 < rows) {
                    board[i][j] ^= 1;
                    board[i + 1][j] ^= 1;
                    moves++;
                    if (show) {
                        cout << "Шаг номер " <<  moves << endl;
                        printBoard(rows, cols, board);}}}}}

    return moves;}