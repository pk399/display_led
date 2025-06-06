#include <cstdlib>

#include <iostream>
#include <vector>
#include <stdexcept>




class Matrix {
private:
    std::vector<std::vector<int> > data;
    size_t rows;
    size_t cols;

public:
    // Конструктор по умолчанию (пустая матрица)
    Matrix() : rows(0), cols(0) {}

    // Конструктор с указанием размеров (инициализация нулями)
    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        data.resize(rows, std::vector<int>(cols, 0));
    }

    // Конструктор из вектора векторов
    Matrix(const std::vector<std::vector<int> >& input) : data(input) {
        rows = input.size();
        if (rows > 0) {
            cols = input[0].size();
            // Проверка, что все строки одинаковой длины
            for (auto& v: data) {
                if (v.size() != cols) {
                    // BOLT throw std::invalid_argument("All rows must have the same length");
                    v.resize(cols);
                }
            }
        } else {
            cols = 0;
        }
    }

    // Получение размеров матрицы
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    // Доступ к элементам матрицы
    int& operator()(size_t row, size_t col) {
        if (row >= rows || col >= cols) {
            // BOLT throw std::out_of_range("Matrix indices out of range");
            return data[0][0];
        }
        return data[row][col];
    }

    const int& operator()(size_t row, size_t col) const {
        if (row >= rows || col >= cols) {
            // BOLT throw std::out_of_range("Matrix indices out of range");
            return data[0][0];
        }
        return data[row][col];
    }

    // Оператор сложения матриц
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            // BOLT throw std::invalid_argument("Matrix dimensions must agree for addition");
            return Matrix{};
        }
        
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] + other(i, j);
            }
        }
        return result;
    }

    // Оператор вычитания матриц
    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            // BOLT throw std::invalid_argument("Matrix dimensions must agree for subtraction");
            return Matrix{};
        }
        
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] - other(i, j);
            }
        }
        return result;
    }

    // Умножение матрицы на число
    Matrix operator*(int scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result(i, j) = data[i][j] * scalar;
            }
        }
        return result;
    }

    // Умножение матрицы на матрицу
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            // BOLT throw std::invalid_argument("Number of columns in first matrix must equal number of rows in second matrix");
            return Matrix{};
        }
        
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                for (size_t k = 0; k < cols; ++k) {
                    result(i, j) += data[i][k] * other(k, j);
                }
            }
        }
        return result;
    }

    // Оператор вывода матрицы
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (size_t i = 0; i < matrix.rows; ++i) {
            for (size_t j = 0; j < matrix.cols; ++j) {
                os << matrix(i, j) << ' ';
            }
            os << '\n';
        }
        return os;
    }
};


const int fld_width = 7;
const int fld_height = 6;

const int shp_width = 3;
const int shp_height = 3;


const std::vector<std::vector<int> > Default_f= {
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1}

};

const std::vector<std::vector<std::vector<int> >> Forms = 
{
    {{0,1,0}, {0,1,0}, {0,1,0}},
    {{0,0,0}, {1,1,1}, {0,0,0}},
    {{1,1,0}, {0,1,0}, {0,0,0}},
    {{0,1,1}, {0,1,0}, {0,0,0}},
};





class Figure{
    int x,y;
    Matrix form;
    int color;

public:

    Figure() : x(1), y(-3) {
        // Инициализация генератора случайных чисел
        // BOLT std::srand(std::time(0));
        
        // Выбор случайного цвета (1-3)
        color = std::rand() % 3 + 1;
        
        // Выбор случайной формы и умножение на цвет
        int form_index = std::rand() % Forms.size();
        form = Matrix(Forms[form_index]) * color;
    }

    Figure(int x, int y) : x(x), y(y) {
        // Инициализация генератора случайных чисел
        // BOLT std::srand(std::time(0));
        
        // Выбор случайного цвета (1-3)
        color = std::rand() % 3 + 1;
        
        // Выбор случайной формы и умножение на цвет
        int form_index = std::rand() % Forms.size();
        form = Matrix(Forms[form_index]) * color;
    }


    // Геттеры и сеттеры
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setdX(int newX) { x += newX; }
    void setdY(int newY) { y += newY; }
    void decX() { x--; }
    void incX() { x++; }
    void incY() { y++; }
    const Matrix& getForm() const { return form; }
    int getColor() const { return color; }



    Matrix broadcast() {
        // Создаем целевую матрицу, заполненную нулями
        Matrix result(fld_height, fld_width);

        // Определяем размеры исходной матрицы
        const int src_rows = form.getRows();
        const int src_cols = form.getCols();

        // Вставляем элементы исходной матрицы
        for (int i = 0; i < src_rows; ++i) {
            for (int j = 0; j < src_cols; ++j) {
                const int target_x = x + j;
                const int target_y = y + i;
                // Проверяем, что координаты в пределах целевой матрицы
                if (target_x >= 0 && target_x < fld_width &&
                    target_y >= 0 && target_y < fld_height) {
                    result(target_y, target_x) = form(i, j);
                }
            }
        }

        return result;
}
};

class Tetris: public Program{
    Figure cur_fig;
    Matrix stat_field;
    Matrix cur_field;
    unsigned time = 0;
    int last_input = 0;

    public:
    Tetris(){
        cur_fig = Figure();
        stat_field = Matrix(Default_f);
        cur_field = stat_field + cur_fig.broadcast(); 
    }

    int next_turn(const int inputs){

        cur_fig.setdX(inputs);

        Matrix cur_fig_matrix = cur_fig.broadcast();
        

        // BOLT if (cur_fig_matrix.getRows() != stat_field.getRows() || cur_fig_matrix.getCols() != stat_field.getCols()) {
        // BOLT     throw std::invalid_argument("Matrix dimensions must agree for addition");
        // BOLT }
        

        Matrix result(fld_height, fld_width);

        for (size_t i = 0; i < fld_height; ++i) {


            for (size_t j = 0; j < fld_width; ++j) {
                if(cur_fig_matrix(i, j) != 0 && stat_field(i, j)!=0){
                    if(i == 0) return 1; // GAMEOVER значит что текущий блок столкнулся с блоком окружения наход. на 0 высоте

                    if(stat_field(i - 1, j) == 0){
                        stat_field = cur_field;
                        cur_fig = Figure(3,0);
                        cur_field = stat_field + cur_fig.broadcast(); 
                        return 0;
                    }

                    else{
                        cur_fig.setdX(-inputs);
                        return 0;
                    }


                }
                else result(i, j) = cur_fig_matrix(i, j) + stat_field(i, j);


            }
        }
        cur_field = result;
        return 0;

    }






    std::optional<Picture> update(unsigned delta_us, const Input& inp)
    {
        if (inp.size()) last_input = inp[0] - 1;

        time += delta_us;

        if (time > 1000000)
        {
            time = 0;
        
            cur_fig.incY();
            if(next_turn(last_input)){
                // ABOB std::cout << "GAMEOVER" << std::endl;
                return std::nullopt;
            }

            last_input = 0;
        }
        // ABOB std::cout<<cur_field;
        Picture pic{};        

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (cur_field(ROWS - 1 - i, j))
                {
                    pic[j + i*COLS] = GREEN;
                }
            }
        }

        return pic;
    }
    
    unsigned short preferred_fps() {
        return 30;
    }
 };


/*
#ifndef main
 int main(){
    Tetris game;

    for (int i = 0; i<50;i++){
        int input;
        std::cin >> input;
        game.update(52, std::vector<int>{input});
    }


 }
#endif // main
*/
