#pragma once

#include <string>
#include <vector>

#include <godot_cpp/variant/vector2.hpp>

namespace godot
{
struct Vector2;
}

namespace treesim
{

struct Block
{
    double dirty = 0;

    godot::Vector2 com = godot::Vector2(0.5, 0.5);

    godot::Vector2 v;

    double wet = 0;

    double woody = 0;

    std::string toString() const;
};

struct World
{
    World();

    void reset();

    void physics(double delta, double gravity);

    Block& b(int x, int y);

    static constexpr int HEIGHT = 40;
    static constexpr int WIDTH = 40;

    std::vector<Block> blocks;
};

}
