#version 450

out vec4 fragColor;

uniform vec2 u_pos;
uniform vec2 u_offset;
uniform float u_max_iter;
uniform float u_zoom;
uniform sampler2D u_gradient;

vec4 maldebrot(vec2 pos)
{
    vec2 c = vec2( (pos.x - u_offset.x) * u_zoom - u_pos.x, (pos.y - u_offset.y) * u_zoom - u_pos.y );
    vec2 z = vec2(0.0);
	float num_iter = 0;
    for (float i = 0.0; i < u_max_iter; i+=1)
    {
        z = vec2( (pow(z.x, 2) - pow(z.y, 2) + c.x), (2 * z.x * z.y + c.y));
        if (pow(z.x, 2) + pow(z.y, 2) > 4)
            break;
        num_iter++;
    }
    vec2 col = vec2(num_iter / u_max_iter, num_iter / u_max_iter);
	return texture2D(u_gradient, col);
}

void main()
{
	fragColor = maldebrot(gl_FragCoord.xy);
}