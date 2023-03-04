//=============================================================================
//
// Xファイル読み込み
// Author : 有田明玄
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//インクルード
#include"main.h"
#include"ObjectX.h"

//マクロ定義
#define PLAYER_LEGPARTS	(13)
#define	NUM_PLAYERPARTS	(1+PLAYER_LEGPARTS)

//前方宣言
class CShadow;

class CModel
{
public:
	explicit CModel();
	~CModel();
	//プロトタイプ宣言
	HRESULT		Init();
	void		Uninit(void);
	void		Update(void);
	void		Draw(D3DXMATRIX pMtx);
	static		CModel*Create();
	void		Shadow();
	void		Siz();

	//セッター
	void		SetModel(char* Filename);
	void		SetParent(CModel*pModel) ;
	void		SetRot(D3DXVECTOR3 rot);	
	void		SetPos(D3DXVECTOR3 pos);
	void		SetIdx(int Idx) { m_nIdx = Idx; };
	void		SetDPos(D3DXVECTOR3 pos) { m_DefaultPos = pos; };
	void		SetDRot(D3DXVECTOR3 rot) { m_DefaultRot = rot; };
	void		SetCol(D3DXCOLOR col) { m_col = col; };

	//ゲッター
	CModel*		GetParent();
	D3DXMATRIX	GetMtx();
	D3DXVECTOR3 GetRot();
	D3DXVECTOR3	GetPos();
	float		GetWidth();
	int			GetIdx() { return m_nIdx; };
	D3DXVECTOR3	GetDPos() { return m_DefaultPos; };
	D3DXVECTOR3	GetDRot() { return m_DefaultRot; };
	D3DXCOLOR	GetCol() { return m_col; };



private:
	LPD3DXBUFFER		m_pBuffMat;		//マテリアル情報へのポインタ
	DWORD				m_nNumMat;		//マテリアル情報の数
	LPD3DXMESH			m_pMesh;		//メッシュへのポインタ
	D3DXMATRIX			m_mtxParent;	//親のワールドマトリックス
	D3DXMATRIX			m_mtxWorld;		//子のワールドマトリックス
	D3DXMATRIX			m_mtxShadow;	//影用ワールドマトリックス
	D3DXVECTOR3			m_pos;			//位置
	D3DXVECTOR3			m_posold;		//前回の位置
	D3DXVECTOR3			m_rotDest;		//目的の角度の保存
	D3DXVECTOR3			m_rot;			//向き
	D3DXVECTOR3			m_vtxMax;		//モデルの最大座標
	D3DXVECTOR3			m_vtxMin;		//モデルの座標の最小値
	int					m_nIdx;			//自身の番号
	CModel*				m_pParent;		//親のモデルへのポインタ
	D3DXVECTOR3			m_DefaultPos;	//初期位置
	D3DXVECTOR3			m_DefaultRot;	//初期位置
	D3DXCOLOR			m_col;			//色
};

#endif

