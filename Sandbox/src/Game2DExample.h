#pragma once
#include "ParticleSystem.h"
#include "Rise.h"

class Game2DExample final : public Rise::Layer
{
public:
	Game2DExample();

	Game2DExample(const Game2DExample&) = delete;
	Game2DExample& operator=(const Game2DExample&) = delete;

	Game2DExample(Game2DExample&&) = delete;
	Game2DExample& operator=(Game2DExample&&) = delete;

	~Game2DExample() override = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Rise::TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(Rise::Event& e) override;
private:
	Rise::OrthographicCameraController m_CameraController;

	Rise::Ref<Rise::Texture2D> m_SpriteSheetTexture;
	Rise::Ref<Rise::SubTexture2D> m_StairsTex, m_BarrelTex, m_TreeTex;

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle{};

	std::unordered_map<char, Rise::Ref<Rise::SubTexture2D>> s_TextureMap;

};
