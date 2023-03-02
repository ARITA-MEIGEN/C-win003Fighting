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

CParticle::CParticle(int nPriority):CObject(nPriority)
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
	m_nSpeed = 1;															//�p�[�e�B�N���̍ő呬�x
	m_nTimer = 0;															//�p�[�e�B�N���̔��ˊ��o
	m_moverot = 628;														//�g�U	�傫����Α傫���قǋ����Ȃ�

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
	switch (m_ParticleType)
	{
	case CParticle::PAR_FIREFLOWER:
			m_nLife = 10;
			m_nNumber = 100;
			m_fRot = D3DX_PI*BULLET_ANGLE;

		break;
	case CParticle::MAX_PARTICLE:
		break;
	default:
		break;
	}

	for (int i = 0; i < m_nNumber; i++)
	{
		float fAngle = ((float)(rand() % m_moverot - m_moverot / 2) + nMovewidthMin) / 100.0f;
		float fSpeed = ((float)(rand() % m_nSpeed)) + 2.0f;
		D3DXVECTOR3 randpos = D3DXVECTOR3((float)(rand() % nStartRandPosX - (nStartRandPosX / 2)), (float)(rand() % nStartRandPosY - (nStartRandPosY / 2)), 0.0f);			//�����̏����ʒu
		D3DXVECTOR3 randmove = D3DXVECTOR3(sinf(fAngle)*fSpeed + 0.0f, cosf(fAngle)*fSpeed + 0.0f, 0.0f);
		float randrot = (float)(rand() % 314) / 100 + 0.0f;	//�g�U�x

		CEffect::Create(m_pos + randpos,				//�ʒu�̐ݒ�
			m_siz,										//���a�̑傫���̐ݒ�
			m_fRot+ randrot,							//�摜�̊p�x
			randmove,									//�����_���Ȉړ���
			m_nLife,									//����
			m_col,										//���_�J���[�̐ݒ�	
			1,
			true);
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
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, Particle type)
{
	CParticle* pParticle = new CParticle(2);
	pParticle->Init();
	pParticle->m_pos = pos;
	pParticle->m_pos.y += siz.x / 2;
	pParticle->m_siz = siz;
	pParticle->m_col = col;
	pParticle->m_ParticleType = type;
	return pParticle;
}
