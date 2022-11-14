#include "ControlBase.h"

ControlBase::~ControlBase()
{
}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void (ControlBase::* ControlBase::initTable[])(DebugCamera* camera) = {
	nullptr, //�^�C�g��
	&ControlBase::Init_Tutorial, //�`���[�g���A��
	&ControlBase::Init_Play, //�v���[�V�[��
	&ControlBase::Init_Boss, //�{�X�V�[��
	nullptr //�}�b�v�G�f�B�^
};

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
/*scneManager�̃V�[�����ƍ��킹��*/
void (ControlBase::* ControlBase::updateTable[])(DebugCamera* camera) = {
	nullptr, //�^�C�g��
	&ControlBase::Update_Tutorial, //�`���[�g���A��
	&ControlBase::Update_Play, //�v���[�V�[��
	&ControlBase::Update_Boss, //�{�X�V�[��
	nullptr //�}�b�v�G�f�B�^
};

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
void (ControlBase::* ControlBase::drawTable[])() = {
	nullptr, //�^�C�g��
	&ControlBase::Draw_Tutorial, //�`���[�g���A��
	&ControlBase::Draw_Play, //�v���[�V�[��
	&ControlBase::Draw_Boss, //�{�X�V�[��
	nullptr //�}�b�v�G�f�B�^
};

#include"SceneManager.h"

void ControlBase::Update(DebugCamera* camera)
{
	//�V�[���ɉ������X�V����
	(this->*updateTable[SceneManager::GetInstance()->GetScene()])(camera);
}

void ControlBase::Initialize(DebugCamera* camera)
{
	//�V�[���ɉ������X�V����
	(this->*initTable[SceneManager::GetInstance()->GetScene()])(camera);
}

void ControlBase::Draw()
{
	//�V�[���ɉ������X�V����
	(this->*drawTable[SceneManager::GetInstance()->GetScene()])();
}
