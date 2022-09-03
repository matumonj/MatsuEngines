#pragma once
#include "CreateObjManager.h"
#include"Enemy.h"
class PlaceEnemy :
    public CreateObjManager
{
public:
	void Initialize(DebugCamera* camera)override;

	void Update(DebugCamera* camera)override;

	void ImGui_Draw()override;

	void FileWriting()override;

	void ArgMent(DebugCamera* camera)override;

	void Draw()override;

	bool ErrorJudg()override;
private:
	//Enemy�̂ݕ�����ޔz�u����̂ł܂��ʂ̃t���O�p��
	bool BossArgmentFlag;
	std::vector<std::unique_ptr<Enemy>>enemys;

};

