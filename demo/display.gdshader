shader_type canvas_item;

uniform int[1600] world;

void fragment() {
	const int width = 40;
	const int height = 40;
	const float a = 0.00392156862745098039;

    // Pullman Brown.
	const vec3 brownH = vec3(100.0*a,65.0*a,23.0*a);

	// Saddle Brown.
	const vec3 brownM = vec3(139.0*a,68.0*a,19.0*a);

	// Wood Brown.
	const vec3 brownL = vec3(193.0*a,154.0*a,107.0*a);

	const vec3 blue = vec3(155.0*a,226.0*a,250.0*a);

	int x = int(UV.x * float(width));
	int y = int(UV.y * float(height));

	int i = int(y * width + x) % world.length();
	if (world[i] == 0) {
		COLOR = vec4(blue.r, blue.g, blue.b, 1.0);
	} else if (world[i] == 1) {
		COLOR = vec4(brownH.r, brownH.g, brownH.b, 1.0);
	} else if (world[i] == 2) {
		COLOR = vec4(brownM.r, brownM.g, brownM.b, 1.0);
	} else if (world[i] == 3) {
		COLOR = vec4(brownL.r, brownL.g, brownL.b, 1.0);
	} else {
		COLOR = vec4(1.0, 0.0, 0.0, 1.0);
	}
}