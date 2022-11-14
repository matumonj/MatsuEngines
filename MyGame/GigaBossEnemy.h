#pragma once
#include "ObjectManager.h"
#include<memory>
#include"Texture.h"

class GigaBossEnemy :
	public ObjectManager
{
public:
private:
	std::unique_ptr<Texture> FissureTex;

	float f_time;

	float disrad;
	float fissureAlpha;
	bool stopanimation;
	bool ArmonGround;
public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;

	static GigaBossEnemy* GetInstance();

public:
	enum Attack
	{
		NON,
		BEAM,
		SLAM,
		NORMAL
	};

	void SetMotion(Attack motion) { nowAttack = motion; }

	bool EndSlamMotion()
	{
		if (f_time >= 4.0f) { return true; }
		return false;
	}

private:
	Attack nowAttack;

	void fbxtimeControl();
	void FissureTexUpdate(DebugCamera* camera);
};
