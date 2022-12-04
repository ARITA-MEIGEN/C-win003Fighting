//=================================================
// Content     (ゲームの設定)(.cpp)
// Author     : 有田明玄1
//=================================================

//インクルード
#include"mesh.h"
#include"Application.h"
#include"renderer.h"
#include"Object3D.h"

//===========================
//コンストラクタ
//===========================
CMesh::CMesh(int nPriorty):CObject(nPriorty)
{

}

//===========================
//デストラクタ
//===========================
CMesh::~CMesh()
{

}

//===========================
//初期化処理
//===========================
HRESULT CMesh::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	////テクスチャの読み込み処理
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\block000.jpg",
	//	&m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD)*MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D*pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//中心点
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//テクスチャ座標の設定
	int nCnt = 0;
	int nCnt2 = 0;
	for (nCnt = 0; nCnt < m_Zpolyogn + 1; nCnt++)
	{
		for (nCnt2 = 0; nCnt2 < m_Xpolyogn + 1; nCnt2++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(POLIGON_SIZE*nCnt2,/*rand()%20*/sinf(nCnt*0.62f)*10.0f, -POLIGON_SIZE*nCnt);

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(5.0f*nCnt2, 5.0f*nCnt);

			pVtx++;
		}
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//インデックスバッファのロック
	WORD*pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//インデックスの設定
	for (nCnt2 = 0; nCnt2 < m_Zpolyogn; nCnt2++)
	{
		if (nCnt2 > 0)
		{//縮退ポリゴン
			nCnt = 0;
			pIdx[0] = (WORD)(nCnt + (m_Xpolyogn + 1) + (m_Xpolyogn + 1)*nCnt2);
			pIdx++;
		}

		for (nCnt = 0; nCnt < m_Xpolyogn + 1; nCnt++)
		{
			pIdx[0] = (WORD)(nCnt + (m_Xpolyogn + 1) + (m_Xpolyogn + 1)*nCnt2);
			pIdx[1] = (WORD)(nCnt + (m_Xpolyogn + 1)*nCnt2);
			pIdx += 2;
		}

		if (nCnt2 < m_Zpolyogn)
		{//縮退ポリゴン
			pIdx[0] = (WORD)(nCnt - 1 + (m_Xpolyogn + 1)*nCnt2);
			pIdx++;
		}
	}

	/*pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;*/

	//インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
	return S_OK;
}

//===========================
//終了処理
//===========================
void CMesh::Uninit(void)
{
	//頂点バッファを破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//テクスチャを破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//インデックスバッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	CObject::Release();
}
//===========================
//更新処理
//===========================
void  CMesh::Update(void)
{

}

//===========================
//描画処理
//===========================
void CMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
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

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//メッシュフィールドの描画       
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,				//メッシュフィールドの形
		0, 0, MESHFIELD_VERTEX_NUM, 0, MESHFIELD_PRIMITIVE_NUM);	//頂点の開始場所

	//テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//===========================
//生成
//===========================
CMesh * CMesh::Create(int x,int z)
{
	CMesh*pMesh;
	pMesh = new CMesh(CObject::OBJTYPE_MAP);
	pMesh->m_Xpolyogn = x;
	pMesh->m_Zpolyogn = z;
	pMesh->Init();

	return pMesh;
}

//===========================
//判定
//===========================
float CMesh::Collision(D3DXVECTOR3 pos)
{
	VERTEX_3D*pVtx = NULL;
	D3DXVECTOR3 V1, V2, VecA, VecB, nor, P1, P2, P3;
	D3DXVECTOR3	aNor[2560] = {};
	memset(&aNor[0], 0, sizeof(aNor));
	float fVec[3];
	bool bReturn = false;
	float nSave;

	// インデックスバッファのロック
	WORD*pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int max = MESHFIELD_INDEX_NUM - 2;
	//XZの外積で判定
	for (int i = 0; i < max; i++)
	{
		P1 = pVtx[pIdx[i]].pos;
		P2 = pVtx[pIdx[i + 1]].pos;
		P3 = pVtx[pIdx[i + 2]].pos;

		if (P1 != P2 && P2 != P3 &&P1 != P3)
		{
			VecA = P2 - P1;
			VecB = pos - P1;

			fVec[0] = (VecA.x*VecB.z - VecA.z*VecB.x);

			VecA = P3 - P2;
			VecB = pos - P2;

			fVec[1] = (VecA.x*VecB.z - VecA.z*VecB.x);

			VecA = P1 - P3;
			VecB = pos - P3;

			fVec[2] = (VecA.x*VecB.z - VecA.z*VecB.x);

			//外積の公式
			V1 = P2 - P1;
			V2 = P3 - P1;

			//面法線ベクトルの作成
			D3DXVec3Cross(&nor, &V1, &V2);
			D3DXVec3Normalize(&nor, &nor);
			VecA = pos - (P1);
			VecB = nor;

			//面法線ベクトルの加算
			if (i % 2 == 0)
			{
				aNor[pIdx[i]] += nor;
				aNor[pIdx[i + 1]] += nor;
				aNor[pIdx[i + 2]] += nor;
			}
			else
			{
				aNor[pIdx[i]] -= nor;
				aNor[pIdx[i + 1]] -= nor;
				aNor[pIdx[i + 2]] -= nor;
			}

			for (int j = 0; j < 3; j++)
			{//正規化
				D3DXVec3Normalize(&aNor[pIdx[i + j]], &aNor[pIdx[i + j]]);

				pVtx[pIdx[i + j]].nor = aNor[pIdx[i + j]];
			}

			if (bReturn==false&&(fVec[0] > 0 && fVec[1] > 0 && fVec[2] > 0) || (fVec[0] < 0 && fVec[1] < 0 && fVec[2] < 0))
			{
				//内積の公式
				nSave = ((P1.y) - ((pos.x - (P1.x))*VecB.x + (pos.z - (P1.z))*VecB.z) / VecB.y);
				bReturn = true;
			}
		}
	}
	//バッファをアンロック
	m_pIdxBuff->Unlock();
	m_pVtxBuff->Unlock();

	
	if (bReturn == true)
	{//ポリゴンの中にいた場合
		return nSave;
	}
	return pos.y;
}