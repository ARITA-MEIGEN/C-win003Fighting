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

//===========================
//コンストラクタ
//===========================
CModel::CModel() 
{
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
	if (m_pMesh != nullptr)
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

		//親の座標とかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

		//影の生成
		//Shadow()

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//マテリアルの描画
		for (int nCnt2 = 0; nCnt2 < (int)m_nNumMat; nCnt2++)
		{
			pMat[nCnt2].MatD3D.Emissive = m_col;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCnt2].MatD3D);

			//プレイヤーパーツの描画
			m_pMesh->DrawSubset(nCnt2);
		}
		//保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//===========================
//生成
//===========================
CModel * CModel::Create()
{
	CModel*pModel;
	pModel = new CModel();
	pModel->Init();
	pModel->m_nIdx = 0;
	return pModel;
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
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		col[0] = pMat[i].MatD3D.Diffuse;			//色の保存
		col[1] = pMat[i].MatD3D.Emissive;			//発光の保存

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

//===========================
//位置の取得
//===========================
void CModel::Siz()
{
	BYTE*pVtxBuff;		//頂点バッファへのポインタ
	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ

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

	m_vtxMax = vtxMax;							//頂点座標の最大値
	m_vtxMin = vtxMin;							//頂点座標の最小値
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

//===========================
//Xの幅の取得
//===========================
float CModel::GetWidth()
{
	 return m_vtxMax.x - m_vtxMin.x ;
}

//===========================
//モデルの設定
//===========================
void CModel::SetModel(char* Filename)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(Filename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
}