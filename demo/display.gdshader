shader_type canvas_item;

uniform float[1600] world;

void fragment() {
	int width = 40;
	int height = 40;
	int x = int(UV.x * float(width));
	int y = int(UV.y * float(height));

	int i = int(y * width + x) % world.length();
	float r = 0.0;
	float g = 0.0;
	float b = world[i];
	COLOR = vec4(r, g, b, 1.0);
	//COLOR = vec4(1.0, 0.0, 0.0, 1.0);

	//int i = int(y * 10.0 + UV.x) % world.length();
	//float r = 0.0;
	//float g = 0.0;
	//float b = world[90];

	//COLOR = vec4(r,g,b,1.0);
	//COLOR = vec4(5,13,1,1);
	//COLOR = vec4(UV.x, UV.y, 0.5, 1.0);
}