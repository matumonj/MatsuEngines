#include "Chest.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"Player.h"
Chest::~Chest()
{
	//delete m_Model;
}
void Chest::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Model = ModelManager::GetIns()->GetModel(ModelManager::CHEST);

	//�t�B�[���h�Ƀ��f�����蓖��
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
	//�t�B�[���h
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

		//�ǉ�
		if (!ChestLost) {
			DefaultEffect->Add(120, Position, vel, acc, 3.0f, 0.0f);
		}
		DefaultEffect->SetColor({1.0f,0.8f,0.1f,0.8f});
	}
		DefaultEffect->Update(DefaultEffect->NORMAL);
}

void Chest::Draw()
{
	Draw_Obj();
	// 3D�I�u�W�F�N�g�`��O����
	ParticleManager::PreDraw();
	// 3D�I�u�N�W�F�N�g�̕`��
	DefaultEffect->Draw();
	// 3D�I�u�W�F�N�g�`��㏈��
	ParticleManager::PostDraw();

}
bool Chest::CollideChest()
{
	//playerOBB.m_NormaDirect[0] = { Player::GetInstance()->GetMatrot().r[0].m128_f32[0],Player::GetInstance()->GetMatrot().r[0].m128_f32[1],Player::GetInstance()->GetMatrot().r[0].m128_f32[2] };
	//playerOBB.m_NormaDirect[1] = { Player::GetInstance()->GetMatrot().r[1].m128_f32[0],Player::GetInstance()->GetMatrot().r[1].m128_f32[1],Player::GetInstance()->GetMatrot().r[1].m128_f32[2] };
	//playerOBB.m_NormaDirect[2] = { Player::GetInstance()->GetMatrot().r[2].m128_f32[0],Player::GetInstance()->GetMatrot().r[2].m128_f32[1],Player::GetInstance()->GetMatrot().r[2].m128_f32[2] };
	//playerOBB.m_fLength[0] = 1;//x�����̒���
	//playerOBB.m_fLength[1] = 1;//y�����̒���
	//playerOBB.m_fLength[2] = 1;//z�����̒���
	////OBB ��]�x�N�g��
	//woodOBB.m_NormaDirect[0] = { m_Object->GetMatrot().r[0].m128_f32[0],m_Object->GetMatrot().r[0].m128_f32[1],m_Object->GetMatrot().r[0].m128_f32[2] };
	//woodOBB.m_NormaDirect[1] = { m_Object->GetMatrot().r[1].m128_f32[0], m_Object->GetMatrot().r[1].m128_f32[1], m_Object->GetMatrot().r[1].m128_f32[2] };
	//woodOBB.m_NormaDirect[2] = { m_Object->GetMatrot().r[2].m128_f32[0], m_Object->GetMatrot().r[2].m128_f32[1], m_Object->GetMatrot().r[2].m128_f32[2] };
	//woodOBB.m_fLength[0] = 1;//x�����̒���
	//woodOBB.m_fLength[1] = 20;//y�����̒���
	//woodOBB.m_fLength[2] = 1;//z�����̒���
	////OBB�̐ݒ�ʒu
	//playerOBB.m_Pos = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y,Player::GetInstance()->GetPosition().z };
	//woodOBB.m_Pos = { m_Object->GetPosition().x,  m_Object->GetPosition().y, m_Object->GetPosition().z };

	//if (Collision::GetLength(Player::GetInstance()->GetPosition(), Position) < 20) {
	//	if (ps0->ColOBBs(playerOBB, woodOBB)) {
	//		Player::GetInstance()->isOldPos();
	//		return true;
	//	} else {
	//		return false;
	//	}
	//}
	return false;
}
