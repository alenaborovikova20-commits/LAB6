#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>   
#include <iomanip>    
#include <vector>     
#include <string>     
#include <cmath>  
#include <random>    
using namespace std;


void task1() {
    cout << "\nЗАДАНИЕ 1\n";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(10, 50);
    
    int M, N;
    cout << "Введите M и N: ";
    cin >> M >> N;
    
    int** matrix = new int*[M];
    for (int i = 0; i < M; i++)
        matrix[i] = new int[N];
    
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dis(gen);
    }
}

    cout << "\nИсходная матрица:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }
    
    int* diff = new int[M];
    
    
    for (int i = 0; i < M; i++) {
        int minIdx = 0, maxIdx = 0;
        
        for (int j = 1; j < N; j++) {
            if (matrix[i][j] < matrix[i][minIdx]) minIdx = j;
            if (matrix[i][j] > matrix[i][maxIdx]) maxIdx = j;
        }
        swap(matrix[i][minIdx], matrix[i][maxIdx]);
        
        
        int rowMin = matrix[i][0], rowMax = matrix[i][0];
        for (int j = 1; j < N; j++) {
            if (matrix[i][j] < rowMin) rowMin = matrix[i][j];
            if (matrix[i][j] > rowMax) rowMax = matrix[i][j];
        }
        diff[i] = rowMax - rowMin;
    }
    
    cout << "\nМатрица после обмена первого min и первого max:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++)
            cout << setw(4) << matrix[i][j];
        cout << endl;
    }
    
    int maxDiffRow = 0;
    for (int i = 1; i < M; i++)
        if (diff[i] > diff[maxDiffRow]) maxDiffRow = i;
    
    cout << "\nРазности по строкам: ";
    for (int i = 0; i < M; i++)
        cout << diff[i] << " ";
    cout << "\nНомер строки с максимальной разностью: " << maxDiffRow + 1 << endl;
    
    for (int i = 0; i < M; i++)
        delete[] matrix[i];
    delete[] matrix;
    delete[] diff;
}


int sumOfDigits(int num) {
    int sum = 0;
    num = abs(num);
    while (num > 0) {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

void task2() {
    cout << "\nЗАДАНИЕ 2\n";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100, 200);
    
    int M, N;
    cout << "Введите M и N: ";
    cin >> M >> N;
    
    int** matrix = new int*[M];
    for (int i = 0; i < M; i++)
        matrix[i] = new int[N];
    

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = dis(gen);
    }
    }
    
    cout << "\nМатрица:\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cout << setw(4) << matrix[i][j];
        }
        cout << endl;
    }
    

    int* rowSums = new int[M]();
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            rowSums[i] += sumOfDigits(matrix[i][j]);
        }
        cout << "Сумма цифр строки " << i+1 << ": " << rowSums[i] << endl;
    }
    
    
    int maxSumRow = 0;
    for (int i = 1; i < M; i++)
        if (rowSums[i] > rowSums[maxSumRow]) maxSumRow = i;
    
    cout << "\nНомер строки с наибольшей суммой цифр: " << maxSumRow + 1 << endl;
    
    
    for (int i = 0; i < M; i++)
        delete[] matrix[i];
    delete[] matrix;
    delete[] rowSums;
}



// Подсчёт живых соседей
int countNeighbors(bool** grid, int size, int x, int y) {
    int cnt = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < size && ny >= 0 && ny < size)
                cnt += grid[nx][ny];
        }
    }
    return cnt;
}

// Вычисление следующего поколения
void nextGeneration(bool** current, bool** next, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int neighbors = countNeighbors(current, size, i, j);
            if (current[i][j])
                next[i][j] = (neighbors == 2 || neighbors == 3);
            else
                next[i][j] = (neighbors == 3);
        }
    }
}

// Очистка и вывод поля
void printGrid(bool** grid, int size, int generation, const string& title) {
    system("clear");
    
    cout << "\n";
    cout << "                    " << title;
    int spaces = 48 - title.length();
    for (int i = 0; i < spaces; i++) cout << " ";
    cout << "\n";
    cout << "  Поколение: " << generation;
    int genSpaces = 38 - to_string(generation).length();
    for (int i = 0; i < genSpaces; i++) cout << " ";
    cout << "\n";
    
    for (int i = 0; i < size; i++) {
        cout << "|  ";
        for (int j = 0; j < size; j++) {
            cout << (grid[i][j] ? "0" : " ");
            if (j < size-1) cout << " ";
        }
        cout << "  |\n";
    }
    
    cout << "\n";
    
    int live = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (grid[i][j]) live++;
    cout << "Живых клеток: " << live << endl;
}


void initStableFigure(bool** grid, int size, const string& figure) {
  
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = false;
    
    int c = size / 2;  // центр
    
    if (figure == "Block") {
        grid[c][c] = grid[c][c+1] = grid[c+1][c] = grid[c+1][c+1] = true;
    }
    else if (figure == "Beehive") {
        grid[c][c+1] = grid[c][c+2] = true;
        grid[c+1][c] = grid[c+1][c+3] = true;
        grid[c+2][c+1] = grid[c+2][c+2] = true;
    }
    else if (figure == "Loaf") {
        grid[c][c+1] = grid[c][c+2] = true;
        grid[c+1][c] = grid[c+1][c+3] = true;
        grid[c+2][c+1] = grid[c+2][c+3] = true;
        grid[c+3][c+2] = true;
    }
    else if (figure == "Boat") {
        grid[c][c] = grid[c][c+1] = true;
        grid[c+1][c] = grid[c+1][c+2] = true;
        grid[c+2][c+1] = true;
    }
}


void initRandom(bool** grid, int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);
    
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            grid[i][j] = (dis(gen) <= 30);
}

// Демонстрация устойчивых фигур
void demoStableFigures(int size) {
    string figures[] = {"Block", "Beehive", "Loaf", "Boat"};
    int numFigures = 4;
    
    for (int f = 0; f < numFigures; f++) {
        string fig = figures[f];
        
        bool** grid = new bool*[size];
        bool** next = new bool*[size];
        for (int i = 0; i < size; i++) {
            grid[i] = new bool[size];
            next[i] = new bool[size];}
        
        initStableFigure(grid, size, fig);
        
        cout << "\nФигура: " << fig << "\n";
        cout << "Нажмите Enter...";
        cin.get();
        
        for (int gen = 0; gen < 4; gen++) {
            printGrid(grid, size, gen, fig);
            usleep(800000);
            nextGeneration(grid, next, size);

            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    grid[i][j] = next[i][j];
        }
        
        cout << "\nФигура осталась неизменной. Нажмите Enter...";
        cin.get();
        
        for (int i = 0; i < size; i++) {
            delete[] grid[i];
            delete[] next[i];
        }
        delete[] grid;
        delete[] next;
    }
}

// Демонстрация случайного поля
void demoRandomField(int size) {
    bool** grid = new bool*[size];
    bool** next = new bool*[size];
    for (int i = 0; i < size; i++) {
        grid[i] = new bool[size];
        next[i] = new bool[size];
    }
    
    initRandom(grid, size);
    
    cout << "\nСлучайное начальное поле\n";
    cout << "Нажмите Enter...";
    cin.get();
    
    for (int gen = 0; gen < 50; gen++) {
        printGrid(grid, size, gen, "СЛУЧАЙНОЕ ПОЛЕ");
        usleep(300000);
        nextGeneration(grid, next, size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                grid[i][j] = next[i][j];
    }
    

    for (int i = 0; i < size; i++) {
        delete[] grid[i];
        delete[] next[i];
    }
    delete[] grid;
    delete[] next;
}

void task3() {
    cout << "\nЗАДАНИЕ 3\n";
    
    int size;
    cout << "Введите размер игрового поля (10-20): ";
    cin >> size;
    cin.ignore();
    
    demoStableFigures(size);
    demoRandomField(size);
    
    cout << "\nДемонстрация завершена.\n";
}


int main() {
    srand(time(nullptr));
    
    task1();
    cin.get();
    
    task2();
    cin.get();
    
    task3();
    
    return 0;
}