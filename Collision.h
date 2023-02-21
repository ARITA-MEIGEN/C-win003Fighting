//=================================================
// Content     (�Q�[���̐ݒ�)(Line.h)
// Author     : �L�c����
//=================================================
#ifndef _LINE_H_		//���̃}�N����`������ĂȂ�������
#define _LINE_H_		//��d�C���N���[�h�h�~�̃}�N����`
#include"main.h"
#include"Object.h"

//���f���̍\����
class CCollision :public CObject
{
public:
	enum COLLISION
	{
		COLLI_AXIS = 0,		//���̔���
		COLLI_DAMAGE,		//�_���[�W����
		COLLI_SLOW,			//��������
		COLLI_HURT,			//���ꔻ��
		COLLI_HURTSLOW,		//�����̂��ꔻ��
	};

	//�U���̑���
	enum EDAMAGE_POINT
	{
		DP_HIGH = 0,		//��i
		DP_MIDDLE,			//���i
		DP_LOW,				//���i
		DP_MAX,
	};

	explicit CCollision(int nPriority = 3);
	~CCollision();

	//�v���g�^�C�v�錾
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CCollision* Create(D3DXVECTOR3 pos, COLLISION type);
	//�Q�b�^�[�ƃZ�b�^�[

	//�Z�b�^�[
	void SetPos(D3DXVECTOR3(pos)) { m_pos = pos; };
	void SetSiz(D3DXVECTOR3 size) { m_width = size; };
	void SetUse(bool use) { m_bUse = use; };					//�I���I�t�̐؂�ւ�
	void SetStartf(int start) { m_nStart = start; };			//�J�n�t���[��
	void SetEndf(int end) { m_nEnd = end; };					//�I���t���[��
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };				//����
	void SetDPos(D3DXVECTOR3 pos) { m_defaultpos = pos; };		//�����ʒu
	void SetDRot(D3DXVECTOR3 rot) { m_defaultrot = rot; };		//��������
	void SetDP(EDAMAGE_POINT pro) { m_Property = pro; };		//�����̐ݒ�
	void SetDmg(bool dmg) { m_bDmg = dmg; };					//�_���[�W�����𖳌��ɂ��邩�ǂ���
	void SetHitRig(int dmgrig) { m_nHitRig = dmgrig; };			//�_���[�W�d���̐ݒ�
	void SetGuardRig(int dmgrig) { m_nGuardRig = dmgrig; };		//�K�[�h�d���̐ݒ�



	//�Q�b�^�[
	int				GetStartf() { return m_nStart; };			//�J�n�t���[��
	int				GetEndf() { return m_nEnd; };				//�I���t���[��
	D3DXVECTOR3		GetRot() { return m_rot; };					//����
	D3DXVECTOR3		GetDPos() { return m_defaultpos; };			//�����ʒu
	D3DXVECTOR3		GetDRot() { return m_defaultrot; };			//��������
	int				GetDamage() { return m_nDamage; };			//�_���[�W�ʂ̎擾
	EDAMAGE_POINT	GetProperty() { return m_Property; };		//�����̎擾
	bool			GetUse() { return m_bUse; };				//�I���I�t�̐؂�ւ�
	bool			GetUseDmg() { return m_bDmg; };				//�_���[�W�����𖳌��ɂ��邩�ǂ���
	D3DXVECTOR3		GetWidth() { return m_width; };				//�����蔻��̑傫��
	D3DXVECTOR3		GetPos() { return m_pos; };					//�����蔻��̈ʒu
	int				GetHitRig() { return m_nHitRig; };			//�_���[�W�d���̎擾
	int				GetGuardRig() { return m_nGuardRig; };		//�K�[�h�d���̎擾

private:
	D3DXVECTOR3				m_pos;			//�ʒu
	D3DXVECTOR3				m_posold;		//�ʒu
	D3DXVECTOR3				m_defaultpos;	//�ʒu

	D3DXVECTOR3				m_width;		//��
	D3DXVECTOR3				m_rot;			//����
	D3DXVECTOR3				m_defaultrot;	//����

	D3DXCOLOR				m_col;			//�F
	D3DXMATRIX				m_mtxWorld;		//���[���h�}�g���b�N�X
	bool					m_bUse;			//�g�p���Ă��邩�ǂ���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@
	COLLISION				m_Colltype;		//�����蔻��̎��
	int						m_nStart;		//�J�n�t���[��
	int						m_nEnd;			//�I���t���[��
	int						m_nDamage;		//�_���[�W��
	EDAMAGE_POINT			m_Property;		//�U���̑���
	bool					m_bDmg;			//�_���[�W�̃I���I�t�؂�ւ�
	int						m_nHitRig;		//�_���[�W�d��
	int						m_nGuardRig;	//�K�[�h�d��
};

#endif



