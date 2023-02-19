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
	void Init(UINT num);
	void Upda(float addspeed = 0.1f, float addalpha = 0.02f);
	void Bleath();
	void Upda_B(bool loop = false);
	void Fire();
	void Draw();

	enum ParticleTexName
	{
		SMOKE=64,
		BOM=65,
		TOURCHFIRE=66
	};

	void CreateParticle(bool f, XMFLOAT3 pos)
	{
		createpos = pos;
		if (f && (m_particles[NORMAL].phase == NON || m_particles[
			NORMAL].phase == END))
		{
			m_particles[NORMAL].phase = INIT;
		}
	}

	void BCreateParticle(bool f, XMFLOAT3 pos)
	{
		createpos = pos;
	}

private:
	UINT parnum;

	enum Type
	{
		NORMAL_1,
		FIRE
	} type;

	enum Phase
	{
		NON,
		INIT,
		UPDA,
		END
	};

	struct ParParam
	{
		bool CreateJudg;
		std::vector<std::unique_ptr<Texture>> partex;
		int size = 0;
		std::vector<XMFLOAT3> vel = {{0.0f, 0.0f, 0.0f}};
		std::vector<float> Frame;
		std::vector<XMFLOAT4> ParColor;
		std::vector<float> angle = {0.0f};
		std::vector<XMFLOAT2> scl = {{1.0f, 1.0f}};
		std::vector<float> speed = {0.0f};
		std::vector<float> alpha = {0.0f};
		std::vector<bool> EndParUpda;
		std::vector<bool> f;
		Phase phase;
	};

	enum ParType
	{
		NORMAL,
	};

	int partype;
	static constexpr int ParNum = 2;
	XMFLOAT3 createpos;
	ParParam m_particles[ParNum];
	XMFLOAT2 BeginParScl;
	XMFLOAT4 BeginParColor;
	void SetParType(int size, ParParam& parparam, Texture* tex);
	void ParticleUpda();
	void EndParticleUpda(ParParam& parparam);

	void InitNormal(ParParam& parparam, XMFLOAT3 pos);
	void UpadaNormal_A(ParParam& parparam, float addspeed, float addalpha);
	void UpadaNormal_B(ParParam& parparam, bool loop);
	void UpadaBleath(ParParam& parparam);
	void Charge(ParParam& parparam);

	enum BleathPhase
	{
		BNON,
		BCHARGE,
		BBLEATH,
		BEND
	} bPhase;

public:
	void SetParScl(XMFLOAT2 scl) { BeginParScl = scl; }
	void SetParColor(XMFLOAT4 color) { BeginParColor = color; }

	void SetParF(int type)
	{
		partype = type;
		for (int i = 0; i < m_particles[NORMAL].size; i++) { m_particles[NORMAL].f[i] = false; }
	};

	void EndUpda(bool f)
	{
		for (int i = 0; i < m_particles[NORMAL].size; i++) { m_particles[NORMAL].EndParUpda[i] = f; }
	}

	void StartUpda(bool f)
	{
		for (int i = 0; i < m_particles[NORMAL].size; i++) { m_particles[NORMAL].EndParUpda[i] = f; }
	}


	bool GetParPhase_End()
	{
		if (m_particles[NORMAL].phase == END) { return true; }
		return false;
	}

private:
	bool isAryEqual(std::vector<float> num);
};
