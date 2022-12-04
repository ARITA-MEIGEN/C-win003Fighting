//=============================================================================
//
// Xファイル読み込み
// Author : 有田明玄
//
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//インクルード
#include"main.h"
#include"Object.h"

//前方宣言
class CShadow;

class CObjectX:public CObject
{
public:
	explicit CObjectX(int nPriority = 3);
	~CObjectX();
	//プロトタイプ宣言
	virtual HRESULT Init()override;
	virtual void Uninit(void)override;
	virtual void Update(void)override;
	virtual void Draw(void)override;
	static CObjectX*Create(D3DXVECTOR3 pos, int Priority, LPCTSTR name);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuff, DWORD pNumMat);
	void Shadow();

private:
	LPD3DXMESH		m_pMesh;			//メッシュへのポインタ
	LPD3DXBUFFER	m_pBuffMat;			//マテリアル情報へのポインタ
	DWORD			m_nNumMat;			//マテリアル情報の数
	D3DXMATRIX		m_mtxWorld;			//ワールドマトリックス
	D3DXVECTOR3		m_pos;				//位置
	D3DXVECTOR3		m_addPos;			//位置
	D3DXVECTOR3		m_rot;				//向き
	D3DXVECTOR3		m_addRot;			//向き
	D3DXVECTOR3		m_size;				//大きさ
	D3DXVECTOR3		m_vtxMax;			//モデルの最大座標
	D3DXVECTOR3		m_vtxMin;			//モデルの座標の最小値
	D3DXVECTOR3		m_move;				//移動量
	D3DXVECTOR3		m_posold;			//前回の位置
	int				m_nIdxModelParent;	//親モデルのインデックスaModelの番号
	CShadow*		m_Shadow;			//影
	D3DXVECTOR3		m_rotDest;			//目的の角度の保存
	D3DXMATRIX		m_mtxShadow;		//影のマトリクス
	LPCTSTR			m_modelname;		//モデルの名前
};
#endif



