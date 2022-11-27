#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include<memory>
class GameOver :
	public BaseScene
{
public:
	GameOver(SceneManager* sceneManager);
private:

public:
	std::unique_ptr<Sprite>GameOverTex;
	float TexAlpha;
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SpriteDraw();
	void Finalize() override;
};

