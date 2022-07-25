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
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

private:
	std::unique_ptr<Player>player;
	int Enemy_Quantity;
	std::vector<std::unique_ptr<Enemy>>enemys;
	std::vector <XMFLOAT3>Load_EnemyPosition;

	std::vector<XMFLOAT3> stpos;
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	std::vector<int> Num;

	BehaviorTree behavior;
public:
	void LoadEnemy(DebugCamera*camera);

	void Initialize(DebugCamera* camera);

	void Update(DebugCamera* camera);

	void Draw();

	void ImGuiDraw();
};

