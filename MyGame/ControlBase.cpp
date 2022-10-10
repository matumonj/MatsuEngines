#include "ControlBase.h"
ControlBase::~ControlBase()
{

}

/*------------------------*/
/*--------�X�V����---------*/
/*------------------------*/
/*scneManager�̃V�[�����ƍ��킹��*/
void (ControlBase::* ControlBase::updateTable[])(DebugCamera* camera) = {
	nullptr,//�^�C�g��
	&ControlBase::Update_Tutorial,//�`���[�g���A��
	&ControlBase::Update_Play,//�v���[�V�[��
	&ControlBase::Update_Boss,//�{�X�V�[��
	nullptr//�}�b�v�G�f�B�^
};

#include"SceneManager.h"
void ControlBase::Update(DebugCamera* camera)
{
	//�V�[���ɉ������X�V����
	(this->*updateTable[SceneManager::GetInstance()->GetScene()])(camera);
}