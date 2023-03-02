//=================================================
// Content     (ゲームの設定)(light.h)
// Author     : 有田明玄
//=================================================
#ifndef _LIGHT_H_		//このマクロ定義がされてなかったら
#define _LIGHT_H_		//二重インクルード防止のマクロ定義
#include"main.h"

//マクロ定義
#define MAX_LIGHT	(3)	//ライトの数

class CLight
{
public:
	//プロトタイプ宣言
	void Init(void);
	void Uninit(void);
	void Update(void);
	D3DXVECTOR3 GetVec(int number);

private:
	D3DLIGHT9 m_alight[MAX_LIGHT];	//ライト情報

};

#endif

