//=================================================
// Content     (�p�[�e�B�N���̏���)(Particle.cpp)
// Author     : �L�c����
// Since      : (�쐬��2021/09/07)
//=================================================
#include"Particle.h"
#include"effect.h"
#include"Application.h"
#include"renderer.h"
#include"Game.h"
#include"Player.h"

CParticle::CParticle(int nPriority)
{
}
CParticle::~CParticle()
{
}
//============================
//�p�[�e�B�N���̏���������
//============================
HRESULT CParticle::Init()
{
	m_nSpeed = 2;															//�p�[�e�B�N���̍ő呬�x
	m_nTimer = 0;															//�p�[�e�B�N���̔��ˊ��o
	m_moverot = 314;															//�g�U	�傫����Α傫���قǋ����Ȃ�

	nStartRandPosX = 20;	//�����ʒu�̗���
	nStartRandPosY = 20;	//�����ʒu�̗���
	nMovewidthMin = 0;
	return S_OK;
}

//===========================
//�p�[�e�B�N���̏I������
//===========================
void CParticle::Uninit(void)
{
	CObject::Release();
}

//===========================
//�p�[�e�B�N���̍X�V����
//===========================
void CParticle::Update(void)
{
	//�ړ��ʂ̐ݒ�
	for (int nCnt = 0; nCnt < m_nNumber; nCnt++)
	{
		float fAngle = ((float)(rand() % m_moverot - m_moverot / 2) + nMovewidthMin) / 100.0f;
		float fSpeed = ((float)(rand() % m_nSpeed)) + 2.0f;
		D3DXVECTOR3 randpos = D3DXVECTOR3((float)(rand() % nStartRandPosX - (nStartRandPosX / 2)), (float)(rand() % nStartRandPosY - (nStartRandPosY / 2)), 0.0f);			//�����̏����ʒu
		D3DXVECTOR3 randmove = D3DXVECTOR3(sinf(fAngle)*fSpeed + 0.0f, cosf(fAngle)*fSpeed + 0.0f, 0.0f);
		float randrot = (float)(rand() % 314) / 100 + 0.0f;	//�e�_�x
		//if (m_pos.x >= CGame::GetPlayer(1 ^ m_PlayerNumber)->GetPos().x)
		//{//2P���Ō����t�]
		//	randrot *= -1;
		//}

		CEffect::Create(m_pos+ randpos,					//�ʒu�̐ݒ�
			m_siz,										//���a�̑傫���̐ݒ�
			m_fRot+ randrot,							//�摜�̊p�x
			randmove,									//�ړ��ʂ̐ݒ�
			m_nLife,									//����
			m_col,										//���_�J���[�̐ݒ�	
			2,											//�g�p����e�N�X�`���̔ԍ�
			true);										//���Z�������邩�ǂ���
	}

	Uninit();
	return;
}

void CParticle::Draw(void)
{
}

//===========================
//�e�̐���
//===========================
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, float rot, int life, int numeffect)
{
	CParticle* pParticle = new CParticle(2);

	pParticle->m_pos = pos;
	pParticle->m_pos.y += siz.x / 2;
	pParticle->m_siz = siz;
	pParticle->m_col = col;
	pParticle->m_fRot = rot;
	pParticle->m_nLife = life;
	pParticle->m_nNumber = numeffect;															//��x�ɕ��o����p�[�e�B�N���̐�
	pParticle->Init();
	return pParticle;
}
