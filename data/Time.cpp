//=============================================================================
//
// �^�C�}�[
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include"renderer.h"
#include"Application.h"
#include"Object.h"
#include"Game.h"
#include"Player.h"
#include "Time.h"
#include"sound.h"


LPDIRECT3DTEXTURE9 CTimer::m_apTexture[NUM_TEXTIME] = {};
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTimer::CTimer(int nPriority)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CTimer::Init()
{
	m_pObject2D = new CObject2D(CObject::OBJTYPE_UI);
	m_pObject2D->Init();
	m_pObject2D->SetSiz(D3DXVECTOR2(100.0f, 100.0f));
	m_pObject2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pObject2D->BindTexture(m_apTexture[0]);
	m_pObject2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH/2, 60.0f, 0.0f));

	for (int i = 0; i < 2; i++)
	{
		m_apNumber[i] = new CObject2D(CObject::OBJTYPE_UI);
		m_apNumber[i]->Init();
		m_apNumber[i]->SetSiz(D3DXVECTOR2(40.0f, 80.0f));
		m_apNumber[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apNumber[i]->BindTexture(m_apTexture[1]);
	}
	m_apNumber[0]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2 - 17.0f, 70.0f, 0.0f));
	m_apNumber[1]->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2 + 17.0f, 70.0f, 0.0f));
	m_nTimer = DEFAULT_TIME;
	m_apNumber[0]->SetUV(0.1f*(m_nTimer / 10), 0.1f*((m_nTimer / 10) + 1), 0.0f, 1.0f);
	m_apNumber[1]->SetUV(0.1f*(m_nTimer % 10), 0.1f*((m_nTimer % 10) + 1), 0.0f, 1.0f);

	return S_OK;
}

//=============================================================================
//�I��
//=============================================================================
void CTimer::Uninit()
{

}

//=============================================================================
// �X�V
//=============================================================================
void CTimer::Update()
{
	m_nCreateTimer++;
	if (m_nCreateTimer % 60 == 0)
	{
		m_nTimer--;
		if (m_nTimer < 0)
		{
			m_nTimer++;
		}
		m_apNumber[0]->SetUV(0.1f*(m_nTimer / 10), 0.1f*((m_nTimer / 10) + 1),0.0f, 1.0f);
		m_apNumber[1]->SetUV(0.1f*(m_nTimer % 10), 0.1f*((m_nTimer % 10) + 1), 0.0f, 1.0f);
	}
}

//=============================================================================
// �`��
//=============================================================================
void CTimer::Draw()
{

}

//=============================================================================
// �f�[�^��ǂݍ���
//=============================================================================
HRESULT CTimer::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�^�C�g�����S
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\TIME.png",
		&m_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number002.png",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// �f�[�^��j��
//=============================================================================
void CTimer::Unload()
{
	for (int i = 0; i <NUM_TEXTIME; i++)
	{
		if (m_apTexture[i] != nullptr)
		{//�e�N�X�`���̔j��
			m_apTexture[i]->Release();
			m_apTexture[i] = nullptr;
		}
	}
}

//=============================================================================
// �w�i����
//=============================================================================
CTimer * CTimer::Create()
{
	CTimer*pLife;
	pLife = new CTimer;
	if (pLife != nullptr)
	{// �|���S���̏���������
		pLife->Init();
	}
	return pLife;
}
