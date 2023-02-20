//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
//�C���N���[�h
#include"main.h"
#include"Result.h"
#include"input.h"
#include"Fade.h"
#include"Application.h"

//�ÓI�����o�ϐ�
CBg*CResult::m_pBg = nullptr;
CScore*CResult::m_pScore=nullptr;

//====================================
//�R���X�g���N�^
//====================================
CResult::CResult()
{
}

//====================================
//�f�X�g���N�^
//====================================
CResult::~CResult()
{

}

//====================================
//������
//====================================
HRESULT CResult::Init()
{
	//�e�N�X�`���̓ǂݍ���

	//�w�i�̐���


	return S_OK;
}

//====================================
//�I��
//====================================
void CResult::Uninit()
{
	//�e�N�X�`���̔j��
}

//====================================
//�X�V
//====================================
void CResult::Update()
{
	CInput* pInput = CApplication::GetInput();

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (pInput->Trigger(DIK_RETURN) == true)		//ENTER�L�[
		{//�G���^�[�Ń����L���O��
		 //���[�h�ݒ�
			//PlaySound(SOUND_LABEL_SE_GAMESTART);
			CApplication::GetFade()->SetFade(CApplication::MODE_TITLE);
		}
	}
}

//====================================
//�`��
//====================================
void CResult::Draw()
{

}

//====================================
//�w�i�̎擾
//====================================
CBg * CResult::GetBg()
{
	return m_pBg;
}

//====================================
//�X�R�A�̎擾
//====================================
CScore*CResult::GetScore()
{
	return m_pScore;
}
