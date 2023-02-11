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

	class createimgui
	{
	public:
		createimgui(std::string num, float scl, XMFLOAT3 pos);
		void CreateImguiList();
	private:
		std::vector<std::string> listnum;

		float CreateScl;
		XMFLOAT3 CreatePos;
		XMFLOAT3 Pos;
		XMFLOAT3 Rot;
		float Scl;
		XMFLOAT4 Col;
		bool Del;
		int Hp;
		bool Select;
		int imnumber;
	public:
		float GetScl() { return Scl; }
		XMFLOAT3 GetPos() { return Pos; }
		bool GetDelF() { return Del; }
		bool GetSelectThis() { return Select; }
		void SetImguiNumber(int index) { imnumber = index; }
		int GetImguiNumber() { return imnumber; }
	public:
		enum ENumber
		{
			NON,
			AXEGOLEM,
			LIZARD,
			THROWGOLEM
		} enumber;


		ENumber GetEnumber() { return enumber; }
	};

	std::vector<std::unique_ptr<createimgui>> imguilist;

private:
	//Enemyのみ複数種類配置するのでまた別のフラグ用意
	bool BossArgmentFlag;
	bool BetaArgmentFlag;
	std::vector<std::unique_ptr<Enemy>> enemys;

	enum ENumber
	{
		NON,
		AXEGOLEM,
		LIZARD,
		THROWGOLEM
	};

	std::vector<bool> SelThis;
	std::vector<ENumber> enumbers;
};
