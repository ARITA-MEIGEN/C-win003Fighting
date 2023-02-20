//=================================================
// Content     (ゲームの設定)(Collision.cpp)
// Author     : 有田明玄
//=================================================
//===========================
//インクルード
//===========================
#include"Collision.h"
#include"camera.h"
#include"shadow.h"
#include"Application.h"
#include"renderer.h"

//マクロ定義
#define NUM_BUFF		(24)		//1つの判定につき必要なバッファの数
#define NUM_LINE		(16)		//1つのボックスを形成するのに必要な数


//===========================
//コンストラクタ
//===========================
CCollision::CCollision(int nPriority)
{
}

//===========================
//デストラクタ
//===========================
CCollision::~CCollision()
{
}

//===========================
//初期化処理
//===========================
HRESULT CCollision::Init(void)
{

#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_BUFF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	//頂点座標へのポインタ
	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[2].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[3].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[4].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[6].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[7].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);

	pVtx[8].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[9].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[10].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[11].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[12].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[13].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[14].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[15].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	switch (m_Colltype)
	{//種類に応じて色の設定
	case CCollision::COLLI_AXIS:
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;

	case CCollision::COLLI_DAMAGE:
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;

	case CCollision::COLLI_SLOW:
		m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;

	case CCollision::COLLI_HURT:
		m_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		break;

	case CCollision::COLLI_HURTSLOW:
		m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		break;

	default:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}

	for (int i = 0; i < NUM_BUFF; i++)	//上段
	{
		//頂点座標の設定
		//法線ベクトルの設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//色の設定
		pVtx[i].col = m_col;
	}

#endif // !_DEBUG

	//情報の初期化
	m_bUse = false;
	m_rot = D3DXVECTOR3(0.0f,0.f,0.f);
	m_width = D3DXVECTOR3(50.0f, 50.f, 50.f);

	m_nDamage = 50;

	m_Property = EDAMAGE_POINT::DP_HIGH;	//基本は上段攻撃

	return S_OK;
}

//==============================
//終了処理
//==============================
void CCollision::Uninit(void)
{
	//バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CObject::Release();
}
//==============================
//更新処理
//==============================
void CCollision::Update(void)
{
#ifdef _DEBUG
	if (m_pVtxBuff != NULL)
	{
		//頂点座標へのポインタ
		VERTEX_3D*pVtx = NULL;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[2].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[3].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[4].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[6].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[7].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);

		pVtx[8].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[9].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[10].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[11].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[12].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[13].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[14].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[15].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

		pVtx[16].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[17].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[18].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[19].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[20].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[21].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[22].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[23].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

		//頂点バッファをアンロック
		m_pVtxBuff->Unlock();
	}
#endif // _DEBUG
	m_posold = m_pos;
}

//==============================
//描画処理
//==============================
void CCollision::Draw(void)
{
	if (m_bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
		pDevice = CApplication::GetRenderer()->GetDevice();

		D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

										//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

#ifdef _DEBUG
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画       
		pDevice->DrawPrimitive(D3DPT_LINELIST,	//ポリゴンの形(線)
			0,									//頂点の開始場所
			NUM_LINE);							//プリミティブの数
#endif // !_DEBUG
	}
}

//=====================
//生成
//=====================
CCollision* CCollision::Create(D3DXVECTOR3 pos, COLLISION type)
{
	CCollision*pCollision = new CCollision();

	if (pCollision != nullptr)
	{
		pCollision->m_Colltype = type;
		pCollision->Init();
		pCollision->m_defaultpos = pos;

		pCollision->m_bUse = true;
	}

	return pCollision;
}
