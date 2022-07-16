#include "MobEnemy.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"DebugCamera.h"
#include "PhysicsAction.h"
#include "WalkJudgement.h"
#include"WalkAction.h"
#include"StayJudgment.h"
#include"StayAction.h"
/// <summary>
/// コンストラクタ
/// </summary>
using namespace DirectX;
MobEnemy::MobEnemy(BehaviorTree* ai_tree, float max_hp, float max_mp, float attack, float deffence) :
Enemy(ai_tree,  max_hp, max_mp, attack, deffence)
{
	
}
/// <summary>
/// デストラクタ
/// </summary>
MobEnemy::~MobEnemy()
{
	//delete mob, MobModel;
}

//初期化処理
bool MobEnemy::Initialize(DebugCamera* camera)
{

	MobModel = FbxLoader::GetInstance()->LoadModelFromFile("monster");
	
	RandMove = rand() % 90+20;
	RandMovement = rand() % 100 + 80;
		
	InitAction();
	EnemyHP = 150.0f;
	//モデルの読込
	//MobModel = Model::CreateFromOBJ("subenemy");
	//モデル割り当てcamera
	Object3d::Initialize(camera);
	
	//mob = new Enemy();
	//Object3d::Initialize(camera);
	//Object3d::SetModel(MobModel);
	
	//
	Texture::LoadTexture(11, L"Resources/Sertch.png");
	SearchPlayerTexture = Texture::Create(11, { 0,-50,50 }, { 1,1,1 }, { 1,1,1,1 });
	SearchPlayerTexture->CreateTexture();
	//パラメータのセット
	//Scale = { 2,2,2 };
	Rotation = { 0,180,0 };
	//position = { -12,-5,-2 };

	
	MobObject = new f_Object3d();
	MobObject->Initialize();
	MobObject->SetModel(MobModel);
	MobObject->PlayAnimation();


	float radius = 5.0f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	return true;
}

//更新処理
void MobEnemy::Update(XMFLOAT4 color, DebugCamera* camera)
{
	scale={ 0.02f, 0.02f, 0.02f
		};
	//position = { 0,0,0 };
		Action();
		EnemyPop(150);
		
		//Object3d::SetPosition(position);
		//if (animeflag) {
			MobObject->SeteCurrent(animeflag);
		//}
			MobObject->SetScale(scale);
		//Object3d::SetRotation(Rotation);
			//rotation = { 0,0,0 };
			MobObject->SetPosition({ position.x,position.y-3,position.z });
		MobObject->SetRotation({ rotation.x-70, rotation.y, rotation.z });
		MobObject->Updata(true);


		CollisionField(camera);
}

//描画処理
void MobEnemy::Draw()
{
	//if (Alive) {
		/*Object3d::PreDraw();
		Object3d::Draw();
		Object3d::PostDraw();*/
	MobObject->Draw();
		Texture::PreDraw();
		//if (distance <= 30) {
			SearchPlayerTexture->Draw();
	//	}
		Texture::PostDraw();
	//}
}
//解放処理
void MobEnemy::Finalize()
{
	delete SearchPlayerTexture;
	//delete mob, MobModel;
}

float MobEnemy::Distance(Player* player)
{
	distance = sqrtf(((player->GetPosition().x - position.x) * (player->GetPosition().x - position.x))
		+ ((player->GetPosition().y - position.y) * (player->GetPosition().y - position.y))
		+ ((player->GetPosition().z - position.z) * (player->GetPosition().z - position.z)));
	return distance;
}

void MobEnemy::EnemySearchPlayer(Player* player)
{
	//プレイヤーと敵の距離を計算
	float distance;
	distance = sqrtf(((position.x - player->GetPosition().x) * (position.x - player->GetPosition().x)) +
		((position.y - player->GetPosition().y) * (position.y - player->GetPosition().y)) +
		((position.z - player->GetPosition().z) * (position.z - player->GetPosition().z)));



	XMVECTOR positionA = { player->GetPosition().x, player->GetPosition().y, player->GetPosition().z };
	XMVECTOR positionB = { position.x,position.y,position.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = DirectX::XMVectorSubtract(positionB, positionA);// positionA - positionB;
	if (SearchPlayerFlag == TRUE) {
		//追跡
		//Follow(player);
		//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
		rotx = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);

		Rotation.y = rotx * 60 + 180;//60=角度調整用 180=反転
	}

}



void MobEnemy::CollisionField(DebugCamera* camera)
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

