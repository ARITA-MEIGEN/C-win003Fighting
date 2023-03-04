//=============================================================================
//
// �G�t�F�N�g
// Author : �L�c����
//
//=============================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

//�C���N���[�h
#include"main.h"
#include"Object3D.h"

//�}�N����`
#define EFFECT_HEIGHT	(35)		//�v���C���[�̍���
#define EFFECT_WIDTH	(40)		//�v���C���[�̕�	
#define EFFECT_TEX		(2)			//�e�N�X�`���̖���

//�O���錾

//=============================================================================
// �\���̒�`
//=============================================================================
// ���_�f�[�^
class CEffect : public CObject3D
{
public:
	explicit CEffect(int nPriority = 2);
	~CEffect()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, float lot, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int texnumber, bool alpha);
	static HRESULT Load();
	static void Unload();
private:
	//�����o�֐�
	static LPDIRECT3DTEXTURE9 m_apTexture[EFFECT_TEX];
	int m_nLife;						//����
	D3DXVECTOR3 m_move;
	D3DXMATRIX mtx;					//�}�g���b�N�X
	float fAlphagain;				//�������̃A���t�@������
	bool m_bAlpha;
};
#endif // !_OBJECT_H_
