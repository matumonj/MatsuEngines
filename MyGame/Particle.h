#pragma once
#include"Texture.h"
#include<memory>
#include<vector>
class Particle
{
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	void Init();
	void Upda();
	void Draw();

	void CreateParticle();

private:
	enum Phase {
		INIT,
		UPDA,

	};

	struct ParParam {
		bool CreateJudg;
		std::vector<std::unique_ptr<Texture>>partex = { nullptr };
		int size = 0;
		std::vector < XMFLOAT3> vel = { {0.0f,0.0f,0.0f} };
		std::vector<float> angle = { 0.0f };
		std::vector < XMFLOAT2> scl = { {1.0f,1.0f} };
		std::vector<float> speed = { 0.0f };
		std::vector<float> alpha = { 0.0f };
		Phase phase;
	};
	enum ParType {
		NORMAL,
	};

	static constexpr int ParNum = 1;

	ParParam m_particles[ParNum];
	void SetParType(int size, ParParam& parparam, Texture* tex);
	void ParticleUpda();
	void EndParticleUpda(ParParam& parparam);

	void UpadaNormal(ParParam& parparam);
};

