#ifndef LEVEL_HPP
#define LEVEL_HPP

class LevelCube;
class CubeRenderer;

class Game {
public:
	Game();
	~Game();

	void			update(float timeStep);
	void			render(CubeRenderer& renderer);

private:
	void			push(LevelCube* pCube);
	LevelCube*		remove(LevelCube* pCube);

	LevelCube*		m_pFirstCube;
	int				m_numCubes;
};

#endif // LEVEL_HPP
