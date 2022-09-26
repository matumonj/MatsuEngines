#pragma once
#include "Sprite.h"
#include <vector>
#include <memory>
#include"Texture.h"
class DamageManager {
private:
	DamageManager() = default;
	~DamageManager() = default;

public:

private:
	int score = 100;
	Sprite* num[5][10] = {};
	std::unique_ptr<Texture> Doll;
	std::vector<int> First;
public:
	static DamageManager* GetIns();		//Žæ“¾—p
	void Init();
	void Upda();
	void Draw();

	const int& GetScore() { return score; }
	void AddScore(const int& score) { this->score += score; }
	void ResetScore() { this->score = 0; }

};
