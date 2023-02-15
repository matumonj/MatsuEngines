#pragma once
#include"f_Object3d.h"
#include"ObjectManager.h"
#include"Input.h"
#include"SwordBase.h"
#include"DamageManager.h"
#include<fstream>
#include<string>
#include<sstream>
#include<array>
#include "Particle.h"

class Player : public ObjectManager
{
public:
	~Player() override;

public:
	static Player* Create(Model* model, DebugCamera* camera);
	static Player* GetIns();

public:
	//初期化
	void Initialize() override;
	//更新処理
	void Update() override;
	//描画
	void Draw() override;

public:
	//受攻撃
	void RecvDamage(int Damage);
	//受攻撃後再度攻撃食らうまで
	void RecvDamage_Cool();
	//ゲームオーバー時のパラメータセット
	void ReStartSetParam();


public:
	//接地してるか？
	constexpr bool GetOnGround() { return onGround; }
	//OBBの当たり判定に使うプレイヤーの回転行列取得
	XMMATRIX GetMatrot()
	{
		return m_fbxObject->ExtractRotationMat(m_fbxObject->ExtractPositionMat(m_fbxObject->GetMatRot()));
	};
	//SelectSwordに使うプレイヤーの手のワールド行列
	XMMATRIX GetHanMat() { return m_fbxObject->GetHandBoneMatWorld(); }

	//手のボーン行列からTrans行列抜き出し
	XMFLOAT3 GetHandPos()
	{
		return {
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[0],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[1],
			m_fbxObject->GetHandBoneMatWorld().r[3].m128_f32[2]
		};
	}

	XMMATRIX GetRotMatrix()
	{
		return m_fbxObject->ExtractRotationMat(m_fbxObject->GetMatRot());
	}

	Particle* fire;
	//体力
	constexpr int& GetHP() { return HP; }
	//体力最大値
	constexpr int& GetMaxHP() { return MaxHP; }
	//強制停止
	constexpr bool& GetStopFlag() { return StopFlag; };
	//プレイヤーの回転アングル
	constexpr XMVECTOR& GetPlayerMove() { return Gmove; }

public:
	//プレーヤーの動き止める
	void SetStopFlag(bool f) { StopFlag = f; }
	//強制待機
	void SetIdle(bool f) { idlef = f; }
	//体力セッター
	void SetHP(int HP) { this->HP = HP; }
	//プレイヤーの向きをカメラに合わせるためのアングル
	void SetAngle(float angle) { this->angle = angle; }
private:
	bool DamageEvaF;
	Input* input = Input::GetIns();
	//fbxtime制御
	void FbxAnimationControl();
	//ジャンプ
	void Jump();
	//移動処理まとめ
	void Move();
	//死亡
	void Death();
private:
	//回避フラグ
	bool evasionF = false;
	//イージング用
	float evaTime = 0.0f;
	//回避後の座標
	XMFLOAT3 EvaPos = {};
	//回避処理
	void Evasion();
public:
	//座標の押し戻し
	void isOldPos()
	{
		Position.x = oldpos.x;
		//Position.y = oldpos.y;
		Position.z = oldpos.z;
	}

	//ダメージ受けた時のノックバック
	void KnockBack(XMFLOAT3 rot, float Knock);

private:
	//走った時の土煙
	std::unique_ptr<Particle> runparticle;
	//発生させるかどうか
	bool RunParCreate;

	//プレイヤーが持つ剣
	std::unique_ptr<SwordBase> sword = nullptr;
	float angle;

	float KnockPower;
private:
	//前座標
	XMFLOAT3 oldpos = {};
	//体力周り
	int HP = 0;
	//最大体力
	int MaxHP = 100;
	//teisi
	bool StopFlag;
	//移動の加速値
	float AddSpeed;
	//モデル名
	std::string modelname;
	//ダメージ表記
	std::list<std::unique_ptr<DamageManager>> dMans_;
	//無敵時間
	int DamageCool;

private:
	float vel = 0.0f;
	XMFLOAT3 MoveVECTOR(DirectX::XMVECTOR v, float angle);
private:
	static constexpr int HandIndex = 21;

	XMVECTOR Gmove;
public:
	enum AnimeName
	{
		ANIMENAME_NON,
		ANIMENAME_RUN,
		ANIMENAME_FIRST,
		ANIMENAME_SECOND,
		ANIMENAME_THIRD,
		ANIMENAME_EVASION,
		ANIMENAME_DEATH,
	};

	enum AttackMotion
	{
		NON=9,
		RUN=2,
		FIRST=3,
		SECOND=5,
		THIRD=8,
		EVASION=6,
		DEATH=7
	};

	AttackMotion GetAttackType() { return attackMotion; }
	AttackMotion GetAttackTypeOld() { return OldattackMotion; }

private:
	/*ここから後まとめて消す*/
	AttackMotion OldattackMotion = NON;
	AttackMotion attackMotion = NON;
	bool AnimationEndJudg_FirstAttack = false;
	bool AnimationEndJudg_SecondAttack = false;
	bool AnimationEndJudg_ThirdAttack = false;


	//アニメーションの数
	static constexpr auto AnimationSize = 7;

	//各アニメーションの基礎パラメータ
	struct AnimeState
	{
		AttackMotion AnimeMotion;
		double AnimationSpeed;
		bool AnimeLoop;
	};

	std::array<AnimeState, AnimationSize> _AnimeState;

	AnimeState AnimationSetParam(AttackMotion motion, double speed, bool loop);
public:
	AttackMotion GetAnimeType() { return attackMotion; }

private:
	//待機フラグ
	bool idlef;
	void AnimationContol(AnimeState state);
	void FbxAttackControls(const AnimeName& motiontype, AttackMotion number);

private:
	//パラメータ読み込み
	void LoadCsv();

	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	bool load;

public:
	void DamageTexDisplay();
	void DamageTexDraw();
};
