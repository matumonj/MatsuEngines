#include"GuardianEnemy.h"
#include"Collision.h"
#include"PlayerControl.h"
void GuardianRed::Initialize(DebugCamera* camera)
{
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	EnemyHP = 300.0f;
	MaxHP = 300.0f;
	//パラメータのセット
	Rotation = { -70.0f,180.0f,0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem"));
	m_fbxObject->PlayAnimation();
	m_fbxObject->SetColor({ 1,0,0,alpha });
	//コライダー周り
	radius_adjustment = 0;
	SetCollider();

	//FBX切り替わりのタイム指定
	AttackTime = 1.5f;
	DeathTime = 4.9f;

	//nowAttack = false;
	nowDeath = false;

	//state初期化
	state_mob->Initialize(this);

	//
	Texture::LoadTexture(93, L"Resources/ParticleTex/slash.png");
	SlashTex = Texture::Create(93, { 0.0f ,0.0f ,0.0f }, { 100.0f ,100.0f ,1.0f }, { 1.0f ,1.0f ,1.0f ,1.0f });
	SlashTex->CreateTexture();
	SlashTex->SetAnchorPoint({ 0.5f,0.5f });
	SlashPos = { Position.x,Position.y,Position.z };

	//パーティクルセット
	ParticleManager::LoadTexture(4, L"Resources/ParticleTex/Normal.png");
	particleMan = ParticleManager::Create(4, L"Resources/ParticleTex/Attack.png");
	ParticleManager::LoadTexture(6, L"Resources/ParticleTex/Attack.png");
	particleMan2 = ParticleManager::Create(6, L"Resources/ParticleTex/Attack.png");

}

void GuardianRed::Update(DebugCamera* camera)
{
	state_mob->Update(this);

	if (EnemyHP <= 0) {
		alpha -= 0.005f;
	}
	HandMat = m_fbxObject->GetRot();

	HandSiteOBB.SetOBBParam_Pos(HandMat);
	HandSiteOBB.SetOBBParam_Rot(HandMat);
	HandSiteOBB.SetOBBParam_Scl({ 2.0f,2.0f,2.0f });

	playerOBB.SetOBBParam_Pos(PlayerControl::GetInstance()->GetPlayer()->GetPosition());
	playerOBB.SetOBBParam_Rot(PlayerControl::GetInstance()->GetPlayer()->GetMatrot());
	playerOBB.SetOBBParam_Scl({ 1.0f,1.0f,1.0f });

	if (Collision::CheckOBBCollision(playerOBB, HandSiteOBB) == true) {
		PlayerControl::GetInstance()->GetPlayer()->RecvDamage(10);
	}
	FbxAnimationControl();

	EnemyPop(150);

	AttackCoolTime();

	ParameterSet_Fbx(camera);

	CollisionField(camera);


	//DamageParticleSet();
}

void GuardianRed::Draw()
{
	Draw_Fbx();
	// 3Dオブジェクト描画前処理
	ParticleManager::PreDraw();
	// 3Dオブクジェクトの描画
	particleMan->Draw();
	particleMan2->Draw();
	// 3Dオブジェクト描画後処理
	ParticleManager::PostDraw();
	Texture::PreDraw();

	Texture::PostDraw();
}

//死亡処理
void GuardianRed::Death()
{
	if (f_time < DeathTime) {
		DeathFlag = true;
	}
}

//攻撃後のクールタイム挙動
void GuardianRed::AttackCoolTime()
{
	if (f_time >= DeathTime - 1) {
		AfterAttack = true;
	}
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 480) {
			AfterAttack = false;
		}
	} else {
		cooltime = 0;
	}
}

//Fbxのアニメーション制御
void GuardianRed::FbxAnimationControl()
{
	//1フレーム進める
	f_time += 0.02f;

	if (f_AttackFlag) {
		f_time = AttackTime;
		f_AttackFlag = false;
		nowAttack = true;
	} else {
		if (nowDeath == false) {
			if (!nowAttack && f_time >= AttackTime) {
				f_time = 0.0f;
			}
		}
	}

	if (DeathFlag) {
		f_time = DeathTime;
		nowDeath = true;
		DeathFlag = false;

	}
	if (f_time > DeathTime) {
		nowAttack = false;
	}

	m_fbxObject->SetFbxTime(f_time);
}

void GuardianRed::DamageTexUpdate(DebugCamera* camera)
{

}