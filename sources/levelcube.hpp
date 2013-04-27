#ifndef LEVELCUBES_HPP
#define LEVELCUBES_HPP

class CubeRenderer;

class LevelCube {
public:
	LevelCube(float x, float y, float sx);

	void		update(float timeStep);
	void		render(CubeRenderer& renderer);

	bool		canBeDeleted(float camY) const;

private:
	float		m_size;
	float		m_posX;
	float		m_posY;
	float		m_speedX;

	LevelCube*	m_pPrev;
	LevelCube*	m_pNext;

friend class Game;
};

#endif // LEVELCUBES_HPP
