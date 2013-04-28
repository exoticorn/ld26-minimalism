#include "levelcube.hpp"
#include "cuberenderer.hpp"
#include <math.h>

LevelCube::LevelCube(float x, float y, float sx, CubeType type)
{
	m_type = type;
	m_posX = x;
	m_posY = y;
	m_speedX = sx;
	m_pPrev = 0;
	m_pNext = 0;
	m_size = 1;
	m_time = 0;
}

void LevelCube::update(float timeStep, bool isPlayerAttached) {
	m_posX += m_speedX * timeStep;
	m_time += timeStep;
	if(m_type == CubeType_Shrinking && isPlayerAttached) {
		m_size -= timeStep / 3;
		if(m_size < 0)
			m_size = 0;
	}
}

void LevelCube::render(CubeRenderer& renderer) {
	switch(m_type) {
	case CubeType_Sticky:
		renderer.render(m_posX, m_posY, m_size, 0.6f, 0.6f, 1);
		break;
	case CubeType_Bouncy:
		renderer.render(m_posX, m_posY, m_size + sinf(m_time * 10) * 0.05f, 0.5f, 1, 0.5f);
		break;
	case CubeType_Shrinking:
		renderer.render(m_posX, m_posY, m_size, 0.8f, 0.3f, 0.2f);
		break;
	}
}

bool LevelCube::canBeDeleted(float camY) const {
	if(m_size < 0.3f)
		return true;
	if(m_speedX < 0 && m_posX < -10)
		return true;
	if(m_speedX > 0 && m_posX > 10)
		return true;
	float dy = camY - m_posY;
	dy = dy < 0 ? -dy : dy;
	if(dy > 8)
		return true;
	return false;
}
