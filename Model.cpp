//=================================================
// Content     (ゲームの設定)(player.cpp)
// Author     : 有田明玄
//=================================================

//インクルード
#include"Model.h"
#include"Game.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Light.h"

//マクロ定義
#define PLAYER_SPEED	(2.0f)	//移動速度
#define MIN_SPEED		(0.1f)	//摩擦による最低速度
#define NUM_MODELPARTS	(1)		//モデルのパーツ数

//静的メンバ変数宣言
LPD3DXBUFFER CModel::m_pPlayerBuffMat[NUM_PLAYERPARTS] = {};	//マテリアル情報へのポインタ
DWORD		 CModel::m_nPlayerNumMat[NUM_PLAYERPARTS] = {};		//マテリアル情報の数
LPD3DXMESH	 CModel::m_pPlayerMesh[NUM_PLAYERPARTS] = {};		//メッシュへのポインタ

//===========================
//コンストラクタ
//===========================
CModel::CModel() 
{

}

//===========================
//デストラクタ
//===========================
CModel::~CModel()
{

}

//===========================
//初期化処理
//===========================
HRESULT CModel::Init()
{
	LoadModel();
	return S_OK;
}

//===========================
//終了処理
//===========================
void CModel::Uninit(void)
{

}

//===========================
//更新処理
//===========================
void CModel::Update(void)
{

}

//===========================
//描画処理
//===========================
void CModel::Draw(D3DXMATRIX pMtx)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルの情報
	m_mtxParent = pMtx;

	//現在のマテリアルを維持
	pDevice->GetMaterial(&matDef);

	//パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//パーツのモデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//パーツのモデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//親のマトリックスと掛け合わせる
	if (m_pParent != nullptr)
	{
		m_mtxParent = m_pParent->GetMtx();
	}
	else
	{//現在(最新)のMtxを取得(PlayerのMtx)親のいないモデルが複数ある場合は別対応が必要
		pDevice->GetTransform(D3DTS_WORLD, &m_mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

	//影の生成
	//Shadow()

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pPlayerBuffMat[m_nNumber]->GetBufferPointer();

	//マテリアルの描画
	for (int nCnt2 = 0; nCnt2 < (int)m_nPlayerNumMat[m_nNumber]; nCnt2++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCnt2].MatD3D);

		//プレイヤーパーツの描画
		m_pPlayerMesh[m_nNumber]->DrawSubset(nCnt2);
	}
	//保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//===========================
//操作
//===========================
CModel * CModel::Create(D3DXVECTOR3 pos, int Number)
{
	CModel*pModel;
	pModel = new CModel();
	pModel->Init();
	pModel->m_nNumber = Number;
	pModel->SetPos(pos);
	return pModel;
}

//===========================
//モデル読み込み
//===========================
void CModel::LoadModel()
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\sphere.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[0],
		NULL,
		&m_nPlayerNumMat[0],
		&m_pPlayerMesh[0]);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\LegBase.000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[1],
		NULL,
		&m_nPlayerNumMat[1],
		&m_pPlayerMesh[1]);

	//ベースをテンキーの2として1の方向
	//第一関節
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint1_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[2],
		NULL,
		&m_nPlayerNumMat[2],
		&m_pPlayerMesh[2]);
	//太もも
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh1_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[3],
		NULL,
		&m_nPlayerNumMat[3],
		&m_pPlayerMesh[3]);
	//足
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot1_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[4],
		NULL,
		&m_nPlayerNumMat[4],
		&m_pPlayerMesh[4]);

	//ベースの正面をテンキーの2として9の方向
	//第一関節
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint2_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[5],
		NULL,
		&m_nPlayerNumMat[5],
		&m_pPlayerMesh[5]);
	//太もも
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh2_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[6],
		NULL,
		&m_nPlayerNumMat[6],
		&m_pPlayerMesh[6]);

	//足
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot2_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[7],
		NULL,
		&m_nPlayerNumMat[7],
		&m_pPlayerMesh[7]);

	//ベースの正面をテンキーの2として3の方向
	//第一関節
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint3_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[8],
		NULL,
		&m_nPlayerNumMat[8],
		&m_pPlayerMesh[8]);
	//太もも
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh3_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[9],
		NULL,
		&m_nPlayerNumMat[9],
		&m_pPlayerMesh[9]);

	//足
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot3_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[10],
		NULL,
		&m_nPlayerNumMat[10],
		&m_pPlayerMesh[10]);

	//ベースの正面をテンキーの2として7の方向
	//第一関節
	D3DXLoadMeshFromX("data\\MODEL\\LegJoint4_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[11],
		NULL,
		&m_nPlayerNumMat[11],
		&m_pPlayerMesh[11]);
	//太もも
	D3DXLoadMeshFromX("data\\MODEL\\LegThigh4_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[12],
		NULL,
		&m_nPlayerNumMat[12],
		&m_pPlayerMesh[12]);

	//足
	D3DXLoadMeshFromX("data\\MODEL\\LegFoot4_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pPlayerBuffMat[13],
		NULL,
		&m_nPlayerNumMat[13],
		&m_pPlayerMesh[13]);
	Siz(m_nNumber);
}

//===========================
//影の作成
//===========================
void CModel::Shadow()
{
	D3DXMATERIAL *pMat;			//マテリアルのなんか
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
	pMat = (D3DXMATERIAL*)m_pPlayerBuffMat[m_nNumber]->GetBufferPointer();

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	for (int i = 0; i < (int)m_nPlayerNumMat[m_nNumber]; i++)
	{
		col[0] = pMat[i].MatD3D.Diffuse;			//色の保存
		col[1] = pMat[i].MatD3D.Emissive;			//発光の保存

		pMat[i].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[i].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//プレイヤーパーツの描画
		m_pPlayerMesh[m_nNumber]->DrawSubset(i);

		pMat[i].MatD3D.Diffuse = col[0];
		pMat[i].MatD3D.Emissive = col[1];
	}
}

//===========================
//親モデルの設定
//===========================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}

//===========================
//親モデルの取得
//===========================
CModel * CModel::GetParent()
{
	return m_pParent;
}

//===========================
//マトリックスの取得
//===========================
D3DXMATRIX CModel::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//向きの設定
//===========================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
//向きの加算
//===========================
D3DXVECTOR3 CModel::GetRot()
{
	return m_rot;
}

//===========================
//位置の設定
//===========================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
//位置の取得
//===========================
D3DXVECTOR3 CModel::GetPos()
{
	return m_pos;
}

float CModel::GetWidth()
{
	 return m_vtxMax.x - m_vtxMin.x ;
}

//===========================
//位置の取得
//===========================
void CModel::Siz(int number)
{
	BYTE*pVtxBuff;		//頂点バッファへのポインタ
	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ

	//頂点バッファのロック
	m_pPlayerMesh[number]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点数の取得
	nNumVtx = m_pPlayerMesh[number]->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pPlayerMesh[number]->GetFVF());

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
	m_pPlayerMesh[number]->UnlockVertexBuffer();

	m_vtxMax = vtxMax;							//頂点座標の最大値
	m_vtxMin = vtxMin;							//頂点座標の最小値
}
