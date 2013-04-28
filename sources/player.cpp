#include "player.hpp"
#include "cuberenderer.hpp"
#include "input.hpp"
#include "functions.hpp"
#include <math.h>

Player::Player(float x, float y) {
	m_posX = x;
	m_posY = y;
	m_speedX = m_speedY = 0;
	m_size = 0.2f;
	m_isAiming = false;
	m_blockAiming = 0;
	m_aimX = 0;
	m_aimY = 0;
	m_pAttachedCube = 0;
	m_normalX = 0;
	m_normalY = 0;
	m_particleDelay = 0;
	for(int i = 0; i < NumParticles; ++i)
		m_particles[i].lifetime = 0;
	m_stamina = 1;
	m_attachedTime = 0;
}

void Player::updateFree(float timeStep, const Input& input) {
	m_speedY -= timeStep * gravity();
	if(input.pressed) {
		float sx = input.x - m_posX;
		float sy = input.y - m_posY;
		float l = sqrtf(sx*sx + sy*sy);
		float f = l / 2;
		if(f > 2)
			f = 2;
		m_speedX += timeStep * sx / l * f;
		m_speedY += timeStep * sy / l * f;
		m_particleDelay -= timeStep;
		while(m_particleDelay < 0) {
			int i = 0;
			for(int j = 1; j < NumParticles; ++j)
				if(m_particles[j].lifetime < m_particles[i].lifetime)
					i = j;
			m_particles[i].x = m_posX;
			m_particles[i].y = m_posY;
			m_particles[i].dx = m_speedX - sx / l * f * 4 + frand(-1, 1);
			m_particles[i].dy = m_speedY - sy / l * f * 4 + frand(-1, 1);
			m_particles[i].lifetime = 1;
			m_particleDelay += 0.04f;
		}
		m_stamina -= timeStep * 0.05f;
	} else
		m_stamina = min(1.0f, m_stamina + timeStep * (0.0175f - spaceFactor() * 0.00075f));
	m_posX += m_speedX * timeStep;
	m_posY += m_speedY * timeStep;
	updateParticles(timeStep);
	m_blockAiming = 0.25f;
	m_attachedTime = 0;
}

void Player::updateAttached(float timeStep, float speedX, const Input& input) {
	m_speedX = speedX;
	m_speedY = 0;

	m_blockAiming -= timeStep;
	m_attachedTime += timeStep;

	if(m_attachedTime > (m_normalY + 2) * (m_normalY + 2) * (0.25f - spaceFactor() * 0.1f) && m_posY > 1)
		m_stamina += timeStep * (m_normalY - 1) * (0.05f + spaceFactor() * 0.03f);

	if(!input.pressed || m_blockAiming <= 0) {
		m_blockAiming = 0;
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

	updateParticles(timeStep);
}

void Player::updateParticles(float timeStep) {
	for(int i = 0; i < NumParticles; ++i) {
		m_particles[i].lifetime -= timeStep * 2;
		if(m_particles[i].lifetime > 0) {
			m_particles[i].x += m_particles[i].dx * timeStep;
			m_particles[i].y += m_particles[i].dy * timeStep;
		}
	}
}

void Player::getJumpSpeed(float* pX, float* pY) const {
	float l = sqrtf(m_aimX * m_aimX + m_aimY * m_aimY);
	float speed = l * 1.25f / 5;
	if(speed > 1) {
		speed = 1;
	}
	speed *= 5 - spaceFactor() * 1.75f;
	*pX = m_speedX + m_aimX / l * speed;
	*pY = m_speedY + m_aimY / l * speed;
}

float Player::spaceFactor() const {
	return clamp(m_posY / 250, 0.0f, 1.0f);
}

void Player::render(CubeRenderer& renderer) {
	renderer.render(m_posX, m_posY, m_size, 1, 0.4f, 0.4f);

	for(int i = 0; i < NumParticles; ++i) {
		if(m_particles[i].lifetime > 0) {
			renderer.render(m_particles[i].x, m_particles[i].y, m_particles[i].lifetime * 0.1f, 1, 1, 1);
		}
	}

	if(m_isAiming) {
		float px = m_posX;
		float py = m_posY;
		float sx, sy;
		getJumpSpeed(&sx, &sy);

		float timeStep = 0.3f;
		for(int i = 0; i < 15; ++i) {
			sy -= timeStep * gravity();
			px += sx * timeStep;
			py += sy * timeStep;
			renderer.render(px, py, 0.1f, 1, 1, 1);
		}
	}
}
