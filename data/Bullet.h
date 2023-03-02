#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"
#include"Object2D.h"
#include"Object.h"

#define KAKUDO	(-100)			//�ړ�����
#define KAKUSANDO (20)			//���̊p�x
#define KAKUSANDOB (10)			//���̊p�x
#define	POS_ADJUSTMENT	(30.0f)	//�������p
#define	DAMAGE	(70)	//�������p

class CCollision;

class CBullet:public CObject
{
public:
	explicit CBullet(int nPriority = 0);
	~CBullet();
	//�v���g�^�C�v�錾
	HRESULT Init(void);		//�p�[�e�B�N���̏���������
	void Uninit(void);		//�p�[�e�B�N���̏I������
	void Update(void);		//�p�[�e�B�N���̍X�V����
	void Draw(void);		//�p�[�e�B�N���̍X�V����

	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, D3DXVECTOR3 move,float rot,int life, int PlayerNumber);	//�p�[�e�B�N���̏��̎擾

	void Hit();	//�p�[�e�B�N���̏��̎擾

	//�Q�b�^�[
	D3DXVECTOR3	GetPos() { return m_pos; };

private:
	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3  m_move;	//�ړ���
	D3DXCOLOR m_col;		//���_�J���[
	int m_nTimer;			//�^�C�}�[(�G�t�F�N�g�̔��ˊԊu)
	int  m_nSpeed;			//�G�t�F�N�g�̑��x
	int  m_nNumber;			//�G�t�F�N�g�̐�
	D3DXVECTOR3  m_siz;		//�傫��
	int  m_nLife;			//����
	float m_fRot;			//�p�x
	int m_moverot;			
	CCollision *	m_Collision;	//�����蔻��
	int m_PlayerNumber;		//�v���C���[�ԍ�
};


#endif 

#pragma once
