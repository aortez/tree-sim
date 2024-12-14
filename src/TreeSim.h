#pragma once

#include <godot_cpp/classes/sprite2d.hpp>

#include <vector>

namespace godot {

class TreeSim : public Sprite2D {
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

    enum class BlockDisplayType { Air, DirtHeavy, DirtMedium, DirtLight, Water };

    struct Block {
    	BlockDisplayType render() const;

    	float dirty = 0;

    	float vx = 0;

		float vy = 0;

    	float wet = 0;
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
