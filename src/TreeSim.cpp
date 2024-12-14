#include <godot_cpp/core/class_db.hpp>

#include <cstdlib>

#include "TreeSim.h"

using namespace godot;

void TreeSim::_bind_methods() {
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

TreeSim::TreeSim() {
	amplitude = 10.0;
	speed = 1.0;
	time_emit = 1.0;
	time_passed = 0.0;

	blocks.resize(HEIGHT * WIDTH);

	for (int x = 3; x < WIDTH - 3; x++) {
		const int y = HEIGHT / 2;
		b(x, y).dirty = 1;
	}

	for (int x = 3; x < WIDTH - 3; x++) {
		const int y = 0;
		b(x, y).dirty = 1;
	}
}

TreeSim::~TreeSim() {
}

TreeSim::Block& TreeSim::b(int x, int y) {
	return blocks.at(x + y * WIDTH);
}

TreeSim::BlockDisplayType TreeSim::Block::render() const {
	if (dirty > 0.99) {
		return BlockDisplayType::DirtHeavy;
	} else if (dirty > 0.3) {
		return BlockDisplayType::DirtMedium;
	} else if (dirty > 0.01) {
		return BlockDisplayType::DirtLight;
	} else {
		return BlockDisplayType::Air;
	}
}

namespace {
TreeSim::Block& chooseLeastDirty(TreeSim::Block& left, TreeSim::Block& right) {
	if (left.dirty == right.dirty) {
		if (std::rand() / static_cast<double>(RAND_MAX) < 0.5) {
			return left;
		}
		return right;
	}
	if (left.dirty < right.dirty) {
		return left;
	}
	return right;
}
}

void TreeSim::physics(const double delta) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (y == 0) {
				continue;
			}

			Block &centerBlock = b(x, y);

			if (centerBlock.dirty <= 0) {
				continue;
			}

			// If the block below isn't already full of dirty, make some dirt fall.
			if (Block &below = b(x, y - 1); below.dirty < 1) {
				const float percent = std::min(amplitude * 0.9 * delta, 1.0);
				const float amount = std::min(centerBlock.dirty * percent,
						1 - below.dirty);
				if (amount > 0) {
					centerBlock.dirty -= amount;
					below.dirty += amount;
					continue;
				}
			}

			// If nothing has fallen yet, allow just a little to fall to the side.
			if (centerBlock.dirty < 0.1) {
				continue;
			}
			if (x > 0 && x < (WIDTH - 1)) {
				Block &left = b(x - 1, y);
				Block &right = b(x + 1, y);
				Block &cleanest = chooseLeastDirty(left, right);

				if (centerBlock.dirty > (cleanest.dirty * 5)) {
					const float percent = std::min(amplitude * 0.2 * delta, 0.2);
					const float amount = std::min(centerBlock.dirty * percent,
							1 - cleanest.dirty);
					if (amount > 0) {
						centerBlock.dirty -= amount;
						cleanest.dirty += amount;
						continue;
					}
				}
			}
		}
	}
}

void TreeSim::_process(double delta) {
	time_passed += speed * delta;

	Vector2 new_position = Vector2(
			amplitude + (amplitude * sin(time_passed * 2.0)),
			amplitude + (amplitude * cos(time_passed * 1.5)));

	set_position(new_position);

	time_emit += delta;
	if (time_emit > 0.1) {
		emit_signal("position_changed", this, new_position);

		// Add some dirt.
		if (time_passed < 5) {
			b(WIDTH / 2, HEIGHT - 1).dirty = 1;
		}

		time_emit = 0.0;

		godot::PackedVector2Array a;
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				const Block& block = b(x, HEIGHT - y - 1);
				const godot::Vector2 v(block.dirty, block.wet);
				a.append(v);
			}
		}
		emit_signal("world_updated", this, a);
	}

	physics(speed * delta);
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
