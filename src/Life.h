//=============================================================================
//
// �̗̓Q�[�W
// Author : �L�c����
//
//=============================================================================

#ifndef _LIFE_H_
#define _LIFE_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"
#include"Object2D.h"

//�O���錾
class Player;

//�}�N����`
#define NUM_TEXLIFE		(3)
#define LIFEBAR_WIDTH	(450.0f)
#define LIFE_POS_P1		(550.0f)	//�̗̓o�[�̈ʒu
#define LIFE_POS_Y		(60.0f)	//�̗̓o�[�̈ʒu



class CLife
{
public:
	explicit CLife(int nPriority = 3);
	~CLife();
	//�v���g�^�C�v�錾
	HRESULT			Init(float Width, float Height);
	void			Uninit(void);
	void			Update(void);
	void			Draw(void);

	static 	HRESULT Load();
	static 	void Unload();

	static	CLife * Create();

	void SetPos(D3DXVECTOR3 pos, int number);

private:
	CObject2D* m_apObject2D[NUM_TEXLIFE*2];
	static LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXLIFE];
	int m_anMAX[2];

};

#endif

