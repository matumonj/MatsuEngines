#include "ControlBase.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
#include"Enemy.h"
#include"DebugCamera.h"
#include<memory>
#include"GigaBossEnemy.h"
class EnemyControl :
    public ControlBase
{
public:
	~EnemyControl() {};
private:
	std::vector <XMFLOAT3>Load_EnemyPosition;
	std::vector < std::vector <std::unique_ptr<Enemy>>>enemys;
	std::unique_ptr<GigaBossEnemy>gigaboss;

	XMFLOAT3 tutorial_pos;
	XMFLOAT3 boss_pos;
	
	Enemy* encountEnemy;
private:
	enum {
		ALPHAENEMY=1,
		BETAENEMY=2,
	};
public:
	static EnemyControl* GetInstance();

	void Load(DebugCamera*camera)override;

	void Finalize()override;

	enum EnemyType {
		NON,
		TUTORIAL,
		PLAYSCENE,
		BOSS,
	};
	int index = 0;
	std::vector<std::unique_ptr<Enemy>>& GetEnemy(EnemyType type);

	std::unique_ptr<GigaBossEnemy>& GetGigaBossEnemy() { return gigaboss; };

	Enemy* GetEncountEnemy() { if(encountEnemy!=nullptr)return encountEnemy; }
private:
	/*更新処理*/
	void Init_Tutorial(DebugCamera* camera)override;

	void Init_Play(DebugCamera* camera)override;

	void Init_Boss(DebugCamera* camera)override;

	/*更新処理*/
	void Update_Tutorial(DebugCamera*camera)override;

	void Update_Play(DebugCamera*camera)override;

	void Update_Boss(DebugCamera*camera)override;

	/*更新処理*/
	void Draw_Tutorial()override;

	void Draw_Play()override;

	void Draw_Boss()override;
	void GuardianSetPos();
};

