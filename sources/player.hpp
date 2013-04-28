#ifndef PLAYER_HPP
#define PLAYER_HPP

class CubeRenderer;
class LevelCube;
struct Input;

class Player {
public:
	Player(float x, float y);

	void	updateFree(float timeStep, const Input& input);
	void	updateAttached(float timeStep, float speedX, const Input& input);
	void	render(CubeRenderer& renderer);

private:
	void	getJumpSpeed(float* pX, float* pY) const;
	void	updateParticles(float timeStep);
	float	spaceFactor() const;
	float	gravity() const { return 2 - spaceFactor(); }
	LevelCube*	m_pAttachedCube;
	float	m_size;
	float	m_posX;
	float	m_posY;
	float	m_speedX;
	float	m_speedY;

	float	m_normalX;
	float	m_normalY;

	bool	m_isAiming;
	float	m_blockAiming;
	float	m_aimX;
	float	m_aimY;

	float	m_stamina;
	float	m_attachedTime;

	enum { NumParticles = 20 };
	struct Particle {
		float x, y, dx, dy, lifetime;
	}		m_particles[NumParticles];
	float	m_particleDelay;

	friend class Game;
};

#endif // PLAYER_HPP
