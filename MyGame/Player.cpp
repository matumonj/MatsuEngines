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
#define PI 3.145265
using namespace DirectX;
XMFLOAT3 Player::Effect_Pos = { -50,-10,-100 };

Player::~Player()
{
	delete fbxmodel;
	delete object1;
}
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	//インスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	instance->Initialize( camera);//	delete instance;
	//	assert(0);

	if (model) {
		instance->SetModel(model);
	}
	return instance;
}

bool Player::Initialize( DebugCamera* camera)
{
	HP = MaxHP;
	rotate = RotationPrm::FRONT;
	
	if (!Object3d::Initialize(camera))
	{
		return false;
	}

	position = {125,-25,-760 };

	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("Knight");

	float radius = 5.0f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	//FBXモデルの生成
	object1 = new f_Object3d();
	object1->Initialize();
	object1->SetModel(fbxmodel);
	object1->PlayAnimation();
	
	return true;
}
void Player::Update(XMFLOAT4 color, DebugCamera* camera)
{
	oldpos = position;
	//エフェクトのパラメータセット
	RotationStatus();
	//position.y = -2;
	object1->SetScale({0.01f, 0.01f, 0.01f
});

	scale = { 0.01f,0.01f,0.01f };
	//移動ベクトルをy軸周りの角度で回転
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

		if (input->Pushkey(DIK_W) || input->Pushkey(DIK_A) || input->Pushkey(DIK_D) || input->Pushkey(DIK_S)
			|| (input->LeftTiltStick(input->Left)|| input->LeftTiltStick(input->Right)|| input->LeftTiltStick(input->Up)|| input->LeftTiltStick(input->Down))){
		position.x += move.m128_f32[0] * movespeed;
		position.z += move.m128_f32[2] * movespeed;
		stopf = false;
	}
		else {
			stopf = true;
		}
		if (Collision::GetCollideOBB() == true) {
			position = oldpos;
			
}
	object1->SetPosition(position);
	object1->SetRotation({ rotation.x, rotation.y+180, rotation.z });
	object1->Updata( TRUE);
	//sw->SetMatrot(object1->GetmatRot());

	object1->SeteCurrent(stopf);
	CollisionField(camera);

	//行列の更新とか
	//Object3d::Update({ 1,1,1,1 }, camera);
}

void Player::OnCollision(const CollisionInfo& info)
{

}
void Player::normalAttack(TargetMarker*target, std::vector<std::unique_ptr<Enemy>>enemy, DebugCamera* camera)
{
	
	if (input->TriggerKey(DIK_SPACE) && !coolflag) {
		if (!attackflag) {
			attackflag = true;
		}
	}
	if (attackflag) {
		coolflag = true;
		float distance = sqrtf(((enemy[target->GetNearIndex()]->GetPosition().x - position.x) * (enemy[target->GetNearIndex()]->GetPosition().x - position.x))
			+ ((enemy[target->GetNearIndex()]->GetPosition().y - position.y) * (enemy[target->GetNearIndex()]->GetPosition().y - position.y))
			+ ((enemy[target->GetNearIndex()]->GetPosition().z - position.z) * (enemy[target->GetNearIndex()]->GetPosition().z - position.z)));

		if (distance <= 10) {
			enemy[target->GetNearIndex()]->SetHP(enemy[target->GetNearIndex()]->GetHP() - 1);
		}
		attackflag = false;
	}
	if (coolflag) {
		cooldowntime++;
		if (cooldowntime > CoolTime) {
			coolflag = false;
			cooldowntime = 0;
		}
	}
	
}

void Player::RotationStatus()
{

	//左方向への移動
	if (rotate != RotationPrm::LEFT && input->LeftTiltStick(input->Left)) {//今向いている方向が左じゃなくAキーが押され、
		if (rotate == RotationPrm::FRONT) {//右以外を向いていたら
			rotation.y = rotation.y - 90;
		} 
		else if(rotate == RotationPrm::BACK){//右を向いていたら
			rotation.y = rotation.y +90;
		}
		else if (rotate == RotationPrm::RIGHT) {
			rotation.y = rotation.y - 180;
		}
		//向きの情報を左に
		rotate = RotationPrm::LEFT;
	}

	//右方向への移動
	else if (rotate != RotationPrm::RIGHT && input->LeftTiltStick(input->Right)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::FRONT) {//右以外を向いていたら
			rotation.y = rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {//右を向いていたら
			rotation.y = rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {
			rotation.y = rotation.y + 180;
		}
		//向きの情報を右に
		rotate = RotationPrm::RIGHT;
	}

	//前方向への移動
	if (rotate != RotationPrm::FRONT && input->LeftTiltStick(input->Up)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::RIGHT) {//もし右を向いてたら
			rotation.y = rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {//もし左を向いてたら
			rotation.y = rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {
			rotation.y -= 180;
		}
		//向きの情報を前に
		rotate = RotationPrm::FRONT;
	}

	//後ろ方向へ移動
	if (rotate != RotationPrm::BACK && input->LeftTiltStick(input->Down)) {//今向いている方向が右じゃなくＤキーが押され、
		if (rotate == RotationPrm::RIGHT) {//もし右を向いてたら
			rotation.y = rotation.y + 90;
		} else if (rotate == RotationPrm::LEFT) {//もし左を向いてたら
			rotation.y = rotation.y - 90;
		} else if (rotate == RotationPrm::FRONT) {
			rotation.y += 180;
		}
		//向きの情報を前に
		rotate = RotationPrm::BACK;
	}
}



void Player::CollisionField(DebugCamera* camera)
{

	// ワールド行列更新
	UpdateWorldMatrix();
	float x, z;
	x = position.x;
	z = position.z;
	// 落下処理
	if (!onGround) {
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		//position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		//position.z += fallV.m128_f32[2];

	}
	// ジャンプ操作
	else if (Input::GetInstance()->TriggerButton(Input::GetInstance()->Button_A)) {
		onGround = false;
		const float jumpVYFist = 0.2f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}
	

	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
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
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround) {
		
		if (PosSavetime % 60 == 0) {
			OldPos_Onground = position;
		}
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 5.0f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
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
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	// 行列の更新など
	Object3d::Update({ 1,1,1,1 }, camera);

}

void Player::Draw()
{
	object1->Draw();
	
}

void Player::ImguiDraw()
{
	ImGui::Begin("Player_State");
	//Position
	if (ImGui::TreeNode("Position")) {
		ImGui::Text(" PositionX   [%5f]", position.x);
		ImGui::Text(" PositionY   [%5f]", position.y);
		ImGui::Text(" PositionZ   [%5f]", position.z);

		XMFLOAT3 swp = { 1,1,1 };
		ImGui::Text(" bPositionX   [%5f]",swp.x);
		ImGui::Text(" bPositionY   [%5f]", swp.y);
		ImGui::Text(" bPositionZ   [%5f]", swp.z);
		ImGui::TreePop();
	}
	//Rotation
	if (ImGui::TreeNode("Rotation")) {
		ImGui::Text(" RotationX   [%5f]", rotation.x);
		ImGui::Text(" RotationY   [%5f]", rotation.y);
		ImGui::Text(" RotationZ   [%5f]", rotation.z);
		ImGui::TreePop();
	}
	ImGui::SliderInt("HP", &HP,1,MaxHP);
	ImGui::SliderFloat("MoveSpeed", &movespeed, 1, 15);
	//もし落下したとき地面に戻ってくる 1秒前の接地位置に戻ってくる
	PosSavetime++;
	if (PosSavetime > 60) {
		PosSavetime = 0;
	}
	if (ImGui::RadioButton("ReturnGround", &ReturnGround)) {
		position = OldPos_Onground;
		ReturnGround = false;
	}

	ImGui::End();
}