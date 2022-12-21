#include "Field.h"
#include"CameraControl.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"SceneManager.h"
#include"ModelManager.h"
#include"Destroy.h"
#include"CollisionPrimitive.h"
#include"PlayerControl.h"
#include"EnemyControl.h"
#include"ChestControl.h"
#include "mHelper.h"
#define PI 3.14f
Field::~Field()
{
}

void Field::Finalize()
{
	Destroy_unique(CelestialSphereObject);
	Destroy(FieldObject);
	Destroy_unique(BackObject);
	for (int i = 0; i < objNum; i++)
	{
		Destroy_unique(m_object[i]);
	}
	Enemyicon.clear();
}

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Setplay()
{
}

void Field::Initialize()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	PedestalDownF = false;
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		//FieldObject->CreateGraphicsPipeline(L"Resources/Shader/Object3dVS.hlsl", L"Resources/Shader/Object3dPS.hlsl", L"Resources/Shader/BasicGS.hlsl");
		//�~�j�}�b�v(�{�X�t�B�[���h)
		//�_���[�W�G���A
		SetFieldModel(DAMAGEAREA, ModelManager::GetIns()->GetModel(ModelManager::DAMAGEAREA), camera);
		//�V��
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
		//�{�X�̔w�iobj
		SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);

		//Boss�̖��O�\���p�ƃt�B�[���h�O���̃_���[�W�G���A�ʒm
		Sprite::LoadTexture(40, L"Resources/BossName.png");
		Sprite::LoadTexture(41, L"Resources/warning1.png");

		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::SANDFIELD), camera);

	}

	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY || SceneManager::GetInstance()->GetScene() ==
		SceneManager::TUTORIAL)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
		//�~�j�}�b�v(�ʏ�ӂ��|���)
		//�V��
		SetFieldModel(CELESTIALSPHERE, ModelManager::GetIns()->GetModel(ModelManager::CELESTIALSPHERE), camera);
	}

	

	if (SceneManager::GetInstance()->GetScene() == SceneManager::MAPCREATE)
	{
		FieldObject = TouchableObject::Create(ModelManager::GetIns()->GetModel(ModelManager::FIELD), camera);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		pedestalpos = { -300.0f, -32, 270 };
		Texture::LoadTexture(12, L"Resources/2d/enemy/GuardArea.png");
		Texture* l_tex[8];
		for (int i = 0; i < 8; i++) {
			l_tex[i] = Texture::Create(12);
			GuardArea[i].reset(l_tex[i]);
			GuardArea[i]->CreateTexture();
			GuardArea[i]->SetAnchorPoint({ 0.5f,0.5f });

			GuardAreaAngle[i] = static_cast<float>(i) * 45.f;
			//�ʒu�̏�����
			GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
			GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;

		}
		SetFieldModel(PEDESTAL, ModelManager::GetIns()->GetModel(ModelManager::ICECRYSTAL), camera);
		SetFieldModel(BOSSBACK, ModelManager::GetIns()->GetModel(ModelManager::BOSSFIELD), camera);
	}
	//�{�X�̃l�[���v���[�g
	BossName = Sprite::Create(40, {WinApp::window_width / 2, WinApp::window_height / 2});
	BossName->SetAnchorPoint({0.5f, 0.5f});
	BossName->SetPosition({WinApp::window_width / 2, WinApp::window_height / 2});
	BossName->SetSize({800, 800});
}

void Field::PedestalMoving()
{
	if (Collision::GetLength(PlayerControl::GetInstance()->GetPlayer()->GetPosition(),
	                         pedestalpos) < 10)
	{
		PedestalDownF = true;
	}
	if (PedestalDownF)
	{
		if (pedestalpos.y > -200)
		{
			pedestalpos.y -= 0.2f;
		}
	}
}

void Field::Update_Tutorial()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (FieldObject == nullptr)
	{
		return;
	}
	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetFogCenter({125.0f, -25.0f, -680.0f});
	FieldObject->setFog(TRUE);
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 30.0f, 0.0f}, {40.0f, 40.0f, 40.0f}, FALSE, TRUE);
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);
}

void Field::Update_Play()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	if (CameraControl::GetInstance()->GetCameraState_Spline() != CameraControl::PLAYCUTEND)
	{
		if (camera != nullptr)
		{
			FogCenterPos = camera->GetEye();
		}
	}
	else
	{
		FogCenterPos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();
	}

	if (FieldObject == nullptr)
	{
		return;
	}
	CelestalRot += 0.1f;
	m_object[CELESTIALSPHERE]->SetRotation({90.0f, CelestalRot, 180.0f});
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 2290.0f, 0.0f}, {30.0f, 30.0f, 30.0f}, FALSE, TRUE);
	SetFieldUpdate(PEDESTAL, camera, pedestalpos, {2, 2, 2});

	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->SetFogCenter(FogCenterPos);
	FieldObject->setFog(true);
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);

	m_object[BOSSBACK]->SetRotation({0.f, 180.f, 0.f });
	SetFieldUpdate(BOSSBACK, camera, {22.f, -70.f, 1010.f }, {1.0f, 1.0f, 1.0f}, FALSE, TRUE);

	GuardAreaTexUpda();
	PedestalMoving();
}

void Field::GuardAreaTexUpda()
{
	if (EnemyControl::GetInstance()->GetGuardianEnemy() == nullptr)return;
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();
	XMFLOAT3 ppos = PlayerControl::GetInstance()->GetPlayer()->GetPosition();

	XMVECTOR positionA = {
	pedestalpos.x,
	pedestalpos.y,
	pedestalpos.z
	};
	XMVECTOR positionB[8];
	//�v���C���[�ƓG�̃x�N�g���̒���(��)�����߂�
	XMVECTOR SubVector[8];
	//�����p
	float RotY[8];
	//�J��������v���C���[�ɂ����Ă̐���
	camera_to_player.start = { camera->GetEye().x,camera->GetEye().z };
	camera_to_player.end = { ppos.x,ppos.z };

	//�p�x�̎擾 �v���C���[���G�̍��G�ʒu�ɓ�������������v���C���[�̕���
	for (int i = 0; i < 8; i++)
	{
		positionB[i] = {GuardareaPos[i].x,GuardareaPos[i] .y,GuardareaPos[i] .z};
		SubVector[i] = XMVectorSubtract(positionB[i], positionA); // positionA - positionB;
		RotY[i] = atan2f(SubVector[i].m128_f32[0], SubVector[i].m128_f32[2]);

		//�e�N�X�`����]�p�A���O��
		GuardAreaAngle[i]++;
		//�ړ������e�N�X�`���̍��W
		GuardareaPos[i].y = -30.0f;
		GuardareaPos[i].x = pedestalpos.x + sinf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		GuardareaPos[i].z = pedestalpos.z + cosf(GuardAreaAngle[i] * (PI / 180.0f)) * 80.0f;
		//�ړ������e�N�X�`���̉�]�l
		GuardAreaRot[i].y = RotY[i]*60;
		GuardAreaRot[i].x = 180.f;

		//�Փ˔���p�[�[�_
		GuardAreaPoint[i].x = GuardareaPos[i].x;
		GuardAreaPoint[i].y = GuardareaPos[i].z;
	

		//�K�[�f�B�A�����񂾂�ǂ̃A���t�@�l�����Ă���
		if(EnemyControl::GetInstance()->GetGuardianEnemy()->GetisAlive()==false)
		{
			GuardAreaAlphaEtime[i] -= 0.05f;
		}
		else//����ȊO
		{
			//�J�������W����v���C���[�̕��֐����ݒ肵��
			if (Collision::IsCollidingLineAndCircle(camera_to_player,GuardAreaPoint[i]))
			{
				//����������A���t�@�l������
				GuardAreaAlphaEtime[i] -= 0.05f;
			}
			else
			{
				//�������ĂȂ��Ȃ�グ��
				if (EnemyControl::GetInstance()->GetGuardianEnemy()->GetisAlive()) {
					GuardAreaAlphaEtime[i] += 0.05f;
				}
			}
			for (int i = 0; i < 8; i++)
			{
				//�v���C���[�̈ړ������ݒ�
				if (Collision::GetLength(ppos, GuardareaPos[i]) <= 20.f)
				{
					PlayerControl::GetInstance()->GetPlayer()->isOldPos();
				
				}

			}
		}
		
		//�A���t�@�l�̏㉺
		GuardAreaAlpha[i] = Easing::EaseOut(GuardAreaAlphaEtime[i], 0.0f, 0.8f);

		//�e�p�����[�^�̍X�V����
		GuardArea[i]->SetColor({ 1.f,1.f,1.f,GuardAreaAlpha[i] });
		GuardArea[i]->SetPosition(GuardareaPos[i]);
		GuardArea[i]->SetRotation(GuardAreaRot[i]);
		GuardArea[i]->SetScale({ 6.f,6.f,4.f });
		GuardArea[i]->SetBillboard(FALSE);
		GuardArea[i]->Update(camera);

		//�A���t�@�l�̏���Ɖ���
		GuardAreaAlphaEtime[i] = min(GuardAreaAlphaEtime[i], 1.0f);
		GuardAreaAlphaEtime[i] = max(GuardAreaAlphaEtime[i], 0.0f);
	}
}

void Field::Update_Edit()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	FieldObject->SetPosition({0.0f, -25.0f, 0.0f});
	FieldObject->SetFogCenter({0.0f, -20.0f, 0.0f});
	FieldObject->SetColor({0.2f, 0.2f, 0.2f, 1.0f});
	FieldObject->Update({0.2f, 0.2f, 0.2f, 1.0f}, camera);
}

void Field::Update_Boss()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (FieldObject == nullptr)
	{
		return;
	}
	SpriteFeed(TexAlpha_BossName, feed_BossName, feedSpeed_BossName, 1.5f);
	if (CameraControl::GetInstance()->GetCameraState() == CameraControl::PLAYER)
	{
		SpriteFeed(t, feed, feedSpeed_Explanation, 2.5f);
	}

	//�V���I�u�W�F�N�g�X�V
	SetFieldUpdate(CELESTIALSPHERE, camera, {0.0f, 30.0f, 0.0f}, {40.0f, 40.0f, 40.0f}, FALSE, TRUE);
	//�O���_���[�W�G���A�X�V
	SetFieldUpdate(DAMAGEAREA, camera, {0.0f, -19.2f, 0.0f}, {1.0f, 1.0f, 1.0f}, TRUE, FALSE);
	//�w�i�̃R���V�A���X�V
	SetFieldUpdate(BOSSBACK, camera, {0, -19, 0}, {1.0f, 1.0f, 1.0f}, FALSE, TRUE);

	FieldObject->SetPosition({0.0f, -19.0f, 0.0f});
	FieldObject->SetFogCenter(camera->GetEye());
	FieldObject->setFog(TRUE);
	FieldObject->SetColor({0.8f, 0.8f, 0.8f, 1.0f});
	FieldObject->Update({0.8f, 0.8f, 0.8f, 1.0f}, camera);

	//�t�B�[���h�O���ƃv���C���[�̓����蔻��(�����_�ł͋�`�Ɠ_)
	//FieldDamageAreaCol();
}

void Field::Update()
{
	DebugCamera* camera = CameraControl::GetInstance()->GetCamera();

	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		Update_Tutorial();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		Update_Play();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		Update_Boss();
	}

	t = min(t, 2.5f);
	t = max(t, 0.0f);
	TexAlpha_BossName = min(TexAlpha_BossName, 3.0f);
	TexAlpha_BossName = max(TexAlpha_BossName, 0.0f);
}

void Field::MiniFieldDraw()
{
	Object3d::PreDraw();
	//m_object[MINI]->Draw();
	Object3d::PostDraw();
}

#include"imgui.h"

void Field::Draw()
{
	if (FieldObject == nullptr)
	{
		return;
	}
	/*ImGui::Begin("field");
	ImGui::SliderFloat("PosX", &BossFieldPos.x, -700, 700);
	ImGui::SliderFloat("Posy", &BossFieldPos.y, -70, 70);

	ImGui::SliderFloat("Posz", &BossFieldPos.z, 0, 1700);
	ImGui::End();
	*/ //	if (FieldObject == nullptr)return;
	Object3d::PreDraw();
	if (SceneManager::GetInstance()->GetScene() != SceneManager::MAPCREATE)
	{
		m_object[CELESTIALSPHERE]->Draw();
	}


	FieldObject->Draw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		ModelDraw_nullCheck(BOSSBACK);
		ModelDraw_nullCheck(DAMAGEAREA);
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		ModelDraw_nullCheck(PEDESTAL);
		ModelDraw_nullCheck(BOSSBACK);
		
	}
	Object3d::PostDraw();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		BossName->setcolor({1.0f, 1.0f, 1.0f, TexAlpha_BossName});
	}

}

void Field::WarningDraw()
{
	Sprite::PreDraw();
	BossName->Draw();
	//Explanation->Draw();
	Sprite::PostDraw();
}

void Field::FieldDamageAreaCol()
{
}

void Field::SpriteFeed(float& alpha, bool& feed, const float feedSpeed, const float MaxAlphaValue)
{
	if (feed)
	{
		alpha -= 0.02f;
	}
	else
	{
		alpha += feedSpeed;
	}
	if (alpha >= MaxAlphaValue)
	{
		feed = true;
	}
}

void Field::SetFieldModel(ObjType type, Model* model, DebugCamera* camera)
{
	m_object[type] = std::make_unique<Object3d>();
	m_object[type]->SetModel(model);
	m_object[type]->Initialize(camera);
}

void Field::SetFieldUpdate(ObjType type, DebugCamera* camera, XMFLOAT3 Pos, XMFLOAT3 Scl, bool uvscroll, bool fog)
{
	if (m_object[type] == nullptr)
	{
		return;
	}

	m_object[type]->SetPosition(Pos);
	m_object[type]->SetScale(Scl);
	m_object[type]->SetUVf(uvscroll);
	m_object[type]->setFog(fog);
	m_object[type]->Update({1.0f, 1.0f, 1.0f, 1.0f}, camera);
}

void Field::ModelDraw_nullCheck(ObjType type)
{
	if (m_object[type] == nullptr)
	{
		return;
	}
	m_object[type]->Draw();
}

void Field::GuardAreaDraw()
{
	
		Texture::PreDraw();
		for (int i = 0; i < 8; i++)
		{
			if (GuardArea[i] == nullptr)continue;
			GuardArea[i]->Draw();
		}
		Texture::PostDraw();
	
}
