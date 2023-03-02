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
#include"input.h"
#include"Fade.h"
#include"Object2D.h"
#include"renderer.h"
#include"sound.h"

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
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DTEXTURE9 tex;

	//�w�i�̐���
	m_pBg = new CObject2D(CObject::OBJTYPE_UI);
	m_pBg->Init();
	m_pBg->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,0.0f));
	m_pBg->SetSiz(D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT));
	m_pBg->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Title000.png",
		&tex);

	m_pBg->BindTexture(tex);
	PlaySound(SOUND_LABEL_BGM_TITLE);


	return S_OK;
}

//====================================
//�I��
//====================================
void CTitle::Uninit()
{
	StopSound();
}

//====================================
//�X�V
//====================================
void CTitle::Update()
{
	CInput* pInput = CApplication::GetInput();

		//�w��̃L�[�������ꂽ���ǂ���
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if (pInput->Trigger(KEY_ALL))
		{
			CApplication::GetFade()->SetFade(CApplication::MODE_GAME);
			PlaySound(SOUND_LABEL_SE_START);
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