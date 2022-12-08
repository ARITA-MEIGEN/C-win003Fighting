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
	for (int i = 0; i < NUM_PARTS; i++)
	{//プレイヤーの生成
		m_apModel[i] = CModel::Create();
	}

	//モデルとモーションの読み込み
	ReadMotion();

	//回転マトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//===========================
//終了処理
//===========================
void CPlayer::Uninit(void)
{

	for (int i = 0; i < NUM_PARTS; i++)
	{//プレイヤーの生成
		m_apModel[i]->Uninit();
		delete m_apModel[i];
		m_apModel[i] = nullptr;
	}

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

	CDebugProc::Print("現在のプレイヤーの座標:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
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
	//D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fRolling);

	//回転用マトリックスの生成
	//D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//パーツのモデルの向きを反映
	//D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);					//クォータニオンの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//パーツのモデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//Shadow();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int i = 0; i < NUM_PARTS; i++)
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

	//rolling();
}

//===========================
//操作
//===========================
CPlayer * CPlayer::Create()
{
	CPlayer*pPlayer;
	pPlayer = new CPlayer(CObject::OBJTYPE_MODEL);
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
//モーション読み込み
//===========================
void CPlayer::ReadMotion()
{
	const int lenLine = 2048;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	char Read[lenLine];			//読み取る用
	int	modelnumber = 0;		//モデルの番号
	int motionnumber = 0;		//モーションの番号
	int nNumKey = 0;			//1モーションのキーの総数
	int key = 0;
	int model = 0;
	int Idx = 0;


	//ファイル読み込み
	FILE*fp = fopen("data/TXT/Motion.txt", "r");		//ファイル読み込み
	if (fp == nullptr)
	{//開けなかった時用
		assert(false);
	}
	if (fp != NULL)
	{
		while (fgets(Read, lenLine, fp) != nullptr)
		{
			//文字列の分析
			sscanf(Read, "%s", &strLine);
			if (strcmp(&strLine[0], "SCRIPT") == 0)	//比較して一致するかどうか調べる
			{
				while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ(一行読み込み)
				{//モデルの初期設定
					ZeroMemory(strLine,sizeof(char)* 2048);	//文字列リセット

					//文字列の分析
					sscanf(Read, "%s", &strLine);

					if (strcmp(&strLine[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else if (strcmp(&strLine[0], "NUM_MODEL") == 0)
					{
						sscanf(Read, "%s = %d", &strLine, &m_nNumModel);	//読み込んだ文字ごとに設定する
					}
					else if (strcmp(&strLine[0], "MODEL_FILENAME") == 0)
					{
						sscanf(Read, "%s = %s", &strLine, &m_nModelpass[0]);	//モデルのパスの設定

						m_apModel[modelnumber]->SetModel(&m_nModelpass[0]);
						modelnumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//初期位置の設定
						while (fgets(Read, lenLine, fp) != nullptr)//一行読み込み
						{//キャラクターの設定
							//文字列の分析
							ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//読み込みループ //一行読み込み
								{//パーツの設定	
									ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

									//文字列の分析
									sscanf(Read, "%s", &strLine);
									if (strcmp(&strLine[0], "END_PARTSSET") == 0)
									{//パーツの設定終了
										Idx++;
										break;
									}
									else if (strcmp(&strLine[0], "INDEX") == 0)
									{//インデックスの設定
										sscanf(Read, "%s = %d", &strLine, &Idx);	//モデルのパスの設定
									}
									else if (strcmp(&strLine[0], "PARENT") == 0)
									{//親モデルの設定
										int Parent;
										sscanf(Read, "%s = %d", &strLine, &Parent);	//モデルのパスの設定
										m_apModel[Idx]->SetParent(m_apModel[Parent]);
									}
									else if (strcmp(&strLine[0], "POS") == 0)
									{//位置
										D3DXVECTOR3 pos;
										sscanf(Read, "%s = %f%f%f", &strLine, &pos.x, &pos.y, &pos.z);	//座標の取得
										m_apModel[Idx]->SetPos(pos);
									}
									else if (strcmp(&strLine[0], "ROT") == 0)
									{//向き
										D3DXVECTOR3 rot;
										sscanf(Read, "%s = %f%f%f", &strLine, &rot.x, &rot.y, &rot.z);
										m_apModel[Idx]->SetRot(rot);
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "MOTIONSET") == 0)
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ
						{
							ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

							//文字列の分析
							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
							{
								//モーションの番号繰り上げ
								motionnumber++;
								break;
							}
							else if (strcmp(&strLine[0], "NUM_KEY") == 0)
							{
								sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//キーの総数
							}
							else if (strcmp(&strLine[0], "LOOP") == 0)
							{//ループするかしないか
								sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//ループするかどうか
							}
							else if (strcmp(&strLine[0], "KEYSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)
								{
									ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

									//文字列の分析
									sscanf(Read, "%s", &strLine);

									//keyはモデルのキーの番号
									if (strcmp(&strLine[0], "END_KEYSET") == 0)
									{
										key++;
										break;
									}
									else if (strcmp(&strLine[0], "FRAME") == 0)
									{//キーの再生時間の設定
										sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[0].nFrame);	//再生時間の設定
									}
									else if (strcmp(&strLine[0], "KEY") == 0)
									{//キー設定
										while (fgets(Read, lenLine, fp) != nullptr)
										{
											ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

											//文字列の分析
											sscanf(Read, "%s", &strLine);

											if (strcmp(&strLine[0], "END_KEY") == 0)
											{
												model++;
												break;
											}
											else if (strcmp(&strLine[0], "POS") == 0)
											{
												sscanf(Read, "%s = %f %f %f", &strLine,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosX,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosY,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosZ);	//再生時間の設定
											}
											else if (strcmp(&strLine[0], "ROT") == 0)
											{
												sscanf(Read, "%s = %f %f %f", &strLine,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fRotX,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fRotY,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fRotZ);
											}
										}
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "#") == 0)
					{
						continue;
					}
				}
			}
			else if (strcmp(&strLine[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	fclose(fp);
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
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fPosX - m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fPosX,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fPosY - m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fPosY,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fPosZ - m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fPosZ);	//差分の取得

				fDiffRot = D3DXVECTOR3(
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fRotX - m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fRotX,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fRotY - m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fRotY,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fRotZ - m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fRotZ);	//差分の取得

				fRela = (float)m_MotionCnt / m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].nFrame;							//相対値
			}
			else
			{
				fDiffPos = D3DXVECTOR3(
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fPosX -m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fPosX,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fPosY -m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fPosY,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fPosZ -m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fPosZ);	//差分の取得

				fDiffRot = D3DXVECTOR3(
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fRotX -m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fRotX,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fRotY -m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fRotY,
					m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].aKey[i].fRotZ -m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey-1].aKey[i].fRotZ);	//差分の取得

				fRela = (float)m_MotionCnt / m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].nFrame;						//相対値
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
	if (m_MotionCnt++ >= m_apMotion[m_MotionCnt].aModelKey[m_nCurrentKey].nFrame)
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

