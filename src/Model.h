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
	static		CModel*Create();
	void		Shadow();
	void		Siz();

	//�Z�b�^�[
	void		SetModel(char* Filename);
	void		SetParent(CModel*pModel) ;
	void		SetRot(D3DXVECTOR3 rot);	
	void		SetPos(D3DXVECTOR3 pos);
	void		SetIdx(int Idx) { m_nIdx = Idx; };
	void		SetDPos(D3DXVECTOR3 pos) { m_DefaultPos = pos; };
	void		SetDRot(D3DXVECTOR3 rot) { m_DefaultRot = rot; };
	void		SetCol(D3DXCOLOR col) { m_col = col; };

	//�Q�b�^�[
	CModel*		GetParent();
	D3DXMATRIX	GetMtx();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3	GetPos();
	float		GetWidth();
	int			GetIdx() { return m_nIdx; };
	D3DXVECTOR3	GetDPos() { return m_DefaultPos; };
	D3DXVECTOR3	GetDRot() { return m_DefaultRot; };
	D3DXCOLOR	GetCol() { return m_col; };



private:
	LPD3DXBUFFER		m_pBuffMat;		//�}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;		//�}�e���A�����̐�
	LPD3DXMESH			m_pMesh;		//���b�V���ւ̃|�C���^
	D3DXMATRIX			m_mtxParent;	//�e�̃��[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxWorld;		//�q�̃��[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxShadow;	//�e�p���[���h�}�g���b�N�X
	D3DXVECTOR3			m_pos;			//�ʒu
	D3DXVECTOR3			m_posold;		//�O��̈ʒu
	D3DXVECTOR3			m_rotDest;		//�ړI�̊p�x�̕ۑ�
	D3DXVECTOR3			m_rot;			//����
	D3DXVECTOR3			m_vtxMax;		//���f���̍ő���W
	D3DXVECTOR3			m_vtxMin;		//���f���̍��W�̍ŏ��l
	int					m_nIdx;			//���g�̔ԍ�
	CModel*				m_pParent;		//�e�̃��f���ւ̃|�C���^
	D3DXVECTOR3			m_DefaultPos;	//�����ʒu
	D3DXVECTOR3			m_DefaultRot;	//�����ʒu
	D3DXCOLOR			m_col;			//�F
};

#endif

