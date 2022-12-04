//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
//�C���N���[�h
#include"main.h"
#include"Title.h"
#include"Application.h"
#include"InputJoyPad.h"
#include"InputKeyBoard.h"
#include"Fade.h"
#include"Object2D.h"

//�ÓI�����o�ϐ�
CObject2D*CTitle::m_pBg = nullptr;
//====================================
//�R���X�g���N�^
//====================================
CTitle::CTitle()
{
}

//====================================
//�f�X�g���N�^
//====================================
CTitle::~CTitle()
{

}

//====================================
//������
//====================================
HRESULT CTitle::Init()
{
	//�e�N�X�`���̓ǂݍ���
	//�w�i�̐���
	m_pBg = new CObject2D(CObject::OBJTYPE_UI);
	m_pBg->Init();
	m_pBg->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f));
	m_pBg->SetSiz(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_pBg->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

//====================================
//�I��
//====================================
void CTitle::Uninit()
{
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}
	//�e�N�X�`���̔j��
}

//====================================
//�X�V
//====================================
void CTitle::Update()
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	CInputJoyPad*pJoypad = CApplication::GetInputJoypad();

		//�w��̃L�[�������ꂽ���ǂ���
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_Y)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_X)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_A)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_B)
			|| pJoypad->GetJoypadTrigger(CInputJoyPad::JOYKEY_START))
		{
			CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
		}
	}
}

//====================================
//�`��
//====================================
void CTitle::Draw()
{

}

//====================================
//�w�i�̎擾
//====================================
CObject2D* CTitle::GetBg()
{
	return m_pBg;
}