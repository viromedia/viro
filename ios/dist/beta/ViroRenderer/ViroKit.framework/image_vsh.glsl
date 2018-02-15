#version 300 es

in vec2 position;
in vec2 texcoord;

out vec2 v_texcoord;

void main() {
    v_texcoord = texcoord;
	gl_Position = vec4(position.xy, 0.0, 1.0);
}
