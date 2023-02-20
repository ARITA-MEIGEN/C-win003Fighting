//=================================================
// Content     (�p�[�e�B�N���̏���)(Particle.cpp)
// Author     : �L�c����
// Since      : (�쐬��2021/09/07)
//=================================================
#include"Bullet.h"
#include"effect.h"
#include"Application.h"
#include"renderer.h"
#include"Game.h"
#include"Collision.h"
#include"Player.h"
#include"sound.h"
#include"Camera.h"

CBullet::CBullet(int nPriority)
{
}
CBullet::~CBullet()
{
}
//============================
//�p�[�e�B�N���̏���������
//============================
HRESULT CBullet::Init()
{

	m_nNumber = 2;															//��x�ɕ��o����p�[�e�B�N���̐�
	m_nSpeed = 2;															//�p�[�e�B�N���̍ő呬�x
	m_nTimer = 0;															//�p�[�e�B�N���̔��ˊ��o
	m_moverot = 1;															//�e�_�x	�傫����Α傫���قǋ����Ȃ�
	m_Collision = CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
	m_Collision->SetSiz(D3DXVECTOR3(50.0f,50.0f,10.0f));
	PlaySound(SOUND_LABEL_SE_FIRE);

	return S_OK;
}

//===========================
//�p�[�e�B�N���̏I������
//===========================
void CBullet::Uninit(void)
{
	CObject::Release();
}

//===========================
//�p�[�e�B�N���̍X�V����
//===========================
void CBullet::Update(void)
{
	m_nTimer++;

	//�ړ��ʂ̐ݒ�
		for (int nCnt = 0; nCnt < m_nNumber; nCnt++)
		{
			float fAngle = ((float)(rand() % m_moverot - m_moverot / 2) + KAKUDO) / 100.0f;
			float fSpeed = ((float)(rand() % m_nSpeed)) + 2.0f;
			D3DXVECTOR3 randpos = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), 0.0f);
			D3DXVECTOR3 randmove = D3DXVECTOR3(sinf(fAngle)*fSpeed +0.0f, cosf(fAngle)*fSpeed +0.0f, 0.0f);
			float randrot = (float)(rand() % KAKUSANDO) / KAKUSANDOB +0.0f;	//�e�_�x

			CEffect::Create(m_pos,							//�ʒu�̐ݒ�
				m_siz ,										//���a�̑傫���̐ݒ�
				m_fRot + randrot,							//�摜�̊p�x
				randmove*0.0f,								//�ړ��ʂ̐ݒ�
				m_nLife,									//����
				m_col,										//���_�J���[�̐ݒ�	
				0,											//�g�p����e�N�X�`���̔ԍ�
				true);										//���Z�������邩�ǂ���
		}

	//�ʒu�X�V
	m_pos += m_move;
	m_Collision->SetPos(m_pos);

	Hit();
	
	float Centerpos = CGame::GetPlayer(0)->GetPos().x - CGame::GetPlayer(1)->GetPos().x;

	if (Centerpos - m_pos.x > FIELD_WIDTH || Centerpos - m_pos.x < -FIELD_WIDTH)
	{
		m_Collision->SetUse(false);
		m_Collision->Uninit();
		CGame::GetPlayer(m_PlayerNumber)->SetBullet(false);
		Uninit();
	}
}

void CBullet::Draw(void)
{
}

//===========================
//�e�̐���
//===========================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col,D3DXVECTOR3 move, float rot, int life,int PlayerNumber)
{
	CBullet* pParticle = new CBullet(2);

	pParticle->Init();
	pParticle->m_pos = pos;
	pParticle->m_pos.y += siz.x/2;
	pParticle->m_siz = siz;
	pParticle->m_col = col;
	pParticle->m_move = move;
	pParticle->m_fRot = rot;
	pParticle->m_nLife = life;
	pParticle->m_PlayerNumber = PlayerNumber;
	return pParticle;
}

//===========================
//�e�̓����蔻��
//===========================
bool CBullet::Hit()
{
	CPlayer* pPlayer = CGame::GetPlayer(1^ m_PlayerNumber);
	int i = 0;
	D3DXVECTOR3 Hurt, col, HurtWidth, colWidth;

	//�G�̓����蔻����Q��
	while (pPlayer->GetHurt(i) != nullptr)
	{
		CCollision*pCollision = pPlayer->GetHurt(i);

		Hurt = pCollision->GetPos();
		col = m_Collision->GetPos();
		HurtWidth = pCollision->GetWidth();
		colWidth = m_Collision->GetWidth();
		pCollision = pPlayer->GetHurt(i);

		if (Hurt.x + HurtWidth.x / 2 >= col.x - colWidth.x / 2 && Hurt.x - HurtWidth.x / 2 <= col.x + colWidth.x / 2 && Hurt.y + HurtWidth.y / 2 >= col.y - colWidth.y / 2 && Hurt.y - HurtWidth.y / 2 <= col.y + colWidth.y / 2)
		{
			pPlayer->Damage_Cal(DAMAGE, (CCollision::EDAMAGE_POINT)0, 10, 10);
			Uninit();
			m_Collision->SetUse(false);
			m_Collision->Uninit();
			PlaySound(SOUND_LABEL_SE_SPECIAL);
			CGame::GetPlayer(m_PlayerNumber)->SetBullet(false);
			break;
		}
		i++;
	}
	
	return true;
}
