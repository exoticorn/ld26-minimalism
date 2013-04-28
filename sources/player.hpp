#ifndef PLAYER_HPP
#define PLAYER_HPP

class CubeRenderer;
class LevelCube;
struct Input;

class Player {
public:
	Player(float x, float y);

	void	updateFree(float timeStep);
	void	updateAttached(float timeStep, float speedX, const Input& input);
	void	render(CubeRenderer& renderer);

private:
	void	getJumpSpeed(float* pX, float* pY) const;
	LevelCube*	m_pAttachedCube;
	float	m_size;
	float	m_posX;
	float	m_posY;
	float	m_speedX;
	float	m_speedY;

	float	m_normalX;
	float	m_normalY;

	bool	m_isAiming;
	float	m_aimX;
	float	m_aimY;

	friend class Game;
};

#endif // PLAYER_HPP
