#pragma once
#include"ControlBase.h"
#include"Chest.h"
#include"ParticleManager.h"

class ChestControl : public ControlBase
{
public:
	static ChestControl* GetIns();

	void Load() override;


	void Finalize() override;

public:
	void GetChestAction();

	void ChestDestroy();

	bool GetTutorialChest() { return GetTutorialChestJudg; }

	int ChestCount() { return GetChestCount; }

	bool NowEvent() { return nowevent; };

	enum Color
	{
		RED,
		BLUE,
		GREEN,
		YELLOW
	};

	Chest* GetChest(Color col) { return chests[col].get(); }
	void SetChestAppearance(Color color, XMFLOAT3 position);
private:
	enum Event
	{
		FEEDIN,
		FEEDOUT,
		GETCHEST,
		FEEDIN2,
		FEEDOUT2,
		END,
		NON
	};

private:
	//�󔠃I�u�W�F
	std::vector<std::unique_ptr<Chest>> chests;
	//���W
	std::vector<XMFLOAT3> Load_ChestPosition;
	//�`���[�g���A���p	
	std::vector<std::unique_ptr<Chest>> Tutorial_chest;

	//�����
	int GetChestCount = 0;
	//�`���[�g���A���̕󔠉�����ꂽ��
	bool GetTutorialChestJudg;
	int ChestMax;

	bool nowevent;

	std::unique_ptr<Object3d> ChestCage;
	XMFLOAT3 CagePos;

	void CageOBBSet();
	void CageColPlayer();
private:
	struct ParticleParam
	{
		ParticleManager* particleMan = nullptr;
		bool ParticleCreateF = false;
		XMFLOAT3 particlePos;
		int ParticleLife = 420;
		int pCount = 0;
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};

		Event ChestEvent = NON;
	};

	int u = 0;
	ParticleParam TutorialPchest;
	ParticleParam PlayPchest[4];
	void GetChestEffect(Chest* chest, ParticleParam& pParam);
	void GetChestEvent(Chest* chest, ParticleParam& pParam);

public:
	bool GetChestEventNothing()
	{
		int i;
		for (i = 0; i < 4; i++)
		{
			if (PlayPchest[i].ChestEvent != NON && PlayPchest[i].ChestEvent != END)
			{
				break;
			}
		}
		if (i == 4)
		{
			return false;
		}
		return true;
	};
private:
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*�X�V����*/
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*�X�V����*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;
};
