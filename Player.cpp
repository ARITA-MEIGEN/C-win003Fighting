//=================================================
// Content     (ゲームの設定)(player.cpp)
// Author     : 有田明玄
//=================================================

//インクルード
#include"Player.h"
#include"Application.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Model.h"
#include"Game.h"

//マクロ定義
#define PLAYER_SPEED	(1.0f)	//移動速度
#define MIN_SPEED		(0.1f)	//摩擦による最低速度
#define NUM_MODELPARTS	(1)		//モデルのパーツ数

//グローバル変数
CPlayer::KEY_SET g_aKeySet[]	//キーの数あとでメンバ変数に
{
	{//最初
		60,
		{
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//胴体
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//脚の土台
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1足
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2足
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }, 	//3足
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }}		//4足
	},
	{//2個
		60,
		{
		{ 0.0f,-10.0f,0.0f ,0.0f,0.0f,0.0f },		//胴体
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//脚の土台
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//1足
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//2足
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//3太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }, 	//3足
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4関節
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f },		//4太もも
		{ 0.0f,0.0f,0.0f ,0.0f,0.0f,0.0f }}		//4足
	},
};
//===========================
//コンストラクタ
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{

}

//===========================
//デストラクタ
//===========================
CPlayer::~CPlayer()
{

}

//===========================
//初期化処理
//===========================
HRESULT CPlayer::Init()
{
	m_nNumKey = 1;

	for (int i = 0; i < NUM_PLAYERPARTS; i++)
	{//プレイヤーの生成
		m_apModel[i] = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), i);
	}

	//親の設定
	//脚の土台
	m_apModel[1]->SetParent(m_apModel[0]);
	for (int i = 0; i < 4; i++)
	{//4つの脚
		for (int j = 0; j < 2; j++)
		{//脚の親設定(3パーツで1脚)親パーツは第一関節
			m_apModel[(i * 3) + 3+j]->SetParent(m_apModel[(i * 3) + 2]);
		}
		//第一関節の作成(親は脚の土台)
		m_apModel[(i * 3) + 2]->SetParent(m_apModel[1]);
	}

	while ((sizeof(g_aKeySet) / sizeof(KEY_SET)) > m_nNumKey)
	{//キーの最大数の確認
		m_nNumKey++;
	}

	//回転マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//===========================
//終了処理
//===========================
void CPlayer::Uninit(void)
{
	CObject::Release();
}

//===========================
//更新処理
//===========================
void CPlayer::Update(void)
{
	m_posold = m_pos;		//前回の位置の保存
	m_pos += m_move;		//位置の更新
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の更新


	ControlPlayer();		//操作
	//MotionPlayer();			//プレイヤーのモーション

	m_pos.y = CGame::GetMesh()->Collision(m_pos);

	////移動量更新(減衰させる)
	//m_move.x += (0.0f - m_move.x)*MIN_SPEED;
	//m_move.z += (0.0f - m_move.z)*MIN_SPEED;
}

//===========================
//描画処理
//===========================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用

	//現在のマテリアルを維持
	pDevice->GetMaterial(&matDef);

	//パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//クォータニオンの設定
	D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fRolling);

	//回転用マトリックスの生成
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//パーツのモデルの向きを反映
	D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);					//クォータニオンの反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	//パーツのモデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y+ 10.0f, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//Shadow();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int i = 0; i < 1; i++)
	{
		m_apModel[i]->Draw(m_mtxWorld);
	}
}

//===========================
//操作
//===========================
void CPlayer::ControlPlayer(void)
{
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//移動
	if (pKeyboard->GetPress(DIK_UP))
	{
		m_move.x = sinf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*1.0f + pCamera->Getrot().y;
	}
	 if (pKeyboard->GetPress(DIK_DOWN))
	{
		m_move.x = sinf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*0.0f + pCamera->Getrot().y;
	}
	if (pKeyboard->GetPress(DIK_LEFT))
	{
		m_move.x = sinf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*0.5f + pCamera->Getrot().y;
	}
	 if (pKeyboard->GetPress(DIK_RIGHT))
	{
		m_move.x = sinf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		//m_rotDest.y = D3DX_PI*-0.5f + pCamera->Getrot().y;
	}

	////一定量離れていたら逆回転
	//if (m_rotDest.y - m_rot.y >= D3DX_PI)
	//{
	//	m_rotDest.y -= D3DX_PI * 2;
	//}
	//else if (m_rotDest.y - m_rot.y <= -D3DX_PI)
	//{
	//	m_rotDest.y += D3DX_PI * 2;
	//}

	//m_rot.y += (m_rotDest.y - m_rot.y)*0.1f;

	//角度の正規化
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	rolling();
}

//===========================
//操作
//===========================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, int Priority)
{
	CPlayer*pPlayer;
	pPlayer = new CPlayer(Priority);
	pPlayer->Init();

	return pPlayer;
}

//===========================
//マトリックスの取得
//===========================
D3DXMATRIX CPlayer::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//パーツのモーション
//===========================
void CPlayer::MotionPlayer()
{
	D3DXVECTOR3 pos, rot, fDiffPos, fDiffRot;
	float fRela;
	for (int i = 0; i < NUM_PLAYERPARTS; i++)
	{
		if (m_apModel[i] != nullptr)
		{
			if (m_nCurrentKey != 0)
			{
				fDiffPos = D3DXVECTOR3(
					g_aKeySet[m_nCurrentKey].aKey[i].fPosX - g_aKeySet[m_nCurrentKey - 1].aKey[i].fPosX,
					g_aKeySet[m_nCurrentKey].aKey[i].fPosY - g_aKeySet[m_nCurrentKey - 1].aKey[i].fPosY,
					g_aKeySet[m_nCurrentKey].aKey[i].fPosZ - g_aKeySet[m_nCurrentKey - 1].aKey[i].fPosZ);	//差分の取得

				fDiffRot = D3DXVECTOR3(
					g_aKeySet[m_nCurrentKey].aKey[i].fRotX - g_aKeySet[m_nCurrentKey - 1].aKey[i].fRotX,
					g_aKeySet[m_nCurrentKey].aKey[i].fRotY - g_aKeySet[m_nCurrentKey - 1].aKey[i].fRotY,
					g_aKeySet[m_nCurrentKey].aKey[i].fRotZ - g_aKeySet[m_nCurrentKey - 1].aKey[i].fRotZ);	//差分の取得

				fRela = (float)m_MotionCnt / g_aKeySet[m_nCurrentKey].nFrame;							//相対値
			}
			else
			{
				fDiffPos = D3DXVECTOR3(
					g_aKeySet[0].aKey[i].fPosX - g_aKeySet[m_nNumKey - 1].aKey[i].fPosX,
					g_aKeySet[0].aKey[i].fPosY - g_aKeySet[m_nNumKey - 1].aKey[i].fPosY,
					g_aKeySet[0].aKey[i].fPosZ - g_aKeySet[m_nNumKey - 1].aKey[i].fPosZ);	//差分の取得

				fDiffRot = D3DXVECTOR3(
					g_aKeySet[0].aKey[i].fRotX - g_aKeySet[m_nNumKey - 1].aKey[i].fRotX,
					g_aKeySet[0].aKey[i].fRotY - g_aKeySet[m_nNumKey - 1].aKey[i].fRotY,
					g_aKeySet[0].aKey[i].fRotZ - g_aKeySet[m_nNumKey - 1].aKey[i].fRotZ);	//差分の取得

				fRela = (float)m_MotionCnt / g_aKeySet[m_nCurrentKey].nFrame;						//相対値
			}

			//キーの設定
			pos = (fDiffPos* fRela) + m_apModel[i]->GetPos();
			rot = (fDiffRot* fRela) + m_apModel[i]->GetRot();

			//位置の設定
			m_apModel[i]->SetPos(pos);
			m_apModel[i]->SetRot(rot);
		}
	}
	//カウンター更新
	if (m_MotionCnt++ >= g_aKeySet[m_nCurrentKey].nFrame)
	{//キー番号の更新とカウンターのリセット
		m_nCurrentKey++;
		m_MotionCnt = 0;
		if (m_nCurrentKey >= m_nNumKey)
		{//キー番号が最大数を超えた場合リセット
			m_nCurrentKey = 0;
		}
	}
}

//===========================
//回転テスト
//===========================
void CPlayer::rolling()
{
	//回転軸の設定
	m_vecAxis = D3DXVECTOR3(m_move.z, 0.0f, -m_move.x);

	//回転量の設定
	float Radius = m_apModel[0]->GetWidth();
	//m_fRolling = (m_move.x * m_move.z) * 2 * D3DX_PI / (Radius *D3DX_PI);
	m_fRolling = D3DXVec3Length(&m_move) * 2 * D3DX_PI / (Radius *D3DX_PI);
}



