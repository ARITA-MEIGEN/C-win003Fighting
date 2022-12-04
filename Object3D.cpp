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

#define POLIGON_SIZE	(500.0f)	//ポリゴンの大きさ

//=============================================================================
// コンストラクタ
//=============================================================================
CObject3D::CObject3D(int nPriority) :CObject(nPriority)
{
	m_pTexture = nullptr;											//ポリゴンのテクスチャ
	m_pVtxBuff = nullptr;											//ポリゴンの頂点バッファ
	m_fLength = 0.0f;												//対角線の長さ
	m_fAngle = 0.0f;												//対角線の角度
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//位置を初期化する
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//向きを初期化
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject3D::~CObject3D()
{
	assert(m_pVtxBuff == nullptr);		//ポリゴンの頂点バッファ
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT  CObject3D::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	VERTEX_3D*pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_Pos.x - (m_Siz.x / 2), m_Pos.y, m_Pos.z - (m_Siz.z / 2));
	pVtx[1].pos = D3DXVECTOR3(m_Pos.x - (m_Siz.x / 2), m_Pos.y, m_Pos.z + (m_Siz.z / 2));
	pVtx[2].pos = D3DXVECTOR3(m_Pos.x + (m_Siz.x / 2), m_Pos.y, m_Pos.z - (m_Siz.z / 2));
	pVtx[3].pos = D3DXVECTOR3(m_Pos.x + (m_Siz.x / 2), m_Pos.y, m_Pos.z + (m_Siz.z / 2));

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点カラー設定
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].col = m_Col;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//終了
//=============================================================================
void  CObject3D::Uninit()
{
	//頂点バッファを破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}

//=============================================================================
// 更新
//=============================================================================
void  CObject3D::Update()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	VERTEX_3D*pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3( - (m_Siz.x / 2),0.0f, - (m_Siz.z / 2));
	pVtx[1].pos = D3DXVECTOR3( - (m_Siz.x / 2),0.0f, + (m_Siz.z / 2));
	pVtx[2].pos = D3DXVECTOR3( + (m_Siz.x / 2),0.0f, - (m_Siz.z / 2));
	pVtx[3].pos = D3DXVECTOR3(+ (m_Siz.x / 2),0.0f, + (m_Siz.z / 2));

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点カラー設定
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].col = m_Col;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画
//=============================================================================
void  CObject3D::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldPolygon);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorldPolygon, &m_mtxWorldPolygon, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldPolygon);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);


	//ポリゴンの描画       
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		0,										//頂点の開始場所
		4);
}

//=============================================================================
// 位置の設定
//=============================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// サイズの設定
//=============================================================================
void CObject3D::SetSiz(D3DXVECTOR3 siz)
{
	m_Siz = siz;
}

//=============================================================================
// 位置の設定
//=============================================================================
void CObject3D::SetCol(D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
// 生成
//=============================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, int Priority)
{
	CObject3D*pObject3D;
	pObject3D = new CObject3D(Priority);
	pObject3D->Init();
	pObject3D->SetPos(pos);
	pObject3D->SetSiz(siz);

	return pObject3D;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CObject3D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}

