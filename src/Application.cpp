//=============================================================================
//
// �A�v���P�[�V����
// Author : �L�c����
//
//=============================================================================

// �C���N���[�h
#include "Application.h"
#include"renderer.h"
#include"main.h"
#include"input.h"
#include"DebugProc.h"
#include"Object.h"
#include"Fade.h"
#include"sound.h"
#include"Title.h"
#include"Game.h"
#include"Result.h"

//====================================
//�ÓI�����o�ϐ�
//====================================
CRenderer*CApplication::m_pRenderer = nullptr;
CDebugProc*CApplication::m_pDebugProc = nullptr;
CFade* CApplication::m_pFade = nullptr;
int CApplication::m_nWinner = 0;

//�R���g���[���[����
CInput*CApplication::m_pInput = nullptr;

//��ʑJ�ڎ���
CApplication::MODE CApplication::m_mode;
CTitle*CApplication::m_pTitle = nullptr;
CGame*CApplication::m_pGame = nullptr;
CResult*CApplication::m_pResult = nullptr;

//====================================
//�R���X�g���N�^
//====================================
CApplication::CApplication()
{

}

//====================================
//�f�X�g���N�^
//====================================
CApplication::~CApplication()
{

}

//====================================
//������
//====================================
HRESULT CApplication::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	m_pRenderer = new CRenderer;

	//����������
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//�����������Ɏ��s�����ꍇ
		return-1;
	}

	//-------------------------------
	//�L�[�{�[�h�ƃW���C�p�b�h�̐���
	//-------------------------------
	m_pInput = CInput::Create();

	//���͏����̏�����
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return-1;
	}

	//----------------------------
	//�T�E���h������
	//----------------------------

	InitSound(hWnd);

	//----------------------------
	// ���[�h�̐ݒ�
	//----------------------------
	m_pFade = new CFade;
	SetMode(MODE_TITLE);
	m_pFade->Init(MODE_TITLE);

	//----------------------------
	// �f�o�b�O�p�����̐���
	//----------------------------
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();

	return S_OK;
}

//====================================
//�I��
//====================================
void CApplication::Uninit()
{
	//----------------------------
	//�����_�����O�N���X�̔j��
	//----------------------------
	if (m_pRenderer != nullptr)
	{
		// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//----------------------------
	// �^�C�g���̏I��
	//----------------------------
	if (m_pTitle != nullptr)
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}

	//----------------------------
	// �Q�[���̏I��
	//----------------------------
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	//----------------------------
	// ���U���g�̏I��
	//----------------------------
	if (m_pResult != nullptr)
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}

	//----------------------------
	// �t�F�[�h�̏I��
	//----------------------------
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	//----------------------------
	// �f�o�b�O�p�����̏I��
	//----------------------------
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	//----------------------------
	// ���͏����̏I��
	//----------------------------
	if (m_pInput != nullptr)
	{
		m_pInput->Uninit();
	}

	//�T�E���h�����̏I��
	UninitSound();
}

//====================================
//�X�V
//====================================
void CApplication::Update()
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�ւ̃|�C���^
																			//���C���[�t���[��
	if (m_pInput->Trigger(DIK_1))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (m_pInput->Trigger(DIK_2))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
#endif // _DEBUG

	//�����_���[�X�V
	m_pRenderer->Update();

	//�L�[�{�[�h�ƃW���C�p�b�h�̍X�V
	if (m_pInput != nullptr)
	{
		m_pInput->Update();
	}

	//���[�h���Ƃ̍X�V
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_GAME:
		m_pGame->Update();
		break;

	case MODE_RESULT:
		m_pResult->Update();
		break;

	default:
		break;
	}

	//�t�F�[�h�̍X�V
	m_pFade->Update();
}

//====================================
//�`��
//====================================
void CApplication::Draw()
{
	m_pRenderer->Draw();

	//���[�h���Ƃ̕`��
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Draw();
		break;

	case MODE_GAME:
		m_pGame->Draw();
		break;

	case MODE_RESULT:
		m_pResult->Draw();
		break;

	default:
		break;
	}
}

//====================================
//�����_���[�̎擾
//====================================
CRenderer * CApplication::GetRenderer()
{
	return m_pRenderer;
}

//====================================
//�f�o�b�O���̎擾
//====================================
CDebugProc * CApplication::GetDebugProc()
{
	return m_pDebugProc;
}

//====================================
//�t�F�[�h�̎擾
//====================================
CFade * CApplication::GetFade()
{
	return m_pFade;
}

//======================================================
//���[�h�̐ݒ�
//======================================================
void CApplication::SetMode(MODE mode)
{
	//�V�������(���[�h)�̏���������
	switch (m_mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle=nullptr;
		break;

	case MODE_GAME:		//�Q�[�����
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
		break;

	case MODE_RESULT:	//���U���g���
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
		break;
	}
	m_mode = mode;	//���݂̉��(���[�h)��؂�ւ���
	CObject::ReleaseAll();

	//�V�������(���[�h)�̏���������
	switch (m_mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		m_pTitle = nullptr;
		m_pTitle = new CTitle;
		m_pTitle->Init();
		break;

	case MODE_GAME:		//�Q�[�����
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
		break;

	case MODE_RESULT:	//���U���g���
		m_pResult = nullptr;
		m_pResult = new CResult;
		m_pResult->Init();
		break;
	}
}