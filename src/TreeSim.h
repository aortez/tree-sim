#pragma once

#include "World.h"

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

    treesim::World world;

protected:
    static void _bind_methods();

public:
    TreeSim();
    ~TreeSim();

    void _process(double delta) override;
};

}
