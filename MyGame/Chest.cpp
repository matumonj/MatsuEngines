#include "Chest.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"Player.h"
#include"Destroy.h"
Chest::~Chest()
{
	Destroy(DefaultEffect);
}
void Chest::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Model = ModelManager::GetIns()->GetModel(ModelManager::CHEST);

	//フィールドにモデル割り当て
	m_Object->Initialize(camera);
	m_Object->SetModel(m_Model);
	//ps0 = new OBBCollision();
	Scale = { 30,30,30 };
	Rotation = { 0,90,0 };
	radius_adjustment = 0;
	SetCollider();

	ParticleManager::LoadTexture(2,L"Resources/ParticleTex/normal.png");

	DefaultEffect = ParticleManager::Create(2, L"Resources/ParticleTex/normal.png");
	//DefaultEffect->CreateModel();
	ObjAlpha = 1.0f;
	ChestLost = false;
}

void Chest::Update(DebugCamera* camera)
{
	ParameterSet_Obj(camera);
	//フィールド
	//CollideWood();
	for (int i = 0; i < 1; i++) {
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//追加
		if (!ChestLost) {
			DefaultEffect->Add(120, Position, vel, acc, 3.0f, 0.0f);
		}
		DefaultEffect->SetColor(pColor);
	}
		DefaultEffect->Update(DefaultEffect->NORMAL);
}

void Chest::Draw()
{
	Draw_Obj();
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	DefaultEffect->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();

}
bool Chest::CollideChest()
{
	//}
	return false;
}
