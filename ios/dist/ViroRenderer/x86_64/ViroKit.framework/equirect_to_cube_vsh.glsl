#version 300 es

in vec3 position;
out vec3 v_world_position;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

void main() {
    v_world_position = position;
	gl_Position = projection_matrix * view_matrix * vec4(position, 1.0);
}
