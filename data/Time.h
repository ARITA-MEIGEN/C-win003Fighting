//=============================================================================
//
// 時間
// Author : 有田明玄
//
//=============================================================================

#ifndef _TIME_H_
#define _TIME_H_

//インクルード
#include"main.h"
#include"Object.h"
#include"Object2D.h"

//前方宣言
class Player;

//マクロ定義
#define NUM_TEXTIME		(2)
#define	DEFAULT_TIME	(99)

class CTimer
{
public:
	explicit CTimer(int nPriority = 3);
	~CTimer();
	//プロトタイプ宣言
	HRESULT			Init();
	void			Uninit(void);
	void			Update(void);
	void			Draw(void);
	static 	HRESULT Load();
	static 	void	Unload();
	static	CTimer* Create();

	//ゲッター
	int GetTimer() { return m_nTimer; };

private:
	CObject2D* m_pObject2D;
	CObject2D* m_apNumber[NUM_TEXTIME];

	static LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTIME];
	int m_nTimer;
	int m_nCreateTimer;

};

#endif

