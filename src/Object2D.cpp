//=============================================================================
//
// 2Dオブジェクト
// Author : 有田明玄
//
//=============================================================================

//インクルード
#include "Object2D.h"
#include "renderer.h"
#include"Application.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CObject2D::CObject2D(int nPriority) :CObject(nPriority)
{
	m_pTexture = nullptr;											//ポリゴンのテクスチャ
	m_pVtxBuff = nullptr;											//ポリゴンの頂点バッファ
	m_fLength = 0.0f;												//対角線の長さ
	m_fAngle = 0.0f;												//対角線の角度
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置を初期化する
	m_Rot = 0.0f;													//向きを初期化
	m_Col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_UV[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_UV[1] = D3DXVECTOR2(1.0f, 1.0f);
	m_nRControl = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject2D::~CObject2D()
{
	assert(m_pVtxBuff == nullptr);		//ポリゴンの頂点バッファ
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT  CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;
	//デバイスの取得
	pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//対角線の長さを算出する
	m_fLength = sqrtf((m_Siz.y*m_Siz.y) + (m_Siz.x*m_Siz.x)) / 2.0f;

	//対角線の角度を算出する
	m_fAngle = atan2f(m_Siz.x, m_Siz.y);//敵の弾として使う場合は()の中を敵とプレイヤーの間の距離にする

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = m_Pos.x + sinf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.y = m_Pos.y + cosf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x =m_Pos.x + sinf(m_Rot + D3DX_PI -m_fAngle)*m_fLength;
	pVtx[1].pos.y =m_Pos.y + cosf(m_Rot + D3DX_PI -m_fAngle)*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.y = m_Pos.y + cosf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x =m_Pos.x + sinf(m_Rot +m_fAngle)*m_fLength;
	pVtx[3].pos.y =m_Pos.y + cosf(m_Rot +m_fAngle)*m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//終了
//=============================================================================
void  CObject2D::Uninit()
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
void  CObject2D::Update()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	VERTEX_2D*pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
 	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = m_Pos.x + sinf(m_Rot - D3DX_PI + m_fAngle)*m_fLength-m_nLControl;
	pVtx[0].pos.y = m_Pos.y + cosf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_Pos.x + sinf(m_Rot + D3DX_PI - m_fAngle)*m_fLength+m_nRControl;
	pVtx[1].pos.y = m_Pos.y + cosf(m_Rot + D3DX_PI - m_fAngle)*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_Rot - m_fAngle)*m_fLength - m_nLControl;
	pVtx[2].pos.y = m_Pos.y + cosf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_Pos.x + sinf(m_Rot + m_fAngle)*m_fLength + m_nRControl;
	pVtx[3].pos.y = m_Pos.y + cosf(m_Rot + m_fAngle)*m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//テクスチャの設定
	/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);*/

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画
//=============================================================================
void  CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画       
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
		0,										//頂点の開始場所
		2);										//プリミティブの数

												//テクスチャの解除
	pDevice->SetTexture(0, NULL);
}
//=============================================================================
// 位置の設定
//=============================================================================
void  CObject2D::Setposition(D3DXVECTOR3 move)
{
	m_Pos += move;
}

//=============================================================================
// テクスチャの設定
//=============================================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}

//=============================================================================
// 当たり判定
//=============================================================================
bool CObject2D::Hit(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, D3DXVECTOR2 Siz, D3DXVECTOR2 TargetSiz)
{
	//当たり判定の式
	if (pos.x - Siz.x / 2 <= TargetPos.x + TargetSiz.x/2
		&& pos.x + Siz.x / 2 >= TargetPos.x - TargetSiz.x / 2
		&& pos.y - Siz.y / 2 <= TargetPos.y + TargetSiz.y / 2
		&& pos.y + Siz.y / 2 >= TargetPos.y - TargetSiz.y / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CObject2D::GetPos()
{
	return m_Pos;
}

//=============================================================================
// サイズの取得
//=============================================================================
D3DXVECTOR2 CObject2D::GetSiz()
{
	return m_Siz;
}

//=============================================================================
//色の取得
//=============================================================================
D3DXCOLOR CObject2D::GetCol()
{
	return m_Col;
}

//=============================================================================
//角度の取得
//=============================================================================
float CObject2D::GetRot()
{
	return m_Rot;
}

//=============================================================================
//UV座標の取得
//=============================================================================
D3DXVECTOR2 CObject2D::GetUV(int number)
{
	return m_UV[number];
}

//=============================================================================
// 位置の設定
//=============================================================================
void CObject2D::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// サイズの設定
//=============================================================================
void CObject2D::SetSiz(D3DXVECTOR2 Siz)
{
	m_Siz = Siz;

	m_fLength = sqrtf((m_Siz.y*m_Siz.y) + (m_Siz.x*m_Siz.x)) / 2.0f;

	//対角線の角度を算出する
	m_fAngle = atan2f(m_Siz.x, m_Siz.y);
}

//=============================================================================
// 色の設定
//=============================================================================
void CObject2D::SetCol(D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
// 角度の設定
//=============================================================================
void CObject2D::SetRot(float Rot)
{
	m_Rot = Rot;
}

//=============================================================================
// UV座標の設定
//=============================================================================
void CObject2D::SetUV(float U1, float U2, float V1, float V2)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	VERTEX_2D*pVtx;

	m_UV[0] = D3DXVECTOR2(U1, V1);
	m_UV[1] = D3DXVECTOR2(U2, V2);

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(m_UV[0].x, m_UV[0].y);
	pVtx[1].tex = D3DXVECTOR2(m_UV[1].x, m_UV[0].y);
	pVtx[2].tex = D3DXVECTOR2(m_UV[0].x, m_UV[1].y);
	pVtx[3].tex = D3DXVECTOR2(m_UV[1].x, m_UV[1].y);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 生成
//=============================================================================
CObject2D * CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 siz,int Priority)
{
	CObject2D*pObject2D;
	pObject2D = new CObject2D(Priority);
	pObject2D->SetSiz(siz);
	pObject2D->Init();
	pObject2D->SetPos(pos);
	return pObject2D;
}
