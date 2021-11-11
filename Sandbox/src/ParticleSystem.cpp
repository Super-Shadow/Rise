#include "ParticleSystem.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>

class Random
{
public:
	static void Init()
	{
		s_RandomEngine.seed(std::random_device()()) ;
	}

	static float Float()
	{
		return static_cast<float>(s_Distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<std::mt19937::result_type>::max());
	}

private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

std::mt19937 Random::s_RandomEngine;  // NOLINT(cert-msc51-cpp) Init() sets random seed
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

ParticleSystem::ParticleSystem(const int maxParticles) : m_PoolIndex(maxParticles - 1)
{
	m_ParticlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(const Rise::TimeStep ts)
{
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= static_cast<float>(ts);
		particle.Position += particle.Velocity * static_cast<float>(ts);
		particle.Rotation += 0.01f * static_cast<float>(ts);
	}
}

void ParticleSystem::OnRender(const Rise::OrthographicCamera& camera)
{
	Rise::Renderer2D::BeginScene(camera);

	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Fade away particles
		const float life = particle.LifeRemaining / particle.LifeTime;
		const glm::vec4 colour = glm::lerp(particle.ColourEnd, particle.ColourBegin, life);
		//color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		Rise::Renderer2D::DrawQuad({ .position{ particle.Position.x,particle.Position.y, 0.2f }, .rotation = particle.Rotation, .size{ size, size }, .tintColour{colour} });

	}

	Rise::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_PoolIndex];
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
	particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

	// Colour
	particle.ColourBegin = particleProps.ColourBegin;
	particle.ColourEnd = particleProps.ColourEnd;

	particle.LifeTime = particleProps.LifeTime;
	particle.LifeRemaining = particleProps.LifeTime;
	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}