#pragma once
#include"BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include"LightGroup.h"
#include"DebugCamera.h"
#include"DirectXCommon.h"

class TitleScene : public BaseScene
{
public:
	TitleScene(SceneManager* sceneManager);
private:
	//�e�N�X�`��[GolemFantasy]
	std::unique_ptr<Sprite> titlesprite = nullptr;
	//�t�B�[���h�I�u�W�F�N�g
	std::unique_ptr<Object3d> field = nullptr;
	//�V���I�u�W�F�N�g
	std::unique_ptr<Object3d> celestal = nullptr;
	//�e�N�X�`��[B�ŃQ�[����ʂ�]
	std::unique_ptr<Sprite> TitleMenu = {nullptr};
	//���[�h�V�[��
	std::unique_ptr<Sprite> LoadMenuSprite = { nullptr };

	//�ǂݍ��ݒ��Ɗ����̕���
	struct NowLoadSprite {
		std::unique_ptr<Sprite> LWordSprite = { nullptr };
		XMFLOAT2 Pos;
		float Color;
	};
	std::array<NowLoadSprite, 10>LoadWords;
	//�^�C�g���p�J����
	std::unique_ptr<DebugCamera> camera = nullptr;

	//�`���[�g���A���{�^�������ꂽ���H
	bool menujudg_Play = false;
	//�t�F�[�h���o�p
	bool feedf = false;
	//�J���������ɉ�����
	bool CameraBackF = false;

	//�{�^���e�N�X�`���̍��W�A�X�P�[���A�A���t�@�l
	float menuAlpha = 1.0;
	XMFLOAT2 MenuScale = {0.0f, 0.0f};
	XMFLOAT2 MenuPos = {0.0f, 0.0f};

	//�J�����̉�]���x
	float CamAngleSpeed;
	float Cangle = 0.0f;
	XMFLOAT2 CameraPos = {0.0f, 0.0f};
	//�t�B�[���h��]�p
	float FieldRotY = 0.0f;
	bool FadeFlag;
public:
	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//�J��
	void Finalize() override;
private:
	//���C�g�X�V
	void LightUpdate() override;
	//�`��Q�I�u�W�F�N�g
	void MyGameDraw() override;
	//�`��Q�X�v���C�g
	void SpriteDraw() override;

	bool ff;
	float timef;
private:
	static constexpr float RotSpeed = 0.1f;
	static constexpr XMFLOAT3 FieldScl = {0.15f, 0.15f, 0.15f};
	static constexpr XMFLOAT3 CelestalScl = {30.f, 30.1f, 30.1f};

	//�e�N�X�`���̍X�V�ƕ`��
	void TitleTexInit();
	void TitleTexUpda();
	void TitleTexDraw();

	//�t�B�[���h�I�u�W�F�N�g�̍X�V�ƕ`��
	bool ChangeScene();
	void TitleFieldInit();
	void TitleFieldUpda();
};
