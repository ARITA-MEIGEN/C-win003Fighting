//=================================================
// Content     (ゲームの設定)(player.cpp)
// Author     : 有田明玄
//=================================================

//インクルード
#include"ObjectX.h"
#include"Game.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Light.h"

//マクロ定義
#define PLAYER_SPEED	(2.0f)	//移動速度
#define MIN_SPEED		(0.1f)	//摩擦による最低速度
#define NUM_MODELPARTS	(1)		//モデルのパーツ数

static int g_nIdxShadow;		//影のID

//===========================
//コンストラクタ
//===========================
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{

}

//===========================
//デストラクタ
//===========================
CObjectX::~CObjectX()
{

}

//===========================
//初期化処理
//===========================
HRESULT CObjectX::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	
	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズint nNumVtx;		//頂点数
	BYTE*pVtxBuff;		//頂点バッファへのポインタ

	//Xファイルの読み込み
	D3DXLoadMeshFromX(m_modelname,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

		//頂点バッファのロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		//頂点数の取得
		nNumVtx = m_pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		//頂点座標の代入
		//すべての頂点のposを取得する
		D3DXVECTOR3 vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);	//最大値の保存用
		D3DXVECTOR3 vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);	//最小値の保存用
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
			//頂点座標を比較してモデルの最小値最大値を取得
			if (vtx.x > vtxMax.x)
			{//Xの最大値を取得
				vtxMax.x = vtx.x;
			}
			if (vtx.x < vtxMin.x)
			{//Xの最小値を取得
				vtxMin.x = vtx.x;
			}
			if (vtx.y > vtxMax.y)
			{//Yの最大値を取得
				vtxMax.y = vtx.y;
			}
			if (vtx.y < vtxMin.y)
			{//Yの最小値を取得
				vtxMin.y = vtx.y;
			}
			if (vtx.z > vtxMax.z)
			{//Zの最大値を取得
				vtxMax.z = vtx.z;
			}
			if (vtx.z < vtxMin.z)
			{//Zの最小値を取得
				vtxMin.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
		//頂点バッファのアンロック
		m_pMesh->UnlockVertexBuffer();

		//情報の初期化
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//座標
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		m_vtxMax = vtxMax;							//頂点座標の最大値
		m_vtxMin = vtxMin;							//頂点座標の最小値
		m_size = vtxMax - vtxMin;					//パーツのサイズ
	
	//情報の初期化
	m_pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//影の生成
//	m_Shadow = CShadow::Create(m_pos, D3DXVECTOR3(50.0f,0.0f,50.0f));

	return S_OK;
}

//===========================
//終了処理
//===========================
void CObjectX::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	CObject::Release();
}

//===========================
//更新処理
//===========================
void CObjectX::Update(void)
{
	m_posold = m_pos;		//前回の位置の保存
	m_pos += m_move;		//位置の更新

//	m_Shadow->SetPos(m_pos);

	//移動量更新(減衰させる)
	m_move.x += (0.0f - m_move.x)*MIN_SPEED;
	m_move.z += (0.0f - m_move.z)*MIN_SPEED;

}

//===========================
//描画処理
//===========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルのなんか
	D3DXMATRIX mtxParent;						//多分計算用のマトリックス

	//現在のマテリアルを維持
	pDevice->GetMaterial(&matDef);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	////親の向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	////親の位置を反映	
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	for (int nCnt = 0; nCnt < NUM_MODELPARTS; nCnt++)
	{
		//パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//パーツのモデルの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//パーツのモデルの位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		Shadow();

		//親のマトリクスを代入
		mtxParent = m_mtxWorld;

		//モデルのマトリックスとの掛け算　親子関係の掛け算
		//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//マテリアルデータへのポインタを取得
		//マテリアルの描画
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCnt2 = 0; nCnt2 < (int)m_nNumMat; nCnt2++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCnt2].MatD3D);

			//プレイヤーパーツの描画
			m_pMesh->DrawSubset(nCnt2);
		}
	}
	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================
//操作
//===========================
CObjectX * CObjectX::Create(D3DXVECTOR3 pos,int Priority, LPCTSTR name)
{
	CObjectX*pObjectX;
	pObjectX = new CObjectX(Priority);
	pObjectX->m_modelname = name;
	pObjectX->Init();
	pObjectX->SetPos(pos);

	return pObjectX;
}

//===========================
//位置設定
//===========================
void CObjectX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
//方向設定
//===========================
void CObjectX::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
//モデル読み込み
//===========================
void CObjectX::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuff, DWORD pNumMat)
{
	m_pBuffMat = pBuff;
	m_pMesh = pMesh;
	m_nNumMat = pNumMat;
}

//===========================
//影の作成
//===========================
void CObjectX::Shadow()
{
	D3DXMATERIAL *pMat;							//マテリアルのなんか
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXCOLOR col[2];			//色と発光の設定

	//影を描画する処理
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	//シャドウマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxShadow);

	//ライトの逆方向最後の数値はディレクショナルライト以外の場合動かすらしい？
	D3DXVECTOR3 lightvec = CGame::GetLight()->GetVec(2);		//ライトの取得
	vecLight = D3DXVECTOR4(-lightvec.x, -lightvec.y, -lightvec.z, 0.0f);

	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);
	D3DXMatrixShadow(&m_mtxShadow, &vecLight, &planeField);

	//モデルのマトリックスとの掛け算
	D3DXMatrixMultiply(&m_mtxShadow, &m_mtxShadow, &m_mtxWorld);

	//影の描画
	//マテリアルの描画
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		col[0] = pMat[i].MatD3D.Diffuse;
		col[1] = pMat[i].MatD3D.Emissive;

		pMat[i].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[i].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//プレイヤーパーツの描画
		m_pMesh->DrawSubset(i);

		pMat[i].MatD3D.Diffuse = col[0];
		pMat[i].MatD3D.Emissive = col[1];
	}
}