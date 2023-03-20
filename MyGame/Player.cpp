#include "Player.h"
#include <algorithm>

#include"mHelper.h"
#include <FbxLoader.h>
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

Player::AnimeState Player::AnimationSetParam(AttackMotion motion, double speed, bool loop)
{
	AnimeState state;

	//アニメーション速度やループするかをセット
	state.AnimeLoop = loop;
	state.AnimeMotion = motion;
	state.AnimationSpeed = speed;


	return state;
}

void Player::Initialize()
{
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	StopFlag = false;
	//オブジェクトの生成、初期化
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);

	//歩く際の土煙
	runparticle = std::make_unique<Particle>();
	runparticle->Init(Particle::ParticleTexName::SMOKE);
	runparticle->SetParScl({0.3f, 0.3f});
	runparticle->SetParColor({1.f, 1.f, 1.f, 1.f});


	//地形判定のコライダーセット
	SetCollider();

	Rotation = {-90.0f, 0.0f, 0.0f};
	Position = {0.0f, 0.0f, 0.0f};
	Scale = {0.02f, 0.02f, 0.02f};

	//移動処理用
	vel /= 5.0f;

	ObjCol = {0.9f, 0.9f, 0.9f, 1.f};

	AttackEffect::GetIns()->Init();
}

void Player::KnockBack(XMFLOAT3 pos)
{
	// 自分の位置と接触してきたオブジェクトの位置とを計算して、距離と方向を出して正規化(速度ベクトルを算出)
	XMVECTOR pdistination = {Position.x, Position.y, Position.z};
	XMVECTOR bdistance = {pos.x, pos.y, pos.z};

	XMVECTOR dis = XMVector3Normalize(pdistination - bdistance);

	Position.x += dis.m128_f32[0] * bKnockPower;
	Position.z += dis.m128_f32[2] * bKnockPower;

	KnockEase += 0.02f;

	KnockEase = std::clamp(KnockEase, 0.f, 1.f);

	bKnockPower = Easing::EaseOut(KnockEase, 2.f, 0.f);
}
void Player::Walk()
{
	//移動停止フラグ動けない
	if (evasionF)
	{
		return;
	}
	//前フレーム座標保存
	isOld = false;

	XMFLOAT3 pos = Position;
	XMFLOAT3 rot = Rotation;

	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float STICK_MAX = 32768.0f;

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
			AnimationContol(_AnimeState[ANIMENAME_RUN]);
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
		rot.y = angle + atan2f(StickX, StickY) * (PI_180 / PI);

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
		attackMotion = AttackMotion::NON;
		RunParCreate = false;
		//静止時間
		if ((!m_AnimationStop))
		{
			AnimationContol(_AnimeState[ANIMENAME_NON]);
		}
	}

}

void Player::Move()
{
	
	//移動停止フラグと回避モーション時は動けない
	if (StopFlag || HP <= 0 || DamageEvaF)
	{
		return;
	}
	
	Walk();
	//回避
	Evasion();
	//探索ステージでの移動制限値
	float l_MoveLimit_x[2];
	float l_MoveLimit_z[2];

	if (SceneManager::GetIns()->GetScene() != SceneManager::TUTORIAL)
	{
		//探索シーンでの移動制限
		if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
		{
			l_MoveLimit_x[0] = -300.f;
			l_MoveLimit_x[1] = 300.f;
			l_MoveLimit_z[0] = -200.f;
			l_MoveLimit_z[1] = 400.f;
		}
		//ボスエリアでの移動制限
		else if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
		{
			l_MoveLimit_x[0] = -130.f;
			l_MoveLimit_x[1] = 130.f;
			l_MoveLimit_z[0] = -130.f;
			l_MoveLimit_z[1] = 130.f;
		}

		Position.x = std::clamp(Position.x, l_MoveLimit_x[0], l_MoveLimit_x[1]);
		Position.z = std::clamp(Position.z, l_MoveLimit_z[0], l_MoveLimit_z[1]);
	}

	//走り状態以外は足元の土煙出さない
	if (attackMotion != RUN)
	{
		RunParCreate = false;
	}
	else
	{
		RunParCreate = true;
	}
}

void Player::ReStartSetParam()
{
	m_AnimationStop = false;
	AnimationContol(_AnimeState[ANIMENAME_NON]);
}

void Player::Death()
{
	//死亡時
	if (HP > 0)
	{
		return;
	}
	//アニメーションを死亡に
	AnimationContol(_AnimeState[ANIMENAME_DEATH]);

	m_AnimationStop = true;
}


void Player::Evasion()
{
	//回避時
	//例外設定
	if (HP <= 0 || DamageEvaF)
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
		AnimationContol(_AnimeState[ANIMENAME_EVASION]);

		m_AnimationStop = true;

		//回避の進む距離はイージングをもとに計算
		evaTime += l_EvaETimeAccVal;
		if (evaTime < 1.0f)
		{
			if (!isOld)
			{
				//プレイヤーの向いてる方向にイージングで飛ぶ
				Position.x += Gmove.m128_f32[0] * Easing::EaseOut(evaTime, l_EvaEaseMax, l_EvaEaseMin);
				Position.z += Gmove.m128_f32[2] * Easing::EaseOut(evaTime, l_EvaEaseMax, l_EvaEaseMin);
			}
		}
		else
		{
			if (m_fbxObject->GetAnimeTime() >= m_fbxObject->GetEndTime() - 0.1f)
			{
				AnimationContol(_AnimeState[ANIMENAME_EVASION]);
				m_AnimationStop = false;

				evasionF = false;
			}
			StopFlag = false;
		}
	}
	else
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

	//アニメーションの基礎パラメータセット
	_AnimeState[ANIMENAME_NON] = AnimationSetParam(NON, 1.0, true);
	_AnimeState[ANIMENAME_RUN] = AnimationSetParam(RUN, 1.0, true);
	_AnimeState[ANIMENAME_FIRST] = AnimationSetParam(FIRST, SelectSword::GetIns()->GetSword()->GetAnimationTime(),
	                                                 false);
	_AnimeState[ANIMENAME_SECOND] = AnimationSetParam(SECOND, SelectSword::GetIns()->GetSword()->GetAnimationTime(),
	                                                  false);
	_AnimeState[ANIMENAME_THIRD] = AnimationSetParam(THIRD, SelectSword::GetIns()->GetSword()->GetAnimationTime(),
	                                                 false);
	_AnimeState[ANIMENAME_DEATH] = AnimationSetParam(DEATH, 1.0, false);
	_AnimeState[ANIMENAME_EVASION] = AnimationSetParam(EVASION, 1.0, false);

	//各種パラメータをCSVから
	if (!load)
	{
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

	m_fbxObject->SetColor(ObjCol);
	ParameterSet_Fbx3();


	DamageFlash();
	//持つ武器の更新
	SelectSword::GetIns()->Update();
	//攻撃エフェクト
	AttackEffect::GetIns()->Upda();

	//回避、停止時は土煙出さない
	if (!RunParCreate || evasionF)
	{
		runparticle->EndUpda(true);
	}
	else
	{
		runparticle->EndUpda(false);
	}

	//土煙のパーティクルの速度アルファ値減算値
	constexpr float l_ParticleVelSpeed = 0.01f;
	constexpr float l_ParticleAlphaSubVal = 0.04f;

	runparticle->CreateParticle(true, {Position.x, Position.y - 2.f, Position.z});
	runparticle->Upda(l_ParticleVelSpeed, l_ParticleAlphaSubVal);
}


void Player::Draw()
{
	//モデル描画
	Draw_Fbx();
	//土煙描画
	runparticle->Draw();
}

void Player::DamageTexDraw()
{
	//ダメージテクスチャ描画
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->Draw();
	}
}

void Player::FbxAttackControls(const AnimeName& motiontype, AttackMotion number)
{
	if (attackMotion == number)
	{
		//FBXタイムを剣に持たせたTIME値に開始時に合わせる
		AnimationContol(_AnimeState[motiontype]);
		m_AnimationStop = true;

		if (m_fbxObject->GetAnimeTime() > m_fbxObject->GetEndTime() - 0.05)
		{
			OldattackMotion = number;
			StopFlag = false;
			attackMotion = NON;
		}
		else
		{
			RunParCreate = false;
			StopFlag = true;
		}
	}
}

void Player::AnimationContol(AnimeState state)
{
	constexpr double l_HitStopAnimeSpeed = 0.3;

	//各パラメータをFBXに反映
	if (m_Number != state.AnimeMotion)
	{
		m_AnimeLoop = state.AnimeLoop;
		m_Number = state.AnimeMotion;
		m_fbxObject->PlayAnimation(m_Number);
	}

	//ヒットストップのときはスロー
	if (PlayerAttackState::GetIns()->GetHitStopJudg())
	{
		m_AnimeSpeed = l_HitStopAnimeSpeed;
	}
	else
	{
		m_AnimeSpeed = state.AnimationSpeed;
	}
}

void Player::FbxAnimationControl()
{
	if (HP <= 0 || DamageEvaF)
	{
		return;
	}
	if (evasionF)
	{
		attackMotion = NON;
		return;
	}
	if (StopFlag)
	{
		//静止時間
		if ((!m_AnimationStop))
		{
			AnimationContol(_AnimeState[ANIMENAME_NON]);
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

	//攻撃時のアニメーション遷移
	FbxAttackControls(ANIMENAME_FIRST, FIRST);
	FbxAttackControls(ANIMENAME_SECOND, THIRD);
	FbxAttackControls(ANIMENAME_THIRD, SECOND);
}

void Player::RecvDamage(int Damage)
{
	//攻撃受けたあと2秒は無敵
	if (DamageCool != 0 || evasionF || HP < 0)
	{
		return;
	}
	if (CameraControl::GetIns()->GetCameraState() != CameraControl::PLAYER)
	{
		return;
	}

	DamageFlashF = true;
	//ダメージくらった時にカメラシェイク
	if (!HUD::GetIns()->GetRecvDamageFlag())
	{
		CameraControl::GetIns()->ShakeCamera();

		HUD::GetIns()->SetRecvDamageFlag(true); //プレイヤーHPのHUD用
	}
	//体力減産
	HP = HP - Damage;

	AttackEffect::GetIns()->SetDamageEffectCreate(true);
	//ダメージテクスチャ生成
	std::unique_ptr<DamageManager> newdTex;

	newdTex = std::make_unique<DamageManager>(
		XMFLOAT3(Position.x, Position.y + static_cast<float>(rand() % 5 - 2), Position.z), Damage);
	dMans_.push_back(std::move(newdTex));
}

void Player::DamageTexDisplay()
{
	//ダメージスプライト生成
	for (std::unique_ptr<DamageManager>& dTex : dMans_)
	{
		dTex->DamageDisPlay_Green(1, {1, 1, 1, 1});
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
	if (HUD::GetIns()->GetRecvDamageFlag())
	{
		DamageCool = 120; //無敵時間
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
			std::getline(line_stream, word, ',');
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
			auto l_modelname = word.c_str();
			modelname = l_modelname;
			break;
		}
	}

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetIns()->LoadModelFromFile(modelname));
	m_fbxObject->LoadAnimation();
	m_fbxObject->PlayAnimation(NON);

	//体力初期化
	HP = MaxHP;
}

void Player::DamageFlash()
{
	if (!DamageFlashF)
	{
		return;
	}

	constexpr int MaxFlashCount = 6;
	//イージングでアルファ値の上げ下げ
	FlashEaseT += 0.05f;

	//6回光ったらフラグ切る
	if (FlashCount >= MaxFlashCount)
	{
		FlashCount = 0;
		DamageFlashF = false;
	}
	else
	{
		if (FlashEaseT >= 1.f)
		{
			//点滅回数＋＋
			FlashCount++;
			FlashEaseT = 0.f;
		}
	}
	ObjCol.y = Easing::EaseOut(FlashEaseT, 1.f, 4.f);
	ObjCol.z = Easing::EaseOut(FlashEaseT, 1.f, 4.f);
	ObjCol.x = Easing::EaseOut(FlashEaseT, 1.f, 4.f);
}
