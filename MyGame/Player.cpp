#include "Player.h"

#include <algorithm>

#include"Input.h"
#include"TargetMarker.h"
#include"Collision.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"WoodControl.h"
#include"CustomButton.h"
#include"BigSword.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"HUD.h"

#include"CameraControl.h"
#include "EnemyControl.h"

using namespace DirectX;

Player::~Player()
{
	Destroy_unique(m_Object);
	Destroy_unique(m_fbxObject);
}

Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	return GetInstance();
}

void Player::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	StopFlag = false;
	//オブジェクトの生成、初期化
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("ko"));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(1);

	//地形判定のコライダーセット
	SetCollider();

	Rotation = {-90.0f, 0.0f, 0.0f};
	Position = {0.0f, 0.0f, 0.0f};
	Scale = {0.02f, 0.02f, 0.02f};

	//体力初期化
	HP = MaxHP;
	//移動処理用
	vel /= 5.0f;


	AttackEffect::GetIns()->Init();
}

//ジャンプ
void Player::Jump()
{
	//接地時のみ
	if (onGround)
	{
		if (CustomButton::GetInstance()->GetJumpAction())
		{
			onGround = false;
			const float jumpVYFist = 0.3f;
			fallV = {0.0f, jumpVYFist, 0.0f, 0.0f};
		}
	}
	
}
//ジャンプ
void Player::DamageJump(bool judg,float knockpower)
{
	//接地時のみ
	if (onGround)
	{
			onGround = false;
		const float jumpVYFist = 1.3f;
			fallV = { 0.0f, jumpVYFist, 0.0f, 0.0f };
	}

}
void Player::Move()
{
	//移動停止フラグと回避モーション時は動けない
	if (evasionF || StopFlag || HP <= 0 || DamageEvaF)
	{
		return;
	}
	if (attackMotion != RUN && attackMotion != NON)
	{
		//	return;
	}
	XMFLOAT3 pos = Position;
	XMFLOAT3 rot = Rotation;

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float pi = 3.14159f;
	const float STICK_MAX = 32768.0f;
	if (TargetMarker::GetInstance()->GetNearIndex() != -1 && EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size() > 0) {
		nearindex = TargetMarker::GetInstance()->GetNearIndex();
		Enemy* NearEnemy = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[nearindex].get();


		//敵がプエレイヤーの方向く処理
		XMVECTOR positionA = {
		Position.x,Position.y,Position.z
		};
		XMVECTOR positionB = { NearEnemy->GetPosition().x, NearEnemy->GetPosition().y,  NearEnemy->GetPosition().z };
		//プレイヤーと敵のベクトルの長さ(差)を求める
		XMVECTOR SubVector = XMVectorSubtract(positionA, positionB); // positionA - positionB;

		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
		if (CustomButton::GetInstance()->GetAttackAction() &&
			Collision::GetLength(Position, NearEnemy->GetPosition()) < 30.f) {
			Rotation.y = RotY * 60 + 180.f;
		}

	}
	//スティックの移動処理
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		//状態を走りに
		attackMotion = RUN;

		//アニメーションを走りにセット
		if ((!m_AnimationStop))
		{
			AnimationContol(RUNNING, 2, 0.7, true);
		}

		//上入力
		if (input->TiltPushStick(Input::L_UP, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, vel, 0 }, angle);
		}
		//下入力
		if (input->TiltPushStick(Input::L_DOWN, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, -vel, 0 }, angle);
		}
		//右入力
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ vel, 0, 0, 0 }, angle);
		}
		//左入力
		if (input->TiltPushStick(Input::L_LEFT, 0.0f))
		{
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -vel, 0, 0, 0 }, angle);
		}

		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};

		vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		rot.y = angle + atan2f(StickX, StickY) * (180.0f / pi);

		//プレイヤーの回転角を取る
		Rotation = { rot.x, rot.y, rot.z };

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
		move = XMVector3TransformNormal(move, matRot);

		//向いた方向に進む
		Position.x += move.m128_f32[0] * movespeed * 5;
		Position.z += move.m128_f32[2] * movespeed * 5;
		Gmove = move;
	} else
	{
		//静止時間
		if ((!m_AnimationStop))
		{
			AnimationContol(IDLE, 9, 1, true);
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		Position.x = std::clamp(Position.x, -615.f, 600.f);
		Position.z = std::clamp(Position.z, -385.f, 822.f);
	}
	else if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{

	}

}

void Player::ReStartSetParam()
{
	m_AnimationStop = false;
	AnimationContol(IDLE, 8, 1.0, false);
}

void Player::Death()
{
	if (HP > 0)
	{
		return;
	}

	AnimationContol(DEATH, 7, 1.0, false);

	m_AnimationStop = true;
}

#include"mHelper.h"

void Player::Evasion()
{
	if (HP <= 0||DamageEvaF||StopFlag)
	{
		return;
	}

	if (evasionF)
	{
		//FBXタイムを回避モーション開始時に合わせる
		AnimationContol(EVASION, 6, 1.0,false);

		m_AnimationStop = true;

		//回避の進む距離はイージングをもとに計算
		if (evaTime < 1.0f)
		{
			//プレイヤーの向いてる方向にイージングで飛ぶ
			evaTime += 0.03f;
			Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
			Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, 15.0f, 0.0f);
		}
		else
		{
			if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime()-0.3f) {
				//attackMotion = NON;
				AnimationContol(IDLE, 9, 1, false);
				m_AnimationStop = false;

				//StopFlag = false;
				evasionF = false;
			}
		}
		
	}
	else
	{
		evaTime = 0.0f;
	}
}

void Player::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (m_Object == nullptr || m_fbxObject == nullptr)
	{
		return;
	}

	//１フレーム前の座標を保存
	oldpos = Position;
	//攻撃受けた後のクールタイム
	RecvDamage_Cool();

	Move();
	// ジャンプ操作
	Jump();

	Death();
	//落下防止
	if (!onGround)
	{
		//一定時間落下状態なら最終の接地位置に戻る
		falltime++;
		//３秒
		if (falltime > 180)
		{
			Position = oldpos;
			falltime = 0;
		}
	} else
	{
		//座標を一定間隔ごとに保存
		falltime = 0;
		savetime++;
		if (savetime % 60 == 0)
		{
			oldpos = Position;
			savetime = 0;
		}
	}
	if (DamageEvaF)
	{
		//FBXタイムを回避モーション開始時に合わせる
		AnimationContol(DEATH, 7, 15.0, false);
	//	m_AnimeLoop = true;
		m_AnimationStop = true;


		if (onGround&&m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.3f) {
			//attackMotion = NON;
			AnimationContol(IDLE, 9, 1, false);
			m_AnimationStop = false;

			DamageEvaF= false;
		}
	}
	//回避
	if (input->TriggerButton(input->X)&&StopFlag==false)
	{
		evasionF = true;
	}
	//
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		if (Collision::GetLength(Position, { 0, -19, 0 }) > 90)
		{
			if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
			{
				RecvDamage(10);
			}
		}
	}
	//回避
	Evasion();

	DamageTexDisplay();
	//手のボーン位置設定
	m_fbxObject->SetHandBoneIndex(hindex);
	m_fbxObject->SetFogPos(Position);
	//3d_fbx更新
	FbxAnimationControl();
	//fbxのタイマー処理
	//m_fbxObject->SetFbxTime(f_time);
	//当たり判定
	CollisionField();


	ParameterSet_Obj();

	m_fbxObject->SetPosition({ Position.x, Position.y - 1, Position.z });
	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);

	m_Object->SetPosition(Position);

	m_fbxObject->Update(m_AnimeLoop, m_AnimeSpeed, m_AnimationStop);
	//持つ武器の更新
	SelectSword::GetInstance()->Update();
	//攻撃エフェクト
	AttackEffect::GetIns()->Upda();


	
}


void Player::Draw()
{
	Draw_Fbx();
	ImGui::Begin("fg");
	ImGui::SliderInt("num", &hindex, 0, 27);
	ImGui::Text("%f", Position.x);
	ImGui::Text("%f", Position.z);
	ImGui::End();
	AttackEffect::GetIns()->Draw();
}

void Player::ParticleDraw()
{
	SelectSword::GetInstance()->SwordDraw();
}
void Player::DamageTexDraw()
{
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->Draw();
	}
}
void Player::FbxAnimationControls(const AttackMotion& motiontype, const AttackMotion nextmotiontype, AnimeName name,
                                  int number)
{
	//複数アニメーション読み込んだらこれら消す
	
	
	if (attackMotion == motiontype)
	{
		
		//FBXタイムを回避モーション開始時に合わせる
		AnimationContol(name, number, SelectSword::GetInstance()->GetSword()->GetAnimationTime(), false);
		m_AnimationStop = true;

		if (m_fbxObject->GetAnimeTime() > m_fbxObject->GetEndTime() - 0.05)
		{
			OldattackMotion = motiontype;
			StopFlag = false;
			attackMotion = NON;
		}
		else
		{
			StopFlag = true;
		}
	}
	/*歩きと待機モーションどうするか*/
}

void Player::AnimationContol(AnimeName name, int animenumber, double speed, bool loop)
{
	if (PlayerAttackState::GetInstance()->GetHitStopJudg() == true)
	{
		m_AnimeSpeed = 0.3;
	} else {
		m_AnimeSpeed = speed;
	}

	if (m_Number != animenumber)
	{
		m_AnimeLoop = loop;
		m_Number = animenumber;
		
		m_fbxObject->PlayAnimation(m_Number);
	}
}

void Player::FbxAnimationControl()
{
	if (evasionF || noAttack || HP <= 0||DamageEvaF)
	{
		return;
	}
	float timespeed = 0.02f;
	if (StopFlag)
	{
		//静止時間
		if ((!m_AnimationStop))
		{
			AnimationContol(IDLE, 9, 1, true);
		}
	}
	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == NON)
	{
		attackMotion = FIRST;
	}

	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == FIRST)
	{
		attackMotion = SECOND;
	}
	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == SECOND)
	{
		attackMotion = THIRD;
	}
	if (CustomButton::GetInstance()->GetAttackAction() == true && OldattackMotion == THIRD)
	{
		attackMotion = FIRST;
	}

	FbxAnimationControls(FIRST, SECOND, ATTACK1, 3);
	FbxAnimationControls(THIRD, FIRST, ATTACK2, 5);
	FbxAnimationControls(SECOND, THIRD, ATTACK3, 8);
}


XMMATRIX Player::GetMatrot()
{
	return m_fbxObject->GetMatrot();
}

void Player::RecvDamage(int Damage)
{
	//攻撃受けたあと2秒は無敵
	if (CoolTime != 0 || evasionF)
	{
		return;
	}
	if (CameraControl::GetInstance()->GetCameraState() != CameraControl::PLAYER)
	{
		return;
	}


	//ダメージくらった時にカメラシェイク
	if (!HUD::GetInstance()->GetRecvDamageFlag())
	{
		CameraControl::GetInstance()->ShakeCamera();
		HUD::GetInstance()->SetRecvDamageFlag(true); //プレイヤーHPのHUD用
	}

	if (HP >= 0)
	{
		HP = HP - Damage;
		std::unique_ptr<DamageManager> newdTex;
		
			newdTex = std::make_unique<DamageManager>(
				XMFLOAT3(Position.x, Position.y + rand() % 5 -2, Position.z), Damage);
		
		dMans_.push_back(std::move(newdTex));

	}
}

void Player::DamageTexDisplay()
{
	//ダメージスプライト生成
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->DamageDisPlay_Green(1, { 1, 1, 1, 1 });
	}

	//アルファ値一定以下なったら破棄
	dMans_.remove_if([](std::unique_ptr<DamageManager>& dTex)
		{
			return dTex->GetAlpha() <= 0.1f;
		});
}
XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = {v.m128_f32[0], v.m128_f32[1], v.m128_f32[2]};
	return pos;
}

void Player::RecvDamage_Cool()
{
	if (HUD::GetInstance()->GetRecvDamageFlag())
	{
		CoolTime = 120; //無敵時間
	}
	//カウント開始
	CoolTime--;

	CoolTime = min(CoolTime, 120);
	CoolTime = max(CoolTime, 0);
}
