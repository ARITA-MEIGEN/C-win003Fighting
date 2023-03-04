//=============================================================================
//
// 体力ゲージ
// Author : 有田明玄
//
//=============================================================================

#ifndef _LIFE_H_
#define _LIFE_H_

//インクルード
#include"main.h"
#include"Object.h"
#include"Object2D.h"

//前方宣言
class Player;

//マクロ定義
#define NUM_TEXLIFE		(3)
#define LIFEBAR_WIDTH	(450.0f)
#define LIFE_POS_P1		(550.0f)	//体力バーの位置
#define LIFE_POS_Y		(60.0f)	//体力バーの位置



class CLife
{
public:
	explicit CLife(int nPriority = 3);
	~CLife();
	//プロトタイプ宣言
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

