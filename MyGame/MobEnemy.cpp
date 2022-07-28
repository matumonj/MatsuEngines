#include "MobEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"StayJudgment.h"
#include"StayAction.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
MobEnemy::MobEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
Enemy(ai_tree,  max_hp, max_mp, attack, deffence)
{
	
}
/// <summary>
/// デストラクタ
/// </summary>
MobEnemy::~MobEnemy()
{
	//delete mob, MobModel;
}

//初期化処理
bool MobEnemy::Initialize(DebugCamera* camera)
{

	MobModel = FbxLoader::GetInstance()->LoadModelFromFile("monster");
	
	RandMove = rand() % 90+20;
	RandMovement = rand() % 100 + 80;
		
	InitAction();
	EnemyHP = 150.0f;
	//モデルの読込
	//MobModel = Model::CreateFromOBJ("subenemy");
	//モデル割り当てcamera
	Object3d::Initialize(camera);

	Texture::LoadTexture(11, L"Resources/Sertch.png");
	SearchPlayerTexture = Texture::Create(11, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	SearchPlayerTexture->CreateTexture();
	//パラメータのセット
	//Scale = { 2,2,2 };
	Rotation = { 0,180,0 };
	//position = { -12,-5,-2 };

	
	MobObject = new f_Object3d();
	MobObject->Initialize();
	MobObject->SetModel(MobModel);
	MobObject->PlayAnimation();


	float radius = 5.0f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

//更新処理
void MobEnemy::Update(XMFLOAT4 color, DebugCamera* camera)
{
	if (FollowFlag) {
		MobObject->SetColor({ 1,0,0,1 });
	}
	else {
		MobObject->SetColor({ 1,1,1,1 });
	}
	scale={ 0.02f, 0.02f, 0.02f
		};
	//position = { 0,0,0 };
		Action();

		EnemyPop(150);
		
		//Object3d::SetPosition(position);
		//if (animeflag) {
			MobObject->SeteCurrent(animeflag);
		//}
			MobObject->SetScale(scale);
		//Object3d::SetRotation(Rotation);
			//rotation = { 0,0,0 };
			MobObject->SetPosition({ position.x,position.y-3,position.z });
		MobObject->SetRotation({ rotation.x-70, rotation.y, rotation.z });
		MobObject->Updata(true);


		CollisionField(camera);
}

//描画処理
void MobEnemy::Draw()
{
	//if (Alive) {
		/*Object3d::PreDraw();
		Object3d::Draw();
		Object3d::PostDraw();*/
	MobObject->Draw();
		Texture::PreDraw();
		//if (distance <= 30) {
			SearchPlayerTexture->Draw();
	//	}
		Texture::PostDraw();
	//}
}
//解放処理
void MobEnemy::Finalize()
{
	delete SearchPlayerTexture;
	//delete mob, MobModel;
}

float MobEnemy::Distance(Player* player)
{
	distance = sqrtf(((player->GetPosition().x - position.x) * (player->GetPosition().x - position.x))
		+ ((player->GetPosition().y - position.y) * (player->GetPosition().y - position.y))
		+ ((player->GetPosition().z - position.z) * (player->GetPosition().z - position.z)));
	return distance;
}

void MobEnemy::EnemySearchPlayer(Player* player)
{
	//プレイヤーと敵の距離を計算
	float distance;
	distance = sqrtf(((position.x - player->GetPosition().x) * (position.x - player->GetPosition().x)) +
		((position.y - player->GetPosition().y) * (position.y - player->GetPosition().y)) +
		((position.z - player->GetPosition().z) * (position.z - player->GetPosition().z)));



	XMVECTOR positionA = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR positionB = { position.x,position.y,position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;
	if (SearchPlayerFlag == TRUE) {
		//追跡
		//Follow(player);
		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		rotx = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		Rotation.y = rotx * 60 + 180;//60=角度調整用 180=反転
	}

}


