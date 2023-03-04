//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
//�C���N���[�h
#include"main.h"
#include"Application.h"
#include"Game.h"
#include"Player.h"
#include"Floor.h"
#include"Player.h"
#include"Mesh.h"
#include"Camera.h"
#include"Light.h"
#include"Shadow.h"
#include"Fade.h"
#include"Life.h"
#include"sound.h"
#include"input.h"
#include"effect.h"
#include"Time.h"
#include"UI.h"

//�ÓI�����o�ϐ�
CPlayer*CGame::m_pPlayer[2] = {};
CLife*CGame::m_Life = nullptr;				//�̗̓Q�[�W
CGame::GAME CGame::m_gamestate;
bool CGame::bDebugCamera = nullptr;

CCamera*CGame::m_pCamera = nullptr;
CLight*CGame::m_pLight = nullptr;
CFloor*CGame::m_pFloor = nullptr;
CTimer*CGame::m_pTimer = nullptr;
CUI*CGame::m_pUI = nullptr;
//====================================
//�R���X�g���N�^
//====================================
CGame::CGame()
{
}

//====================================
//�f�X�g���N�^
//====================================
CGame::~CGame()
{

}

//====================================
//������
//====================================
HRESULT CGame::Init()
{
	//�e�N�X�`���̓ǂݍ���
	CShadow::Load();
	CFloor::Load();
	CLife::Load();
	CEffect::Load();
	CTimer::Load();
	CShadow::Load();

	//�v���C���[�̐���
	m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI*0.5f, 0.0f));
	m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	m_pPlayer[0]->SetEnemy(m_pPlayer[1]);
	m_pPlayer[1]->SetEnemy(m_pPlayer[0]);

	//�̗̓Q�[�W
	m_Life = CLife::Create();

	//�J�����̐ݒ�
	m_pCamera = CCamera::Create();

	//���C�g�̐ݒ�
	m_pLight = new CLight;
	m_pLight->Init();

	//��
	CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f),D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	CFloor::Create(D3DXVECTOR3(200.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));
	CFloor::Create(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f));

	//���̕�
	CFloor::Create(D3DXVECTOR3(0.0f, 100.0f, 300.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f,0.0f , 0.0f));	
	CFloor::Create(D3DXVECTOR3(200.0f, 100.0f, 300.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, 0.0f, 0.0f));
	CFloor::Create(D3DXVECTOR3(-200.0f, 100.0f, 300.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, 0.0f, 0.0f));

	//���̕�
	CFloor::Create(D3DXVECTOR3(STAGE_WIDTH, 100.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, D3DX_PI*0.5f, 0.0f));
	CFloor::Create(D3DXVECTOR3(-STAGE_WIDTH, 100.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f, 500.0f), D3DXVECTOR3(D3DX_PI*-0.5f, D3DX_PI*-0.5f, 0.0f));

	//�f�o�b�O�p�J�������샂�[�h
	bDebugCamera = false;

	PlaySound(SOUND_LABEL_BGM_BATTLE001);

	m_pTimer = CTimer::Create();
	m_Timer = 0;

	//UI����
	m_pUI = CUI::Create();

	m_gamestate = GAME_START;

	return S_OK;
}

//====================================
//�I��
//====================================
void CGame::Uninit()
{
	//�e�N�X�`���̔j��
	CShadow::Unload();
	CFloor::Unload();
	CLife::Unload();
	CEffect::Unload();
	CTimer::Unload();
	CShadow::Unload();

	//�J�����̐ݒ�
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
	}

	//���C�g�̐ݒ�
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
	}

	if (m_Life != nullptr)
	{
		m_Life->Uninit();
		delete m_Life;
	}

	if (m_pUI != nullptr)
	{
		m_pUI->Uninit();
		delete m_pUI;
	}
	
	if (m_pTimer != nullptr)
	{
		m_pTimer->Uninit();
		delete m_pTimer;
	}

	StopSound();
}

//====================================
//�X�V
//====================================
void CGame::Update()
{
	CInput* pInput = CApplication::GetInput();
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		m_pUI->Update();	//UI
		m_pTimer->Update();

#ifdef _DEBUG
		//�w��̃L�[�������ꂽ���ǂ���
		if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			if (pInput->Trigger(DIK_RETURN))
			{
				CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
			}
		}
#endif // !_DEBUG
	}
	m_pCamera->Update();
	m_pLight->Update();
	m_Life->Update();

	//�Е����񂾏ꍇ�^�C�}�[���Z
	if (m_pPlayer[0]->GetLife() <= 0 || m_pPlayer[1]->GetLife() <= 0 || m_pTimer->GetTimer() <= 0)
	{
		if (m_pPlayer[0]->GetNowMotion() == CPlayer::PM_DOWN|| m_pPlayer[1]->GetNowMotion() == CPlayer::PM_DOWN|| m_pTimer->GetTimer() <= 0)
		{
			CGame::SetGame(CGame::GAME_END);
		}

		m_Timer++;
		if (m_pPlayer[1]->GetLife() == m_pPlayer[0]->GetLife())
		{
			CApplication::SetWinner(2);
		}
		else if (m_pPlayer[0]->GetLife() < m_pPlayer[1]->GetLife())
		{
			CApplication::SetWinner(1);
		}
		else if (m_pPlayer[1]->GetLife() < m_pPlayer[0]->GetLife())
		{
			CApplication::SetWinner(0);
		}
	}

	//�^�C�}�[�����l�ȏ�Ń��U���g�ֈڍs
	if (m_Timer >= END_TIMER&&CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		CApplication::GetFade()->SetFade(CApplication::MODE_RESULT);
	}

	if (CGame::GetTimer()->GetTimer() == DEFAULT_TIME&& CGame::GetTimer()->GetTimer() < 0)
	{
		m_gamestate = GAME_END;
	}	
}

//====================================
//�`��
//====================================
void CGame::Draw()
{
	m_pCamera->Set();
}

//====================================
//�Q�[���̏�Ԑݒ�
//====================================
void CGame::SetGame(GAME gamestate)
{
	m_gamestate = gamestate;
}

//====================================
//�Q�[���̏�Ԋl��
//====================================
CGame::GAME CGame::GetGame()
{
	return m_gamestate;
}

//====================================
//�J�����̎擾
//====================================
CCamera * CGame::GetCamera()
{
	return m_pCamera;
}

//====================================
//���̏��
//====================================
CFloor * CGame::GetFloor()
{
	return m_pFloor;
}

//====================================
//���C�g�̏��
//====================================
CLight * CGame::GetLight()
{
	return m_pLight;
}