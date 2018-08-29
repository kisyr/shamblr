#version 330

uniform sampler2D u_sampler;
in vec2 f_texcoord;
out vec4 gl_Color;

void main() {
	gl_Color = vec4(0);
	gl_Color += texture2D(u_sampler, f_texcoord);
}
