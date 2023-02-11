#include "Player.h"
#include <algorithm>

#include "AttackEffect.h"
#include"TargetMarker.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"CameraControl.h"
#include "HUD.h"

using namespace DirectX;

Player::~Player()
{
	Destroy_unique(m_Object);
	Destroy_unique(m_fbxObject);
}

Player* Player::GetIns()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	return GetIns();
}

Player::AnimeState Player::AnimationSetParam( AttackMotion motion, double speed, bool loop)
{
	_AnimeState[motion].AnimationSpeed = speed;
	_AnimeState[motion].AnimeLoop = loop;
	_AnimeState[motion].AnimeMotion = motion;
	if(PlayerAttackState::GetIns()->GetHitStopJudg())
	{
		_AnimeState[motion].AnimationSpeed = 0.3;
	}
}

void Player::Initialize()
{
	//アニメーションの基礎パラメータセット
	_AnimeState[AttackMotion::NON]=AnimationSetParam(NON,1.0,true);
	_AnimeState[AttackMotion::RUN] = AnimationSetParam(RUN, 1.0, true);
	_AnimeState[AttackMotion::FIRST] = AnimationSetParam(FIRST, 1.0, false);
	_AnimeState[AttackMotion::SECOND] = AnimationSetParam(SECOND, 1.0, false);
	_AnimeState[AttackMotion::THIRD] = AnimationSetParam(THIRD, 1.0, false);
	_AnimeState[AttackMotion::DEATH] = AnimationSetParam(DEATH, 1.0, false);

	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	StopFlag = false;
	//オブジェクトの生成、初期化
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	//歩く際の土煙
	runparticle= std::make_unique<Particle>();
	runparticle->Init(Particle::ParticleTexName::SMOKE);
	runparticle->SetParScl({ 0.3f,0.3f });
	runparticle->SetParColor({ 1.f, 1.f, 1.f, 1.f });

	//地形判定のコライダーセット
	SetCollider();

	Rotation = { -90.0f, 0.0f, 0.0f };
	Position = { 0.0f, 0.0f, 0.0f };
	Scale = { 0.02f, 0.02f, 0.02f };
	
	//移動処理用
	vel /= 5.0f;

	AttackEffect::GetIns()->Init();
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

	//スティックの移動処理
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		RunParCreate = true;
		//状態を走りに
		attackMotion = RUN;

		//アニメーションを走りにセット
		if ((!m_AnimationStop))
		{
			AnimationContol( 2, 0.7, true);
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
		Position.x += move.m128_f32[0] * AddSpeed;
		Position.z += move.m128_f32[2] * AddSpeed;
		
		Gmove = move;
	} else
	{
		RunParCreate = false;
		//静止時間
		if ((!m_AnimationStop))
		{
			AnimationContol(9, 1, true);
		}
	}

	//探索ステージでの移動制限
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		Position.x = std::clamp(Position.x, -300.f, 300.f);
		Position.z = std::clamp(Position.z, -200.f, 400.f);
	}
	//ボスエリアでの移動制限
	else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		Position.x = std::clamp(Position.x, -100.f, 100.f);
		Position.z = std::clamp(Position.z, -100.f, 100.f);
	}
	if(attackMotion!=RUN)RunParCreate = false;
	
}

void Player::ReStartSetParam()
{
	m_AnimationStop = false;
	AnimationContol( 8, 1.0, false);
}

void Player::Death()
{
	//死亡時
	if (HP > 0)
	{
		return;
	}
	//アニメーションを死亡に
	AnimationContol( 7, 1.0, false);

	m_AnimationStop = true;
}

#include"mHelper.h"
#include <FbxLoader.h>

void Player::Evasion()
{
	//回避時
	//例外設定
	if (HP <= 0 || DamageEvaF )
	{
		return;
	}

	//回避速度
	constexpr float l_EvaETimeAccVal = 0.03f;
	//回避時の移動距離
	constexpr float l_EvaEaseMin = 0.f;
	constexpr float l_EvaEaseMax = 15.f;

	//回避
	if (input->TriggerButton(input->X))
	{
		evasionF = true;
	}

	if (evasionF)
	{
		//回避地ヒットストップ切る
		PlayerAttackState::GetIns()->SetHitStopJudg(false);
		//FBXタイムを回避モーション開始時に合わせる
		AnimationContol( 6, 1.0, false);

		m_AnimationStop = true;

		//回避の進む距離はイージングをもとに計算
		evaTime += l_EvaETimeAccVal;
		if (evaTime < 1.0f)
		{
			//プレイヤーの向いてる方向にイージングで飛ぶ
			Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, l_EvaEaseMax, l_EvaEaseMin);
			Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, l_EvaEaseMax, l_EvaEaseMin);
		} else
		{
			if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.1f) {
				AnimationContol( 9, 1, false);
				m_AnimationStop = false;

				evasionF = false;
			}
			StopFlag = false;
		}

	} else
	{
		evaTime = 0.0f;
	}
}

void Player::Update()
{
	//カメラのインスタンス取得
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	//例外設定
	if (m_Object == nullptr)
	{
		return;
	}

	if (!load) {
		LoadCsv();
		load = true;
	}
		//１フレーム前の座標を保存
		oldpos = Position;
		//攻撃受けた後のクールタイム
		RecvDamage_Cool();

		//移動
		Move();
		//死亡
		Death();

		//回避
		Evasion();
		//プレイヤーに対しての被ダメージ表記
		DamageTexDisplay();
		//手のボーン位置設定
		m_fbxObject->SetHandBoneIndex(HandIndex);
		m_fbxObject->SetFogPos(Position);
		//3d_fbx更新
		FbxAnimationControl();
		//当たり判定
		CollisionField();

		//オブジェクトの更新
		ParameterSet_Obj();
		ParameterSet_Fbx3();

		//持つ武器の更新
		SelectSword::GetIns()->Update();
		//攻撃エフェクト
		AttackEffect::GetIns()->Upda();

		if (!RunParCreate||evasionF)
		{
			runparticle->EndUpda(true);
		}
		else
		{
			runparticle->EndUpda(false);
		}
	runparticle->CreateParticle(true, { Position.x,Position.y-2.f,Position.z });

	runparticle->Upda(0.01f,0.04f);


}


void Player::Draw()
{


	Draw_Fbx();
	
	runparticle->Draw();

}

void Player::DamageTexDraw()
{
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->Draw();
	}
}
void Player::FbxAttackControls(const AttackMotion& motiontype, const AttackMotion nextmotiontype,
	int number)
{
	//複数アニメーション読み込んだらこれら消す


	if (attackMotion == motiontype)
	{
		//RunParCreate = false;
		//FBXタイムを剣に持たせたTIME値に開始時に合わせる
		AnimationContol(number, SelectSword::GetIns()->GetSword()->GetAnimationTime()+0.7, false);
		m_AnimationStop = true;

		if (m_fbxObject->GetAnimeTime() > m_fbxObject->GetEndTime() - 0.05)
		{
			OldattackMotion = motiontype;
			StopFlag = false;
			attackMotion = NON;
		} else
		{
			StopFlag = true;
		}
	}
	/*歩きと待機モーションどうするか*/
}

void Player::AnimationContol(int animenumber, double speed, bool loop)
{
	if (PlayerAttackState::GetIns()->GetHitStopJudg() == true)
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
	if ( HP <= 0 || DamageEvaF)
	{
		return;
	}
	if(evasionF)
	{
		attackMotion = NON;
		return;
	}
	float timespeed = 0.02f;
	if (StopFlag)
	{
		//静止時間
		if ((!m_AnimationStop))
		{
			AnimationContol( 9, 1, true);
		}
	}
	if (input->TriggerButton(input->B) && OldattackMotion == NON)
	{
		attackMotion = FIRST;
	}

	if (input->TriggerButton(input->B) && OldattackMotion == FIRST)
	{
		attackMotion = SECOND;
	}
	if (input->TriggerButton(input->B) && OldattackMotion == SECOND)
	{
		attackMotion = THIRD;
	}
	if (input->TriggerButton(input->B) && OldattackMotion == THIRD)
	{
		attackMotion = FIRST;
	}

	FbxAttackControls(FIRST, SECOND, 3);
	FbxAttackControls(THIRD, FIRST, 5);
	FbxAttackControls(SECOND, THIRD,  8);
}


void Player::KnockBack(XMFLOAT3 rot, float Knock)
{
	//if (KnockPower <= 0)return;
	KnockPower = Knock;
	if (KnockPower > 0.f) {
		//移動ベクトルをy軸周りの角度で回転
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };

		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rot.y));

		move = XMVector3TransformNormal(move, matRot);

		Position.x += move.m128_f32[0] * KnockPower;
		Position.z += move.m128_f32[2] * KnockPower;

		KnockPower -= 0.1f;
	}
}

void Player::RecvDamage(int Damage)
{

	//攻撃受けたあと2秒は無敵
	if ( evasionF||HP<0)
	{
		return;
	}
	if (CameraControl::GetIns()->GetCameraState() != CameraControl::PLAYER)
	{
		return;
	}


	//ダメージくらった時にカメラシェイク
	if (!HUD::GetIns()->GetRecvDamageFlag())
	{
		CameraControl::GetIns()->ShakeCamera();
	
		HUD::GetIns()->SetRecvDamageFlag(true); //プレイヤーHPのHUD用
	}

	if (HP >= 0)
	{
		HP = HP - Damage;
		std::unique_ptr<DamageManager> newdTex;

		newdTex = std::make_unique<DamageManager>(
			XMFLOAT3(Position.x, Position.y + rand() % 5 - 2, Position.z), Damage);

		dMans_.push_back(std::move(newdTex));

	}
}
void BossEnemy::Move()
{
	
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
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}

void Player::RecvDamage_Cool()
{
	if (HUD::GetIns()->GetRecvDamageFlag())
	{
		DamageCool= 120; //無敵時間
	}
	//カウント開始
	DamageCool--;

	DamageCool = min(DamageCool, 120);
	DamageCool = max(DamageCool, 0);
}

void Player::LoadCsv()
{
		file.open("Param_CSV/CharaParam/Player_Param.csv");

		popcom << file.rdbuf();

		file.close();

		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find("HP") == 0)
			{
				std::getline(line_stream, word,',');
				int l_HP = static_cast<int>(std::atof(word.c_str()));
				MaxHP = l_HP;
			}
			if (word.find("RunSpeed") == 0)
			{
				std::getline(line_stream, word, ',');
				float l_addspeed = static_cast<float>(std::atof(word.c_str()));
				AddSpeed = l_addspeed;
			}
			if (word.find("Model") == 0)
			{
				std::getline(line_stream, word, ',');
				std::string l_modelname = static_cast<std::string>((word.c_str()));
				modelname = l_modelname;
				break;
			}
		}

		m_fbxObject = std::make_unique<f_Object3d>();
		m_fbxObject->Initialize();
		m_fbxObject->SetModel(FbxLoader::GetIns()->LoadModelFromFile(modelname));
		m_fbxObject->LoadAnimation();
		m_fbxObject->PlayAnimation(1);
	
	//体力初期化
	HP = MaxHP;
}
