//=============================================================================
//
// X�t�@�C���ǂݍ���
// Author : �L�c����
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"
#include"ObjectX.h"
#include"Model.h"

//�O���錾
class CShadow;
class CModel;

class CPlayer :public CObject
{
public:
	//�L�[�v�f
	struct KEY
	{
		float fPosX;
		float fPosY;
		float fPosZ;

		float fRotX;
		float fRotY;
		float fRotZ;
	};

	//�L�[���
	struct KEY_SET 
	{
		int nFrame;				//���̃��[�V�����܂ł̎���
		KEY aKey[NUM_PLAYERPARTS];
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//�v���g�^�C�v�錾
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	void			ControlPlayer(void);				//�v���C���[�̑���
	static CPlayer*	Create(D3DXVECTOR3 pos, int Priority);
	D3DXMATRIX		GetMtx();
	void			MotionPlayer();
	void			rolling();
private:
	CModel*			m_apModel[NUM_PLAYERPARTS];	//���f���̃C���X�^���X
	D3DXMATRIX		m_mtxWorld;					//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;						//�ʒu
	D3DXVECTOR3		m_rot;						//����
	D3DXVECTOR3		m_move;						//�ړ���
	D3DXVECTOR3		m_posold;					//�O��̈ʒu
	//CShadow*		m_Shadow;					//�e
	D3DXVECTOR3		m_rotDest;					//�ړI�̊p�x�̕ۑ�
	int				m_MotionCnt;				//���[�V�����J�E���^�[
	int				m_nNumKey;					//�L�[�̑���
	int				m_nCurrentKey;				//���݂̃L�[�ԍ�
	D3DXMATRIX		m_mtxRot;					//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION	m_quat;						//�N�H�[�^�j�I��
	D3DXVECTOR3		m_vecAxis;					//��]��
	float			m_fRolling;					//��]�ʁ@(��]�p)
};

#endif
