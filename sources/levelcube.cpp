#include "levelcube.hpp"
#include "cuberenderer.hpp"

LevelCube::LevelCube(float x, float y, float sx) {
	m_posX = x;
	m_posY = y;
	m_speedX = sx;
	m_pPrev = 0;
	m_pNext = 0;
}

void LevelCube::update(float timeStep) {
	m_posX += m_speedX * timeStep;
}

void LevelCube::render(CubeRenderer& renderer) {
	renderer.render(m_posX, m_posY, 1, 0.6f, 0.6f, 1);
}

bool LevelCube::canBeDeleted() const {
	if(m_speedX < 0 && m_posX < -8)
		return true;
	if(m_speedX > 0 && m_posX > 8)
		return true;
	return false;
}
