#include "cuberenderer.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const float s_vertices[6*2*3*6] = {
	-1, -1, -1,		0, -1, 0,
	1, -1, -1,		0, -1, 0,
	-1, -1, 1,		0, -1, 0,

	1, -1, -1,		0, -1, 0,
	1, -1, 1,		0, -1, 0,
	-1, -1, 1,		0, -1, 0,

	-1, 1, -1,		0, 0, -1,
	1, -1, -1,		0, 0, -1,
	-1, -1, -1,		0, 0, -1,

	-1, 1, -1,		0, 0, -1,
	1, 1, -1,		0, 0, -1,
	1, -1, -1,		0, 0, -1,

	-1, 1, 1,		0, 0, 1,
	-1, -1, 1,		0, 0, 1,
	1, -1, 1,		0, 0, 1,

	-1, 1, 1,		0, 0, 1,
	1, -1, 1,		0, 0, 1,
	1, 1, 1,		0, 0, 1,

	-1, 1, 1,		0, 1, 0,
	1, 1, -1,		0, 1, 0,
	-1, 1, -1,		0, 1, 0,

	-1, 1, 1,		0, 1, 0,
	1, 1, 1,		0, 1, 0,
	1, 1, -1,		0, 1, 0,

	-1, 1, 1,		-1, 0, 0,
	-1, -1, -1,		-1, 0, 0,
	-1, -1, 1,		-1, 0, 0,

	-1, 1, 1,		-1, 0, 0,
	-1, 1, -1,		-1, 0, 0,
	-1, -1, -1,		-1, 0, 0,

	1, 1, -1,		1, 0, 0,
	1, -1, 1,		1, 0, 0,
	1, -1, -1,		1, 0, 0,

	1, 1, -1,		1, 0, 0,
	1, 1, 1,		1, 0, 0,
	1, -1, 1,		1, 0, 0
};

#ifdef EMSCRIPTEN
#	define LOWP " lowp "
#	define MEDIUMP " mediump "
#else
#	define LOWP " "
#	define MEDIUMP " "
#endif

static const char* s_vertexShader =
		"varying" LOWP "vec3 vColor;\n"
		"attribute vec3 position;\n"
		"attribute vec3 normal;\n"
		"void main() {\n"
		"  gl_Position.xyz = position; gl_Position.w = position.z + 5.0;\n"
		"  gl_Position.x += 2.0; gl_Position.y -= 2.5;\n"
		"  float light = clamp(dot(normal, vec3(-0.74, 0.557, -0.371)), 0.0, 1.0);\n"
		"  vColor = vec3(1.0, 1.0, 0.7) * light;\n"
		"}";

static const char* s_fragmentShader =
		"varying" LOWP "vec3 vColor;\n"
		"void main() {\n"
		"  gl_FragColor = vec4(vColor, 1.0);\n"
		"}";

static GLuint compileShader(GLuint type, const char* pSource) {
	GLint length = strlen(pSource);
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &pSource, &length);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE) {
		char log[1024];
		glGetShaderInfoLog(shader, sizeof(log), 0, log);
		printf("Shader compiler error:\n%s\n", log);
		exit(1);
	}
	return shader;
}

CubeRenderer::CubeRenderer() {
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);

	GLuint vshader = compileShader(GL_VERTEX_SHADER, s_vertexShader);
	GLuint fshader = compileShader(GL_FRAGMENT_SHADER, s_fragmentShader);
	m_shader = glCreateProgram();
	glAttachShader(m_shader, vshader);
	glAttachShader(m_shader, fshader);
	glLinkProgram(m_shader);
	GLint status;
	glGetProgramiv(m_shader, GL_LINK_STATUS, &status);
	if(status != GL_TRUE) {
		char log[1024];
		glGetProgramInfoLog(m_shader, sizeof(log), 0, log);
		printf("Shader link error:\n%s\n", log);
		exit(1);
	}

	m_positionAttr = glGetAttribLocation(m_shader, "position");
	m_normalAttr = glGetAttribLocation(m_shader, "normal");
}

CubeRenderer::~CubeRenderer() {
}

void CubeRenderer::render(float x, float y, float size, float r, float g, float b) {
	glUseProgram(m_shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(m_positionAttr);
	glEnableVertexAttribArray(m_normalAttr);
	glVertexAttribPointer(m_positionAttr, 3, GL_FLOAT, GL_FALSE, 6*4, (void*)0);
	glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, 6*4, (void*)12);
	glDrawArrays(GL_TRIANGLES, 0, 12*3);
}
