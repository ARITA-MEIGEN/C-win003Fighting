//=============================================================================
//
// エフェクト
// Author : 有田明玄
//
//=============================================================================

#ifndef _EFFECT_H_
#define _EFFECT_H_

//インクルード
#include"main.h"
#include"Object3D.h"

//マクロ定義
#define EFFECT_HEIGHT	(35)		//プレイヤーの高さ
#define EFFECT_WIDTH	(40)		//プレイヤーの幅	
#define EFFECT_TEX		(2)			//テクスチャの枚数

//前方宣言

//=============================================================================
// 構造体定義
//=============================================================================
// 頂点データ
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
	//メンバ関数
	static LPDIRECT3DTEXTURE9 m_apTexture[EFFECT_TEX];
	int m_nLife;						//寿命
	D3DXVECTOR3 m_move;
	D3DXMATRIX mtx;					//マトリックス
	float fAlphagain;				//生成時のアルファ減少量
	bool m_bAlpha;
};
#endif // !_OBJECT_H_
