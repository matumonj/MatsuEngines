#pragma once
#include "SwordBase.h"

class NormalSword :
	public SwordBase
{
public:
	~NormalSword() override;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	int bindex = 0;
};
