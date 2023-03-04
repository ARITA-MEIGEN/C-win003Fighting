//=================================================
// Content     (ゲームの設定)(player.cpp)
// Author     : 有田明玄
//=================================================

//インクルード
#include"Player.h"
#include"Application.h"
#include"renderer.h"
#include"Camera.h"
#include"input.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Model.h"
#include"Game.h"
#include"Collision.h"
#include"sound.h"
#include"Bullet.h"
#include"Time.h"
#include"effect.h"
#include"Particle.h"

//静的メンバ変数
int CPlayer::m_nNumPlayer = 0;	//プレイヤーの数
//===========================
//コンストラクタ
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	m_nPlayerNumber = m_nNumPlayer;
	m_nNumPlayer++;
}

//===========================
//デストラクタ
//===========================
CPlayer::~CPlayer()
{
	m_nNumPlayer--;
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
				m_apMotion[i].aKey[j].aKey[k].fPos = { 0.0f,0.0f,0.0f };

				m_apMotion[i].aKey[j].aKey[k].fRot = { 0.0f,0.0f,0.0f };
			}
			m_apMotion[i].aKey[j].nNumCollision = 0;
			m_apMotion[i].aKey[j].nFrame = 1;
		}
	}

	m_AxisBox = CCollision::Create(m_pos, CCollision::COLLI_AXIS);						//押し出し判定(プレイヤーの軸)
	m_pShadow = CShadow::Create(m_pos, D3DXVECTOR3(80.0f, 0.0f, 80.0f));

	//モデルとモーションの読み込み
	ReadMotion();

	for (int i = 0; i < NUM_PARTS; i++)
	{
		//プレイヤーの生成
		m_apModel[i]->SetPos(m_apMotion[0].aKey[0].aKey[i].fPos + m_apModel[i]->GetDPos());	//初期位置の設定
		m_apModel[i]->SetRot(m_apMotion[0].aKey[0].aKey[i].fRot + m_apModel[i]->GetDRot());	//差分の取得

		if (m_nPlayerNumber == 0)
		{
			m_apModel[i]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_apModel[i]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	//デバッグ用設定後でファイル入出力できるようにする
	m_nJump = 50;			//ジャンプの全体フレーム
	m_nLife = 1000;			//体力
	m_nJumpTransTime = 4;	//ジャンプ移行

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
	if (m_pBullet!=nullptr)
	{
		m_pBullet = nullptr;
	}

	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}	

	CObject::Release();
}

//===========================
//更新処理
//===========================
void CPlayer::Update(void)
{
	if (CGame::GetGame()!=CGame::GAME_END&&CGame::GetGame()!=CGame::GAME_START)
	{
		
		Input();					//入力処理
			
		if ((m_nHitStop <= 0 && m_pEne->m_nHitStop <= 0) && m_nRig <= 0)
		{//ヒットストップがない場合
			Down();					//ダウン＆起き上がり
			Updatepos();			//座標更新
			//カメラ端
			if (m_pos.x - m_pEne->GetPos().x > FIELD_WIDTH || m_pos.x - m_pEne->GetPos().x < -FIELD_WIDTH)
			{//一定以上離れると1フレーム前の位置に移動
				m_pos.x = m_posold.x;
			}

			if (m_nLife > 0 && m_Motion != PM_DOWN&&m_Motion != PM_STANDUP)
			{//体力が残っててダウンしていない場合
				if (m_bMotion == false && m_bJump == false)
				{
					m_Motion = PM_ST_NEUTRAL;
				}

				ControlPlayer();		//操作
				Axis();					//軸の押し出し判定
				Jump();					//ジャンプ
				AutoTurn();				//自動振り向き

				if (m_NextMotion != PM_ST_NEUTRAL)
				{
					m_Motion = m_NextMotion;
					m_NextMotion = PM_ST_NEUTRAL;	//PM_ST_NEUTRALは使っていない状態
				}
				FireBall();
			}
			if (m_State == PST_AIR)
			{//重力
				m_move.y -= 0.25f;				//少しずつ減速
			}
			StageEdge();		//ステージの端の処理
		}
		else
		{//ヒットストップもしくは硬直中の場合
			m_nHitStop--;
			Cancel();				//攻撃キャンセル
			m_nRig--;
		}

		DrawCollision();		//当たり判定表示
		Damage();				//ダメージ処理
		Die();					//死亡処理
		MotionManager();		//モーション再生
		Normalization();		//角度の正規化
		m_pShadow->SetPos({ m_pos.x, 1.0f, m_pos.z });

#ifdef _DEBUG
		CDebugProc::Print("現在のプレイヤーの座標:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
		CDebugProc::Print("現在のモーション:%d ", (int)m_Motion);
		CDebugProc::Print("現在の状態:%d ", (int)m_State);
		CDebugProc::Print("現在のフレーム:%d", m_frame);

#endif // _DEBUG
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
	if (m_Motion != PM_CR_HURT && m_Motion != PM_ST_HURT && m_Motion != PM_JP_HURT && m_Motion != PM_DOWN && m_Motion != PM_STANDUP)
	{//被弾状態じゃない場合
		//移動
		if (m_bAttack == false&&m_bMotion==false)
		{
			Command();
		}
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
//モーション読み込み
//===========================
void CPlayer::ReadMotion()
{
	const int lenLine = 2048;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	char Read[lenLine];			//読み取る用
	int	modelnumber = 0;		//モデルの番号
	int motionnumber = 0;		//モーションの番号
	int key = 0;
	int partsmotion = 0;
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

						m_apModel[modelnumber]->SetModel(&m_nModelpass[0]);
						modelnumber++;
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
												else if (strcmp(&strLine[0], "HITSTOP") == 0)
												{//ヒットストップの時間設定
													sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nHitStopTimer);
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
															partsmotion = 0;	//番号リセット
															break;
														}
														else if (strcmp(&strLine[0], "FRAME") == 0)
														{//キーの再生時間の設定
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aKey[key].nFrame);	//再生時間の設定
														}
														else if (strcmp(&strLine[0], "COLLISIONSET") == 0)
														{
															m_apMotion[motionnumber].aKey[key].Collision[m_apMotion[motionnumber].aKey[key].nNumCollision]
																= CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット
																sscanf(Read, "%s", &strLine);					//文字列の分析

																//keyはモデルのキーの番号
																if (strcmp(&strLine[0], "END_COLLISIONSET") == 0)
																{
																	m_apMotion[motionnumber].aKey[key].nNumCollision++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//判定の開始時間
																	int start, select;
																	select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &start);	//ループするかどうか
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//判定の開始時間
																	int end;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &end);	//ループするかどうか
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	D3DXVECTOR3 dpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &dpos.x, &dpos.y, &dpos.z);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetDPos(dpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  dsiz;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %f%f%f", &strLine, &dsiz.x, &dsiz.y, &dsiz.z);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetSiz(dsiz);
																}
																else if ((strcmp(&strLine[0], "DAMAGE_POINT") == 0))
																{//上段の場合は書かなくていい
																	int dp;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &dp);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetDP((CCollision::EDAMAGE_POINT)dp);
																}
																else if ((strcmp(&strLine[0], "HIT") == 0))
																{//ヒット硬直の設定
																	int hit;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &hit);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetHitRig(hit);
																}
																else if ((strcmp(&strLine[0], "GUARD") == 0))
																{//ガード硬直の設定
																	int Guard;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &Guard);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetGuardRig(Guard);
																}
															}
														}
														else if (strcmp(&strLine[0], "HURTSET") == 0)
														{
															m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]
																= CCollision::Create(m_pos, CCollision::COLLI_HURT);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																//文字列の分析
																sscanf(Read, "%s", &strLine);

																//keyはモデルのキーの番号
																if (strcmp(&strLine[0], "END_HURTSET") == 0)
																{
																	m_apMotion[motionnumber].aKey[key].nNumHurtCol++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//判定の開始時間
																	int start;
																	sscanf(Read, "%s = %d", &strLine, &start);	//ループするかどうか
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//判定の開始時間
																	int end;
																	sscanf(Read, "%s = %d", &strLine, &end);	//ループするかどうか
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	D3DXVECTOR3 hpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &hpos.x, &hpos.y, &hpos.z);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetDPos(hpos);
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetPos(hpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  hsiz;
																	int select = m_apMotion[motionnumber].aKey[key].nNumHurtCol;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %f%f%f", &strLine, &hsiz.x, &hsiz.y, &hsiz.z);	//キーの総数
																	m_apMotion[motionnumber].aKey[key].HurtCol[select]->SetSiz(hsiz);
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
																	partsmotion++;
																	break;
																}
																else if (strcmp(&strLine[0], "POS") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fPos.x,
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fPos.y,
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fPos.z);	//再生時間の設定
																}
																else if (strcmp(&strLine[0], "ROT") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fRot.x,
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fRot.y,
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fRot.z);
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
void CPlayer::MotionPlayer()
{
	D3DXVECTOR3 RelaPos, RelaRot;		//1フレームごとの移動量
	D3DXVECTOR3 pos, rot, DiffPos, DiffRot;

	//カウンター更新
	if (m_nCurKey == m_apMotion[m_Motion].nNumKey&& m_apMotion[m_Motion].bLoop == false)
	{
		//ループモーションが終わったらニュートラルにする
		m_bMotion = false;
		m_bAttack = false;
		switch (m_State)
		{
		case CPlayer::PST_STAND:
			m_Motion = PM_ST_NEUTRAL;
			break;
		case CPlayer::PST_CROUCH:
			m_Motion = PM_CR_NEUTRAL;
			break;
		case CPlayer::PST_AIR:
			m_Motion = PM_JP_NEUTRAL;
			break;
		case CPlayer::PST_DIE:
			m_Motion = PM_DOWN;
			break;
		}
		PlayFirstMotion();
		return;
	}
	else
	{
		for (int i = 0; i < NUM_PARTS; i++)
		{//パーツ全部のモーション再生
			if (m_apModel[i] != nullptr)
			{
				if (m_nCurKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == true)
				{//ループする場合最初のモーションに移行する
					DiffPos = D3DXVECTOR3(
						m_apMotion[m_Motion].aKey[0].aKey[i].fPos -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);	//差分の取得

					DiffRot = D3DXVECTOR3(
						m_apMotion[m_Motion].aKey[0].aKey[i].fRot -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);	//差分の取得
				}
				else if (m_nCurKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == false)
				{//ループしない場合ニュートラルモーションに戻す
					if (m_Motion == PM_ST_DIE || m_Motion == PM_CR_DIE || m_Motion == PM_JP_DIE)
					{//死亡モーションの場合そのまま死亡設定にする
						m_State = PST_DIE;
					}

					switch (m_State)
					{//現在の状態に戻す
					case CPlayer::PST_STAND:
						DiffPos = D3DXVECTOR3(//座標差分の取得
							m_apMotion[0].aKey[0].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//向き差分の取得
							m_apMotion[0].aKey[0].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					case CPlayer::PST_CROUCH:
						DiffPos = D3DXVECTOR3(//座標差分の取得
							m_apMotion[PM_CR_NEUTRAL].aKey[0].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//向き差分の取得
							m_apMotion[PM_CR_NEUTRAL].aKey[0].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					case CPlayer::PST_AIR:
						DiffPos = D3DXVECTOR3(//座標差分の取得
							m_apMotion[0].aKey[0].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//向き差分の取得
							m_apMotion[0].aKey[0].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					case CPlayer::PST_DIE:
						DiffPos = D3DXVECTOR3(//座標差分の取得
							m_apMotion[PM_DOWN].aKey[1].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//向き差分の取得
							m_apMotion[PM_DOWN].aKey[1].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					default:
						break;
					}
				}
				else
				{
					if (m_nCurKey == 0 && m_frame == 0)
					{//位置座標の設定しなおし
						PlayFirstMotion();
					}
					DiffPos = D3DXVECTOR3(//座標差分の取得
						m_apMotion[m_Motion].aKey[m_nCurKey + 1].aKey[i].fPos -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

					DiffRot = D3DXVECTOR3(//向き差分の取得
						m_apMotion[m_Motion].aKey[m_nCurKey + 1].aKey[i].fRot -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
				}
			}

			//位置
			RelaPos = (DiffPos / (float)m_apMotion[m_Motion].aKey[m_nCurKey].nFrame);		//相対値
			RelaRot = (DiffRot / (float)m_apMotion[m_Motion].aKey[m_nCurKey].nFrame);

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
	if (m_frame >= m_apMotion[m_Motion].aKey[m_nCurKey].nFrame)
	{//キー番号の更新とカウンターのリセット
		m_nCurKey++;
		m_frame = 0;
		if (m_nCurKey >= m_apMotion[m_Motion].nNumKey)
		{//キー番号が最大数を超えた場合リセット
			if (m_apMotion[m_Motion].bLoop == true)
			{
				m_nCurKey = 0;
			}
		}
		for (int j = 0; j < m_apMotion[m_Motion].aKey[m_nCurKey].nNumCollision; j++)
		{//ダメージフラグ復活
			m_apMotion[m_Motion].aKey[m_nCurKey].Collision[j]->SetDmg(true);
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
		MotionPlayer();		//プレイヤーのモーション
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
			m_apMotion[m_Motion].aKey[0].aKey[i].fPos) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_Motion].aKey[0].aKey[i].fRot) + m_apModel[i]->GetDRot());	//差分の取得
	}
	m_frame = 0;
	m_nCurKey = 0;
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
		{//プレイヤー1
			//ダメージ判定
			for (int j = 0; j < m_apMotion[i].aKey[k].nNumCollision; j++)
			{//違うモーションの当たり判定をオフにする

				 //攻撃判定再設定
				if (m_bSide == false)
				{//敵より右側の場合
					//プレイヤー1
					m_apMotion[i].aKey[k].Collision[j]->SetPos(m_pos + m_apMotion[i].aKey[k].Collision[j]->GetDPos());
				}
				else
				{//右向き

					//プレイヤー1
					m_apMotion[i].aKey[k].Collision[j]->SetPos(D3DXVECTOR3(
						m_pos.x - m_apMotion[i].aKey[k].Collision[j]->GetDPos().x,				//X
						m_pos.y + m_apMotion[i].aKey[k].Collision[j]->GetDPos().y,
						m_pos.z - m_apMotion[i].aKey[k].Collision[j]->GetDPos().z));
				}

				if (m_Motion == i&&m_nCurKey == k&&m_frame >= m_apMotion[i].aKey[k].Collision[j]->GetStartf() && m_frame <= m_apMotion[i].aKey[k].Collision[j]->GetEndf())
				{//キーとモーションが一致している場合のみ表示
					if (m_apMotion[i].aKey[k].Collision[j] != nullptr)
					{
						m_apMotion[i].aKey[k].Collision[j]->SetUse(true);
					}
				}
				else
				{
					if (m_apMotion[i].aKey[k].Collision[j] != nullptr)
					{
						m_apMotion[i].aKey[k].Collision[j]->SetUse(false);
					}
				}
			}

			//やられ判定
			for (int j = 0; j < m_apMotion[i].aKey[k].nNumHurtCol; j++)
			{
				if (m_bSide == true)
				{//右向き(1P)
					m_apMotion[i].aKey[k].HurtCol[j]->SetPos(D3DXVECTOR3(
						m_pos.x - m_apMotion[i].aKey[k].HurtCol[j]->GetDPos().x,
						m_pos.y + m_apMotion[i].aKey[k].HurtCol[j]->GetDPos().y,
						m_pos.z + m_apMotion[i].aKey[k].HurtCol[j]->GetDPos().z));
				}
				else
				{//左向き(2P)
					m_apMotion[i].aKey[k].HurtCol[j]->SetPos(m_apMotion[i].aKey[k].HurtCol[j]->GetDPos() + m_pos);
				}

				if (m_Motion == i&&m_nCurKey == k&&m_apMotion[i].aKey[k].HurtCol[j] != nullptr&&
					m_frame >= m_apMotion[i].aKey[k].HurtCol[j]->GetStartf() && m_frame <= m_apMotion[i].aKey[k].HurtCol[j]->GetEndf())
				{
					m_apMotion[i].aKey[k].HurtCol[j]->SetUse(true);
				}
				else
				{
					if (m_apMotion[i].aKey[k].HurtCol[j] != nullptr)
					{
						m_apMotion[i].aKey[k].HurtCol[j]->SetUse(false);
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
	if (m_pos.y - m_AxisBox->GetWidth().y / 2 <= m_pEne->m_pos.y + m_pEne->m_AxisBox->GetWidth().y 
		&& m_pos.y + m_AxisBox->GetWidth().y / 2 >= m_pEne->m_pos.y - m_pEne->m_AxisBox->GetWidth().y )
	{
			//X軸
			//左側
			if (m_posold.x + m_AxisBox->GetWidth().x / 2 <= m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x / 2	//前回のプレイヤーの位置がブロックの位置より右にいる場合
				&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x / 2 	//現在のプレイヤーの位置がブロックの位置より左にいる(めり込んでいる)場合
				)//プレイヤーの左右にブロックが当たった場合
			{//プレイヤ-とブロックが当たった時
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
			}
			//右側
			if (m_posold.x - m_AxisBox->GetWidth().x / 2 >= m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x / 2 	//前回のプレイヤーの位置がブロックの位置より右にいる場合
				&& m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x / 2	//現在のプレイヤーの位置がブロックの位置より左にいる(めり込んでいる)場合
				)//プレイヤーの左右にブロックが当たった場合
			{//プレイヤ-とブロックが当たった時
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
			}
	}
	if (m_posold.y + m_pEne->m_AxisBox->GetWidth().y >= m_pEne->m_pos.y + m_pEne->m_AxisBox->GetWidth().y
		&& m_pos.y+ m_pEne->m_AxisBox->GetWidth().y<= m_pEne->m_pos.y + m_pEne->m_AxisBox->GetWidth().y )
	{//ジャンプした時の押し合いの判定
		if (m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x / 2
			&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x / 2)
		{
			if (m_pos.x >= m_pEne->m_pos.x)
			{//中心より→
				m_pos.x = m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x+1;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = -m_move.x;
			}
			else
			{//中心より←
				m_pos.x = m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x-1;
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
		if (m_MotionOld == PM_JP_HURT)
		{
			m_Motion = PM_JP_HURT;
		}
		else
		{
			if (m_Motion == PM_JP_MOVEFORWARD)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_nJumpCount == 0)
				{
					m_move.x = sinf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
					m_move.z = cosf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
				}
				else if (m_nJumpCount == 0)
				{
					m_move.x = -sinf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
					m_move.z = -cosf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
				}
			}
			else if (m_Motion == PM_JP_MOVEBACK)
			{
				if (m_pos.x < m_pEne->m_pos.x&&m_nJumpCount == 0)
				{
					m_move.x = -sinf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
					m_move.z = -cosf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
				}
				else if (m_nJumpCount == 0)
				{
					m_move.x = sinf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
					m_move.z = cosf(D3DX_PI*0.5f)*PLAYER_SPEED*JUMP_FACTOR_X;
				}
			}
			if (m_nJumpCount == 0)
			{
				m_move.y = INITIAL_VELOCITY;	//初速を設定
			}
		}
		
		m_nJumpCount++;
		if (m_pos.y < 0.0f)
		{//着地時の設定
			m_nJumpCount = 0;				//ジャンプカウント初期化
			m_State = PST_STAND;			//立ち状態に変更
			m_pos.y = 0.0f;					//位置設定
			m_move.y = 0.0f;				//移動量を0に戻す
			m_move.x = 0.0f;				//移動量を0に戻す
			m_bJump = false;				//ジャンプ状態解除
			m_bAttack = false;				//攻撃状態を解除
			m_bMotion = false;
			if (m_Motion == PM_JP_HURT)
			{
				m_Motion = PM_DOWN;
			}
			else
			{
				m_Motion = PM_ST_NEUTRAL;		//ニュートラルモーションに戻す
			}
		}
	}
}

//=====================
//自動振り向き
//=====================
void CPlayer::AutoTurn(void)
{
	if (m_State == PST_STAND)
	{//地上時のみ相手の方向位振りむく
		if (m_pos.x >= m_pEne->m_pos.x)
		{
			m_bSide = false;
			m_rot.y = D3DX_PI*0.5f;
		}
		else
		{
			m_bSide = true;
			m_rot.y = -D3DX_PI*0.5f;
		}
	}
}

//=====================
//ダメージ処理
//=====================
void CPlayer::Damage()
{
	for (int i = 0; i < m_apMotion[m_Motion].aKey[m_nCurKey].nNumHurtCol; i++)
	{
		for (int j = 0; j < m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].nNumCollision; j++)
		{
			if (m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->GetUseDmg() == true)	//ダメージ判定残ってるかどうか
			{
				if (ColJudge(i, j)==true)	//矩形の判定
				{
					//ダメージ処理
					Damage_Cal(m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->GetDamage(),
						m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->GetProperty(),
						m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->GetHitRig(),
						m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->GetGuardRig(),
						true);

					m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->SetDmg(false);
					m_nHitStop = m_pEne->m_apMotion[m_pEne->m_Motion].nHitStopTimer;
					m_bAttack = false;
					m_bMotion = false;
				}
			}
		}
	}
}

//=====================
//ガード処理
//=====================
bool CPlayer::Guard(CCollision::EDAMAGE_POINT dp)
{
	switch (m_Motion)
	{
	case PM_ST_GUARD:	//立ちガード
		if (dp == CCollision::DP_LOW)
		{

			return false;	//ダメージが通る
		}
		else
		{//移動量の設定
			CParticle::Create(m_pos + D3DXVECTOR3{ -20.0f,30.0f,0.0f },			//位置の設定
				D3DXVECTOR3{ GUARD_EFFECTSIZ,GUARD_EFFECTSIZ ,GUARD_EFFECTSIZ },		//半径の大きさの設定
				D3DXCOLOR(0.05f, 0.05f, 0.45f, 1.0f),							//頂点カラーの設定	
				CParticle::PAR_FIREFLOWER);
			return true;	//ガード成功
		}

	case PM_CR_GUARD:
		if (dp == CCollision::DP_MIDDLE)
		{
			return false;	//ダメージが通る
		}
		else
		{
			CParticle::Create(m_pos + D3DXVECTOR3{ -20.0f,20.0f,0.0f },			//位置の設定
				D3DXVECTOR3{ GUARD_EFFECTSIZ,GUARD_EFFECTSIZ ,GUARD_EFFECTSIZ },		//半径の大きさの設定
				D3DXCOLOR(0.05f, 0.05f, 0.45f, 1.0f),							//頂点カラーの設定	
				CParticle::PAR_FIREFLOWER);
			return true;	//ガード成功
		}

	case PM_JP_GUARD:
		return true;	//ガード成功

	default:

		//移動量の設定
		CParticle::Create(m_pos + D3DXVECTOR3{ 0.0f,30.0f,0.0f },							//位置の設定
			D3DXVECTOR3{ HIT_EFFECTSIZ,HIT_EFFECTSIZ ,HIT_EFFECTSIZ },				//半径の大きさの設定
			D3DXCOLOR(0.35f, 0.16f, 0.0f, 1.0f),				//頂点カラーの設定	
			CParticle::PAR_FIREFLOWER);
		return false;	//ダメージが通る
	}
}

//=====================
//操作処理
//=====================
void CPlayer::Command()
{
	if (m_pEne->m_nLife >= 0)
	{
		//移動
		if (m_State != PST_AIR)
		{
			//ニュートラル
			if ((m_anInput[0] & INPUT5) == INPUT5)
			{
				m_Motion = PM_ST_NEUTRAL;
				m_move.x = 0.0f;
			}

			//左
			if ((m_anInput[0] & INPUT4) == INPUT4)
			{
				switch (m_State)
				{
				case CPlayer::PST_STAND:
					m_move.x = sinf(D3DX_PI*-0.5f)*PLAYER_SPEED;
					m_move.z = cosf(D3DX_PI*-0.5f)*PLAYER_SPEED;
					m_Motion = PM_ST_MOVE;

					if (m_pos.x <= m_pEne->m_pos.x)
					{
						if (m_pEne->m_bAttack == true)
						{
							m_Motion = PM_ST_GUARD;
							m_move = { 0.0f,0.0f,0.0f };
						}
						else if (m_pEne->m_bBullet == true)
						{//弾の防御処理
							if (m_pEne->m_pBullet->GetPos().x - m_pos.x <= 100.0f&&m_pEne->m_pBullet->GetPos().x - m_pos.x >= -100.0f)
							{
								m_Motion = PM_ST_GUARD;
								m_move = { 0.0f,0.0f,0.0f };
							}
						}
					}
					break;
				}
			}

			//右
			if ((m_anInput[0] & INPUT6) == INPUT6)
			{
				switch (m_State)
				{
				case PST_STAND:
					m_move.x = sinf(D3DX_PI*0.5f)*PLAYER_SPEED;
					m_move.z = cosf(D3DX_PI*0.5f)*PLAYER_SPEED;
					m_Motion = PM_ST_MOVE;
					if (m_pos.x >= m_pEne->m_pos.x)
					{
						if (m_pEne->m_bAttack == true)
						{
							m_Motion = PM_ST_GUARD;
							m_move = { 0.0f,0.0f,0.0f };
						}
						else if (m_pEne->m_bBullet == true)
						{//弾の防御処理
							if (m_pEne->m_pBullet->GetPos().x - m_pos.x <= 100.0f&&m_pEne->m_pBullet->GetPos().x - m_pos.x >= -100.0f)
							{
								m_Motion = PM_ST_GUARD;
								m_move = { 0.0f,0.0f,0.0f };
							}
						}
					}
					break;
				}
			}

			//右右
			if (CheckInput(CMD66) == true && m_State == PST_STAND)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_Motion != PM_ST_DASH)
				{//右向きの場合
					//前ダッシュ
					m_Motion = PM_ST_DASH;
				}
				else
				{//左向き(2P側)の場合
					//バックステップ
					m_Motion = PM_ST_BACKSTEP;
					m_State = PST_AIR;
					m_move = { BACKSTEP_MOVE_X,BACKSTEP_MOVE_Y,0.0f };
					m_bJump = true;
					m_nJumpCount = 1;
					PlaySound(SOUND_LABEL_SE_BACKSTEP);
				}
			}

			if (CheckInput(CMD44) == true && m_State == PST_STAND)
			{
				if (m_pos.x > m_pEne->m_pos.x&&m_Motion != PM_ST_DASH)
				{//左向き(2P側)の場合
				 //前ダッシュ
					m_Motion = PM_ST_DASH;
				}
				else
				{//右向きの場合
				 //バックステップ
					m_Motion = PM_ST_BACKSTEP;
					m_move = { -BACKSTEP_MOVE_X,BACKSTEP_MOVE_Y,0.0f };
					m_State = PST_AIR;
					m_nJumpCount = 1;
					m_bJump = true;
					PlaySound(SOUND_LABEL_SE_BACKSTEP);
				}
			}

			//左右同時押しでニュートラル
			if ((m_anInput[0] & INPUT6) == INPUT6 && (m_anInput[0] & INPUT4) == INPUT4)
			{
				m_Motion = PM_ST_NEUTRAL;

				m_move.x = 0.0f;
				m_move.z = 0.0f;
			}

			//下(しゃがみ)
			if ((m_anInput[0] & INPUT2) == INPUT2)
			{
				if (m_State != PST_AIR)
				{
					m_Motion = PM_CR_NEUTRAL;
					m_State = PST_CROUCH;
					m_move = { 0.0f,0.0f,0.0f };

					if ((m_pos.x >= m_pEne->m_pos.x && (m_anInput[0] & INPUT6) == INPUT6) || (m_pos.x < m_pEne->m_pos.x && (m_anInput[0] & INPUT4) == INPUT4))
					{
						if (m_pEne->m_bAttack == true)
						{
							m_Motion = PM_CR_GUARD;
							m_move = { 0.0f,0.0f,0.0f };
						}
						else if (m_pEne->m_bBullet == true)
						{//弾の防御処理
							if (m_pEne->m_pBullet->GetPos().x - m_pos.x <= 100.0f&&m_pEne->m_pBullet->GetPos().x - m_pos.x >= -100.0f)
							{
								m_Motion = PM_CR_GUARD;
								m_move = { 0.0f,0.0f,0.0f };
							}
						}
					}
				}
			}

			//上(ジャンプ)
			if ((m_anInput[0] & INPUT8) == INPUT8)
			{
				m_Motion = PM_JP_NEUTRAL;
				m_State = PST_AIR;
				m_bJump = true;

			}

			//右上
			if ((m_anInput[0] & INPUT9) == INPUT9)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{//1P側
					m_Motion = PM_JP_MOVEFORWARD;
				}
				else
				{//2P側
					m_Motion = PM_JP_MOVEBACK;
				}
			}

			//左上
			if ((m_anInput[0] & INPUT7) == INPUT7)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{//1P側
					m_Motion = PM_JP_MOVEBACK;
				}
				else
				{//2P側
					m_Motion = PM_JP_MOVEFORWARD;
				}
			}
		}

		//攻撃
		if (m_Motion != PM_ST_BACKSTEP)
		{
			//弱攻撃
			if ((m_anInput[0] & INPUT_LATK) == INPUT_LATK && (m_anInput[1] & INPUT_NOT_LATK) == INPUT_NOT_LATK)
			{
				switch (m_State)
				{

				case CPlayer::PST_STAND:
					m_Motion = PM_ST_LATTACK;
					break;

				case CPlayer::PST_AIR:
					m_Motion = PM_JP_LATTACK;
					break;

				case CPlayer::PST_CROUCH:
					m_Motion = PM_CR_LATTACK;
					break;

				}
				m_bAttack = true;
				m_bMotion = true;
			}

			//中攻撃
			if ((m_anInput[0] & INPUT_MATK) == INPUT_MATK && (m_anInput[1] & INPUT_NOT_MATK) == INPUT_NOT_MATK)
			{
				switch (m_State)
				{
				case CPlayer::PST_STAND:
					m_Motion = PM_ST_MATTACK;
					break;

				case CPlayer::PST_AIR:
					m_Motion = PM_JP_MATTACK;
					break;

				case CPlayer::PST_CROUCH:
					m_Motion = PM_CR_MATTACK;
					break;

				}
				m_bAttack = true;
				m_bMotion = true;
			}

			//強攻撃
			if ((m_anInput[0] & INPUT_HATK) == INPUT_HATK && (m_anInput[1] & INPUT_NOT_HATK) == INPUT_NOT_HATK)
			{
				switch (m_State)
				{
				case CPlayer::PST_STAND:
					m_Motion = PM_ST_HATTACK;
					break;

				case CPlayer::PST_AIR:
					m_Motion = PM_JP_HATTACK;
					break;

				case CPlayer::PST_CROUCH:
					m_Motion = PM_CR_HATTACK;
					break;

				}
				m_bAttack = true;
				m_bMotion = true;
			}

			//波動拳・竜巻
			//強
			if (CheckInput(CMD236H) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_Motion = PM_236H;
				}
				else if (m_pos.x > m_pEne->m_pos.x)
				{
				}
			}

			//中
			if (CheckInput(CMD236M) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_Motion = PM_236M;
				}
				else if (m_pos.x > m_pEne->m_pos.x)
				{

				}
			}

			//弱
			if (CheckInput(CMD236L) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_Motion = PM_236L;
				}
				else if (m_pos.x > m_pEne->m_pos.x)
				{

				}
			}

			//2P側
			//強
			if (CheckInput(CMD214H) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_pos.x > m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_Motion = PM_236H;
				}
			}

			//中
			if (CheckInput(CMD214M) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_pos.x > m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_Motion = PM_236M;
				}
			}

			//弱
			if (CheckInput(CMD214L) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_pos.x > m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_Motion = PM_236L;
				}
			}
		}
		StateManagement();	//状態管理
	}
}

//==============================================
//コマンドの入力判定
//==============================================
bool CPlayer::CheckInput(const int * command)
{
	//コマンド
	int i = 0, j = 0;
	//コマンドの最後を調べる
	while (!(command[i] & INPUT_END))i++;

	//コマンドの入力時間を取得
	int time = command[i] & ~INPUT_END;

	//入力時間を調べて正しい場合はtrueそれ以外はfalse
	for (i--; i >= 0; i--)
	{
		int input = command[i];

		//履歴からコマンドの要素を探す
		while (j < MAX_KEYMEMORY && ((m_anInput[j] & input) != input)) j++;

		//入力時間を超えたり、履歴の末尾に達したら、falseを返す
		if (j >= time || j == MAX_KEYMEMORY)return false;
	}

	//コマンドの要素が見つかったらtrue
	return true;
}

//==============================================
//プレイヤーの状態管理
//==============================================
void CPlayer::StateManagement()
{
	//移動していない場合の処理
	if (m_Motion == PM_ST_NEUTRAL || m_Motion == PM_CR_NEUTRAL || m_bAttack == true)
	{
		if (m_bJump == false)
		{
			m_move.x = 0;
			m_move.z = 0;
		}
	}

	//下押していない&地上時&攻撃していない場合
	if ((m_anInput[0] & INPUT_NOT2) == INPUT_NOT2&&m_State != PST_AIR&&m_bAttack == false)
	{//下押していない場合立ち状態へ
		m_State = PST_STAND;
	}

	//ダッシュ処理
	if (m_MotionOld == PM_ST_DASH&&m_bAttack == false&& m_State == PST_STAND)
	{//攻撃していない場合、地上時のみダッシュ維持
		if (m_pos.x <= m_pEne->m_pos.x && (m_anInput[0] & INPUT6) == INPUT6 && (m_anInput[0] & INPUT_NOT4) == INPUT_NOT4)
		{//前押しっぱなしでかつ後ろ入力なし
			m_Motion = PM_ST_DASH;
			m_move.x = DASH_SPEED;
		}
		else if (m_pos.x > m_pEne->m_pos.x && (m_anInput[0] & INPUT4) == INPUT4 && (m_anInput[0] & INPUT_NOT6) == INPUT_NOT6)
		{
			m_Motion = PM_ST_DASH;
			m_move.x = -DASH_SPEED;
		}
	}
}

//==============================================
//入力設定
//==============================================
void CPlayer::Input()
{
	CInput* pInput = CApplication::GetInput();
	int Key = 0;
	pInput->PressDevice(KEY_DOWN_RIGHT);
	//レバー
	{
		//左下
		Key |= (pInput->Press(DIK_A) && pInput->Press(DIK_S)) || pInput->Press(JOYPAD_DOWN_LEFT, m_nPlayerNumber) ? INPUT1 : INPUT_NOT1;
		//下
		Key |= pInput->Press(DIK_S) || pInput->Press(JOYPAD_DOWN, m_nPlayerNumber)|| pInput->Press(JOYPAD_DOWN_LEFT, m_nPlayerNumber) || pInput->Press(JOYPAD_DOWN_RIGHT, m_nPlayerNumber) ? INPUT2 : INPUT_NOT2;
		//右下
		Key |= (pInput->Press(DIK_D) && pInput->Press(DIK_S)) || pInput->Press(JOYPAD_DOWN_RIGHT, m_nPlayerNumber) ? INPUT3 : INPUT_NOT3;
		//左
		Key |= pInput->Press(DIK_A) || pInput->Press(JOYPAD_LEFT, m_nPlayerNumber) || pInput->Press(JOYPAD_DOWN_LEFT, m_nPlayerNumber) || (pInput->Press(JOYPAD_UP_LEFT, m_nPlayerNumber)) ? INPUT4 : INPUT_NOT4;
		//右
		Key |= pInput->Press(DIK_D) || pInput->Press(JOYPAD_RIGHT, m_nPlayerNumber) || (pInput->Press(JOYPAD_UP_RIGHT, m_nPlayerNumber)) || pInput->Press(JOYPAD_DOWN_RIGHT, m_nPlayerNumber) ? INPUT6 : INPUT_NOT6;
		//左上
		Key |= (pInput->Press(DIK_A) && pInput->Press(DIK_SPACE)) || (pInput->Press(JOYPAD_UP_LEFT, m_nPlayerNumber)) ? INPUT7 : INPUT_NOT7;
		//上
		Key |= pInput->Press(DIK_SPACE) || pInput->Press(JOYPAD_UP, m_nPlayerNumber) || (pInput->Press(JOYPAD_UP_RIGHT, m_nPlayerNumber)) || (pInput->Press(JOYPAD_UP_LEFT, m_nPlayerNumber)) ? INPUT8 : INPUT_NOT8;
		//右上
		Key |= (pInput->Press(DIK_D) && pInput->Press(DIK_SPACE)) || (pInput->Press(JOYPAD_UP_RIGHT, m_nPlayerNumber)) ? INPUT9 : INPUT_NOT9;

		//ニュートラル
		Key |= (m_anInput[0] & INPUT_NOT6) == INPUT_NOT6 && (m_anInput[0] & INPUT_NOT2) == INPUT_NOT2 && (m_anInput[0] & INPUT_NOT4) == INPUT_NOT4 && (m_anInput[0] & INPUT_NOT8) == INPUT_NOT8 ? INPUT_NOT5 : INPUT5;

		//攻撃
		//弱
		Key |= pInput->Press(DIK_U) || pInput->Press(JOYPAD_A, m_nPlayerNumber) ? INPUT_LATK : INPUT_NOT_LATK;
		//中
		Key |= pInput->Press(DIK_I) || pInput->Press(JOYPAD_B, m_nPlayerNumber) ? INPUT_MATK : INPUT_NOT_MATK;
		//強
		Key |= pInput->Press(DIK_O) || pInput->Press(JOYPAD_R1, m_nPlayerNumber) ? INPUT_HATK : INPUT_NOT_HATK;
	}

	//コマンド入れ替え処理
	for (int i = MAX_KEYMEMORY - 1; i > 0; i--)
	{
		m_anInput[i] = m_anInput[i - 1];
	}
	m_anInput[0] = Key;

}

//==============================================
//座標の更新
//==============================================
void CPlayer::Updatepos()
{
	m_posold = m_pos;		//前回の位置の保存
	m_pos += m_move;		//位置の更新
	m_AxisBox->SetPos(m_pos + D3DXVECTOR3(0.0f, m_AxisBox->GetDPos().y, 0.0f));	//軸の判定の位置更新
}

//==============================================
//キャンセル
//==============================================
void CPlayer::Cancel()
{
	if (m_bAttack == true)
	{//攻撃キャンセル
		switch (m_Motion)
		{
		case CPlayer::PM_ST_LATTACK:
			if ((m_anInput[0] & INPUT_MATK) == INPUT_MATK)
			{
				m_NextMotion = PM_ST_MATTACK;

				if ((m_anInput[0] & INPUT2) == INPUT2)
				{
					m_NextMotion = PM_CR_MATTACK;
				}
			}
			break;

		case CPlayer::PM_ST_MATTACK:
			if ((m_anInput[0] & INPUT_HATK) == INPUT_HATK)
			{
				m_NextMotion = PM_ST_HATTACK;
				if ((m_anInput[0] & INPUT2) == INPUT2)
				{
					m_NextMotion = PM_CR_HATTACK;
				}
			}
			break;

		case CPlayer::PM_ST_HATTACK:
			//波動拳・竜巻
			//強
			if (CheckInput(CMD236H) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_NextMotion = PM_236H;
				}
			}

			//中
			if (CheckInput(CMD236M) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{
					m_NextMotion = PM_236M;
				}
			}

			//弱
			if (CheckInput(CMD236L) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{
					m_NextMotion = PM_236L;
				}
			}

			//2P側
			//強
			if (CheckInput(CMD214H) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_bBullet == false)
				{
					m_NextMotion = PM_236H;
				}
			}

			//中
			if (CheckInput(CMD214M) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_bBullet == false)
				{
					m_NextMotion = PM_236M;
				}
			}

			//弱
			if (CheckInput(CMD214L) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_bBullet == false)
				{
					m_Motion = PM_236L;
				}
			}
			break;

		case CPlayer::PM_JP_LATTACK:
			break;
		case CPlayer::PM_JP_MATTACK:
			break;
		case CPlayer::PM_JP_HATTACK:
			break;

		case CPlayer::PM_CR_LATTACK:
			if ((m_anInput[0] & INPUT_MATK) == INPUT_MATK)
			{
				if ((m_anInput[0] & INPUT2) == INPUT2)
				{
					m_NextMotion = PM_CR_MATTACK;
				}
				else
				{
					m_NextMotion = PM_ST_MATTACK;
				}
			}
			break;

		case CPlayer::PM_CR_MATTACK:
			if ((m_anInput[0] & INPUT_HATK) == INPUT_HATK)
			{
				if ((m_anInput[0] & INPUT2) == INPUT2)
				{
					m_NextMotion = PM_CR_HATTACK;
				}
				else
				{
					m_NextMotion = PM_ST_HATTACK;
				}
			}
			//波動拳・竜巻
			//強
			if (CheckInput(CMD236H) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_NextMotion = PM_236H;
				}
				else if (m_pos.x > m_pEne->m_pos.x)
				{

				}
			}

			//中
			if (CheckInput(CMD236M) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_NextMotion = PM_236M;
				}
				else if (m_pos.x > m_pEne->m_pos.x)
				{

				}
			}

			//弱
			if (CheckInput(CMD236L) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_NextMotion = PM_236L;
				}
				else if (m_pos.x > m_pEne->m_pos.x)
				{

				}
			}

			//2P側
			//強
			if (CheckInput(CMD214H) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_bBullet == false)
				{
					m_NextMotion = PM_236H;
				}
			}

			//中
			if (CheckInput(CMD214M) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_bBullet == false)
				{
					m_NextMotion = PM_236M;
				}
			}

			//弱
			if (CheckInput(CMD214L) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{

				}
				else if (m_bBullet == false)
				{
					m_Motion = PM_236L;
				}
			}
			break;

		case CPlayer::PM_CR_HATTACK:
			break;
		case CPlayer::PM_236L:
			break;
		case CPlayer::PM_236M:
			break;
		case CPlayer::PM_236H:
			break;
		case CPlayer::PM_214L:
			break;
		case CPlayer::PM_214M:
			break;
		case CPlayer::PM_214H:
			break;
		case CPlayer::PM_623L:
			break;
		case CPlayer::PM_623M:
			break;
		case CPlayer::PM_623H:
			break;
		default:
			break;
		}
	}
}

//==============================================
//当たり判定チェック
//==============================================
bool CPlayer::ColJudge(int hurtnumber, int colnum)
{
	D3DXVECTOR3 Hurt, col, HurtWidth, colWidth;
	Hurt = m_apMotion[m_Motion].aKey[m_nCurKey].HurtCol[hurtnumber]->GetPos();
	col = m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[colnum]->GetPos();
	HurtWidth = m_apMotion[m_Motion].aKey[m_nCurKey].HurtCol[hurtnumber]->GetWidth();
	colWidth = m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[colnum]->GetWidth();

	if (Hurt.x + HurtWidth.x / 2 >= col.x - colWidth.x / 2 &&
		Hurt.x - HurtWidth.x / 2 <= col.x + colWidth.x / 2 &&
		Hurt.y + HurtWidth.y / 2 >= col.y - colWidth.y / 2 &&
		Hurt.y - HurtWidth.y / 2 <= col.y + colWidth.y / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//==============================================
//ダウン時の処理
//==============================================
void CPlayer::Down()
{
	if (m_Motion == PM_DOWN&&m_State != PST_DIE)
	{
		m_nRig--;
		if (m_nRig <= 0)
		{//倒れたときの処理
			m_Motion = PM_STANDUP;
			m_State = PST_STAND;
		}
		else
		{
			m_Motion = PM_DOWN;
		}
	}
}

//==============================================
//被弾時の処理時の処理
//==============================================
void CPlayer::Damage_Cal(int Damage, CCollision::EDAMAGE_POINT pro,int HitRig,int GuardRig,bool knockback)
{
	if (Guard(pro) == false)
	{//ヒット
		m_nLife -= Damage;
		m_bAttack = false;
		m_bMotion = false;

		switch (m_State)
		{//やられ状態へ移行
		case CPlayer::PST_STAND:
			m_Motion = PM_ST_HURT;
			break;

		case CPlayer::PST_CROUCH:
			m_Motion = PM_CR_HURT;
			break;

		case CPlayer::PST_AIR:
			m_Motion = PM_JP_HURT;
			break;
		}

		if (m_State!=PST_AIR)
		{
			//ヒット硬直の値を代入
			m_nRig = HitRig;
		}
		
		//弱打撃
		if (m_pEne->m_Motion == PM_JP_LATTACK || m_pEne->m_Motion == PM_ST_LATTACK || m_pEne->m_Motion == PM_CR_LATTACK)
		{
			PlaySound(SOUND_LABEL_SE_LATK_HIT);
		}
		//中打撃
		if (m_pEne->m_Motion == PM_JP_MATTACK || m_pEne->m_Motion == PM_ST_MATTACK || m_pEne->m_Motion == PM_CR_MATTACK)
		{
			PlaySound(SOUND_LABEL_SE_MATK_HIT);
		}
		//強打撃
		if (m_pEne->m_Motion == PM_JP_HATTACK || m_pEne->m_Motion == PM_ST_HATTACK || m_pEne->m_Motion == PM_CR_HATTACK)
		{
			PlaySound(SOUND_LABEL_SE_HATK_HIT);
			switch (m_pEne->m_Motion)
			{//技に応じて吹き飛ばし量を変える
			case PM_JP_HATTACK:
				break;

			case PM_ST_HATTACK:
				m_Motion = PM_JP_HURT;
				m_State = PST_AIR;
				if (m_move.y <= 3.0f)
				{
					m_move.y += 3.0f;
				}
				if (m_pos.x < m_pEne->m_pos.x)
				{
					m_move.x = -2.0f;
				}
				else
				{
					m_move.x = 2.0f;
				}
				m_bJump = true;
				break;

			case PM_CR_HATTACK:
				m_Motion = PM_JP_HURT;
				m_State = PST_AIR;
				m_move.y = 1.0f;
				m_bJump = true;

				break;
			}
		}

		//なんちゃってヒットバック
		if (knockback == true)
		{
			if (m_pos.x < m_pEne->m_pos.x)
			{
				m_pos.x -= 8.0f;	//ヒットバックのつもり

				if (m_pos.x - m_AxisBox->GetWidth().x * 2 <= -FIELD_WIDTH)
				{
					m_pos.x += 8.0f;
					m_pEne->m_pos.x += 8.0f;	//ヒットバックのつもり
				}
			}
			else
			{
				m_pos.x += 8.0f;	//ヒットバックのつもり

				if (m_pos.x + m_AxisBox->GetWidth().x * 2 >= FIELD_WIDTH)
				{
					m_pos.x -= 8.0f;
					m_pEne->m_pos.x -= 8.0f;	//ヒットバックのつもり
				}
			}
		}
	}
	else
	{//ガード成功
	 //ガードの音
		PlaySound(SOUND_LABEL_SE_GUARD);
		if (knockback == true)
		{//ガードバック
			if (m_pos.x < m_pEne->m_pos.x)
			{
				m_pEne->m_pos.x += 10.0f;	//ヒットバックのつもり
			}
			else
			{
				m_pEne->m_pos.x -= 10.0f;	//ヒットバックのつもり
			}
		}

		//ガード硬直の値を代入
		m_nRig = GuardRig;
	}

}

//==============================================
//死亡時の処理
//==============================================
void CPlayer::Die()
{
	if (m_nLife <= 0)
	{//体力が亡くなった時
		switch (m_State)
		{
		case CPlayer::PST_STAND:
			m_Motion = PM_ST_DIE;
			break;
		case CPlayer::PST_CROUCH:
			m_Motion = PM_CR_DIE;
			break;
		case CPlayer::PST_AIR:
			m_Motion = PM_JP_HURT;
			break;
		}

		if ((m_pos.y <= 0.0f) && (m_Motion == PM_JP_HURT) && (m_move.y <= 0.0f))
		{//空中で死んだときの処理
			m_pos.y = 0.0f;
			m_move.y = 0.0f;
			m_move.x = 0.0f;

			m_Motion = PM_DOWN;
			m_State = PST_DIE;
		}
	}
}

//==============================================
//遠距離技
//==============================================
void CPlayer::FireBall()
{
	D3DXVECTOR3 move;
	int life = 0;

	if (m_bBullet == false)
	{//弾発射モーションに応じて性能を変化させる
		if (m_Motion == PM_236L)
		{
			move = D3DXVECTOR3(2.0f, 0.0f, 0.0f);
			life = 10;
		}
		else if (m_Motion == PM_236M)
		{
			move = D3DXVECTOR3(2.5f, 0.0f, 0.0f);
			life = 7;
		}
		else if (m_Motion == PM_236H)
		{
			move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			life = 5;
		}

		float fangle = D3DX_PI*BULLET_ANGLE;
		if (m_pos.x >= m_pEne->m_pos.x)
		{
			move *= -1;
			fangle *= -1;
		}

		if (m_MotionOld!=m_Motion&&(m_Motion == PM_236L || m_Motion == PM_236M || m_Motion == PM_236H))
		{
			m_bAttack = false;
			m_pBullet = CBullet::Create(
				m_pos,
				D3DXVECTOR3(50.0, 10.0f, 70.0f),
				D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.5f),
				move,
				fangle,
				life,
				m_nPlayerNumber);
			m_bBullet = true;
		}
	}
}

//==============================================
//通常投げ
//==============================================
void CPlayer::Slow()
{
}

//==============================================
//角度の正規化
//==============================================
void CPlayer::Normalization()
{
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

//==============================================
//端の処理
//==============================================
void CPlayer::StageEdge()
{
	//ステージの端
	if (m_pos.x - m_AxisBox->GetWidth().x * 2 <= -FIELD_WIDTH)
	{
		m_pos.x = -FIELD_WIDTH + m_AxisBox->GetWidth().x * 2;
	}
	else if (m_pos.x + m_AxisBox->GetWidth().x * 2 >= FIELD_WIDTH)
	{
		m_pos.x = FIELD_WIDTH - m_AxisBox->GetWidth().x * 2;
	}
}
