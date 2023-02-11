#pragma once
#include "SwordBase.h"

class BigSword :
	public SwordBase
{
public:
	~BigSword() override;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
};
