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
#include"renderer.h"
#include"sound.h"

//�ÓI�����o�ϐ�
CObject2D*CResult::m_pBg = nullptr;
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
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DTEXTURE9 tex[3];

	//�w�i�̐���
	m_pBg = new CObject2D(CObject::OBJTYPE_UI);
	m_pBg->Init();
	m_pBg->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 0.0f));
	m_pBg->SetSiz(D3DXVECTOR2((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
	m_pBg->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result000.png",
		&tex[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result001.png",
		&tex[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Result002.png",
		&tex[2]);

	m_pBg->BindTexture(tex[CApplication::GetWinner()]);

	PlaySound(SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//====================================
//�I��
//====================================
void CResult::Uninit()
{
	StopSound();

}

//====================================
//�X�V
//====================================
void CResult::Update()
{
	CInput* pInput = CApplication::GetInput();

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		if ((pInput->Trigger(KEY_ALL)) == true)		//ENTER�L�[
		{//�G���^�[�Ń����L���O��
		 //���[�h�ݒ�
			PlaySound(SOUND_LABEL_SE_START);
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
