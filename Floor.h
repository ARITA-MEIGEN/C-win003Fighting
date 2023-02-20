//=============================================================================
//
// 床
// Author : 有田明玄
//
//=============================================================================

#ifndef _FLOOR_H_
#define _FLOOR_H_

//インクルード
#include"main.h"
#include"Object3D.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CApplication;

//*****************************************************************************
// 構造体定義
//*****************************************************************************

class CFloor : public CObject3D
{
public:
	//メンバ関数
	explicit CFloor(int nPriority = 3);
	~CFloor()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CFloor*Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXVECTOR3 rot);
	static void Load();
	static void Unload();

private:
	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//ポリゴンのテクスチャ
};
#endif // !_OBJECT_H_



