#ifndef LEVEL_HPP
#define LEVEL_HPP

class LevelCube;
class Player;
class CubeRenderer;
struct Input;

class Game {
public:
	Game();
	~Game();

	void			update(float timeStep, const Input& input);
	void			render(CubeRenderer& renderer);

private:
	void			reset();

	void			push(LevelCube* pCube);
	LevelCube*		remove(LevelCube* pCube);
	void			drawNumber(CubeRenderer& renderer, int n, float x, float y);

	void			spawnCube();

	float			m_aimX;
	float			m_aimY;

	float			m_maxY;
	float			m_cameraY;

	Player*			m_pPlayer;
	float			m_attachX;
	float			m_attachY;

	LevelCube*		m_pFirstCube;
	int				m_numCubes;

	int				m_hiscore;
	bool			m_isDead;

	enum { ExplosionCount = 20 };
	float			m_explosionRnd[ExplosionCount];
	float			m_explosionX;
	float			m_explosionY;
	float			m_explosionTime;
};

#endif // LEVEL_HPP
