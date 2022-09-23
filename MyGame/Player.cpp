#include "Player.h"
#include"Input.h"
#include"DebugTxt.h"
#include"SphereCollider.h"
#include"TargetMarker.h"
#include"Collision.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#include"WoodControl.h"
#include"FenceControl.h"
#include"CustomButton.h"
#include"BigSword.h"
#include"SelectSword.h"
#include"SceneManager.h"
#include"HUD.h"
#define PI 3.145265
using namespace DirectX;

Player::~Player()
{
	Destroy_unique( m_Object);
	//delete  m_Model;
}
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	return Player::GetInstance();
}

void Player::Initialize(DebugCamera* camera)
{
	StopFlag = false;
	m_Object = std::make_unique<Object3d>();
	m_Object->Initialize(camera);
	//m_fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo");

	m_fbxObject = std::make_unique<f_Object3d>();
	m_fbxObject->Initialize();
	m_fbxObject->SetModel(FbxLoader::GetInstance()->LoadModelFromFile("monster_golem_demo"));
	m_fbxObject->PlayAnimation();

	SetCollider();

	Rotation = { 0,0,0 };
	Position = { 0,0,0 };
	SelectSword::GetInstance()->Initialize();
	
	HP = MaxHP;

	rotate = RotationPrm::FRONT;

}
void Player::Update(DebugCamera* camera)
{
	if (onGround) {
		onGroundPos = Position;
		nogroundtime = 0;
	}
	else {
		nogroundtime++;
		if (nogroundtime > 120) {
			Position = onGroundPos;
			nogroundtime = 0;
		}
	}
	//１フレーム前の座標を保存
	oldpos = Position;

	RotationStatus();
	
	RecvDamage_Cool();

	Scale={ 0.02f, 0.02f, 0.02f};
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(Rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (!StopFlag) {
		if (input->Pushkey(DIK_W) || input->Pushkey(DIK_A) || input->Pushkey(DIK_D) || input->Pushkey(DIK_S)
			|| (input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Down))) {
			Position.x += move.m128_f32[0] * movespeed;
			Position.z += move.m128_f32[2] * movespeed;
		}
	}
	Gmove = move;
	//3d更新
	ParameterSet_Obj(camera);
	//3d_fbx更新
	FbxAnimationControl();
	ParameterSet_Fbx(camera);
	//フィールド当たり判定
	// ワールド行列更新
	m_Object->UpdateWorldMatrix();

	// 落下処理
	if (!onGround) {
		//nogroundtime++;
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		Position.y += fallV.m128_f32[1];
	}
	// ジャンプ操作
	else if (CustomButton::GetInstance()->GetJumpAction()) {
		onGround = false;
		const float jumpVYFist = 0.5f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}

	// ワールド行列更新
	m_Object->UpdateWorldMatrix();
	m_Object->collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(m_Object->collider);
	assert(sphereCollider);


	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	Position.x += callback.move.m128_f32[0];
	Position.y += callback.move.m128_f32[1];
	Position.z += callback.move.m128_f32[2];
	// ワールド行列更新
	m_Object->UpdateWorldMatrix();
	m_Object->collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius() + radius_adjustment;
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 5.0f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.5f + adsDistance)) {
			onGround = true;
			Position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.5f);
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態

	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// 着地
			onGround = true;
			Position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	// 行列の更新など
	m_Object->Update({ 1,1,1,1 }, camera);

	//CollisionField(camera);
	//手のボーン取得
	HandMat = m_fbxObject->GetRot();

	SelectSword::GetInstance()->Update();
}

void Player::RotationStatus()
{
	if (StopFlag)return;
	//左方向への移動
	if (rotate != RotationPrm::LEFT && input->LeftTiltStick(input->Left)) {//今向いている方向が左じゃなくAキーが押され、
		if (rotate == RotationPrm::FRONT) {//右以外を向いていたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::BACK) {//右を向いていたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::RIGHT) {
			Rotation.y = Rotation.y - 180;
		}
		//向きの情報を左に
		rotate = RotationPrm::LEFT;
	}

	//右方向への移動
	else if (rotate != RotationPrm::RIGHT && input->LeftTiltStick(input->Right)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::FRONT) {//右以外を向いていたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {//右を向いていたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {
			Rotation.y = Rotation.y + 180;
		}
		//向きの情報を右に
		rotate = RotationPrm::RIGHT;
	}

	//前方向への移動
	if (rotate != RotationPrm::FRONT && input->LeftTiltStick(input->Up)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::RIGHT) {//もし右を向いてたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {//もし左を向いてたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {
			Rotation.y -= 180;
		}
		//向きの情報を前に
		rotate = RotationPrm::FRONT;
	}

	//後ろ方向へ移動
	if (rotate != RotationPrm::BACK && input->LeftTiltStick(input->Down)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::RIGHT) {//もし右を向いてたら
			Rotation.y = Rotation.y + 90;
		} else if (rotate == RotationPrm::LEFT) {//もし左を向いてたら
			Rotation.y = Rotation.y - 90;
		} else if (rotate == RotationPrm::FRONT) {
			Rotation.y += 180;
		}
		//向きの情報を前に
		rotate = RotationPrm::BACK;
	}
}


void Player::Draw()
{
	Draw_Fbx();
	SelectSword::GetInstance()->SwordDraw();
	ImguiDraw();
}

void Player::ImguiDraw()
{
	
	//}
	////Rotation
	//if (ImGui::TreeNode("Rotation")) {
	//	XMMATRIX sub = m_fbxObject->GetPos();

	//	XMFLOAT3 rots = { sub.r[3].m128_f32[0],sub.r[3].m128_f32[1], sub.r[3].m128_f32[2] };
	//	ImGui::Text(" RotationX   [%5f]", rots.x);
	//	ImGui::Text(" RotationY   [%5f]", rots.y);
	//	ImGui::Text(" RotationZ   [%5f]", rots.z);
	//	ImGui::TreePop();
	//}
	//ImGui::SliderInt("HP", &HP, 1, MaxHP);
	//ImGui::SliderFloat("MoveSpeed", &movespeed, 1, 15);
	////もし落下したとき地面に戻ってくる 1秒前の接地位置に戻ってくる
	//PosSavetime++;
	//if (PosSavetime > 60) {
	//	PosSavetime = 0;
	//}
	//if (ImGui::RadioButton("ReturnGround", &ReturnGround)) {
	//	Position = OldPos_Onground;
	//	ReturnGround = false;
	//}

}

void Player::FbxAnimationControl()
{
	const float timespeed = 0.02f;

	if (CustomButton::GetInstance()->GetAttackAction() == true) {
		if (HUD::GetInstance()->GetCoolTime() == 1.0f) {//クールスプライトのサイズ０（剣のクールダウン終わり）なったら　後でやり方変える
			AttackFlag = true;
		}
	}
	if (AttackFlag) {
		f_time = AttackTime;
		AttackFlag = false;
		nowattack = true;
	}


	f_time += timespeed;
	if (nowattack) {
		if (f_time >= DeathTime) {
			f_time = 0;
			nowattack = false;
		}
	} else {//歩くモーション　一つのアニメーションで回してるせいで攻撃モーション行く前にタイムを0に戻す
		if (f_time > AttackTime) {
			f_time = 0;
		}
	}
	m_fbxObject->SetFbxTime(f_time);
}

XMMATRIX Player::GetMatrot()
{
	if (m_fbxObject != nullptr) {
		return m_fbxObject->GetMatrot();
	}
}

void Player::RecvDamage(int Damage)
{
	//攻撃受けたあと2秒は無敵
	if (CoolTime != 0)return;
	if (!HUD::GetInstance()->GetRecvDamageFlag()) {
		HUD::GetInstance()->SetRecvDamageFlag(true);//プレイヤーHPのHUD用
	}
	if (HP >= 0) {
		HP = HP - Damage;
	}
}

void Player::RecvDamage_Cool()
{
	if (HUD::GetInstance()->GetRecvDamageFlag()) {
		CoolTime = 120;//無敵時間
	}
	//カウント開始
	CoolTime--;

	CoolTime = min(CoolTime, 120);
	CoolTime = max(CoolTime, 0);
}