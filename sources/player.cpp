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
	m_aimX = 0;
	m_aimY = 0;
	m_pAttachedCube = 0;
	m_normalX = 0;
	m_normalY = 0;
}

void Player::updateFree(float timeStep) {
	m_speedY -= timeStep * 2;
	m_posX += m_speedX * timeStep;
	m_posY += m_speedY * timeStep;
}

void Player::updateAttached(float timeStep, float speedX, const Input& input) {
	m_speedX = speedX;
	m_speedY = 0;

	if(input.pressed) {
		m_isAiming = true;
		m_aimX = input.x - m_posX;
		m_aimY = input.y - m_posY;
		float dot = m_aimX * m_normalX + m_aimY * m_normalY;
		if(dot <= 0.05f) {
			float f = 0.05f - dot;
			m_aimX += m_normalX * f;
			m_aimY += m_normalY * f;
		}
	} else if(m_isAiming == true) {
		m_isAiming = false;
		m_pAttachedCube = 0;
		getJumpSpeed(&m_speedX, &m_speedY);
	}
}

void Player::getJumpSpeed(float* pX, float* pY) const {
	float l = sqrtf(m_aimX * m_aimX + m_aimY * m_aimY);
	float speed = l * 1.25f;
	if(speed > 5) {
		speed = 5;
	}
	*pX = m_speedX + m_aimX / l * speed;
	*pY = m_speedY + m_aimY / l * speed;
}

void Player::render(CubeRenderer& renderer) {
	renderer.render(m_posX, m_posY, m_size, 1, 0.4f, 0.4f);

	if(m_isAiming) {
		float px = m_posX;
		float py = m_posY;
		float sx, sy;
		getJumpSpeed(&sx, &sy);

		float timeStep = 0.3f;
		for(int i = 0; i < 15; ++i) {
			sy -= timeStep * 2;
			px += sx * timeStep;
			py += sy * timeStep;
			renderer.render(px, py, 0.1f, 1, 1, 1);
		}
	}
}
