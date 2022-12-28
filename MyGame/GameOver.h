#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include<memory>

#include "Particle.h"

class GameOver
{
public:
	GameOver();
	static GameOver* GetIns();
private:
	std::unique_ptr<Sprite> GameOverTex;
	std::unique_ptr<Sprite> GameOverTexFrame;
	float TexAlpha;
	float AlphaEaseT;
	bool ReStartF;

	bool PlayerDestF;
	DirectX::XMFLOAT2 FrameSize;
	float FrameScalingT;
	DirectX::XMFLOAT2 FramePos;

	std::unique_ptr<Particle>RestartPar;
public:
	void Initialize();
	void Update();
	void Draw();
	void Draw_DestParticle();
	void Finalize();
	
	bool GetResetF() { return ReStartF; }
};
