#include "ObjectManager.h"

#include "CameraControl.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"
#include"Destroy.h"
#include "SceneManager.h"

ObjectManager::~ObjectManager()
{
	Destroy_unique(m_Object);
	Destroy_unique(m_fbxObject);
	//Destroy(m_Model);
	Destroy(m_fbxModel);
}

void ObjectManager::SetCollider()
{
	float radius = 5.0f;
	m_Object->SetCollider(new SphereCollider(XMVECTOR({10, radius, 10, 0}), radius));
	m_Object->collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void ObjectManager::CollisionField()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	// ワールド行列更新
	m_Object->UpdateWorldMatrix();

	// 落下処理
	if (!onGround)
	{
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.8f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		Position.y += fallV.m128_f32[1];
	}

	// ワールド行列更新
	m_Object->UpdateWorldMatrix();
	m_Object->collider->Update();

	auto sphereCollider = dynamic_cast<SphereCollider*>(m_Object->collider);
	//assert(sphereCollider);


	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere)
		{
		};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) override
		{
			const XMVECTOR up = {0, 1, 0, 0};

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold)
			{
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
	ray.start.m128_f32[1] += sphereCollider->GetRadius() + 3;
	ray.dir = {0, -1, 0, 0};
	RaycastHit raycastHit;

	// 接地状態
	if (onGround)
	{
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 1.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
		                                             sphereCollider->GetRadius() * 2.5f + adsDistance))
		{
			onGround = true;
			Position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 3.0f);
		}
		// 地面がないので落下
		else
		{
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態

	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
		                                             sphereCollider->GetRadius() * 3.0f))
		{
			// 着地
			onGround = true;
			Position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 3.0f);
		}
	}


	m_Object->Update({1, 1, 1, 1}, camera);
}

void ObjectManager::ParameterSet_Obj()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_Object->SetPosition({Position.x, Position.y - 3, Position.z});
	m_Object->SetRotation(Rotation);
	m_Object->SetScale(Scale);
	m_Object->SetColor(Color);
	m_Object->Update({1, 1, 1, ObjAlpha}, camera);
}


void ObjectManager::ParameterSet_Fbx()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_fbxObject->SetPosition({Position.x, Position.y - 1, Position.z});

	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);

	m_Object->SetPosition(Position);

	//m_fbxObject->SetColor({1,1,1,1});
	m_fbxObject->Updata(true);
}

void ObjectManager::ParameterSet_Fbx3()
{
	m_fbxObject->SetPosition({ Position.x, Position.y , Position.z });
	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);

	m_Object->SetPosition(Position);

	m_fbxObject->Update(m_AnimeLoop, m_AnimeSpeed, m_AnimationStop);
}

void ObjectManager::ParameterSet_Fbx2()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	m_fbxObject->SetPosition({Position.x, Position.y, Position.z});

	m_fbxObject->SetRotation(Rotation);
	m_fbxObject->SetScale(Scale);
	if (m_Object != nullptr)
	{
		m_Object->SetPosition({Position.x, Position.y - 1.f, Position.z});
	}
	//m_fbxObject->SetColor({1,1,1,1});
	m_fbxObject->Updata();
}

void ObjectManager::Draw_Obj()
{
	m_Object->PreDraw();
	m_Object->Draw();
	m_Object->PostDraw();
}

void ObjectManager::Draw_Fbx()
{
	m_fbxObject->Draw();
}
