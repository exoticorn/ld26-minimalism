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
	void			push(LevelCube* pCube);
	LevelCube*		remove(LevelCube* pCube);

	float			m_aimX;
	float			m_aimY;

	float			m_cameraY;

	Player*			m_pPlayer;
	float			m_attachX;
	float			m_attachY;

	LevelCube*		m_pFirstCube;
	int				m_numCubes;
};

#endif // LEVEL_HPP
