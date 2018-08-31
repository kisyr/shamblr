#version 330

uniform mat4 u_Mvp;
in vec3 vs_Position;
in vec2 vs_TexCoord;
out vec2 fs_TexCoord;

void main() {
	fs_TexCoord = vs_TexCoord;
	gl_Position = u_Mvp * vec4(vs_Position, 1.0);
}
