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
#include"Collision.h"

//静的メンバ変数
int CPlayer::m_nPlayer = 0;

//===========================
//コンストラクタ
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	m_nPlayerNumber = m_nPlayer;
	m_nPlayer++;
}

//===========================
//デストラクタ
//===========================
CPlayer::~CPlayer()
{
	m_nPlayer--;
}

//===========================
//初期化処理
//===========================
HRESULT CPlayer::Init()
{
	m_Motion = PM_ST_NEUTRAL;
	m_bMotion = false;

	for (int i = 0; i < NUM_PARTS; i++)
	{//プレイヤーの生成
		m_apModel[i] = CModel::Create();
	}

	//エラー防止用に全ての数値に0を入れる
	for (int i = 0; i < MAX_MOTION; i++)
	{//モーションの初期化
		for (int j = 0; j < MAX_KEY; j++)
		{
			for (int k = 0; k < NUM_PARTS; k++)
			{
				m_apMotion[i].aModelKey[j].aKey[k].fPosX = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fPosY = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fPosZ = 0.0f;

				m_apMotion[i].aModelKey[j].aKey[k].fRotX = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fRotY = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fRotZ = 0.0f;
			}
			m_apMotion[i].aModelKey[j].nNumCollision = 0;
			m_apMotion[i].aModelKey[j].nFrame = 1;
		}
	}

	m_AxisBox = CCollision::Create(m_pos,CCollision::COLLI_AXIS);						//押し出し判定(プレイヤーの軸)
	
	//モデルとモーションの読み込み
	ReadMotion();

	for (int i = 0; i < NUM_PARTS; i++)
	{
		//プレイヤーの生成
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fPosX,
			m_apMotion[0].aModelKey[0].aKey[i].fPosY,
			m_apMotion[0].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fRotX,
			m_apMotion[0].aModelKey[0].aKey[i].fRotY,
			m_apMotion[0].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//差分の取得
	}

	//デバッグ用設定後でファイル入出力できるようにする
	m_nJump = 50;
	m_nLife = 1000;

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
	if (m_bMotion == false)
	{
		m_Motion = PM_ST_NEUTRAL;
	}

	if (m_pos.x - m_pEnemy->GetPos().x > 240.0f || m_pos.x - m_pEnemy->GetPos().x < -240.0f)
	{
		m_pos.x = m_posold.x;
	}

	m_posold = m_pos;		//前回の位置の保存
	m_pos += m_move;		//位置の更新
	m_AxisBox->SetPos(m_pos + D3DXVECTOR3(0.0f,m_AxisBox->GetDPos().y,0.0f));
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の更新

	ControlPlayer();		//操作
	Jump();					//ジャンプ
	Axis();					//軸の押し出し判定
	MotionManager();
	AutoTurn();
	//m_pos.y = CGame::GetMesh()->Collision(m_pos);
	DrawCollision();
	SetHitBox();

	CDebugProc::Print("現在のプレイヤーの座標:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("現在のモーション:%d ", (int)m_Motion);
	if (m_nLife <= 0)
	{//体力が亡くなった時

	}
}

//===========================
//描画処理
//===========================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用

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

#ifdef _DEBUG
	if (CGame::GetDebugCamera() == true)
	{
		return;
	}
#endif
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//移動
	if (m_nPlayerNumber == 0 && m_bMotion == false)
	{
		if (pKeyboard->GetPress(DIK_NUMPAD7) )
		{
			m_nLife--;
		}
		if (pKeyboard->GetPress(DIK_S) && m_State != PST_AIR)
		{
			m_Motion = PM_CR_NEUTRAL;
			m_State = PST_CROUCH;
		}
		if (pKeyboard->GetRelease(DIK_S) && m_State != PST_AIR)
		{
			m_State = PST_GROUND;	//立ち状態に戻す
		}
		switch (m_State)
		{
		case CPlayer::PST_GROUND:
			if (pKeyboard->GetPress(DIK_A))
			{
				m_move.x = sinf(D3DX_PI*-0.5f)*PLAYER_SPEED;
				m_move.z = cosf(D3DX_PI*-0.5f)*PLAYER_SPEED;
				m_Motion = PM_ST_MOVE;
				if (m_pEnemy->m_bAttack == true && m_pos.x <= m_pEnemy->m_pos.x)
				{
					m_Motion = PM_ST_GUARD;
					m_move = { 0.0f,0.0f,0.0f };
				}
			}
			if (pKeyboard->GetPress(DIK_D))
			{
				m_move.x = sinf(D3DX_PI*0.5f)*PLAYER_SPEED;
				m_move.z = cosf(D3DX_PI*0.5f)*PLAYER_SPEED;
				m_Motion = PM_ST_MOVE;
				if (m_pEnemy->m_bAttack == true && m_pos.x >= m_pEnemy->m_pos.x)
				{
					m_Motion = PM_ST_GUARD;
					m_move = { 0.0f,0.0f,0.0f };
				}
			}
			if (pKeyboard->GetTrigger(DIK_U))
			{
				m_Motion = PM_ST_LATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetTrigger(DIK_I))
			{
				m_Motion = PM_ST_MATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetTrigger(DIK_O))
			{
				m_Motion = PM_ST_HATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetPress(DIK_SPACE))
			{
				m_Motion = PM_JP_NEUTRAL;
				m_State = PST_AIR;
				if (pKeyboard->GetPress(DIK_D))
				{
					m_Motion = PM_JP_MOVERIGHT;
				}
				else if (pKeyboard->GetPress(DIK_A))
				{
					m_Motion = PM_JP_MOVELEFT;

				}
			}
			break;

		case CPlayer::PST_AIR:
			if (pKeyboard->GetTrigger(DIK_U))
			{
				m_Motion = PM_JP_LATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetTrigger(DIK_I))
			{
				m_Motion = PM_JP_MATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetTrigger(DIK_O))
			{
				m_Motion = PM_JP_HATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}

			break;
		case CPlayer::PST_CROUCH:
			if (pKeyboard->GetTrigger(DIK_U))
			{
				m_Motion = PM_CR_LATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetTrigger(DIK_I))
			{
				m_Motion = PM_CR_MATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			if (pKeyboard->GetTrigger(DIK_O))
			{
				m_Motion = PM_CR_HATTACK;
				m_bAttack = true;
				m_bMotion = true;
			}
			break;
		case CPlayer::PST_DAMAGE:
			break;
		default:
			break;
		}
	}

#ifdef _DEBUG
	if (pKeyboard->GetPress(DIK_P) && m_nPlayerNumber == 1)
	{
		m_Motion = PM_ST_LATTACK;
		m_bAttack = true;
		m_bMotion = true;
	}
	if (pKeyboard->GetPress(DIK_NUMPAD4) && m_nPlayerNumber == 1)
	{
		m_move.x = sinf(D3DX_PI*-0.5f)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*-0.5f)*PLAYER_SPEED;
		m_Motion = PM_ST_MOVE;
		if (m_pEnemy->m_bAttack == true && m_pos.x <= m_pEnemy->m_pos.x)
		{
			m_Motion = PM_ST_GUARD;
			m_move = { 0.0f,0.0f,0.0f };
		}
	}
	if (pKeyboard->GetPress(DIK_NUMPAD6) && m_nPlayerNumber == 1)
	{
		m_move.x = sinf(D3DX_PI*0.5f)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.5f)*PLAYER_SPEED;
		m_Motion = PM_ST_MOVE;
		if (m_pEnemy->m_bAttack == true && m_pos.x >= m_pEnemy->m_pos.x)
		{
			m_Motion = PM_ST_GUARD;
			m_move = { 0.0f,0.0f,0.0f };
		}
	}
#endif // !_DEBUG

	//角度の正規化
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//===========================
//生成
//===========================
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer*pPlayer;
	pPlayer = new CPlayer(CObject::OBJTYPE_MODEL);
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
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
	int	m_pEnemynumber = 0;		//モデルの番号
	int motionnumber = 0;		//モーションの番号
	int key = 0;
	int m_pEnemy = 0;
	int Idx = 0;


	//ファイル読み込み
	FILE*fp = fopen("data/TXT/Player01/Player01.txt", "r");		//ファイル読み込み
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
					ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

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

						m_apModel[m_pEnemynumber]->SetModel(&m_nModelpass[0]);
						m_pEnemynumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//初期位置の設定
						while (fgets(Read, lenLine, fp) != nullptr)//一行読み込み
						{//キャラクターの設定
						 //文字列の分析
							ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//読み込みループ //一行読み込み
								{//パーツの設定	
									ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

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
										m_apModel[Idx]->SetDPos(pos);
									}
									else if (strcmp(&strLine[0], "ROT") == 0)
									{//向き
										D3DXVECTOR3 rot;
										sscanf(Read, "%s = %f%f%f", &strLine, &rot.x, &rot.y, &rot.z);
										m_apModel[Idx]->SetRot(rot);
										m_apModel[Idx]->SetDRot(rot);
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "MOTIONSET") == 0)
					{//モーションの設定
						while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

							//文字列の分析
							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
							{
								if (motionnumber > PM_MAX)
								{
									assert(false);
								}
								break;
							}
							else if (strcmp(&strLine[0], "MOTION") == 0)
							{
								const char cFilename[255] = {};
								sscanf(Read, "%s = %s", &strLine, &cFilename[0]);	//ファイル読み込み

								//ファイル読み込み
								FILE* sta = fopen(cFilename, "r");		//ファイル読み込み
								if (sta == nullptr)
								{//開けなかった時用
									assert(false);
								}
								if (sta != NULL)
								{
									while (fgets(Read, lenLine, sta) != nullptr)	//読み込み用ループ
									{
										ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

										//文字列の分析
										sscanf(Read, "%s", &strLine);

										if (strcmp(&strLine[0], "MOTIONSET") == 0)
										{
											while (fgets(Read, lenLine, sta) != nullptr)	//読み込み用ループ
											{
												ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

												//文字列の分析
												sscanf(Read, "%s", &strLine);
												if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
												{
													//モーションの番号繰り上げ
													motionnumber++;
													key = 0;
													fclose(sta);
													break;
												}
												if (strcmp(&strLine[0], "NUM_KEY") == 0)
												{
													sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//キーの総数
												}
												else if (strcmp(&strLine[0], "LOOP") == 0)
												{//ループするかしないか
													sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//ループするかどうか
												}
												else if (strcmp(&strLine[0], "KEYSET") == 0)
												{
													while (fgets(Read, lenLine, sta) != nullptr)
													{
														ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

														//文字列の分析
														sscanf(Read, "%s", &strLine);

														//keyはモデルのキーの番号
														if (strcmp(&strLine[0], "END_KEYSET") == 0)
														{
															key++;
															m_pEnemy = 0;	//番号リセット
															break;
														}
														else if (strcmp(&strLine[0], "FRAME") == 0)
														{//キーの再生時間の設定
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[key].nFrame);	//再生時間の設定
														}
														else if (strcmp(&strLine[0], "COLLISIONSET") == 0)
														{
															m_apMotion[motionnumber].aModelKey[key].Collision[m_apMotion[motionnumber].aModelKey[key].nNumCollision]
																= CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																											//文字列の分析
																sscanf(Read, "%s", &strLine);

																//keyはモデルのキーの番号
																if (strcmp(&strLine[0], "END_COLLISIONSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumCollision++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//判定の開始時間
																	int start, select;
																	select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &start);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//判定の開始時間
																	int end;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &end);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	D3DXVECTOR3 dpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &dpos.x, &dpos.y, &dpos.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetDPos(dpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  dsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %f%f%f", &strLine, &dsiz.x, &dsiz.y, &dsiz.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetSiz(dsiz);
																}
															}
														}
														else if (strcmp(&strLine[0], "HURTSET") == 0)
														{
															m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]
																= CCollision::Create(m_pos, CCollision::COLLI_HURT);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																//文字列の分析
																sscanf(Read, "%s", &strLine);

																//keyはモデルのキーの番号
																if (strcmp(&strLine[0], "END_HURTSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumHurtCol++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//判定の開始時間
																	int start;
																	sscanf(Read, "%s = %d", &strLine, &start);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//判定の開始時間
																	int end;
																	sscanf(Read, "%s = %d", &strLine, &end);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	D3DXVECTOR3 hpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &hpos.x, &hpos.y, &hpos.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetDPos(hpos);
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetPos(hpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  hsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %f%f%f", &strLine, &hsiz.x, &hsiz.y, &hsiz.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetSiz(hsiz);
																}
															}
														}
														else if (strcmp(&strLine[0], "KEY") == 0)
														{//キー設定
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																//文字列の分析
																sscanf(Read, "%s", &strLine);

																if (strcmp(&strLine[0], "END_KEY") == 0)
																{
																	m_pEnemy++;
																	break;
																}
																else if (strcmp(&strLine[0], "POS") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosX,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosY,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosZ);	//再生時間の設定
																}
																else if (strcmp(&strLine[0], "ROT") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fRotX,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fRotY,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fRotZ);
																}
															}
														}
													}
												}
											}
											break;	//読み込みが終わったらループ脱出
										}
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "AXISSET") == 0)
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

							//文字列の分析
							sscanf(Read, "%s", &strLine);

							if (strcmp(&strLine[0], "END_AXIS") == 0)
							{
								break;
							}
							else if ((strcmp(&strLine[0], "POS") == 0))
							{
								D3DXVECTOR3 axpos;
								sscanf(Read, "%s = %f%f%f", &strLine, &axpos.x, &axpos.y, &axpos.z);	//キーの総数
								m_AxisBox->SetPos(axpos + m_pos);
								m_AxisBox->SetDPos(axpos);

							}
							else if ((strcmp(&strLine[0], "SIZ") == 0))
							{
								D3DXVECTOR3  axsiz;
								sscanf(Read, "%s = %f%f%f", &strLine, &axsiz.x, &axsiz.y, &axsiz.z);	//キーの総数
								m_AxisBox->SetSiz(axsiz);
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
		fclose(fp);
	}
}

//===========================
//パーツのモーション
//===========================
void CPlayer::MotionPlayer(int nNumber)
{
	D3DXVECTOR3 RelaPos, RelaRot;		//1フレームごとの移動量
	D3DXVECTOR3 pos, rot, DiffPos, DiffRot;
	CDebugProc::Print("現在のフレーム:%d", m_frame);

	//カウンター更新
	if (m_nCurrentKey == m_apMotion[m_Motion].nNumKey&& m_apMotion[m_Motion].bLoop == false)
	{
		//エディットじゃないときループモーションが終わったらニュートラルにする
		m_bMotion = false;
		m_bAttack = false;
		PlayFirstMotion();
		return;
	}
	else
	{
		for (int i = 0; i < m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nNumCollision; i++)
		{//ダメージ判定が出現するかどうか(1～4フレームまでダメージ判定を出すみたいな)
			if (m_frame >= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->GetStartf())
			{//開始フレーム以上かどうか
				if (m_frame <= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->GetEndf())
				{//終了フレーム以下かどうか
					m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->SetUse(true);
				}
				else
				{
					m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->SetUse(false);
				}
			}
			else
			{
				m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->SetUse(false);
			}
		}
		for (int i = 0; i < NUM_PARTS; i++)
		{//パーツ全部のモーション再生
			if (m_apModel[i] != nullptr)
			{
				if (m_nCurrentKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == true)
				{//ループする場合最初のモーションに移行する
					DiffPos = D3DXVECTOR3(
						m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosZ - m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	//差分の取得

					DiffRot = D3DXVECTOR3(
						m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	//差分の取得
				}
				else if (m_nCurrentKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == false)
				{//ループしない場合ニュートラルモーションに戻す
					DiffPos = D3DXVECTOR3(//座標差分の取得
						m_apMotion[0].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[0].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[0].aModelKey[0].aKey[i].fPosZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//向き差分の取得
						m_apMotion[0].aModelKey[0].aKey[i].fRotX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[0].aModelKey[0].aKey[i].fRotY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[0].aModelKey[0].aKey[i].fRotZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotZ);
				}
				else
				{
					if (m_nCurrentKey == 0 && m_frame == 0)
					{//位置座標の設定しなおし
						PlayFirstMotion();
					}
					DiffPos = D3DXVECTOR3(//座標差分の取得
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fPosX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fPosY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fPosZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//向き差分の取得
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fRotX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fRotY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fRotZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotZ);
				}
			}

			//位置
			RelaPos = DiffPos / m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nFrame;		//相対値
			RelaRot = DiffRot / m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nFrame;

			//キーの設定
			//再生モードの場合
			pos = RelaPos + m_apModel[i]->GetPos();
			rot = RelaRot + m_apModel[i]->GetRot();

			//位置の設定
			m_apModel[i]->SetPos(pos);
			m_apModel[i]->SetRot(rot);
		}
	}
	//再生モードの場合
	 //カウンター更新
	
	if (m_frame >= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nFrame)
	{//キー番号の更新とカウンターのリセット
		m_nCurrentKey++;
		m_frame = 0;
		if (m_nCurrentKey >= m_apMotion[m_Motion].nNumKey)
		{//キー番号が最大数を超えた場合リセット
			if (m_apMotion[m_Motion].bLoop == true)
			{
				m_nCurrentKey = 0;
			}
		}
	}
	m_frame++;
}

//===========================
//モーション管理
//===========================
void CPlayer::MotionManager()
{
	if (m_MotionOld != m_Motion)
	{//状態が違う場合
		PlayFirstMotion();
	}
		MotionPlayer(m_Motion);		//プレイヤーのモーション
	m_MotionOld = m_Motion;
}

//===========================
//最初のモーションを設定
//===========================
void CPlayer::PlayFirstMotion()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//プレイヤーの生成
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//差分の取得
	}
	m_frame = 0;
	m_nCurrentKey = 0;
}

//===========================
//当たり判定を表示するかどうか
//===========================
void CPlayer::DrawCollision()
{
	//当たり判定の表示
	for (int i = 0; i < PM_MAX; i++)
	{
		for (int k = 0; k < m_apMotion[i].nNumKey; k++)
		{
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumCollision; j++)
			{//違うモーションの当たり判定をオフにする
				if (m_Motion == i&&m_nCurrentKey == k)
				{//キーとモーションが一致している場合のみ表示
					if (m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
					{
						m_apMotion[i].aModelKey[k].Collision[j]->SetUse(true);
						if ( m_pos.x >= m_pEnemy->m_pos.x)
						{//敵より右側の場合
							m_apMotion[i].aModelKey[k].Collision[j]->SetPos(m_pos + m_apMotion[i].aModelKey[k].Collision[j]->GetDPos());

						}
						else
						{
							m_apMotion[i].aModelKey[k].Collision[j]->SetPos(D3DXVECTOR3(
								m_pos.x - m_apMotion[i].aModelKey[k].Collision[j]->GetDPos().x,				//X
								m_pos.y + m_apMotion[i].aModelKey[k].Collision[j]->GetDPos().y,
								m_pos.z - m_apMotion[i].aModelKey[k].Collision[j]->GetDPos().z));
						}
					}
				}
				else
				{
					if (m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
					{
						m_apMotion[i].aModelKey[k].Collision[j]->SetUse(false);
					}
				}
			}
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumHurtCol; j++)
			{//やられ判定
				if (m_Motion == i&&m_nCurrentKey == k&&m_apMotion[i].aModelKey[k].HurtCol[j] != nullptr)
				{
					m_apMotion[i].aModelKey[k].HurtCol[j]->SetUse(true);

					m_apMotion[i].aModelKey[k].HurtCol[j]->SetPos(m_apMotion[i].aModelKey[k].HurtCol[j]->GetDPos() + m_pos);

				}
				else
				{
					if (m_apMotion[i].aModelKey[k].HurtCol[j] != nullptr)
					{
						m_apMotion[i].aModelKey[k].HurtCol[j]->SetUse(false);
					}
				}
			}
		}
	}
}

//=====================
//押し出し判定
//=====================
void CPlayer::Axis(void)
{
	if (m_pos.y - m_AxisBox->GetWidth().y / 2 <= m_pEnemy->m_pos.y + m_pEnemy->m_AxisBox->GetWidth().y 
		&& m_pos.y + m_AxisBox->GetWidth().y / 2 >= m_pEnemy->m_pos.y - m_pEnemy->m_AxisBox->GetWidth().y )
	{
			//X軸
			//左側
			if (m_posold.x + m_AxisBox->GetWidth().x / 2 <= m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x / 2	//前回のプレイヤーの位置がブロックの位置より右にいる場合
				&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x / 2 	//現在のプレイヤーの位置がブロックの位置より左にいる(めり込んでいる)場合
				)//プレイヤーの左右にブロックが当たった場合
			{//プレイヤ-とブロックが当たった時
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = 0;
			}
			//右側
			if (m_posold.x - m_AxisBox->GetWidth().x / 2 >= m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x / 2 	//前回のプレイヤーの位置がブロックの位置より右にいる場合
				&& m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x / 2	//現在のプレイヤーの位置がブロックの位置より左にいる(めり込んでいる)場合
				)//プレイヤーの左右にブロックが当たった場合
			{//プレイヤ-とブロックが当たった時
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = 0;
			}
	}
	if (m_posold.y + m_pEnemy->m_AxisBox->GetWidth().y >= m_pEnemy->m_pos.y + m_pEnemy->m_AxisBox->GetWidth().y
		&& m_pos.y+ m_pEnemy->m_AxisBox->GetWidth().y<= m_pEnemy->m_pos.y + m_pEnemy->m_AxisBox->GetWidth().y )
	{//ジャンプした時の押し合いの判定
		if (m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x / 2
			&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x / 2)
		{
			if (m_pos.x >= m_pEnemy->m_pos.x)
			{//中心より→
				m_pos.x = m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x+1;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = -m_move.x;
			}
			else
			{//中心より←
				m_pos.x = m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x-1;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = -m_move.x;
			}
		}
	}
}

//=====================
//ジャンプ
//=====================
void CPlayer::Jump(void)
{
	if (m_State == PST_AIR)
	{
		if (m_MotionOld == PM_JP_MOVELEFT|| m_Motion == PM_JP_MOVELEFT)
		{
			m_Motion = PM_JP_MOVELEFT;
			m_move.x = sinf(D3DX_PI*-0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
			m_move.z = cosf(D3DX_PI*-0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
		}
		else if (m_MotionOld == PM_JP_MOVERIGHT|| m_Motion == PM_JP_MOVERIGHT)
		{
			m_Motion = PM_JP_MOVERIGHT;
			m_move.x = sinf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
			m_move.z = cosf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
		}
		else if (m_MotionOld == PM_JP_NEUTRAL || m_Motion == PM_JP_NEUTRAL)
		{
			m_Motion = PM_JP_NEUTRAL;
		}
			if (m_nJumpCount >= m_nJump - 1)
			{
				m_State = PST_GROUND;
			}
		if (m_nJumpCount >= m_nJump / 2)
		{
			m_move.y = -Easing::OutSine<float>(m_nJumpCount - m_nJump / 2, m_nJump, MAX_SPEED, INITIAL_VELOCITY);
		}
		else
		{
			m_move.y = Easing::OutSine<float>(m_nJumpCount, m_nJump, MAX_SPEED, INITIAL_VELOCITY);
		}
		m_nJumpCount++;
		if (m_nJumpCount >= m_nJump)
		{
			m_nJumpCount = 0;
			m_Motion = PM_ST_NEUTRAL;
			m_bMotion = false;
		}
	}
}

//=====================
//自動振り向き
//=====================
void CPlayer::AutoTurn(void)
{
	if (m_State == PST_GROUND)
	{//地上時のみ相手の方向位振りむく
		if (m_pos.x >= m_pEnemy->m_pos.x)
		{
			m_rot.y = D3DX_PI*0.5f;
		}
		else
		{
			m_rot.y = -D3DX_PI*0.5f;
		}
	}
}

//===========================
//やられ判定の設定
//===========================
void CPlayer::SetHitBox()
{
	for (int i = 0; i < m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nNumHurtCol; i++)
	{//ダメージ判定が出現するかどうか(1～4フレームまでダメージ判定を出すみたいな)
		if (m_frame >= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->GetStartf())
		{//開始フレーム以上かどうか
			if (m_frame <= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->GetEndf())
			{//終了フレーム以下かどうか
				m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->SetUse(true);
			}
			else
			{
				m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->SetUse(false);
			}
		}
		else
		{
			m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->SetUse(false);
		}
	}
}
