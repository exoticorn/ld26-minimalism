#include "levelcube.hpp"
#include "cuberenderer.hpp"

LevelCube::LevelCube(float x, float y, float sx) {
	m_posX = x;
	m_posY = y;
	m_speedX = sx;
	m_pPrev = 0;
	m_pNext = 0;
	m_size = 1;
}

void LevelCube::update(float timeStep) {
	m_posX += m_speedX * timeStep;
}

void LevelCube::render(CubeRenderer& renderer) {
	renderer.render(m_posX, m_posY, m_size, 0.6f, 0.6f, 1);
}

bool LevelCube::canBeDeleted(float camY) const {
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
