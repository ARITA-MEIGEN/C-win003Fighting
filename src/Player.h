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
#include"Command.h"
#include"Collision.h"

//�O���錾
class CShadow;
class CModel;
class CBullet;

//�}�N����`
#define MAX_MOTION			(5)				//���[�V�����̐�
#define MAX_KEY				(60)			//�L�[�̑���
#define MAX_FRAME			(120)			//�t���[���̍ő吔
#define NUM_PARTS			(14)			//�p�[�c�̐�
#define MAX_COLLISION		(20)			//1�̃p�[�c�ɂ��ݒ�ł��锻��̐�
#define JUMP_HEIGHT			(130.0f)		//�W�����v�̍���
#define PLAYER_SPEED		(2.0f)			//�ړ����x
#define FIELD_WIDTH			(380.0f)		//�[�[�̋���
#define JUMP_FACTOR_X		(1.7f)			//�W�����v�̈ړ��̌W��
#define JUMP_FACTOR_Y		(40.0f)			//�W�����v�̈ړ��̌W��
#define MAX_SPEED			(10.f)			//�W�����v�̍ō����x
#define INITIAL_VELOCITY	(7.0f)			//�W�����v�̏���
#define MAX_KEYMEMORY		(60)			//�L������L�[�̐�
#define	DASH_SPEED			(5.0f)			//�_�b�V�����x
#define BULLET_ANGLE		(1.0f)			//�e�̊p�x
#define	BACKSTEP_MOVE_X		(5.0f)			//�o�b�N�X�e�b�v�̈ړ���
#define	BACKSTEP_MOVE_Y		(2.0f)			//�o�b�N�X�e�b�v�̈ړ���
#define HIT_EFFECTSIZ		(25.0f)			//�q�b�g�G�t�F�N�g�̑傫��
#define GUARD_EFFECTSIZ		(30.0f)			//�K�[�h�G�t�F�N�g�̑傫��



class CPlayer :public CObject
{
public:
	//�L�[�v�f
	struct KEY
	{
		D3DXVECTOR3 fPos;
		D3DXVECTOR3 fRot;
	};

	//�L�[���
	struct KEY_SET 
	{
		KEY aKey[NUM_PARTS];					//���f���̐������W������
		int nFrame;								//�Đ�����
		CCollision* Collision[MAX_COLLISION];	//�����蔻��
		int nNumCollision;						//1�̃L�[�̓����蔻��̐�
		CCollision* HurtCol[MAX_COLLISION];		//���ꔻ��
		int nNumHurtCol;						//1�̃L�[�̓����蔻��̐�
	};

	struct MOTION_SET
	{
		KEY_SET		aKey[NUM_PARTS];		//�L�[�̑���������
		int			nNumKey;					//�L�[�̑���(�t�@�C���œǂݍ���)
		bool		bLoop;						//���[�v���邩�ǂ���
		int			nHitStopTimer;				//�q�b�g�X�g�b�v�̎���
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
		PM_ST_HURT,			//���ꃂ�[�V����

		//��
		PM_JP_NEUTRAL,		//�����W�����v
		PM_JP_MOVEFORWARD,	//�O�W�����v
		PM_JP_MOVEBACK,		//�o�b�N�W�����v
		PM_JP_DASH,			//�O�_�b�V��
		PM_JP_GUARD,		//�K�[�h
		PM_JP_HIT,			//��e
		PM_JP_LATTACK,		//��U��
		PM_JP_MATTACK,		//���U��
		PM_JP_HATTACK,		//���U��
		PM_JP_HURT,			//���ꃂ�[�V����

		//���Ⴊ��
		PM_CR_NEUTRAL,		//�j���[�g����
		PM_CR_MOVE,			//�ړ�(���Ⴊ�݂�������)
		PM_CR_GUARD,		//�K�[�h
		PM_CR_HIT,			//��e
		PM_CR_LATTACK,		//��U��
		PM_CR_MATTACK,		//���U��
		PM_CR_HATTACK,		//���U��
		PM_CR_HURT,			//���ꃂ�[�V����

		//�K�E�Z
		PM_236L,			//��g��
		PM_236M,			//���g��
		PM_236H,			//���g��

		PM_214L,			//�㗳��
		PM_214M,			//������
		PM_214H,			//������

		PM_623L,			//�㏸��
		PM_623M,			//������
		PM_623H,			//������

		//���S
		PM_ST_DIE,			//��
		PM_CR_DIE,			//��
		PM_JP_DIE,			//��

		PM_SLOW,			//����
		PM_DOWN,			//���S&�_�E�������[�V����
		PM_STANDUP,			//�N���オ��
		PM_MAX
	};

	enum PLAYER_STATE
	{
		PST_STAND,	//����
		PST_CROUCH,	//���Ⴊ��
		PST_AIR,	//��
		PST_DIE,	//��e���
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
	D3DXMATRIX		GetMtx() { return m_mtxWorld; };		//�}�g���b�N�X�̎擾
	void			ReadMotion();
	void			MotionPlayer();							//���[�V�����̍Đ��@�����͍Đ����郂�[�V�����̔ԍ�
	void			MotionManager();						//��Ԃɍ��킹�ă��[�V�����Đ�����
	void			PlayFirstMotion();						//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��
	void			DrawCollision();						//�����蔻��̐ݒ�
	void			Axis(void);								//���̉����o������
	void			Jump(void);								//�W�����v
	void			AutoTurn(void);							//�����U��ނ�
	void			Damage();								//�_���[�W����
	bool			Guard(CCollision::EDAMAGE_POINT dp);	//�K�[�h����
	void			Command();								//�R�}���h����	
	bool			CheckInput(const int *command);			//�R�}���h�̓��͔���
	void			StateManagement();						//�v���C���[�̏�ԊǗ�
	void			Input();								//���͏���
	void			Updatepos();							//���W�̍X�V
	void			Cancel();								//�U���L�����Z��
	bool			ColJudge(int hurtnumber,int colnum);	//�����蔻��`�F�b�N
	void			Down();									//���n�ݒ�
	void			Damage_Cal(int Damage, CCollision::EDAMAGE_POINT pro, int HitRig, int GuardRig, bool knockback);							//�_���[�W�ݒ�
	void			Die();
	void			FireBall();								//�������Z
	void			Slow();									//����
	void			Normalization();						//���K��
	void			StageEdge();							//�X�e�[�W�̒[�̏���

	//�Z�b�^�[
	void			SetPos(D3DXVECTOR3 pos) { m_pos = pos; };						//�ʒu�̐ݒ�
	void			SetRot(D3DXVECTOR3 rot) { m_rot = rot; };						//�����̐ݒ�
	void			SetEnemy(CPlayer* ene) { m_pEne = ene; };						//�G�̃|�C���^
	void			SetBullet(bool bullet) { m_bBullet = bullet; };					//�e���g�p���Ă��邩�ǂ���
	void			SetAttack(bool atk) { m_bAttack = atk; };						//�e���g�p���Ă��邩�ǂ���


	//�Q�b�^�[
	D3DXVECTOR3		GetPos() { return m_pos; };
	int				GetLife() { return m_nLife; };	//�̗�
	bool			GetBullet() { return m_bBullet ; };					//�e���g�p���Ă��邩�ǂ���
	CBullet*		GetBulletState() { return m_pBullet; };	//�e�̏��擾
	PLAYER_MOTION	GetNowMotion() { return m_Motion; };

	//�����蔻��̎擾
	CCollision*		GetCollision(int number) { return m_apMotion[m_Motion].aKey[m_nCurKey].Collision[number]; };

	//���ꔻ��̎擾
	CCollision*		GetHurt(int number) { return m_apMotion[m_Motion].aKey[m_nCurKey].HurtCol[number]; };


private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//���f���̃C���X�^���X
	MOTION_SET		m_apMotion[PM_MAX];				//���[�V�����̐������������[�V�����̐�->�L�[�̑���->���f���̐�
	D3DXMATRIX		m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;							//�ʒu
	D3DXVECTOR3		m_rot;							//����
	D3DXVECTOR3		m_move;							//�ړ���
	D3DXVECTOR3		m_posold;						//�O��̈ʒu
	D3DXVECTOR3		m_rotDest;						//�ړI�̊p�x�̕ۑ�
	int				m_MotionCnt;					//���[�V�����J�E���^�[
	int				m_nNumKey;						//�L�[�̑���
	int				m_nCurKey;						//���݂̃L�[�ԍ�
	int				m_nNumModel;					//�ǂݍ��ރ��f���̐�
	char			m_nModelpass[255];				//�ǂݍ��ރ��f���̃p�X
	PLAYER_MOTION	m_Motion;						//���݂̃��[�V����
	PLAYER_MOTION	m_MotionOld;					//�ЂƂO�̃��[�V����
	PLAYER_MOTION	m_NextMotion;					//�L�����Z���Ŕ�������Z
	bool			m_bMotion;						//���[�V�����Đ������ǂ���
	static int		m_nNumPlayer;					//�v���C���[�̐�
	int				m_nPlayerNumber;				//�����̃v���C���[�ԍ�
	CPlayer*		m_pEne;							//�ΐ푊��̃|�C���^
	PLAYER_STATE	m_State;						//�v���C���[�̏��
	bool			m_bAttack;						//�U�������ǂ���
	int				m_nLife;						//�̗�
	bool			m_bSide;						//�ǂ����������Ă邩(true�Ȃ灩)
	int				m_anInput[MAX_KEYMEMORY];		//�R�}���h�F���p
	int				m_nNowKey;						//�L�[�ۑ��p
	int				m_nHitStop;						//�q�b�g�X�g�b�v�̎���
	bool			m_bBullet;						//��ѓ�����g�p���Ă��邩�H
	CBullet*		m_pBullet;						//��ѓ���
	CShadow*		m_pShadow;						//�e
	bool			m_bDamage;						//�_���[�W�t���O

	//�����o������֘A
	CCollision* 	m_AxisBox;						//�����o������(�v���C���[�̎�)
	D3DXVECTOR3		m_aAxisSiz[PST_MAX];			//�����o������̑傫��

	//�W�����v�֌W
	int				m_nJump;						//�W�����v�̑S�̃t���[��
	int				m_nJumpCount;					//�W�����v�J�E���^�[
	int				m_nJumpRigor;					//�W�����v�̒��n�d��
	bool			m_bJump;						//�W�����v�����ǂ���
	int				m_nJumpTransTime;				//�W�����v�ڍs�t���[��
	//�K�[�h�֌W
	int				m_nRig;							//�d������
};

#endif
