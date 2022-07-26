#include "Enemy.h"
#include"mHelper.h"
#include"EnemyWalkState.h"
#include"EnemyAttackState.h"
#include"EnemyStayState.h"
#include"EnemyFollowState.h"
#include "NodeBase.h"
#include"Collision.h"
#include"mHelper.h"
#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
using namespace DirectX;
Enemy::Enemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
	m_AiTree(ai_tree),
	m_BehaviorData(NULL),
	m_ActiveNode(NULL)
{
	
	m_BehaviorData = new BehaviorDatas();
}

Enemy::~Enemy()
{
	delete _state;
}


//初期化処理
bool Enemy::Initialize(DebugCamera* camera)
{
	//Object3d::Initialize(camera);
	return true;
}

//更新処理
void Enemy::Update(XMFLOAT4 color, DebugCamera* camera)
{
}

void Enemy::SearchInit()
{
	//普通のテクスチャ(スプライトじゃないよ)
	Texture::LoadTexture(64, L"Resources/Sertch.png");
	SearchTex = Texture::Create(64, { 0,0,0 }, { 20,20,1 }, { 1,1,1,1 });
	SearchTex->CreateTexture();
}
void Enemy::SearchAction(DebugCamera* camera)
{
	//追跡中でないときに追跡開始フラグ立てる(ビックリマーク)
	if (!SearchNowFlag) {
		searchFlag = false;
		searchTexSet = true;
	}
	else {
		searchTexSet = false;
	}
	//stime->びっくりアイコン出してる時間
	if (stime == 0 && searchTexSet) {
		if (Collision::GetLength(Player::GetInstance()->GetPosition(),position) <10) {
			searchFlag = true;
		}
	}

	if (searchFlag) {
		stime++;
		searchMarkDrawFlag = true;
		if(stime >20){
			searchMarkDrawFlag = false;
			searchFlag = false;
		}
	}
	else {
		stime = 0;
	}
	SearchTex->SetAnchorPoint({ 0.0,0 });
	SearchTex->SetRotation({ 180,0,0 });
	SearchTex->SetScale({ 1,1,1 });
	SearchTex->SetPosition({ position.x,position.y+10,position.z });
	SearchTex->Update(camera);
}
void Enemy::SearchDraw()
{
	Texture::PreDraw();
	if (searchFlag &&searchMarkDrawFlag) {
		SearchTex->Draw();
	}
	Texture::PostDraw();
}
void Enemy::InitAction()
{
	//_state = new EnemyWalkState();
}
void Enemy::Action()
{
	if (m_ActiveNode == NULL)
	{
		m_ActiveNode = m_AiTree->Inference(this, m_BehaviorData);
	}

	if (m_ActiveNode != NULL)
	{
		m_ActiveNode = m_AiTree->Run(this, m_ActiveNode, m_BehaviorData);
	}
	if (onGround == true) {

		if (onGroundTime % 30 == 0) {
			tempx = position.x;
			tempz = position.z;
		}
		if (wf && MoveFlag) {
			position = {
				position.x + move.m128_f32[0],
				position.y,
				position.z + move.m128_f32[2] }
			;
			movement++;
			//enemy->SetMovement(enemy->GetMovement() + 1);
		}

	} else if (onGround == false) {
		if (MoveFlag != false) {
			position = { tempx,position.y,tempz };
		}
	}
	if (EnemyHP < 0) {
		state = DEAD;
	}
	move = { 0,0,0.1f,0 };

	matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));

	move = XMVector3TransformNormal(move, matRot);

}
void Enemy::RecvDamage(int Damage) 
{
	RecvDamagef = true;
	EnemyHP = EnemyHP - Damage;
}
void Enemy::EnemyPop(int HP)
{
	if (state == DEAD) {
		PopCount++;
		if (PopCount >600) {
			position = StartPosition;
			EnemyHP = HP;
			wf = true;
			state = ALIVE; 
			
			
		}
	}
	else {
		PopCount = 0;
		
	}

}
//描画処理
void Enemy::Draw()
{
}

//解放処理
void Enemy::Finalize()
{
}

float Enemy::Distance(Player* player)
{

		distance = sqrtf(((player->GetPosition().x - position.x) * (player->GetPosition().x - position.x))
			+ ((player->GetPosition().y - position.y) * (player->GetPosition().y - position.y))
			+ ((player->GetPosition().z - position.z) * (player->GetPosition().z - position.z)));
		return distance;
	
}
void Enemy::EnemySearchPlayer(Player* player)
{

}

void Enemy::ChangeState(EnemyState* newState)
{
	delete _state;
	_state = newState;
}



void Enemy::Walk()
{
	//索敵範囲
	//const float DetectionRange = 10.0f;
	////プレイヤーが索敵範囲入ったら
	//bool SearchPlayer = Collision::GetLength(enemy->GetPosition(), Player::GetInstance()->GetPosition()) < DetectionRange;
	//if (Player::GetInstance() != nullptr) {
	//	SearchPlayer = false;
	//}
	//移動ベクトルをy軸周りの角度で回転

	//向いてる方向に移動
	//歩いている

	onGroundTime++;
	if (wf) {
		//if (enemy->GetMovement() > enemy->GetRandMoveMent()) {
		if (movement > RandMovement) {
			wf = false;
			movement = 0;
			sf = true;
		}
	}
	animeflag = false;
}

void Enemy::Stop()
{
	//enemy->SetMovement(0);

	if (StayCount == 0) {
		endsearch = false;
		//イージング掛ける前の敵の向き
		BeforeRot = rotation.y;
		//掛けた後の敵の向き
		AfterRot = rotation.y + RandMove;
	}

	if (sf) {
		StayCount++;

		if (StayCount > 190) {//停止時間
			RotTime += 0.01f;
			rotation = {
				rotation.x,
				//enemy->GetRotation().y+80,
				Easing::EaseOut(RotTime,BeforeRot, AfterRot),
				rotation.z
			};

			//enemy->ChangeState(new EnemyWalkState());
		//}

		}

		if (rotation.y >= AfterRot) {
			RotTime = 0;
			StayCount = 0;
			sf = false;
			wf = true;
		}
	}
	if (sf) {
		animeflag = true;
	
	}
	if (wf) {
		animeflag = false;
	}
}

void Enemy::Follow()
{
	animeflag = false;
	float angleX, angleZ, dis;
	//追跡スピード
	float centerSpeed = 0.1f;

	angleX = (Player::GetInstance()->GetPosition().x - position.x);
	angleZ = (Player::GetInstance()->GetPosition().z - position.z);

	//敵とプレイヤーの距離求め
	dis = sqrtf((position.x - Player::GetInstance()->GetPosition().x) * (position.x - Player::GetInstance()->GetPosition().x)
		+ (position.z - Player::GetInstance()->GetPosition().z) * (position.z - Player::GetInstance()->GetPosition().z));


	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = { Player::GetInstance()->GetPosition().x,Player::GetInstance()->GetPosition().y, Player::GetInstance()->GetPosition().z };
	XMVECTOR positionB = { position.x,position.y,position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;

	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

	rotation={0,RotY * 60 + 180,0 };
	//座標のセット
	if (Collision::GetLength(Player::GetInstance()->GetPosition(), position) > 10) {
		position = { position.x + (angleX / dis) * centerSpeed,position.y,position.z + (angleZ / dis) * centerSpeed };
	}
	time++;
	if (time> 210) {
		wf = true;
		//searchFlag = false;
		endsearch = true;
		//GetSearchPlayer = false;
		time = 0;
	}
	else {
		wf = false;
		sf = false;
	}
}

void Enemy::Attack()
{
	
	AttackFlag = true;
	Player::GetInstance()->RecvDamage(10);
	if (AttackFlag) {
		AttackFlag=false;
		AfterAttack = true;
	}
}
int Enemy::AttackCoolTime()
{
	if (AfterAttack) {
		cooltime++;
		if (cooltime > 180) {
			AfterAttack = false;
			cooltime = 0;
	}
	}
	return cooltime;
}
bool Enemy::GetSearchPlayer_Distance() {
	if (Collision::GetLength(Player::GetInstance()->GetPosition(), position) < 10) { return true; }
	else { return false; }
}


void Enemy::CollisionField(DebugCamera* camera)
{

	// ワールド行列更新
	UpdateWorldMatrix();

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
	//RecvDamagef = false;
	// 行列の更新など
	Object3d::Update({ 1,1,1,1 }, camera);

}