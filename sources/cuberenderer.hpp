#ifndef CUBERENDERER_HPP
#define CUBERENDERER_HPP

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

class CubeRenderer {
public:
	CubeRenderer();
	~CubeRenderer();

	void render(float x, float y, float size, float r, float g, float b);

private:
	GLuint	m_vertexBuffer;
	GLuint	m_shader;
	GLint	m_positionAttr;
	GLint	m_normalAttr;
};

#endif // CUBERENDERER_HPP
