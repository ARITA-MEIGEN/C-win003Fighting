//=============================================================================
//
// 3Dオブジェクト
// Author : 有田明玄
//
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//インクルード
#include"main.h"
#include"Object.h"

// 頂点フォーマット
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//*****************************************************************************
//前方宣言
//*****************************************************************************
class CApplication;

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点データ
struct VERTEX_3D
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標;
};

class CObject3D : public CObject
{
public:
	//メンバ関数
	explicit CObject3D(int nPriority = 3);
	~CObject3D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CObject3D*Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, int Priority);
	void BindTexture(LPDIRECT3DTEXTURE9 tex);

	//セッター
	void SetPos(D3DXVECTOR3 pos);
	void SetSiz(D3DXVECTOR3 siz);
	void SetCol(D3DXCOLOR col);
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; };

	//ゲッター
	D3DXVECTOR3 GetPos() { return m_Pos; };
	D3DXVECTOR3 GetSiz() { return m_Siz; };
	D3DXCOLOR GetCol() { return m_Col; };
	D3DXVECTOR3 GetRot() { return m_Rot; };

private:
	//メンバ変数
	LPDIRECT3DTEXTURE9 m_pTexture;		//ポリゴンのテクスチャ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//ポリゴンの頂点バッファ
	float m_fLength;					//対角線の長さ
	float m_fAngle;						//対角線の角度
	D3DXVECTOR3 m_Rot;						//回転
	D3DXMATRIX m_mtxWorldPolygon;
	D3DXVECTOR3 m_Pos;					//位置
	D3DXVECTOR3 m_Siz;					//大きさ
	D3DXCOLOR m_Col;					//色
};
#endif // !_OBJECT_H_


