shader_type canvas_item;

uniform vec4[1600] world;

void fragment() {
	const int width = 40;
	const int height = 40;
	const float a = 0.00392156862745098039;

    // Pullman Brown.
	const vec3 brownH = vec3(100.0 * a, 65.0 * a, 23.0 * a);

	// Saddle Brown.
	const vec3 brownM = vec3(139.0 * a, 68.0 * a, 19.0 * a);

	// Wood Brown.
	const vec3 brownL = vec3(193.0 * a, 154.0 * a, 107.0 * a);

	// For the sky.
	const vec3 blue = vec3(155.0 * a, 226.0 * a, 250.0 * a);

	int x = int(UV.x * float(width));
	int y = int(UV.y * float(height));

	int i = int(y * width + x) % world.length();
	float d = world[i][0];
	float comX = world[i].y;
	float comY = world[i].z;

	if (d > 0.05) {
		//COLOR = vec4(brownH.r, brownH.g, brownH.b, d * d * d * d);
		d = pow(d, 0.2);
		COLOR = vec4(
			brownH.r * d + (blue.r) * (1.0-d),
			brownH.g * d + (blue.g) * (1.0-d),
			brownH.b * d + (blue.b) * (1.0-d), 1.0);
	} else {
		COLOR = vec4(blue.r, blue.g, blue.b, 1.0);
	}
}