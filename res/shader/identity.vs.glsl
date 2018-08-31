#version 330

in vec4 vs_Position;
in vec2 vs_Texture;
out vec2 fs_Texture;

void main() {
	gl_Position = vs_Position;
	fs_Texture = vs_Texture;
}
