#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "program.h"
#include <set>
#include <cstdlib>

namespace pic {
    constexpr Pixel COLORS[] = {RED, GREEN, BLUE};
}

class Tetris : public Program {
    struct Piece {
        unsigned bits;
        Pixel color;
        int x, y;
        
        Piece(unsigned b, Pixel c, int x, int y) : bits(b), color(c), x(x), y(y) {}
        
        bool cell(int dx, int dy) const {
            return bits & (1 << (dy * 3 + dx));
        }
    };
    
    Picture grid{};
    Piece current{0b010010010, RED, 1, 0};
    unsigned lastMove = 0;
    
    bool collide(const Piece& p) const {
        for (int dy = 0; dy < 3; ++dy) {
            for (int dx = 0; dx < 3; ++dx) {
                if (p.cell(dx, dy)) {
                    int nx = p.x + dx, ny = p.y + dy;
                    if (nx < 0 || nx >= COLS || ny >= ROWS) return true;
                    if (ny >= 0 && grid[ny * COLS + nx] != 0) return true;
                }
            }
        }
        return false;
    }
    
    void merge(const Piece& p) {
        for (int dy = 0; dy < 3; ++dy) {
            for (int dx = 0; dx < 3; ++dx) {
                if (p.cell(dx, dy)) {
                    int nx = p.x + dx, ny = p.y + dy;
                    if (ny >= 0) grid[ny * COLS + nx] = p.color;
                }
            }
        }
    }
    
    void clearLines() {
        for (int y = 0; y < ROWS; ++y) {
            bool full = true;
            for (int x = 0; x < COLS; ++x) {
                if (grid[y * COLS + x] == 0) { full = false; break; }
            }
            if (full) {
                for (int x = 0; x < COLS; ++x) {
                    grid[y * COLS + x] = pic::COLORS[x % 3];
                }
            }
        }
    }
    
    void newPiece() {
        current = Piece{
            0b111 << (std::rand() % 3 * 3),
            pic::COLORS[std::rand() % 3],
            COLS/2 - 1,
            -2
        };
    }

public:
    Tetris() { newPiece(); }
    
    std::optional<Picture> update(unsigned delta_us, const Input& inp) override {
        auto inputs = std::set<int>(inp.begin(), inp.end());

        lastMove += delta_us;
        
        if (inputs.contains(0)) current.x--;
        if (inputs.contains(1)) current.x++;
        
        if (lastMove > 1000000 / preferredFPS()) {
            lastMove = 0;
            current.y++;
            
            if (collide(current)) {
                current.y--;
                merge(current);
                clearLines();
                newPiece();
                if (collide(current)) grid.fill((Pixel) 0);
            }
        }

        merge(current);
        return grid;
    }
    
    unsigned short preferredFPS() override {
        return 10;
    }
};

#endif // _TETRIS_H_
