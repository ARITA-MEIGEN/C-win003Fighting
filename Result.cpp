//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
//�C���N���[�h
#include"main.h"
#include"Result.h"
#include"InputJoyPad.h"
#include"InputKeyBoard.h"
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
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	CInputJoyPad*pJoypad = CApplication::GetInputJoypad();

	////�����̃T�C�Y���z����f�J���Ȃ�o�O�ɑ΂��Ẳ��}���u
	//m_pScore->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2 - 70.0f, SCREEN_HEIGHT / 2 + 30.5f, 0.0f), D3DXVECTOR2(RESULT_SCORE_WIDTH, RESULT_SCORE_HEIGHT));

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if ((pKeyboard->GetTrigger(DIK_RETURN) == true ||
			pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_START) == true)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_Y) == true
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_X) == true
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_A) == true
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_B) == true)		//ENTER�L�[
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
