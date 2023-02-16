#pragma once
#include"Sprite.h"
#include<memory>
#include"array"
#include "Texture.h"

#include"Object3d.h"

class Task
{
public:
	static Task* GetIns();

private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static constexpr auto TaskNum = 4;
	static constexpr auto ChestNavNum = 2;
	static constexpr auto TaskListNavNum = 2;
	//�^�X�N
	std::array<std::unique_ptr<Sprite>,TaskNum> TaskFrame;
	std::array<std::unique_ptr<Sprite>, TaskNum> TasksSprite;
	//�N���A�����^�X�N�̑ł�������
	std::array<std::unique_ptr<Sprite>, TaskNum> ClearTaskLine;
	std::array<XMFLOAT2, TaskNum> ClearTaskLineScl;
	std::array<float, TaskNum> ClearTaskLineEaseC;

	//�󔠂̉�����p�̃t���[���Ƃ������Ă�����
	std::array<std::unique_ptr<Sprite>,ChestNavNum> navChestSprite;
	std::array<float, ChestNavNum> navSpriteAlpha;
	//Y�Ń^�X�N�ꗗ��\�����邩�ǂ����̃i�r�e�N�X�`��
	std::array<std::unique_ptr<Sprite>, TaskListNavNum> TaskMenuList;
	std::array<float, TaskListNavNum>TaskMenuListAlpha;
	//�N���A�������ǂ���
	std::array<bool, TaskNum> Judg;
	//�e�N�X�`���A���t�@�l
	std::array<float, TaskNum> TaskSpriteAlpha;
	std::array<std::unique_ptr<Texture>, TaskNum> TargetIcon;
	std::array<float, TaskNum> iconalpha;
	enum Tasks
	{
		TASK_ONE,
		TASK_TWO,
		TASK_THREE,
		TASK_FOUR,
		TASK_FIVE,
		TASK_COMPLEATE
	} tasks = TASK_ONE;

	//�S�Ẵ^�X�N�N���A������
	bool TaskAllClear;
private:
	XMFLOAT2 TaskMenuPos = {};
	XMFLOAT2 FramePos;

	XMFLOAT2 FrameScl;

	std::array<XMFLOAT2,TaskNum> TaskPos;
	XMFLOAT2 TaskScl;

	float SpritePosInter;
	XMFLOAT2 navTaskPos;
	XMFLOAT2 navTaskScl;

private:
	int movement;
public:
	//������
	void Init();
	//�X�V
	void Upda();
	//�`��
	void Draw();
	//�^�X�N�Ώە\��
	void TargetDraw();

	bool ClearTaskONE();
	bool ClearTaskTwo();
	bool ClearTaskThree();
	bool GetAllTaskClear() { return TaskAllClear; }

	inline bool GetGolemDesthCount(int count)
	{
		if (GolemDestCount == count) { return true; }
		return false;
	}

	inline bool GetMiniGolemDesthCount(int count)
	{
		if (MiniGolemDestCount == count) { return true; }
		return false;
	}

	void SetGuardianDeathCount(int count) { GuardianDeathCount = count; }
	bool TaskFourClear(Tasks task) { return Judg[task]; }
	
	void GuardianRset();
private:
	//�S�[�����|������
	int GolemDestCount;
	//�~�j�S�[������������
	int MiniGolemDestCount;

	int GuardianDeathCount;
	void TaskClear(const Tasks& task, const Tasks& nexttask, bool clearjudg, int chestcunt);

	void NavTaskSequence();

	static constexpr float TaskNumInterPos=120.f;
	static constexpr float taskNumSize=0.5f;
public:
	void SetGolemDestroyCount() { GolemDestCount++; }
	void SetMiniGolemDestroyCount() { MiniGolemDestCount++; }

private:
	//�^�X�N�ꗗ�\�����邩�ǂ���
	bool OpenJudg;
	//�C�[�W���O�p�J�E���^
	float MovingFrameECount;

	void OpenTasks();
};
