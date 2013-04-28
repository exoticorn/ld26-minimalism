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
		"uniform vec3 offset;\n"
		"uniform vec3 color;\n"
		"uniform float size;\n"
		"uniform vec4 projection;\n"
		"void main() {\n"
		"  gl_Position.xyw = position * size + offset;\n"
		"  gl_Position.xyz = gl_Position.xyw * projection.xyz;\n"
		"  gl_Position.z += projection.w;\n"
		"  float dot = dot(normal, vec3(-0.74, 0.557, -0.371));\n"
		"  vec3 lightColor = vec3(1.0, 1.0, 0.7) * clamp(dot, 0.0, 1.0);\n"
		"  lightColor += vec3(0.2, 0.2, 0.4) * clamp(-dot, 0.0, 1.0);\n"
		"  lightColor += 0.3;\n"
		"  vColor = color * lightColor;\n"
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
	m_offsetUniform = glGetUniformLocation(m_shader, "offset");
	m_colorUniform = glGetUniformLocation(m_shader, "color");
	m_sizeUniform = glGetUniformLocation(m_shader, "size");
	m_projectionUniform = glGetUniformLocation(m_shader, "projection");

	m_cameraX = 0;
	m_cameraY = 0;
	m_cameraZ = 0;
}

CubeRenderer::~CubeRenderer() {
}

void CubeRenderer::setCamera(float x, float y, float z) {
	m_cameraX = x;
	m_cameraY = y;
	m_cameraZ = z;
}

void CubeRenderer::render(float x, float y, float size, float r, float g, float b) {
	glUseProgram(m_shader);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(m_positionAttr);
	glEnableVertexAttribArray(m_normalAttr);
	glVertexAttribPointer(m_positionAttr, 3, GL_FLOAT, GL_FALSE, 6*4, (void*)0);
	glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, 6*4, (void*)12);
	glUniform3f(m_offsetUniform, x - m_cameraX, y - m_cameraY, 8 - m_cameraZ);
	glUniform3f(m_colorUniform, r, g, b);
	glUniform1f(m_sizeUniform, size);
	float far = 50;
	float near = 0.2f;
	glUniform4f(m_projectionUniform, 0.3f * 4, 0.4f * 4, far/(far-near), -far*near/(far-near));
	glDrawArrays(GL_TRIANGLES, 0, 12*3);
}
