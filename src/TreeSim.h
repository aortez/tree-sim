#pragma once

//#include "vec2.h"

#include <godot_cpp/classes/sprite2d.hpp>

#include <vector>

namespace godot
{

struct Vector2;

class TreeSim: public Sprite2D
{
GDCLASS(TreeSim, Sprite2D)

private:
    void physics(double delta);

    double amplitude;
    double speed;
    double time_emit;
    double time_passed;

    static constexpr int HEIGHT = 40;
    static constexpr int WIDTH = 40;

public:
    void set_amplitude(const double p_amplitude);
    double get_amplitude() const;

    void set_speed(const double p_amplitude);
    double get_speed() const;

    struct Block
    {
        double dirty = 0;

        Vector2 com = Vector2(0.5, 0.5);

        Vector2 v;

        double wet = 0;

        std::string toString() const;
    };

    Block& b(int x, int y);

    std::vector<Block> blocks;

protected:
    static void _bind_methods();

public:
    TreeSim();
    ~TreeSim();

    void _process(double delta) override;
};

}
