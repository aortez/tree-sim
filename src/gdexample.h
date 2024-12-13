#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <godot_cpp/classes/sprite2d.hpp>

namespace godot {

class GDExample : public Sprite2D {
	GDCLASS(GDExample, Sprite2D)

private:
	double amplitude;
	double time_emit;
	double time_passed;
	double speed;

public:
	void set_amplitude(const double p_amplitude);
	double get_amplitude() const;

	void set_speed(const double p_amplitude);
	double get_speed() const;

protected:
	static void _bind_methods();

public:
	GDExample();
	~GDExample();

	void _process(double delta) override;
};

}

#endif
