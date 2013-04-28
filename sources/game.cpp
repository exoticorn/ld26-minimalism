#include "game.hpp"
#include "levelcube.hpp"
#include "player.hpp"
#include "cuberenderer.hpp"
#include "input.hpp"
#include <stdlib.h>

Game::Game() {
	m_pFirstCube = 0;
	m_numCubes = 0;
	m_pPlayer = new Player(0, 1);
	push(new LevelCube(0, 0, 0));
}

Game::~Game() {
	while(m_pFirstCube != 0)
		remove(m_pFirstCube);
}

static float frand() {
	return rand() / (float)RAND_MAX;
}

static float abs(float v) { return v < 0 ? -v : v; }

void Game::update(float timeStep, const Input& input) {
	m_cameraY = m_pPlayer->m_posY + 2;

	Input worldInput;
	worldInput.x = input.x;
	worldInput.y = input.y + m_cameraY;
	worldInput.pressed = input.pressed;
	m_aimX = worldInput.x;
	m_aimY = worldInput.y;
	for(LevelCube* pCube = m_pFirstCube; pCube != 0;) {
		pCube->update(timeStep);
		if(pCube->canBeDeleted(m_cameraY) && pCube != m_pPlayer->m_pAttachedCube)
			pCube = remove(pCube);
		else
			pCube = pCube->m_pNext;
	}
	spawnCube();

	if(m_pPlayer->m_pAttachedCube == 0) {
		m_pPlayer->updateFree(timeStep);
		for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext) {
			float dx = m_pPlayer->m_posX - pCube->m_posX;
			float dy = m_pPlayer->m_posY - pCube->m_posY;
			float minDist = m_pPlayer->m_size + pCube->m_size;
			if(abs(dx) < minDist && abs(dy) < minDist) {
				m_pPlayer->m_pAttachedCube = pCube;
				if(abs(dx) > abs(dy)) {
					m_attachX = dx < 0 ? -minDist : minDist;
					m_attachY = dy;
					m_pPlayer->m_normalX = dx < 0 ? -1 : 1;
					m_pPlayer->m_normalY = 0;
				} else {
					m_attachX = dx;
					m_attachY = dy < 0 ? -minDist : minDist;
					m_pPlayer->m_normalX = 0;
					m_pPlayer->m_normalY = dy < 0 ? -1 : 1;
				}
			}
		}
	}
	if(m_pPlayer->m_pAttachedCube != 0) {
		m_pPlayer->m_posX = m_pPlayer->m_pAttachedCube->m_posX + m_attachX;
		m_pPlayer->m_posY = m_pPlayer->m_pAttachedCube->m_posY + m_attachY;
		m_pPlayer->updateAttached(timeStep, m_pPlayer->m_pAttachedCube->m_speedX, worldInput);
	}
}

void Game::spawnCube() {
	const float laneWidth = 3;
	float start = (int)((m_cameraY - 6) / laneWidth) * laneWidth;
	int lane = rand() % 12;
	float y = start + lane * laneWidth;

	float x = frand() * 16 - 8;
	float sx = frand() < 0.5f ? 0 : (frand() * 2 - 1);
	if(abs(y - m_cameraY) < 5) {
		x = x < 0 ? -8 : 8;
		sx = (x < 0 ? 1 : -1) * (frand() + 0.5f);
	}

	for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext)
		if(abs(pCube->m_posY - y) < 2 &&
				(pCube->m_speedX != 0 || sx != 0 || abs(pCube->m_posX - x) < 4))
			return;

	push(new LevelCube(x, y, sx));
}

void Game::render(CubeRenderer& renderer) {
	renderer.setCamera(0, m_pPlayer->m_posY + 2);
	renderer.render(m_aimX, m_aimY, 0.1f, 0.7f, 0.7f, 0.7f);
	for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext)
		pCube->render(renderer);
	m_pPlayer->render(renderer);
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
