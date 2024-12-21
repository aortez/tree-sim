#include "TreeSim.h"

#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <cassert>
#include <cstdlib>
#include <fstream>

using namespace godot;
using namespace treesim;

namespace
{
double rng()
{
    return static_cast<double>(rand()) / RAND_MAX;
}

std::string toString(const Vector2 &v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

}

void TreeSim::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_amplitude"), &TreeSim::get_amplitude);
    ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"),
            &TreeSim::set_amplitude);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude",
            "get_amplitude");

    ClassDB::bind_method(D_METHOD("get_speed"), &TreeSim::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &TreeSim::set_speed);
    ADD_PROPERTY(
            PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE,
                    "0,20,0.01"), "set_speed", "get_speed");

    ADD_SIGNAL(
            MethodInfo("position_changed",
                    PropertyInfo(Variant::OBJECT, "node"),
                    PropertyInfo(Variant::VECTOR2, "new_pos")));

    ADD_SIGNAL(
            MethodInfo("world_updated", PropertyInfo(Variant::OBJECT, "node"),
                    PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "new_world")));
}

TreeSim::TreeSim()
{
    const std::string logFilename = "treesim.log";

    std::remove(logFilename.c_str());

    plog::init(plog::verbose, logFilename.c_str());

    LOGI << "treesim initializing...";

    amplitude = 100.0;
    speed = 10.0;
    time_emit = 1.0;
    time_passed = 0.0;

}

TreeSim::~TreeSim()
{
}

void TreeSim::physics(const double delta)
{
    world.physics(delta, amplitude);
}

void TreeSim::_process(double delta)
{
    time_passed += speed * delta;

    time_emit += delta;
//    if (time_emit > 0.1)
    {
        // Add some dirt.
        world.b(static_cast<int>(time_passed * 0.2) % (WIDTH - 1), HEIGHT - 1).dirty =
                1;

        time_emit = 0.0;

        godot::PackedVector2Array a;
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = 0; x < WIDTH; x++)
            {
                const Block &block = world.b(x, HEIGHT - y - 1);
                const godot::Vector2 v(block.dirty, block.wet);
                a.append(v);
            }
        }
        emit_signal("world_updated", this, a);
    }

    physics(speed * delta);
}

void TreeSim::set_amplitude(const double p_amplitude)
{
    amplitude = p_amplitude;
}

double TreeSim::get_amplitude() const
{
    return amplitude;
}

void TreeSim::set_speed(const double p_speed)
{
    speed = p_speed;
}

double TreeSim::get_speed() const
{
    return speed;
}
