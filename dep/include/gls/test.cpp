#include <vector>
#include <sstream>
#include <cstring>

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#define GLS_ENABLE_GLM_INTEROP
#include "gls.hpp"

#define R 255,0,0
#define G 0,255,0
#define B 0,0,255
static const GLubyte texels[] = {
    R,R,R,R,R,R,R,R,
    G,G,G,G,G,G,G,G,
    B,B,B,B,B,B,B,B,
    R,R,R,R,R,R,R,R,
    G,G,G,G,G,G,G,G,
    B,B,B,B,B,B,B,B,
    R,R,R,R,R,R,R,R,
    G,G,G,G,G,G,G,G, };
static const GLchar* vSource = 
	"#version 330\n"
	"in vec3 v_Position;"
	"in vec2 v_TexCoord;"
	"out vec2 f_TexCoord;"
	"void main() {"
	"   f_TexCoord = v_TexCoord;"
	"   gl_Position = vec4(v_Position, 1.0);"
	"}";
static const GLchar* fSource = 
	"#version 330\n"
	"uniform float u_Time;"
	"uniform sampler2D u_Sampler;"
	"in vec2 f_TexCoord;"
	"out vec4 gl_Color;"
	"void main() {"
	"   gl_Color = texture2D(u_Sampler, f_TexCoord) * vec4(sin(u_Time));"
	"}";

int main() {
	GLFWwindow* window;

	glfwInit();
	window = glfwCreateWindow(500, 500, "Example", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();

#if 1
	const GLfloat vertices[] = {
		-1,-1,+0, +0,+0,
		+0,+1,+0, +0,+1,
		+1,-1,+0, +1,+0,
	};
	auto vBuffer = gls::BufferCreate(GL_DYNAMIC_DRAW, sizeof(vertices), vertices);

	auto image = gls::TextureImageInfo{
		GL_RGB,
		GL_UNSIGNED_BYTE,
		8, 8, 0,
		texels
	};
	auto texture = gls::TextureCreate2D(GL_RGB, image);

	auto shaders = std::vector<gls::ProgramShaderInfo>{
		{ GL_VERTEX_SHADER, vSource },
		{ GL_FRAGMENT_SHADER, fSource },
	};
	auto program = gls::ProgramCreate(shaders);
#else
	const GLfloat vertices[] = {
		-1,-1,+0, +0,+0,
		+0,+1,+0, +0,+1,
		+1,-1,+0, +1,+0,
	};
	auto vBuffer = gls::BufferCreate();
	gls::BufferBind(GL_ARRAY_BUFFER, vBuffer);
	gls::BufferData(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW, sizeof(vertices), vertices);
	gls::BufferBind(GL_ARRAY_BUFFER, 0);

	auto image = gls::TextureImageInfo{
		GL_RGB,
		GL_UNSIGNED_BYTE,
		8, 8, 0,
		texels
	};
	auto texture = gls::TextureCreate();
	gls::TextureActive(GL_TEXTURE0 + 0);
	gls::TextureBind(GL_TEXTURE_2D, texture);
	gls::TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	gls::TextureParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gls::TextureGenerateMipmap(GL_TEXTURE_2D);
	gls::TextureImage2D(GL_TEXTURE_2D, GL_RGB, image);

	auto vShader = gls::ShaderCreate(GL_VERTEX_SHADER);
	gls::ShaderSource(vShader, vSource);
	gls::ShaderCompile(vShader);

	auto fShader = gls::ShaderCreate(GL_FRAGMENT_SHADER);
	gls::ShaderSource(fShader, fSource);
	gls::ShaderCompile(fShader);

	auto program = gls::ProgramCreate();
	gls::ProgramAttach(program, vShader);
	gls::ProgramAttach(program, fShader);
	gls::ProgramLink(program);
	gls::ShaderDestroy(fShader);
	gls::ShaderDestroy(vShader);
#endif
	auto vArray = gls::VertexArrayCreate();
	gls::BufferBind(GL_ARRAY_BUFFER, vBuffer);
	gls::VertexArrayBind(vArray);
	gls::VertexArrayAttributeEnable(0);
	gls::VertexArrayAttributePointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
	gls::VertexArrayAttributeEnable(1);
	gls::VertexArrayAttributePointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const GLvoid*)(sizeof(GLfloat) * 2));
	gls::VertexArrayBind(0);

	while(!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		const float elapsed = glfwGetTime();

		gls::Clear(GL_COLOR_BUFFER_BIT);

		gls::ProgramBind(program);
		gls::ProgramUniform("u_Time", elapsed);
		gls::ProgramUniform("u_Sampler", 0);
		gls::VertexArrayBind(vArray);
		gls::TextureBind(GL_TEXTURE_2D, texture);

		gls::DrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	gls::VertexArrayDestroy(vArray);
	gls::ProgramDestroy(program);
	gls::TextureDestroy(texture);
	gls::BufferDestroy(vBuffer);

	return 0;
}
