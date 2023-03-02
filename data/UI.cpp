//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
//�C���N���[�h
#include"main.h"
#include"UI.h"
#include"input.h"
#include"Fade.h"
#include"Application.h"
#include"renderer.h"
#include"sound.h"
#include"Game.h"


//�ÓI�����o�ϐ�
CObject2D*CUI::m_pStart[2] = {};

//====================================
//�R���X�g���N�^
//====================================
CUI::CUI(int nPriority)
{
}

//====================================
//�f�X�g���N�^
//====================================
CUI::~CUI()
{

}

//====================================
//������
//====================================
HRESULT CUI::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DTEXTURE9 tex[2];
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\READY.png",
		&tex[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FIGHT.png",
		&tex[1]);

	//�w�i�̐���
	for (int i = 0; i < 2; i++)
	{
		m_pStart[i] = new CObject2D(CObject::OBJTYPE_UI);
		m_pStart[i]->Init();
		m_pStart[i]->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f));
		m_pStart[i]->SetSiz(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		m_pStart[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - i));
		m_pStart[i]->BindTexture(tex[i]);
	}

	m_timer = 0;
	return S_OK;
}

//====================================
//�I��
//====================================
void CUI::Uninit()
{
}

//====================================
//�X�V
//====================================
void CUI::Update()
{
	m_timer++;
	if (m_timer >= 120)
	{
		m_pStart[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		CGame::SetGame(CGame::GAME_ROUND1);
	}
	else  if (m_timer >= 60)
	{
		m_pStart[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pStart[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else  if (m_timer >= 30)
	{
		m_pStart[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_pStart[0]->GetCol().a - 0.05f));
	}
}

//====================================
//����
//====================================
CUI * CUI::Create()
{
	CUI* pUI;
	pUI = new CUI;
	pUI->Init();
	return pUI;
}
