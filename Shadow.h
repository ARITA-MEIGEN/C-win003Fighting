//=============================================================================
//
// Xファイル読み込み
// Author : 有田明玄
//
//=============================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_

//インクルード
#include"main.h"
#include"Object.h"
#include"Object3D.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CApplication;

class CShadow : public CObject3D
{
public:
	//メンバ関数
	explicit CShadow(int nPriority = 3);
	~CShadow()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CShadow*Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz);
	static void Load();
	static void Unload();

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//メンバ変数
};

#endif
