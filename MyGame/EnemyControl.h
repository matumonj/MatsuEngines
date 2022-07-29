#include "ControlBase.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
#include"Enemy.h"
#include"DebugCamera.h"
#include<memory>
class EnemyControl :
    public ControlBase
{
public:
	~EnemyControl() {};
private:
	//std::unique_ptr<Player>player;
	std::vector<std::unique_ptr<Enemy>>enemys;
	std::vector <XMFLOAT3>Load_EnemyPosition;

	BehaviorTree behavior;
private:
	enum {
		ALPHAENEMY=1,
		BETAENEMY=2,
	};
public:
	static EnemyControl* GetInstance();

	void Load(DebugCamera*camera)override;

	void Initialize(DebugCamera* camera)override;

	void Update(DebugCamera* camera)override;

	void Draw()override;

	void ImGuiDraw();

	std::vector<std::unique_ptr<Enemy>> &GetEnemyindex(int index);// { return enemys[index].get(); }

private:
	//::vector<Enemy*>
};

