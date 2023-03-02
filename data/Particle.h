#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"
#include"Object2D.h"
#include"Object.h"

class CParticle :public CObject
{
public:

	enum Particle
	{
		PAR_FIREFLOWER = 0,	//�ΉԌ^
		MAX_PARTICLE
	};
	explicit CParticle(int nPriority = 0);
	~CParticle();
	//�v���g�^�C�v�錾
	HRESULT Init(void);		//�p�[�e�B�N���̏���������
	void Uninit(void);		//�p�[�e�B�N���̏I������
	void Update(void);		//�p�[�e�B�N���̍X�V����
	void Draw(void);		//�p�[�e�B�N���̍X�V����
	static CParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, Particle type);	//�p�[�e�B�N���̏��̎擾

	//�Q�b�^�[
	D3DXVECTOR3	GetPos() { return m_pos; };

private:
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXCOLOR m_col;		//���_�J���[
	int m_nTimer;			//�^�C�}�[(�G�t�F�N�g�̔��ˊԊu)
	int  m_nSpeed;			//�G�t�F�N�g�̑��x
	int  m_nNumber;			//�G�t�F�N�g�̐�
	D3DXVECTOR3  m_siz;		//�傫��
	int  m_nLife;			//����
	float m_fRot;			//�p�x
	int m_moverot;
	int nMovewidthMin;		//�Œ�l
	int nStartRandPosX;	//�����ʒu�̗���
	int nStartRandPosY;	//�����ʒu�̗���
	Particle m_ParticleType;
};


#endif 

#pragma once

