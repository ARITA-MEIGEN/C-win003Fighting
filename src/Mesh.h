//=================================================
// Content     (ゲームの設定)(.h)
// Author     : 有田明玄
//=================================================
#ifndef _MESH_H_	//このマクロ定義がされてなかったら
#define _MESH_H_	//二重インクルード防止のマクロ定義
#include"main.h"
#include"Object.h"

//マクロ定義
#define POLIGON_SIZE				(20.0f)																	//メッシュフィールドの大きさ
#define	MESHFIELD_VERTEX_NUM		((m_Xpolyogn+1)*(m_Zpolyogn+1))											//頂点バッファの数
#define MESHFIELD_INDEX_NUM			((m_Xpolyogn+1)*2*m_Zpolyogn+(m_Zpolyogn-1)*((m_Zpolyogn-1)*2))			//インデックスバッファの数
#define MESHFIELD_PRIMITIVE_NUM		(m_Xpolyogn*m_Zpolyogn*2+(4*(m_Zpolyogn-1)))							//ポリゴンの数

class CMesh:CObject
{
public:
	explicit CMesh(int nPriority = 3);
	~CMesh();
	//プロトタイプ宣言
	HRESULT			Init(void)override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	static CMesh*	Create(int x, int z);
	float			Collision(D3DXVECTOR3 pos);			//衝突判定

private:
	D3DXVECTOR3					m_pos;
	D3DXVECTOR3					m_rot;
	D3DXMATRIX					m_mtxWorld;
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				//頂点バッファ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				//インデックスバッファ
	LPDIRECT3DTEXTURE9			m_pTexture;				//テクスチャへのポインタ
	int							m_Xpolyogn;				//X方向のポリゴンの数
	int							m_Zpolyogn;				//X方向のポリゴンの数
};

#endif


