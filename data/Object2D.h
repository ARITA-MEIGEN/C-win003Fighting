//=============================================================================
//
// 2Dオブジェクト
// Author : 有田明玄
//
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//インクルード
#include"main.h"
#include"Object.h"

// 頂点フォーマット
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CApplication;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点データ
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;	//テクスチャ座標
};

class CObject2D : public CObject
{
public:
	//メンバ関数
	explicit CObject2D(int nPriority=3);
	~CObject2D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void Setposition(D3DXVECTOR3 move);
	void BindTexture(LPDIRECT3DTEXTURE9 tex);
	static bool Hit(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, D3DXVECTOR2 Siz, D3DXVECTOR2 TargetSiz);
	D3DXVECTOR3 GetPos();
	D3DXVECTOR2 GetSiz();
	D3DXCOLOR GetCol();
	float GetRot();
	D3DXVECTOR2 GetUV(int number);
	void SetPos(D3DXVECTOR3 pos);
	void SetPosR(float posR) { m_nRControl = posR; };
	float GetPosR() { return m_nRControl; };

	void SetPosL(float posL) { m_nLControl = posL; };
	float GetPosL() { return m_nLControl; };

	void SetSiz(D3DXVECTOR2 Siz);
	void SetCol(D3DXCOLOR col);
	void SetRot(float Rot);
	void SetUV(float U1, float U2, float V1, float V2);
	static CObject2D*Create(D3DXVECTOR3 pos, D3DXVECTOR2 siz, int Priority);

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9 m_pTexture;		//ポリゴンのテクスチャ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//ポリゴンの頂点バッファ
	float m_fLength;					//対角線の長さ
	float m_fAngle;						//対角線の角度
	float m_Rot;						//回転
	D3DXVECTOR3 m_Pos;					//位置
	D3DXVECTOR2 m_Siz;					//大きさ
	D3DXCOLOR m_Col;					//色
	D3DXVECTOR2 m_UV[2];				//テクスチャ座標
	float m_nRControl;					//座標の右側の制御
	float m_nLControl;					//座標の←側の制御

};
#endif // !_OBJECT_H_

