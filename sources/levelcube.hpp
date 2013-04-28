#ifndef LEVELCUBES_HPP
#define LEVELCUBES_HPP

class CubeRenderer;

enum CubeType {
	CubeType_Sticky,
	CubeType_Bouncy,
	CubeType_Shrinking,
	CubeType_Harmful
};

class LevelCube {
public:
	LevelCube(float x, float y, float sx, CubeType type);

	void		update(float timeStep, bool isPlayerAttached);
	void		render(CubeRenderer& renderer);

	bool		canBeDeleted(float camY) const;

private:
	CubeType	m_type;
	float		m_size;
	float		m_posX;
	float		m_posY;
	float		m_speedX;

	float		m_time;

	LevelCube*	m_pPrev;
	LevelCube*	m_pNext;

friend class Game;
};

#endif // LEVELCUBES_HPP
