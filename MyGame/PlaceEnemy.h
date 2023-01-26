#pragma once
#include "CreateObjManager.h"
#include"Enemy.h"

class PlaceEnemy :
	public CreateObjManager
{
public:
	void Initialize(DebugCamera* camera) override;

	void Update(DebugCamera* camera) override;

	void ImGui_Draw() override;

	void FileWriting() override;

	void ArgMent(DebugCamera* camera) override;

	void Draw() override;

	bool ErrorJudg() override;

	void Finalize() override;

	class createimgui {

	public:
		createimgui(std::string num);
		void CreateImguiList();
	private:
		std::vector<std::string> listnum;

		float Scl;
		XMFLOAT4 Col;
	public:
		float GetScl() { return Scl; }
	};

	std::vector<createimgui*>imguilist;
	
private:
	//Enemyのみ複数種類配置するのでまた別のフラグ用意
	bool BossArgmentFlag;
	bool BetaArgmentFlag;
	std::vector<std::unique_ptr<Enemy>> enemys;
};
