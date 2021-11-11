#pragma once

#include "Rise.h"

struct ParticleProps
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColourBegin, ColourEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime{1.f};
};

class ParticleSystem
{
public:
	explicit ParticleSystem(int maxParticles = 100000);

	void OnUpdate(Rise::TimeStep ts);
	void OnRender(const Rise::OrthographicCamera& camera);

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		glm::vec2 Position{};
		glm::vec2 Velocity{};
		glm::vec4 ColourBegin{}, ColourEnd{};
		float Rotation{0.f};
		float SizeBegin{}, SizeEnd{};

		float LifeTime{1.f};
		float LifeRemaining{0.f};

		bool Active = false;
	};
	std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex;
};