#version 330

uniform mat4 u_mvp;
in vec3 v_position;
in vec2 v_texcoord;
out vec2 f_texcoord;

void main() {
	f_texcoord = v_texcoord;
	gl_Position = u_mvp * vec4(v_position, 1.0);
}
