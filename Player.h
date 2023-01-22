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
class CCollision;


//�}�N����`
#define MAX_MOTION		(5)		//���[�V�����̐�
#define MAX_KEY			(60)	//�L�[�̑���
#define MAX_FRAME		(120)	//�t���[���̍ő吔
#define NUM_PARTS		(14)	//�p�[�c�̐�
#define MAX_COLLISION	(20)	//1�̃p�[�c�ɂ��ݒ�ł��锻��̐�
#define JUMP_HEIGHT		(150.0f)	//�W�����v�̍���
#define PLAYER_SPEED	(2.0f)	//�ړ����x






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
		KEY aKey[NUM_PARTS];		//���f���̐������W������
		int nFrame;					//�Đ�����
		CCollision* Collision[MAX_COLLISION];	//�����蔻��
		int nNumCollision;						//1�̃L�[�̓����蔻��̐�
	};

	struct MOTION_SET
	{
		KEY_SET		aModelKey[NUM_PARTS];		//�L�[�̑���������
		int			nNumKey;					//�L�[�̑���(�t�@�C���œǂݍ���)
		bool		bLoop;						//���[�v���邩�ǂ���
	};

	enum PLAYER_MOTION
	{
		//�n��
		PM_ST_NEUTRAL,		//�j���[�g����
		PM_ST_MOVE,			//�ړ�(���Ⴊ�݂�������)
		PM_ST_DASH,			//�O�_�b�V��
		PM_ST_BACKSTEP,		//�o�b�N�X�e�b�v
		PM_ST_GUARD,		//�K�[�h
		PM_ST_HIT,			//��e
		PM_ST_LATTACK,		//��U��
		PM_ST_MATTACK,		//���U��
		PM_ST_HATTACK,		//���U��
		//��
		PM_JP_NEUTRAL,		//�����W�����v
		PM_JP_MOVELEFT	,	//�ړ�(���Ⴊ�݂�������)
		PM_JP_MOVERIGHT,	//�ړ�(���Ⴊ�݂�������)
		PM_JP_DASH,			//�O�_�b�V��
		PM_JP_GUARD,		//�K�[�h
		PM_JP_HIT,			//��e
		PM_JP_LATTACK,		//��U��
		PM_JP_MATTACK,		//���U��
		PM_JP_HATTACK,		//���U��
		//���Ⴊ��
		PM_CR_NEUTRAL,		//�j���[�g����
		PM_CR_MOVE,			//�ړ�(���Ⴊ�݂�������)
		PM_CR_GUARD,		//�K�[�h
		PM_CR_HIT,			//��e
		PM_CR_LATTACK,		//��U��
		PM_CR_MATTACK,		//���U��
		PM_CR_HATTACK,		//���U��
		PM_MAX
	};

	enum PLAYER_STATE
	{
		PST_GROUND,	//�n��
		PST_AIR,		//��
		PST_DAMAGE,	//��e���
		PST_MAX
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//�v���g�^�C�v�錾
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	void			ControlPlayer(void);			//�v���C���[�̑���
	static CPlayer*	Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXMATRIX		GetMtx();
	void			ReadMotion();
	void			MotionPlayer(int nNumber);		//���[�V�����̍Đ��@�����͍Đ����郂�[�V�����̔ԍ�
	void			MotionManager();				//��Ԃɍ��킹�ă��[�V�����Đ�����
	void			PlayFirstMotion();				//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��
	void			DrawCollision();				//�����蔻��̐ݒ�
	void			Axis(void);						//���̉����o������
	void			Jump(void);						//�W�����v
	void			AutoTurn(void);					//�����U��ނ�

	//�Z�b�^�[
	void			SetPos(D3DXVECTOR3 pos) { m_pos = pos; };						//�ʒu�̐ݒ�
	void			SetRot(D3DXVECTOR3 rot) { m_rot = rot; };						//�����̐ݒ�
	void			SetEnemy(CPlayer* ene) { m_pEnemy = ene; };						//�����̐ݒ�

private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//���f���̃C���X�^���X
	MOTION_SET		m_apMotion[PM_MAX];				//���[�V�����̐������������[�V�����̐�->�L�[�̑���->���f���̐�
	D3DXMATRIX		m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;							//�ʒu
	D3DXVECTOR3		m_rot;							//����
	D3DXVECTOR3		m_move;							//�ړ���
	D3DXVECTOR3		m_posold;						//�O��̈ʒu
	//CShadow*		m_Shadow;						//�e
	D3DXVECTOR3		m_rotDest;						//�ړI�̊p�x�̕ۑ�
	int				m_MotionCnt;					//���[�V�����J�E���^�[
	int				m_nNumKey;						//�L�[�̑���
	int				m_nCurrentKey;					//���݂̃L�[�ԍ�
	D3DXMATRIX		m_mtxRot;						//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION	m_quat;							//�N�H�[�^�j�I��
	D3DXVECTOR3		m_vecAxis;						//��]��
	float			m_fRolling;						//��]�ʁ@(��]�p)
	int				m_nNumModel;
	char			m_nModelpass[255];
	D3DXVECTOR3		m_movepos;
	D3DXVECTOR3		m_moverot;
	PLAYER_MOTION	m_Motion;
	PLAYER_MOTION	m_MotionOld;					//�ЂƂO�̃��[�V����
	CCollision* 	m_AxisBox;						//�����o������(�v���C���[�̎�)
	bool			m_bMotion;						//���[�V�����Đ������ǂ���
	static int		m_nPlayer;						//�v���C���[�ԍ�
	int				m_nPlayerNumber;				//�v���C���[�ԍ�
	int				m_nLife;						//�̗�
	CPlayer*		m_pEnemy;						//�ΐ푊��̃|�C���^
	PLAYER_STATE	m_State;						//�v���C���[�̏��
	bool			m_bAttack;						//�U�������ǂ���

	//�W�����v�֌W
	int				m_nJump;						//�W�����v�̑S�̃t���[��
	int				m_nJumpCount;					//�W�����v�J�E���^�[
	int				m_nJumpRigor;					//�W�����v�̒��n�d��


};

#endif
