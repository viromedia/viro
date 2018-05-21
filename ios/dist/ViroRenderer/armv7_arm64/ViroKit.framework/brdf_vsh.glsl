#version 300 es
layout (location = 0) in highp vec3 position;
layout (location = 1) in highp vec2 texCoords;
out highp vec2 v_texcoord;

void main() {
    v_texcoord = texCoords;
    gl_Position = vec4(position, 1.0);
}