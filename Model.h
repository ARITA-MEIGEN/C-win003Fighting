//=============================================================================
//
// X�t�@�C���ǂݍ���
// Author : �L�c����
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//�C���N���[�h
#include"main.h"
#include"ObjectX.h"

//�}�N����`
#define PLAYER_LEGPARTS	(13)
#define	NUM_PLAYERPARTS	(1+PLAYER_LEGPARTS)

//�O���錾
class CShadow;

class CModel
{
public:
	explicit CModel();
	~CModel();
	//�v���g�^�C�v�錾
	HRESULT		Init();
	void		Uninit(void);
	void		Update(void);
	void		Draw(D3DXMATRIX pMtx);
	static		CModel*Create(D3DXVECTOR3 pos, int Number);
	void		LoadModel();
	void		Shadow();
	void		Siz(int number);
	void		SetParent(CModel*pModel) ;
	CModel*		GetParent();
	D3DXMATRIX	GetMtx();
	void		SetRot(D3DXVECTOR3 rot);	
	D3DXVECTOR3 GetRot();
	void		SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3	GetPos();
	float		GetWidth();

private:
	static LPD3DXBUFFER	m_pPlayerBuffMat[NUM_PLAYERPARTS];	//�}�e���A�����ւ̃|�C���^
	static DWORD		m_nPlayerNumMat[NUM_PLAYERPARTS];	//�}�e���A�����̐�
	static LPD3DXMESH	m_pPlayerMesh[NUM_PLAYERPARTS];		//���b�V���ւ̃|�C���^
	D3DXMATRIX			m_mtxParent;						//�e�̃��[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxWorld;							//�q�̃��[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxShadow;						//�e�p���[���h�}�g���b�N�X
	D3DXVECTOR3			m_pos;								//�ʒu
	D3DXVECTOR3			m_posold;							//�O��̈ʒu
	D3DXVECTOR3			m_rotDest;							//�ړI�̊p�x�̕ۑ�
	D3DXVECTOR3			m_rot;								//����
	D3DXVECTOR3			m_vtxMax;							//���f���̍ő���W
	D3DXVECTOR3			m_vtxMin;							//���f���̍��W�̍ŏ��l
	int					m_nNumber;							//���g�̔ԍ�
	CModel*				m_pParent;							//�e�̃��f���ւ̃|�C���^
};

#endif

