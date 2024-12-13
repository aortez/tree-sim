shader_type canvas_item;

uniform sampler2D tex;
uniform vec2 center;
uniform float scale;
uniform int iter;

//void vertex() {
	//// Called for every vertex the material is visible on.
//}
//void fragment() {
    //COLOR = vec4(UV.x, UV.y, 0.5, 1.0);
//}

void vertex() {
    //VERTEX.x += (10.0 * sin(TIME)); // offset vertex x by sine function on time elapsed
	//VERTEX.y += (10.0 * cos(TIME)); // offset vertex x by sine function on time elapsed
}

void fragment() {
    vec2 z, c;
	int i2;
	int i = 0;

    c = vec2(1.3333, 1.0) * (UV - 0.5) * scale - center;

    z = c;
	while (i < iter) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0)
		{
			i2 = i;
			break;
		}
        z = vec2(x, y);

		i++;
    }

    //COLOR = vec4(UV.x, UV.y, 0.5, 1.0);
	float tIndex = 0.0;
	if (i2 != iter) {
		tIndex = float(i2) / 100.0;
	}
    COLOR = texture(tex, vec2(tIndex, 0.0));
}

//void light() {
	// Called for every pixel for every light affecting the CanvasItem.
	// Uncomment to replace the default light processing function with this one.
//}
