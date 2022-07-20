#include "Player.h"
#include"Input.h"
#include"DebugTxt.h"
#include"SphereCollider.h"
#include"TargetMarker.h"

#include"SphereCollider.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include"PlayerAttackState.h"
#include"imgui.h"
#define PI 3.145265
using namespace DirectX;
XMFLOAT3 Player::Effect_Pos = { -50,-10,-100 };
Player* Player::GetInstance()
{
	static Player instance;

	return &instance;
}

Player* Player::Create(Model* model, DebugCamera* camera)
{
	//�C���X�^���X�𐶐�
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
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
	effect = new mEffekseer();
	//effect->Initialize(dxcomn, camera);

	//�G�t�F�N�g�̃Z�b�g(3��������)
	effect->EffekseerSetting(camera, (const EFK_CHAR*)L"Effect/10/attack.efk", (const EFK_CHAR*)L"effect/10");

	if (!Object3d::Initialize(camera))
	{
		return false;
	}


	//model2 = Model::CreateFromOBJ("newfield");
	swm = Model::CreateFromOBJ("enemy-arm");
	sw = new Object3d();
	
	sw->Initialize(camera);
	sw->SetModel(swm);

	position = { 0,7,-120 };

	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("Knight");

	float radius = 5.0f;
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	//FBX���f���̐���
	object1 = new f_Object3d();
	object1->Initialize();
	object1->SetModel(fbxmodel);
	object1->PlayAnimation();
	
	return true;
}
void Player::Update(XMFLOAT4 color, DebugCamera* camera)
{

	//�G�t�F�N�g�̃p�����[�^�Z�b�g
	RotationStatus();
	//position.y = -2;
	object1->SetScale({0.01f, 0.01f, 0.01f
});
	//�G�t�F�N�g�̍Đ�
	//effect->Load_Effect();
	//�G�t�F�N�g�̃A�j���[�V�����~�߂�
	if (Input::GetInstance()->Pushkey(DIK_C)) {
		effect->Stop_Effect();
	}
	scale = { 0.01f,0.01f,0.01f };
	//�ړ��x�N�g����y������̊p�x�ŉ�]
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�����Ɉړ�

	
	
	
	//if (onGround) {
		//x = position.x;
		//z = position.z;
		if (input->Pushkey(DIK_W) || input->Pushkey(DIK_A) || input->Pushkey(DIK_D) || input->Pushkey(DIK_S)){
		position.x += move.m128_f32[0] * movespeed;
		position.z += move.m128_f32[2] * movespeed;
		stopf = false;
	}
		else {
			stopf = true;
		}
	//else {
	//	position.x = x;
	//	position.z = z;
			//}
	effect->EffekseerUpdate(camera);
	object1->SetPosition(position);
	object1->SetRotation({ rotation.x, rotation.y+180, rotation.z });
	object1->Updata( TRUE);
	//sw->SetMatrot(object1->GetmatRot());
	sw->setr(true);
	sw->SetMatrot(object1->GetHandBone());
	sw->SetMatRot(object1->GetMatRot());
	sw->SetMatTrans(object1->GetMatTrans());
	sw->SetMatScale(object1->GetMatScale());
	//object1.getm
	
	//sw->SetPosition(object1->GetPosition());

	//sw->SetScale({ 1.6,0.6,3 });
	//
	sw->Update({ 1,1,1,1 }, camera);
	//sw->SetRotation(object1->GetmatRot());

	object1->SeteCurrent(stopf);
	CollisionField(camera);

	//�s��̍X�V�Ƃ�
	//Object3d::Update({ 1,1,1,1 }, camera);
}

void Player::OnCollision(const CollisionInfo& info)
{

}
void Player::normalAttack(TargetMarker*target, std::vector<std::unique_ptr<Enemy>>enemy, DebugCamera* camera)
{
	//float ex, ey, ez;
	Effect_Pos.x = position.x;
	Effect_Pos.y = position.y;
	Effect_Pos.z = position.z;
	
		EffectRot.y = rotation.y;
		EffectRot.z = 70;
		effect->SetScale(1, 1, 1);
	effect->SetRotation(EffectRot.x, EffectRot.y, EffectRot.z);
	//effect->SetPosition(ex,ey,ez);
	effect->SetPosition(Effect_Pos.x, Effect_Pos.y, Effect_Pos.z);

	effect->EffekseerUpdate( camera);
	if (input->TriggerKey(DIK_SPACE) && !coolflag) {
		if (!attackflag) {
			attackflag = true;
		}
	}
	if (attackflag) {
		//Effect_Pos.x = enemy[target->GetNearIndex()]->GetPosition().x-10;
		//Effect_Pos.y = enemy[target->GetNearIndex()]->GetPosition().y;
		//Effect_Pos.z = enemy[target->GetNearIndex()]->GetPosition().z;
		effect->Load_Effect();
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
	effect->SetPosition(Effect_Pos.x, Effect_Pos.y, Effect_Pos.z);

	effect->EffekseerDraw();

}

void Player::RotationStatus()
{

	//�������ւ̈ړ�
	if (rotate != RotationPrm::LEFT && input->TriggerKey(DIK_A)) {//�������Ă��������������Ȃ�A�L�[��������A
		if (rotate == RotationPrm::FRONT) {//�E�ȊO�������Ă�����
			rotation.y = rotation.y - 90;
		} 
		else if(rotate == RotationPrm::BACK){//�E�������Ă�����
			rotation.y = rotation.y +90;
		}
		else if (rotate == RotationPrm::RIGHT) {
			rotation.y = rotation.y - 180;
		}
		//�����̏�������
		rotate = RotationPrm::LEFT;
	}

	//�E�����ւ̈ړ�
	else if (rotate != RotationPrm::RIGHT && input->TriggerKey(DIK_D)) {//�������Ă���������E����Ȃ��c�L�[��������A
		if (rotate == RotationPrm::FRONT) {//�E�ȊO�������Ă�����
			rotation.y = rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {//�E�������Ă�����
			rotation.y = rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {
			rotation.y = rotation.y + 180;
		}
		//�����̏����E��
		rotate = RotationPrm::RIGHT;
	}

	//�O�����ւ̈ړ�
	if (rotate != RotationPrm::FRONT && input->TriggerKey(DIK_W)) {//�������Ă���������E����Ȃ��c�L�[��������A
		if (rotate == RotationPrm::RIGHT) {//�����E�������Ă���
			rotation.y = rotation.y - 90;
		} else if (rotate == RotationPrm::LEFT) {//�������������Ă���
			rotation.y = rotation.y + 90;
		} else if (rotate == RotationPrm::BACK) {
			rotation.y -= 180;
		}
		//�����̏���O��
		rotate = RotationPrm::FRONT;
	}

	//�������ֈړ�
	if (rotate != RotationPrm::BACK && input->TriggerKey(DIK_S)) {//�������Ă���������E����Ȃ��c�L�[��������A
		if (rotate == RotationPrm::RIGHT) {//�����E�������Ă���
			rotation.y = rotation.y + 90;
		} else if (rotate == RotationPrm::LEFT) {//�������������Ă���
			rotation.y = rotation.y - 90;
		} else if (rotate == RotationPrm::FRONT) {
			rotation.y += 180;
		}
		//�����̏���O��
		rotate = RotationPrm::BACK;
	}
}



void Player::CollisionField(DebugCamera* camera)
{

	// ���[���h�s��X�V
	UpdateWorldMatrix();
	float x, z;
	x = position.x;
	z = position.z;
	// ��������
	if (!onGround) {
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// �ړ�
		//position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		//position.z += fallV.m128_f32[2];

	}
	// �W�����v����
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 1.2f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}

	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l
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

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	// �ڒn���
	if (onGround) {
		
		if (PosSavetime % 60 == 0) {
			OldPos_Onground = position;
		}
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 5.0f;
		// �ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
			// ���n
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	// �s��̍X�V�Ȃ�
	Object3d::Update({ 1,1,1,1 }, camera);

}

void Player::Draw()
{
	object1->Draw();
	sw->PreDraw();
	sw->Draw();
	sw->PostDraw();
}

void Player::ImguiDraw()
{
	ImGui::Begin("Player_State");
	//Position
	if (ImGui::TreeNode("Position")) {
		ImGui::Text(" PositionX   [%5f]", position.x);
		ImGui::Text(" PositionY   [%5f]", position.y);
		ImGui::Text(" PositionZ   [%5f]", position.z);

		XMFLOAT3 swp = sw->GetPosition();
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
	ImGui::SliderFloat("MoveSpeed", &movespeed, 1, 5);
	//�������������Ƃ��n�ʂɖ߂��Ă��� 1�b�O�̐ڒn�ʒu�ɖ߂��Ă���
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