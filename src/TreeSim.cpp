#include <godot_cpp/core/class_db.hpp>

#include "TreeSim.h"

using namespace godot;

void TreeSim::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_amplitude"), &TreeSim::get_amplitude);
    ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &TreeSim::set_amplitude);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");

    ClassDB::bind_method(D_METHOD("get_speed"), &TreeSim::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &TreeSim::set_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed", PROPERTY_HINT_RANGE, "0,20,0.01"), "set_speed", "get_speed");

    ADD_SIGNAL(MethodInfo("position_changed", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::VECTOR2, "new_pos")));

    ADD_SIGNAL(MethodInfo("world_updated", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::ARRAY, "new_world")));
}

TreeSim::TreeSim() {
    amplitude = 10.0;
    speed = 1.0;
    time_emit = 0.0;
    time_passed = 0.0;
}

TreeSim::~TreeSim() {
}

void TreeSim::_process(double delta) {
    time_passed += speed * delta;

    Vector2 new_position = Vector2(
        amplitude + (amplitude * sin(time_passed * 2.0)),
        amplitude + (amplitude * cos(time_passed * 1.5))
    );

    emit_signal("position_changed", this, new_position);
    set_position(new_position);

    time_emit += delta;
    if (time_emit > 1.0) {
        emit_signal("position_changed", this, new_position);
        time_emit = 0.0;
    }

    const int width = 40;
    const int height = 40;

    godot::Array a;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            a.append(float(y) / float(height));
        }
    }

    emit_signal("world_updated", this, a);
}

void TreeSim::set_amplitude(const double p_amplitude) {
    amplitude = p_amplitude;
}

double TreeSim::get_amplitude() const {
    return amplitude;
}

void TreeSim::set_speed(const double p_speed) {
    speed = p_speed;
}

double TreeSim::get_speed() const {
    return speed;
}
