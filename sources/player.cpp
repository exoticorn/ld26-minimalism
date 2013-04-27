#include "player.hpp"
#include "cuberenderer.hpp"
#include "input.hpp"
#include <math.h>

Player::Player(float x, float y) {
	m_posX = x;
	m_posY = y;
	m_speedX = m_speedY = 0;
	m_size = 0.2f;
	m_isAiming = false;
	m_pAttachedCube = 0;
}

void Player::updateFree(float timeStep) {
	m_speedY -= timeStep * 1.5f;
	m_posX += m_speedX * timeStep;
	m_posY += m_speedY * timeStep;
}

void Player::updateAttached(float timeStep, float speedX, const Input& input) {
	m_speedX = speedX;
	m_speedY = 0;

	if(input.pressed)
		m_isAiming = true;
	else if(m_isAiming == true) {
		m_isAiming = false;
		m_pAttachedCube = 0;
		float dx = input.x - m_posX;
		float dy = input.y - m_posY;
		float l = sqrtf(dx * dx + dy * dy);
		m_speedX += dx / l * 4;
		m_speedY += dy / l * 4;
	}
}

void Player::render(CubeRenderer& renderer) {
	renderer.render(m_posX, m_posY, m_size, 1, 0.4f, 0.4f);
}
