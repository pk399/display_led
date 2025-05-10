#include "program.h"
#include "picture.h"
#include <array>
#include <utility>
#include <vector>

constexpr int COLS = 5;
constexpr int ROWS = 6;

class Tetris : public Program {
private:
    struct Point { int x, y; };
    
    std::array<Pixel, ROWS*COLS> grid{};
    std::vector<Point> currentPiece;
    Point piecePos;
    bool gameOver = false;

    // Фиксированные фигуры из 3 пикселей
    const std::vector<std::vector<Point>> SHAPES = {
        {{0,0}, {1,0}, {2,0}},  // Горизонтальная линия
        {{0,0}, {0,1}, {0,2}},  // Вертикальная линия
        {{0,0}, {1,0}, {0,1}},  // Уголок
        {{0,0}, {1,0}, {1,1}}   // Блок с выступом
    };

public:
    Tetris() {
        resetGame();
    }

    std::pair<bool, std::array<Pixel, ROWS*COLS>> update(unsigned delta_us, Input inputs) override {
        if (gameOver) return std::nullopt;

        // Только движение влево/вправо
        if (inputs[0] && canMove(-1, 0)) piecePos.x--;  // Влево (кнопка 0)
        if (inputs[1] && canMove(1, 0))  piecePos.x++;   // Вправо (кнопка 1)

        // Автоматическое падение
        static unsigned int fallTimer = 0;
        fallTimer += delta_us;
        if (fallTimer > 1000000) {  // Падение каждую секунду (1,000,000 мкс)
            fallTimer = 0;
            if (!moveDown()) {
                lockPiece();
                spawnPiece();
            }
        }

        updateGrid();
        return grid;
    }

    unsigned short preferredFPS() override {
        return 10;  // 10 FPS достаточно для такой простой игры
    }

private:
    void resetGame() {
        grid.fill(static_cast<Pixel>(0));
        gameOver = false;
        spawnPiece();
    }

    void spawnPiece() {
        static int shapeIndex = 0;
        currentPiece = SHAPES[shapeIndex++ % SHAPES.size()];
        piecePos = {COLS/2 - 1, 0};  // Стартовая позиция по центру
        
        if (checkCollision()) gameOver = true;
    }

    bool canMove(int dx, int dy) const {
        for (auto& p : currentPiece) {
            int x = piecePos.x + p.x + dx;
            int y = piecePos.y + p.y + dy;
            if (x < 0 || x >= COLS || y >= ROWS || (y >= 0 && grid[y*COLS + x] != 0))
                return false;
        }
        return true;
    }

    bool moveDown() {
        if (canMove(0, 1)) {
            piecePos.y++;
            return true;
        }
        return false;
    }

    void lockPiece() {
        for (auto& p : currentPiece) {
            int x = piecePos.x + p.x;
            int y = piecePos.y + p.y;
            if (y >= 0) grid[y*COLS + x] = RED;  // Фиксируем фигуры красным
        }
        clearLines();
    }

    void clearLines() {
        for (int y = ROWS-1; y >= 0; y--) {
            bool full = true;
            for (int x = 0; x < COLS; x++)
                if (grid[y*COLS + x] == 0) full = false;
            
            if (full) {
                for (int yy = y; yy > 0; yy--)
                    for (int x = 0; x < COLS; x++)
                        grid[yy*COLS + x] = grid[(yy-1)*COLS + x];
                
                for (int x = 0; x < COLS; x++)
                    grid[x] = static_cast<Pixel>(0);
            }
        }
    }

    bool checkCollision() const {
        return !canMove(0, 0);
    }

    void updateGrid() {
        // Очищаем только незафиксированные пиксели
        for (int i = 0; i < grid.size(); i++) {
            if (grid[i] == GREEN) grid[i] = static_cast<Pixel>(0);  // Стираем предыдущее положение
        }
        
        // Рисуем текущую фигуру зеленым
        for (auto& p : currentPiece) {
            int x = piecePos.x + p.x;
            int y = piecePos.y + p.y;
            if (y >= 0 && x >= 0 && x < COLS && y < ROWS)
                grid[y*COLS + x] = GREEN;
        }
    }
};
