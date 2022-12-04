//=============================================================================
//
// 3Dオブジェクト
// Author : 有田明玄
//
//=============================================================================

//インクルード
#include "Object3D.h"
#include "renderer.h"
#include"Application.h"
#include"Shadow.h"

//静的メンバ変数
 LPDIRECT3DTEXTURE9 CShadow::m_pTexture;

//=============================================================================
// コンストラクタ
//=============================================================================
CShadow::CShadow(int nPriority) :CObject3D(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT  CShadow::Init()
{
	CObject3D::Init();
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
//終了
//=============================================================================
void  CShadow::Uninit()
{
	CObject3D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void  CShadow::Update()
{
	CObject3D::Update();
}

//=============================================================================
// 描画
//=============================================================================
void  CShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	CObject3D::Draw();
	pDevice->SetTexture(0, NULL);

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 生成
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz)
{
	CShadow*pShadow;
	pShadow = new CShadow(0);
	pShadow->Init();
	pShadow->SetPos(pos);
	pShadow->SetSiz(siz);

	return pShadow;
}


//=============================================================================
// データを読み込み
//=============================================================================
void CShadow::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&m_pTexture);
}

//=============================================================================
// データを破棄
//=============================================================================
void CShadow::Unload()
{
	if (m_pTexture != nullptr)
	{//テクスチャの破棄
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}