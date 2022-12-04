//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
#include"Player.h"
#include"Application.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Model.h"
#include"Game.h"

//�}�N����`
#define PLAYER_SPEED	(1.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x
#define NUM_MODELPARTS	(1)		//���f���̃p�[�c��

//�O���[�o���ϐ�
CPlayer::KEY_SET g_aKeySet[]	//�L�[�̐����ƂŃ����o�ϐ���
{
	{//�ŏ�
		60,
		{
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//����
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//�r�̓y��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }, 	//3��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }}		//4��
	},
	{//2��
		60,
		{
		{ 0.0f,-10.0f,0.0f ,0.0f,0.0f,0.0f },		//����
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//�r�̓y��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }, 	//3��
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4�֐�
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4������
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }}		//4��
	},
};
//===========================
//�R���X�g���N�^
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{

}

//===========================
//�f�X�g���N�^
//===========================
CPlayer::~CPlayer()
{

}

//===========================
//����������
//===========================
HRESULT CPlayer::Init()
{
	m_nNumKey = 1;

	for (int i = 0; i < NUM_PLAYERPARTS; i++)
	{//�v���C���[�̐���
		m_apModel[i] = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), i);
	}

	//�e�̐ݒ�
	//�r�̓y��
	m_apModel[1]->SetParent(m_apModel[0]);
	for (int i = 0; i < 4; i++)
	{//4�̋r
		for (int j = 0; j < 2; j++)
		{//�r�̐e�ݒ�(3�p�[�c��1�r)�e�p�[�c�͑��֐�
			m_apModel[(i * 3) + 3+j]->SetParent(m_apModel[(i * 3) + 2]);
		}
		//���֐߂̍쐬(�e�͋r�̓y��)
		m_apModel[(i * 3) + 2]->SetParent(m_apModel[1]);
	}

	while ((sizeof(g_aKeySet) / sizeof(KEY_SET)) > m_nNumKey)
	{//�L�[�̍ő吔�̊m�F
		m_nNumKey++;
	}

	//��]�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//===========================
//�I������
//===========================
void CPlayer::Uninit(void)
{
	CObject::Release();
}

//===========================
//�X�V����
//===========================
void CPlayer::Update(void)
{
	m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
	m_pos += m_move;		//�ʒu�̍X�V
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̍X�V


	ControlPlayer();		//����
	//MotionPlayer();			//�v���C���[�̃��[�V����

	m_pos.y = CGame::GetMesh()->Collision(m_pos);

	////�ړ��ʍX�V(����������)
	//m_move.x += (0.0f - m_move.x)*MIN_SPEED;
	//m_move.z += (0.0f - m_move.z)*MIN_SPEED;
}

//===========================
//�`�揈��
//===========================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p

	//���݂̃}�e���A�����ێ�
	pDevice->GetMaterial(&matDef);

	//�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�N�H�[�^�j�I���̐ݒ�
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fRolling);

	//��]�p�}�g���b�N�X�̐���
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//�p�[�c�̃��f���̌����𔽉f
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);					//�N�H�[�^�j�I���̔��f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	//�p�[�c�̃��f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y+ 10.0f, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//Shadow();

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int i = 0; i < 1; i++)
	{
		m_apModel[i]->Draw(m_mtxWorld);
	}
}

//===========================
//����
//===========================
void CPlayer::ControlPlayer(void)
{
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//�ړ�
	if (pKeyboard->GetPress(DIK_UP))
	{
		m_move.x = sinf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*1.0f + pCamera->Getrot().y;
	}
	 if (pKeyboard->GetPress(DIK_DOWN))
	{
		m_move.x = sinf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*0.0f + pCamera->Getrot().y;
	}
	if (pKeyboard->GetPress(DIK_LEFT))
	{
		m_move.x = sinf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*0.5f + pCamera->Getrot().y;
	}
	 if (pKeyboard->GetPress(DIK_RIGHT))
	{
		m_move.x = sinf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*-0.5f + pCamera->Getrot().y;
	}

	////���ʗ���Ă�����t��]
	//if (m_rotDest.y - m_rot.y >= D3DX_PI)
	//{
	//	m_rotDest.y -= D3DX_PI * 2;
	//}
	//else if (m_rotDest.y - m_rot.y <= -D3DX_PI)
	//{
	//	m_rotDest.y += D3DX_PI * 2;
	//}

	//m_rot.y += (m_rotDest.y - m_rot.y)*0.1f;

	//�p�x�̐��K��
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	rolling();
}

//===========================
//����
//===========================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, int Priority)
{
	CPlayer*pPlayer;
	pPlayer = new CPlayer(Priority);
	pPlayer->Init();

	return pPlayer;
}

//===========================
//�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CPlayer::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//�p�[�c�̃��[�V����
//===========================
void CPlayer::MotionPlayer()
{
	D3DXVECTOR3 pos, rot, fDiffPos, fDiffRot;
	float fRela;
	for (int i = 0; i < NUM_PLAYERPARTS; i++)
	{
		if (m_apModel[i] != nullptr)
		{
			if (m_nCurrentKey != 0)
			{
				fDiffPos = D3DXVECTOR3(
					g_aKeySet[m_nCurrentKey].aKey[i].fPosX - g_aKeySet[m_nCurrentKey - 1].aKey[i].fPosX,
					g_aKeySet[m_nCurrentKey].aKey[i].fPosY - g_aKeySet[m_nCurrentKey - 1].aKey[i].fPosY,
					g_aKeySet[m_nCurrentKey].aKey[i].fPosZ - g_aKeySet[m_nCurrentKey - 1].aKey[i].fPosZ);	//�����̎擾

				fDiffRot = D3DXVECTOR3(
					g_aKeySet[m_nCurrentKey].aKey[i].fRotX - g_aKeySet[m_nCurrentKey - 1].aKey[i].fRotX,
					g_aKeySet[m_nCurrentKey].aKey[i].fRotY - g_aKeySet[m_nCurrentKey - 1].aKey[i].fRotY,
					g_aKeySet[m_nCurrentKey].aKey[i].fRotZ - g_aKeySet[m_nCurrentKey - 1].aKey[i].fRotZ);	//�����̎擾

				fRela = (float)m_MotionCnt / g_aKeySet[m_nCurrentKey].nFrame;							//���Βl
			}
			else
			{
				fDiffPos = D3DXVECTOR3(
					g_aKeySet[0].aKey[i].fPosX - g_aKeySet[m_nNumKey - 1].aKey[i].fPosX,
					g_aKeySet[0].aKey[i].fPosY - g_aKeySet[m_nNumKey - 1].aKey[i].fPosY,
					g_aKeySet[0].aKey[i].fPosZ - g_aKeySet[m_nNumKey - 1].aKey[i].fPosZ);	//�����̎擾

				fDiffRot = D3DXVECTOR3(
					g_aKeySet[0].aKey[i].fRotX - g_aKeySet[m_nNumKey - 1].aKey[i].fRotX,
					g_aKeySet[0].aKey[i].fRotY - g_aKeySet[m_nNumKey - 1].aKey[i].fRotY,
					g_aKeySet[0].aKey[i].fRotZ - g_aKeySet[m_nNumKey - 1].aKey[i].fRotZ);	//�����̎擾

				fRela = (float)m_MotionCnt / g_aKeySet[m_nCurrentKey].nFrame;						//���Βl
			}

			//�L�[�̐ݒ�
			pos = (fDiffPos* fRela) + m_apModel[i]->GetPos();
			rot = (fDiffRot* fRela) + m_apModel[i]->GetRot();

			//�ʒu�̐ݒ�
			m_apModel[i]->SetPos(pos);
			m_apModel[i]->SetRot(rot);
		}
	}
	//�J�E���^�[�X�V
	if (m_MotionCnt++ >= g_aKeySet[m_nCurrentKey].nFrame)
	{//�L�[�ԍ��̍X�V�ƃJ�E���^�[�̃��Z�b�g
		m_nCurrentKey++;
		m_MotionCnt = 0;
		if (m_nCurrentKey >= m_nNumKey)
		{//�L�[�ԍ����ő吔�𒴂����ꍇ���Z�b�g
			m_nCurrentKey = 0;
		}
	}
}

//===========================
//��]�e�X�g
//===========================
void CPlayer::rolling()
{
	//��]���̐ݒ�
	m_vecAxis = D3DXVECTOR3(m_move.z, 0.0f, -m_move.x);

	//��]�ʂ̐ݒ�
	float Radius = m_apModel[0]->GetWidth();
	//m_fRolling = (m_move.x * m_move.z) * 2 * D3DX_PI / (Radius *D3DX_PI);
	m_fRolling = D3DXVec3Length(&m_move) * 2 * D3DX_PI / (Radius *D3DX_PI);
}



