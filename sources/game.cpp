#include "game.hpp"
#include "levelcube.hpp"
#include "player.hpp"
#include "cuberenderer.hpp"
#include "input.hpp"
#include "functions.hpp"
#include <stdlib.h>
#include <math.h>

Game::Game() {
	m_pFirstCube = 0;
	m_pPlayer = 0;
	m_hiscore = 0;
	for(int i = 0; i < ExplosionCount; ++i)
		m_explosionRnd[i] = frand() - 0.5f;
	reset();
}

Game::~Game() {
	while(m_pFirstCube != 0)
		remove(m_pFirstCube);
}

void Game::reset() {
	while(m_pFirstCube != 0)
		remove(m_pFirstCube);
	delete m_pPlayer;
	m_numCubes = 0;
	m_pPlayer = new Player(0, 0);
	push(new LevelCube(0, -1, 0, CubeType_Sticky));
	m_maxY = 0;
	m_cameraY = 0;
	m_explosionTime = 0;
	m_isDead = false;
}

static float abs(float v) { return v < 0 ? -v : v; }

void Game::update(float timeStep, const Input& input) {
	m_explosionTime -= timeStep;

	if(!m_isDead) {
		Input worldInput;
		worldInput.x = input.x;
		worldInput.y = input.y + m_cameraY;
		worldInput.pressed = input.pressed;
		m_aimX = worldInput.x;
		m_aimY = worldInput.y;
		for(LevelCube* pCube = m_pFirstCube; pCube != 0;) {
			pCube->update(timeStep, m_pPlayer->m_pAttachedCube == pCube);
			if(pCube->canBeDeleted(m_cameraY)) {
				if(pCube == m_pPlayer->m_pAttachedCube) {
					m_pPlayer->m_pAttachedCube = 0;
					m_pPlayer->m_speedX += m_pPlayer->m_normalX * 4;
					m_pPlayer->m_speedY += m_pPlayer->m_normalY * 4;
				}
				pCube = remove(pCube);
			} else
				pCube = pCube->m_pNext;
		}
		spawnCube();

		if(m_pPlayer->m_pAttachedCube == 0) {
			m_pPlayer->updateFree(timeStep, worldInput);
			for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext) {
				float dx = m_pPlayer->m_posX - pCube->m_posX;
				float dy = m_pPlayer->m_posY - pCube->m_posY;
				float minDist = m_pPlayer->m_size + pCube->m_size;
				if(abs(dx) < minDist && abs(dy) < minDist) {
					m_pPlayer->m_pAttachedCube = pCube;
					if(abs(dx) > abs(dy)) {
						m_attachX = dx < 0 ? -1 : 1;
						m_attachY = dy / pCube->m_size;
						m_pPlayer->m_normalX = dx < 0 ? -1 : 1;
						m_pPlayer->m_normalY = 0;
					} else {
						m_attachX = dx / pCube->m_size;
						m_attachY = dy < 0 ? -1 : 1;
						m_pPlayer->m_normalX = 0;
						m_pPlayer->m_normalY = dy < 0 ? -1 : 1;
					}
				}
			}
		}
		if(m_pPlayer->m_pAttachedCube != 0) {
			LevelCube& cube = *m_pPlayer->m_pAttachedCube;
			m_pPlayer->m_posX = cube.m_posX + m_attachX * cube.m_size + m_pPlayer->m_normalX * (m_pPlayer->m_size + 0.01f);
			m_pPlayer->m_posY = cube.m_posY + m_attachY * cube.m_size + m_pPlayer->m_normalY * (m_pPlayer->m_size + 0.01f);
			if(cube.m_type == CubeType_Bouncy || cube.m_type == CubeType_Harmful) {
				float dot = m_pPlayer->m_speedX * m_pPlayer->m_normalX +
						m_pPlayer->m_speedY * m_pPlayer->m_normalY;
				if(dot < 0) {
					m_pPlayer->m_speedX += cube.m_speedX * m_pPlayer->m_normalX * m_pPlayer->m_normalX - 2 * m_pPlayer->m_normalX * dot;
					m_pPlayer->m_speedY -= 2 * m_pPlayer->m_normalY * dot;
					m_pPlayer->m_pAttachedCube = 0;
					if(cube.m_type == CubeType_Harmful) {
						m_explosionX = cube.m_posX;
						m_explosionY = cube.m_posY;
						m_explosionTime = 1;
						remove(&cube);
						m_pPlayer->m_stamina -= 0.25f;
					}
				}
			} else
				m_pPlayer->updateAttached(timeStep, cube.m_speedX, worldInput);
		}

		m_maxY = max(m_maxY, m_pPlayer->m_posY);
		m_cameraY = m_maxY - atanf((m_maxY - m_pPlayer->m_posY) / 20) * 20 + 2;

		if(abs(m_pPlayer->m_posX) > 7.0f || m_pPlayer->m_posY - m_cameraY < -5.25f || m_pPlayer->m_stamina < 0) {
			m_isDead = true;
			m_explosionX = m_pPlayer->m_posX;
			m_explosionY = m_pPlayer->m_posY;
			m_explosionTime = 1;
		}
	} else if(m_explosionTime < 0)
		reset();
}

void Game::spawnCube() {
	float y = m_cameraY - 7 + frand() * 14;

	float x = frand() * 16 - 8;
	float sx = frand();
	sx *= sx * sx * sx;
	sx = sx * 2 - 1;
	if(abs(y - m_cameraY) < 6.5f) {
		x = x < 0 ? -8 : 8;
		sx = (x < 0 ? 1 : -1) * (frand() + 0.5f);
	}

	for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext)
		if(abs(pCube->m_posY - y) < (pCube->m_speedX * sx < 0 ? 2.5f : 4)) {
			float dx = abs(pCube->m_posX - x);
			if(dx < 3)
				return;
			float dir = pCube->m_posX < x ? -1 : 1;
			float rs = (sx - pCube->m_speedX) * dir;
			if(rs > 0) {
				float t = (dx - 2.5f) / rs;
				if(abs(pCube->m_posX + t * pCube->m_speedX) < 8)
					return;
			}
		}

	CubeType type = CubeType_Sticky;
	if(frand() < min(m_cameraY / 400 - 0.2f, 0.3f))
		type = CubeType_Harmful;
	else if(frand() < min(m_cameraY / 300 - 0.1f, 0.33f))
		type = CubeType_Bouncy;
	else if(frand() < min(m_cameraY / 100 - 0.1f, 0.85f))
		type = CubeType_Shrinking;

	push(new LevelCube(x, y, sx, type));
}

void Game::render(CubeRenderer& renderer) {
	float sky = 1 - clamp(m_cameraY / 250, 0.0f, 1.0f);
	glClearColor(0.6f * sqrtf(sky), 0.6f * sky, 1 * sky, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer.setCamera(0, m_cameraY);
	renderer.render(m_aimX, m_aimY, 0.1f, 0.7f, 0.7f, 0.7f);
	for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext)
		pCube->render(renderer);
	if(!m_isDead)
		m_pPlayer->render(renderer);

	if(m_explosionTime > 0) {
		float t = 1 - m_explosionTime;
		for(int i = 0; i < ExplosionCount; ++i) {
			renderer.render(
					m_explosionX + m_explosionRnd[i] * t * 3,
					m_explosionY + m_explosionRnd[ExplosionCount-1-i] * t * 3,
					0.2f * m_explosionTime, 1, 1, 1);
		}
	}

	renderer.setCamera(0, 0, 2);
	for(int i = 0; i < 10; ++i) {
		float alpha = min(m_pPlayer->m_stamina * 10 - i, 1.0f);
		if(alpha > 0)
			renderer.render(i * 0.4f - 4.5f, -3.5f, 0.15f, 0.6f, 1 * alpha, 0.6f * alpha);
	}

	int score = (int)m_maxY;
	m_hiscore = max(score, m_hiscore);
	drawNumber(renderer, score, 4.5f, -3.5f);
	drawNumber(renderer, m_hiscore, 4.5f, 2.8f);
}

void Game::drawNumber(CubeRenderer& renderer, int n, float x, float y) {
	while(n > 0) {
		int d = n % 10;
		n /= 10;
		renderer.render(x, y, 0.1f, 0, d >= 5 ? 0.8f : 0, 0);
		d = d % 5;
		for(int i = 0; i < d; ++i)
			renderer.render(x, y + 0.25f + i * 0.15f, 0.05f, 0.4f, 1, 1);
		x -= 0.25f;
	}
}

void Game::push(LevelCube* pCube) {
	++m_numCubes;
	pCube->m_pNext = m_pFirstCube;
	if(m_pFirstCube != 0)
		m_pFirstCube->m_pPrev = pCube;
	m_pFirstCube = pCube;
}

LevelCube* Game::remove(LevelCube* pCube) {
	--m_numCubes;
	if(pCube->m_pPrev == 0)
		m_pFirstCube = pCube->m_pNext;
	else
		pCube->m_pPrev->m_pNext = pCube->m_pNext;
	if(pCube->m_pNext != 0)
		pCube->m_pNext->m_pPrev = pCube->m_pPrev;
	LevelCube* pNext = pCube->m_pNext;
	delete pCube;
	return pNext;
}
