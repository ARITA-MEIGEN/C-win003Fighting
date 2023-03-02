//=============================================================================
//
// プレイヤー
// Author : 有田明玄
//
//=============================================================================

//インクルード
#include"Effect.h"
#include"renderer.h"
#include"Application.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CEffect::m_apTexture[EFFECT_TEX] = {};
//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(int priorty) :CObject3D(priorty)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT  CEffect::Init()
{
	CObject3D::Init();
	return S_OK;
}

//=============================================================================
//終了
//=============================================================================
void  CEffect::Uninit()
{
	CObject3D::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void  CEffect::Update()
{
	CObject3D::Update();

	//大きさを変化させる
	D3DXVECTOR3 Siz = GetSiz();
	D3DXCOLOR col = GetCol();

	Siz -= {1.0f, 0.0f, 1.0f};
	m_nLife--;
	col.a -= fAlphagain;

	//色(アルファ値を変化させる)
	if (col.a <= 0.0f || Siz.x <= 0.0f || Siz.y <= 0.0f || m_nLife <= 0)
	{//アルファ値が0以下の場合強制終了
		Uninit();
		return;
	}
	else
	{
		SetSiz(Siz);
		SetCol(col);
	}

	//移動処理
	D3DXVECTOR3 pos = GetPos();
	pos += m_move;
	SetPos(pos);
}

//=============================================================================
// 描画
//=============================================================================
void  CEffect::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	
	D3DMATRIX mtxView;

	//アルファブレンディングを加算合成に設定
	if (m_bAlpha == true)
	{
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//Zバッファの設定
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//ライト無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtx);

	//カメラの逆行列を設定
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//カメラの逆行列を設定
	mtx._11 = mtxView._11;
	mtx._12 = mtxView._21;
	mtx._13 = mtxView._31;
	mtx._21 = mtxView._12;
	mtx._22 = mtxView._22;
	mtx._23 = mtxView._32;
	mtx._31 = mtxView._13;
	mtx._32 = mtxView._23;
	mtx._33 = mtxView._33;

	//位置を反映
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtx, &mtx, &mtxTrans);

	//テクスチャの設定
	pDevice->SetTexture(0, m_apTexture[0]);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	CObject3D::Draw();

	pDevice->SetTexture(0, NULL);

	//ライト有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテスト無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//Zテストを元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// 生成
//=============================================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, float lot, D3DXVECTOR3 move, int nLife, D3DXCOLOR col,int texnumber,bool alpha)
{
	CEffect*pEffect;
	pEffect = new CEffect(5);
	if (pEffect != nullptr)
	{// ポリゴンの初期化処理
		pEffect->Init();
		pEffect->SetPos(pos);
		pEffect->SetRot(D3DXVECTOR3(lot, D3DX_PI*-0.5f, D3DX_PI*0.5f));
		pEffect->m_nLife = nLife;
		pEffect->SetCol(col);
		pEffect->m_move = move;
		pEffect->SetSiz(siz);
		pEffect->BindTexture(m_apTexture[texnumber]);	//テクスチャの設定
		pEffect->m_bAlpha = alpha;
		pEffect->fAlphagain = col.a / nLife;
	}
	return pEffect;
}

//=============================================================================
// データを読み込み
//=============================================================================
HRESULT CEffect::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Splash01.png",
		&m_apTexture[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&m_apTexture[1]);

	return S_OK;
}

//=============================================================================
// データを破棄
//=============================================================================
void CEffect::Unload()
{
	for (int i = 0; i < EFFECT_TEX; i++)
	{
		if (m_apTexture[i] != nullptr)
		{//テクスチャの破棄
			m_apTexture[i]->Release();
			m_apTexture[i] = nullptr;
		}
	}
}
