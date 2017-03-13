uniform sampler2D map;
varying highp vec2 v_tex;

void main() {
    gl_FragColor = texture2D(map, v_tex);
}