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
	static		CModel*Create(D3DXVECTOR3 pos, int Number);
	void		LoadModel();
	void		Shadow();
	void		Siz(int number);
	void		SetParent(CModel*pModel) ;
	CModel*		GetParent();
	D3DXMATRIX	GetMtx();
	void		SetRot(D3DXVECTOR3 rot);	
	D3DXVECTOR3 GetRot();
	void		SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3	GetPos();
	float		GetWidth();

private:
	static LPD3DXBUFFER	m_pPlayerBuffMat[NUM_PLAYERPARTS];	//マテリアル情報へのポインタ
	static DWORD		m_nPlayerNumMat[NUM_PLAYERPARTS];	//マテリアル情報の数
	static LPD3DXMESH	m_pPlayerMesh[NUM_PLAYERPARTS];		//メッシュへのポインタ
	D3DXMATRIX			m_mtxParent;						//親のワールドマトリックス
	D3DXMATRIX			m_mtxWorld;							//子のワールドマトリックス
	D3DXMATRIX			m_mtxShadow;						//影用ワールドマトリックス
	D3DXVECTOR3			m_pos;								//位置
	D3DXVECTOR3			m_posold;							//前回の位置
	D3DXVECTOR3			m_rotDest;							//目的の角度の保存
	D3DXVECTOR3			m_rot;								//向き
	D3DXVECTOR3			m_vtxMax;							//モデルの最大座標
	D3DXVECTOR3			m_vtxMin;							//モデルの座標の最小値
	int					m_nNumber;							//自身の番号
	CModel*				m_pParent;							//親のモデルへのポインタ
};

#endif

