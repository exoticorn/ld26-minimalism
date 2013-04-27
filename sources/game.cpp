#include "game.hpp"
#include "levelcube.hpp"
#include <stdlib.h>

Game::Game() {
	m_pFirstCube = 0;
	m_numCubes = 0;
	push(new LevelCube(0, -3, 0));
}

Game::~Game() {
	while(m_pFirstCube != 0)
		remove(m_pFirstCube);
}

static float frand() {
	return rand() / (float)RAND_MAX;
}

void Game::update(float timeStep) {
	for(LevelCube* pCube = m_pFirstCube; pCube != 0;) {
		pCube->update(timeStep);
		if(pCube->canBeDeleted())
			pCube = remove(pCube);
		else
			pCube = pCube->m_pNext;
	}
	if(m_numCubes < 6) {
		float dir = frand() < 0.5f ? -1 : 1;
		push(new LevelCube(dir * -8, frand() * 12 - 6, dir * (frand() + 0.5f)));
	}
}

void Game::render(CubeRenderer& renderer) {
	for(LevelCube* pCube = m_pFirstCube; pCube != 0; pCube = pCube->m_pNext)
		pCube->render(renderer);
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
