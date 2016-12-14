attribute vec2 position;
attribute vec2 tex;

varying vec2 v_tex;

void main() {	
	gl_Position = vec4(position.xy, 0.0, 1.0);
	
	v_tex.x = tex.x;
	v_tex.y = tex.y;
}