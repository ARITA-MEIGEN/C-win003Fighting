//=============================================================================
//
// �̗̓o�[
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include"Life.h"
#include"renderer.h"
#include"Application.h"
#include"Object.h"
#include"Game.h"
#include"Player.h"

LPDIRECT3DTEXTURE9 CLife::m_apTexture[NUM_TEXLIFE] = {};
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLife::CLife(int nPriority)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLife::~CLife()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CLife::Init(float Width, float Height)
{
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < NUM_TEXLIFE; i++)
		{
			m_apObject2D[i + j*NUM_TEXLIFE] = new CObject2D(CObject::OBJTYPE_UI);
			m_apObject2D[i + j*NUM_TEXLIFE]->Init();
			m_apObject2D[i + j*NUM_TEXLIFE]->SetSiz(D3DXVECTOR2(0.0f, 30.0f));
			m_apObject2D[i + j*NUM_TEXLIFE]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			m_apObject2D[i + j*NUM_TEXLIFE]->BindTexture(m_apTexture[i]);
		}
	}
	m_apObject2D[0]->SetSiz(D3DXVECTOR2(0.0f, 40.0f));
	m_apObject2D[0]->SetPos(D3DXVECTOR3(LIFE_POS_P1 + 10, LIFE_POS_Y, 0.0f));
	m_apObject2D[1]->SetPos(D3DXVECTOR3(LIFE_POS_P1, LIFE_POS_Y, 0.0f));
	m_apObject2D[2]->SetPos(D3DXVECTOR3(LIFE_POS_P1, LIFE_POS_Y, 0.0f));

	m_apObject2D[3]->SetSiz(D3DXVECTOR2(0.0f, 40.0f));
	m_apObject2D[3]->SetPos(D3DXVECTOR3(SCREEN_WIDTH - (LIFE_POS_P1 + 10), LIFE_POS_Y, 0.0f));
	m_apObject2D[4]->SetPos(D3DXVECTOR3(SCREEN_WIDTH - LIFE_POS_P1, LIFE_POS_Y, 0.0f));
	m_apObject2D[5]->SetPos(D3DXVECTOR3(SCREEN_WIDTH - LIFE_POS_P1, LIFE_POS_Y, 0.0f));

	for (int i = 0; i < NUM_TEXLIFE; i++)
	{
		m_apObject2D[i]->SetPosL(CGame::GetPlayer(0)->GetLife() *(LIFEBAR_WIDTH / CGame::GetPlayer(0)->GetLife()));
	}
	for (int i = 0; i < NUM_TEXLIFE; i++)
	{
		m_apObject2D[i + NUM_TEXLIFE]->SetPosR(CGame::GetPlayer(1)->GetLife() *(LIFEBAR_WIDTH / CGame::GetPlayer(1)->GetLife()));
		m_apObject2D[i + NUM_TEXLIFE]->SetUV(1.0f, 0.0f, 0.0f, 1.0f);
	}
	m_apObject2D[0]->SetPosL(CGame::GetPlayer(0)->GetLife() *(LIFEBAR_WIDTH / CGame::GetPlayer(0)->GetLife()) + 20.0f);
	m_apObject2D[NUM_TEXLIFE]->SetPosR(CGame::GetPlayer(1)->GetLife() *(LIFEBAR_WIDTH / CGame::GetPlayer(1)->GetLife()) + 20.0f);

	m_anMAX[0] = CGame::GetPlayer(0)->GetLife();
	m_anMAX[1] = CGame::GetPlayer(1)->GetLife();

	return S_OK;
}

//=============================================================================
//�I��
//=============================================================================
void CLife::Uninit()
{
	for (int i = 0; i < NUM_TEXLIFE; i++)
	{
		if (m_apTexture[i]!=nullptr)
		{
			m_apTexture[i] = nullptr;
			delete m_apTexture[i];
		}
	}
	
}

//=============================================================================
// �X�V
//=============================================================================
void CLife::Update()
{
	m_apObject2D[2]->SetPosL(CGame::GetPlayer(0)->GetLife() *(LIFEBAR_WIDTH / m_anMAX[0]));
	m_apObject2D[5]->SetPosR(CGame::GetPlayer(1)->GetLife() *(LIFEBAR_WIDTH / m_anMAX[1]));
}

//=============================================================================
// �`��
//=============================================================================
void CLife::Draw()
{

}

//=============================================================================
// �f�[�^��ǂݍ���
//=============================================================================
HRESULT CLife::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�^�C�g�����S
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Frame.png",
		&m_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\LifeBlack.png",
		&m_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Life.png",
		&m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// �f�[�^��j��
//=============================================================================
void CLife::Unload()
{
	for (int i = 0; i <NUM_TEXLIFE; i++)
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
CLife * CLife::Create()
{
	CLife*pLife;
	pLife = new CLife;
	if (pLife != nullptr)
	{// �|���S���̏���������
		pLife->Init((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	}
	return pLife;
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CLife::SetPos(D3DXVECTOR3 pos, int number)
{
	m_apObject2D[number]->SetPos(pos);
}

