//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
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

//�ÓI�����o�ϐ�
int CPlayer::m_nNumPlayer = 0;	//�v���C���[�̐�
//===========================
//�R���X�g���N�^
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	m_nPlayerNumber = m_nNumPlayer;
	m_nNumPlayer++;
}

//===========================
//�f�X�g���N�^
//===========================
CPlayer::~CPlayer()
{
	m_nNumPlayer--;
}

//===========================
//����������
//===========================
HRESULT CPlayer::Init()
{
	m_Motion = PM_ST_NEUTRAL;
	m_bMotion = false;

	for (int i = 0; i < NUM_PARTS; i++)
	{//�v���C���[�̐���
		m_apModel[i] = CModel::Create();
	}

	//�G���[�h�~�p�ɑS�Ă̐��l��0������
	for (int i = 0; i < MAX_MOTION; i++)
	{//���[�V�����̏�����
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

	m_AxisBox = CCollision::Create(m_pos, CCollision::COLLI_AXIS);						//�����o������(�v���C���[�̎�)
	m_pShadow = CShadow::Create(m_pos, D3DXVECTOR3(80.0f, 0.0f, 80.0f));

	//���f���ƃ��[�V�����̓ǂݍ���
	ReadMotion();

	for (int i = 0; i < NUM_PARTS; i++)
	{
		//�v���C���[�̐���
		m_apModel[i]->SetPos(m_apMotion[0].aKey[0].aKey[i].fPos + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�
		m_apModel[i]->SetRot(m_apMotion[0].aKey[0].aKey[i].fRot + m_apModel[i]->GetDRot());	//�����̎擾

		if (m_nPlayerNumber == 0)
		{
			m_apModel[i]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{
			m_apModel[i]->SetCol(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		}
	}

	//�f�o�b�O�p�ݒ��Ńt�@�C�����o�͂ł���悤�ɂ���
	m_nJump = 50;			//�W�����v�̑S�̃t���[��
	m_nLife = 1000;			//�̗�
	m_nJumpTransTime = 4;	//�W�����v�ڍs

	return S_OK;
}

//===========================
//�I������
//===========================
void CPlayer::Uninit(void)
{

	for (int i = 0; i < NUM_PARTS; i++)
	{//�v���C���[�̐���
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
//�X�V����
//===========================
void CPlayer::Update(void)
{
	if (CGame::GetGame()!=CGame::GAME_END&&CGame::GetGame()!=CGame::GAME_START)
	{
		
		Input();					//���͏���
			
		if ((m_nHitStop <= 0 && m_pEne->m_nHitStop <= 0) && m_nRig <= 0)
		{//�q�b�g�X�g�b�v���Ȃ��ꍇ
			Down();					//�_�E�����N���オ��
			Updatepos();			//���W�X�V
			//�J�����[
			if (m_pos.x - m_pEne->GetPos().x > FIELD_WIDTH || m_pos.x - m_pEne->GetPos().x < -FIELD_WIDTH)
			{//���ȏ㗣����1�t���[���O�̈ʒu�Ɉړ�
				m_pos.x = m_posold.x;
			}

			if (m_nLife > 0 && m_Motion != PM_DOWN&&m_Motion != PM_STANDUP)
			{//�̗͂��c���Ăă_�E�����Ă��Ȃ��ꍇ
				if (m_bMotion == false && m_bJump == false)
				{
					m_Motion = PM_ST_NEUTRAL;
				}

				ControlPlayer();		//����
				Axis();					//���̉����o������
				Jump();					//�W�����v
				AutoTurn();				//�����U�����

				if (m_NextMotion != PM_ST_NEUTRAL)
				{
					m_Motion = m_NextMotion;
					m_NextMotion = PM_ST_NEUTRAL;	//PM_ST_NEUTRAL�͎g���Ă��Ȃ����
				}
				FireBall();
			}
			if (m_State == PST_AIR)
			{//�d��
				m_move.y -= 0.25f;				//����������
			}
			StageEdge();		//�X�e�[�W�̒[�̏���
		}
		else
		{//�q�b�g�X�g�b�v�������͍d�����̏ꍇ
			m_nHitStop--;
			Cancel();				//�U���L�����Z��
			m_nRig--;
		}

		DrawCollision();		//�����蔻��\��
		Damage();				//�_���[�W����
		Die();					//���S����
		MotionManager();		//���[�V�����Đ�
		Normalization();		//�p�x�̐��K��
		m_pShadow->SetPos({ m_pos.x, 1.0f, m_pos.z });

#ifdef _DEBUG
		CDebugProc::Print("���݂̃v���C���[�̍��W:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
		CDebugProc::Print("���݂̃��[�V����:%d ", (int)m_Motion);
		CDebugProc::Print("���݂̏��:%d ", (int)m_State);
		CDebugProc::Print("���݂̃t���[��:%d", m_frame);

#endif // _DEBUG
	}
}

//===========================
//�`�揈��
//===========================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p

	//���݂̃}�e���A�����ێ�
	pDevice->GetMaterial(&matDef);

	//�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�p�[�c�̃��f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�p�[�c�̃��f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int i = 0; i < NUM_PARTS; i++)
	{
		m_apModel[i]->Draw(m_mtxWorld);
	}
}

//===========================
//����
//===========================
void CPlayer::ControlPlayer(void)
{
	if (m_Motion != PM_CR_HURT && m_Motion != PM_ST_HURT && m_Motion != PM_JP_HURT && m_Motion != PM_DOWN && m_Motion != PM_STANDUP)
	{//��e��Ԃ���Ȃ��ꍇ
		//�ړ�
		if (m_bAttack == false&&m_bMotion==false)
		{
			Command();
		}
	}
}

//===========================
//����
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
//���[�V�����ǂݍ���
//===========================
void CPlayer::ReadMotion()
{
	const int lenLine = 2048;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	char Read[lenLine];			//�ǂݎ��p
	int	modelnumber = 0;		//���f���̔ԍ�
	int motionnumber = 0;		//���[�V�����̔ԍ�
	int key = 0;
	int partsmotion = 0;
	int Idx = 0;

	//�t�@�C���ǂݍ���
	FILE*fp = fopen("data/TXT/Player01/Player01.txt", "r");		//�t�@�C���ǂݍ���
	if (fp == nullptr)
	{//�J���Ȃ��������p
		assert(false);
	}
	if (fp != NULL)
	{
		while (fgets(Read, lenLine, fp) != nullptr)
		{
			//������̕���
			sscanf(Read, "%s", &strLine);
			if (strcmp(&strLine[0], "SCRIPT") == 0)	//��r���Ĉ�v���邩�ǂ������ׂ�
			{
				while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v(��s�ǂݍ���)
				{//���f���̏����ݒ�
					ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

					//������̕���
					sscanf(Read, "%s", &strLine);

					if (strcmp(&strLine[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else if (strcmp(&strLine[0], "NUM_MODEL") == 0)
					{
						sscanf(Read, "%s = %d", &strLine, &m_nNumModel);	//�ǂݍ��񂾕������Ƃɐݒ肷��
					}
					else if (strcmp(&strLine[0], "MODEL_FILENAME") == 0)
					{
						sscanf(Read, "%s = %s", &strLine, &m_nModelpass[0]);	//���f���̃p�X�̐ݒ�

						m_apModel[modelnumber]->SetModel(&m_nModelpass[0]);
						modelnumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//�����ʒu�̐ݒ�
						while (fgets(Read, lenLine, fp) != nullptr)//��s�ǂݍ���
						{//�L�����N�^�[�̐ݒ�
						 //������̕���
							ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��݃��[�v //��s�ǂݍ���
								{//�p�[�c�̐ݒ�	
									ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g
									//������̕���
									sscanf(Read, "%s", &strLine);
									if (strcmp(&strLine[0], "END_PARTSSET") == 0)
									{//�p�[�c�̐ݒ�I��
										Idx++;
										break;
									}
									else if (strcmp(&strLine[0], "INDEX") == 0)
									{//�C���f�b�N�X�̐ݒ�
										sscanf(Read, "%s = %d", &strLine, &Idx);	//���f���̃p�X�̐ݒ�
									}
									else if (strcmp(&strLine[0], "PARENT") == 0)
									{//�e���f���̐ݒ�
										int Parent;
										sscanf(Read, "%s = %d", &strLine, &Parent);	//���f���̃p�X�̐ݒ�
										m_apModel[Idx]->SetParent(m_apModel[Parent]);
									}
									else if (strcmp(&strLine[0], "POS") == 0)
									{//�ʒu
										D3DXVECTOR3 pos;
										sscanf(Read, "%s = %f%f%f", &strLine, &pos.x, &pos.y, &pos.z);	//���W�̎擾
										m_apModel[Idx]->SetPos(pos);
										m_apModel[Idx]->SetDPos(pos);
									}
									else if (strcmp(&strLine[0], "ROT") == 0)
									{//����
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
					{//���[�V�����̐ݒ�
						while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

							//������̕���
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
								sscanf(Read, "%s = %s", &strLine, &cFilename[0]);	//�t�@�C���ǂݍ���

								//�t�@�C���ǂݍ���
								FILE* sta = fopen(cFilename, "r");		//�t�@�C���ǂݍ���
								if (sta == nullptr)
								{//�J���Ȃ��������p
									assert(false);
								}
								if (sta != NULL)
								{
									while (fgets(Read, lenLine, sta) != nullptr)	//�ǂݍ��ݗp���[�v
									{
										ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

										//������̕���
										sscanf(Read, "%s", &strLine);

										if (strcmp(&strLine[0], "MOTIONSET") == 0)
										{
											while (fgets(Read, lenLine, sta) != nullptr)	//�ǂݍ��ݗp���[�v
											{
												ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

												//������̕���
												sscanf(Read, "%s", &strLine);
												if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
												{
													//���[�V�����̔ԍ��J��グ
													motionnumber++;
													key = 0;
													fclose(sta);
													break;
												}
												if (strcmp(&strLine[0], "NUM_KEY") == 0)
												{
													sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//�L�[�̑���
												}
												else if (strcmp(&strLine[0], "LOOP") == 0)
												{//���[�v���邩���Ȃ���
													sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//���[�v���邩�ǂ���
												}
												else if (strcmp(&strLine[0], "HITSTOP") == 0)
												{//�q�b�g�X�g�b�v�̎��Ԑݒ�
													sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nHitStopTimer);
												}
												else if (strcmp(&strLine[0], "KEYSET") == 0)
												{
													while (fgets(Read, lenLine, sta) != nullptr)
													{
														ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

														//������̕���
														sscanf(Read, "%s", &strLine);

														//key�̓��f���̃L�[�̔ԍ�
														if (strcmp(&strLine[0], "END_KEYSET") == 0)
														{
															key++;
															partsmotion = 0;	//�ԍ����Z�b�g
															break;
														}
														else if (strcmp(&strLine[0], "FRAME") == 0)
														{//�L�[�̍Đ����Ԃ̐ݒ�
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aKey[key].nFrame);	//�Đ����Ԃ̐ݒ�
														}
														else if (strcmp(&strLine[0], "COLLISIONSET") == 0)
														{
															m_apMotion[motionnumber].aKey[key].Collision[m_apMotion[motionnumber].aKey[key].nNumCollision]
																= CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g
																sscanf(Read, "%s", &strLine);					//������̕���

																//key�̓��f���̃L�[�̔ԍ�
																if (strcmp(&strLine[0], "END_COLLISIONSET") == 0)
																{
																	m_apMotion[motionnumber].aKey[key].nNumCollision++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//����̊J�n����
																	int start, select;
																	select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &start);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//����̊J�n����
																	int end;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &end);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	D3DXVECTOR3 dpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &dpos.x, &dpos.y, &dpos.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetDPos(dpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  dsiz;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %f%f%f", &strLine, &dsiz.x, &dsiz.y, &dsiz.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetSiz(dsiz);
																}
																else if ((strcmp(&strLine[0], "DAMAGE_POINT") == 0))
																{//��i�̏ꍇ�͏����Ȃ��Ă���
																	int dp;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &dp);	//�L�[�̑���
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetDP((CCollision::EDAMAGE_POINT)dp);
																}
																else if ((strcmp(&strLine[0], "HIT") == 0))
																{//�q�b�g�d���̐ݒ�
																	int hit;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &hit);	//�L�[�̑���
																	m_apMotion[motionnumber].aKey[key].Collision[select]->SetHitRig(hit);
																}
																else if ((strcmp(&strLine[0], "GUARD") == 0))
																{//�K�[�h�d���̐ݒ�
																	int Guard;
																	int select = m_apMotion[motionnumber].aKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &Guard);	//�L�[�̑���
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
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																//������̕���
																sscanf(Read, "%s", &strLine);

																//key�̓��f���̃L�[�̔ԍ�
																if (strcmp(&strLine[0], "END_HURTSET") == 0)
																{
																	m_apMotion[motionnumber].aKey[key].nNumHurtCol++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//����̊J�n����
																	int start;
																	sscanf(Read, "%s = %d", &strLine, &start);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//����̊J�n����
																	int end;
																	sscanf(Read, "%s = %d", &strLine, &end);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	D3DXVECTOR3 hpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &hpos.x, &hpos.y, &hpos.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetDPos(hpos);
																	m_apMotion[motionnumber].aKey[key].HurtCol[m_apMotion[motionnumber].aKey[key].nNumHurtCol]->SetPos(hpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  hsiz;
																	int select = m_apMotion[motionnumber].aKey[key].nNumHurtCol;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %f%f%f", &strLine, &hsiz.x, &hsiz.y, &hsiz.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aKey[key].HurtCol[select]->SetSiz(hsiz);
																}
															}
														}
														else if (strcmp(&strLine[0], "KEY") == 0)
														{//�L�[�ݒ�
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																//������̕���
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
																		&m_apMotion[motionnumber].aKey[key].aKey[partsmotion].fPos.z);	//�Đ����Ԃ̐ݒ�
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
											break;	//�ǂݍ��݂��I������烋�[�v�E�o
										}
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "AXISSET") == 0)
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

							//������̕���
							sscanf(Read, "%s", &strLine);

							if (strcmp(&strLine[0], "END_AXIS") == 0)
							{
								break;
							}
							else if ((strcmp(&strLine[0], "POS") == 0))
							{
								D3DXVECTOR3 axpos;
								sscanf(Read, "%s = %f%f%f", &strLine, &axpos.x, &axpos.y, &axpos.z);	//�L�[�̑���
								m_AxisBox->SetPos(axpos + m_pos);
								m_AxisBox->SetDPos(axpos);

							}
							else if ((strcmp(&strLine[0], "SIZ") == 0))
							{
								D3DXVECTOR3  axsiz;
								sscanf(Read, "%s = %f%f%f", &strLine, &axsiz.x, &axsiz.y, &axsiz.z);	//�L�[�̑���
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
//�p�[�c�̃��[�V����
//===========================
void CPlayer::MotionPlayer()
{
	D3DXVECTOR3 RelaPos, RelaRot;		//1�t���[�����Ƃ̈ړ���
	D3DXVECTOR3 pos, rot, DiffPos, DiffRot;

	//�J�E���^�[�X�V
	if (m_nCurKey == m_apMotion[m_Motion].nNumKey&& m_apMotion[m_Motion].bLoop == false)
	{
		//���[�v���[�V�������I�������j���[�g�����ɂ���
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
		{//�p�[�c�S���̃��[�V�����Đ�
			if (m_apModel[i] != nullptr)
			{
				if (m_nCurKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == true)
				{//���[�v����ꍇ�ŏ��̃��[�V�����Ɉڍs����
					DiffPos = D3DXVECTOR3(
						m_apMotion[m_Motion].aKey[0].aKey[i].fPos -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);	//�����̎擾

					DiffRot = D3DXVECTOR3(
						m_apMotion[m_Motion].aKey[0].aKey[i].fRot -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);	//�����̎擾
				}
				else if (m_nCurKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == false)
				{//���[�v���Ȃ��ꍇ�j���[�g�������[�V�����ɖ߂�
					if (m_Motion == PM_ST_DIE || m_Motion == PM_CR_DIE || m_Motion == PM_JP_DIE)
					{//���S���[�V�����̏ꍇ���̂܂܎��S�ݒ�ɂ���
						m_State = PST_DIE;
					}

					switch (m_State)
					{//���݂̏�Ԃɖ߂�
					case CPlayer::PST_STAND:
						DiffPos = D3DXVECTOR3(//���W�����̎擾
							m_apMotion[0].aKey[0].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//���������̎擾
							m_apMotion[0].aKey[0].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					case CPlayer::PST_CROUCH:
						DiffPos = D3DXVECTOR3(//���W�����̎擾
							m_apMotion[PM_CR_NEUTRAL].aKey[0].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//���������̎擾
							m_apMotion[PM_CR_NEUTRAL].aKey[0].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					case CPlayer::PST_AIR:
						DiffPos = D3DXVECTOR3(//���W�����̎擾
							m_apMotion[0].aKey[0].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//���������̎擾
							m_apMotion[0].aKey[0].aKey[i].fRot -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
						break;
					case CPlayer::PST_DIE:
						DiffPos = D3DXVECTOR3(//���W�����̎擾
							m_apMotion[PM_DOWN].aKey[1].aKey[i].fPos -
							m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

						DiffRot = D3DXVECTOR3(//���������̎擾
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
					{//�ʒu���W�̐ݒ肵�Ȃ���
						PlayFirstMotion();
					}
					DiffPos = D3DXVECTOR3(//���W�����̎擾
						m_apMotion[m_Motion].aKey[m_nCurKey + 1].aKey[i].fPos -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fPos);

					DiffRot = D3DXVECTOR3(//���������̎擾
						m_apMotion[m_Motion].aKey[m_nCurKey + 1].aKey[i].fRot -
						m_apMotion[m_Motion].aKey[m_nCurKey].aKey[i].fRot);
				}
			}

			//�ʒu
			RelaPos = (DiffPos / (float)m_apMotion[m_Motion].aKey[m_nCurKey].nFrame);		//���Βl
			RelaRot = (DiffRot / (float)m_apMotion[m_Motion].aKey[m_nCurKey].nFrame);

			//�L�[�̐ݒ�
			//�Đ����[�h�̏ꍇ
			pos = RelaPos + m_apModel[i]->GetPos();
			rot = RelaRot + m_apModel[i]->GetRot();

			//�ʒu�̐ݒ�
			m_apModel[i]->SetPos(pos);
			m_apModel[i]->SetRot(rot);
		}
	}

	//�Đ����[�h�̏ꍇ
	 //�J�E���^�[�X�V
	if (m_frame >= m_apMotion[m_Motion].aKey[m_nCurKey].nFrame)
	{//�L�[�ԍ��̍X�V�ƃJ�E���^�[�̃��Z�b�g
		m_nCurKey++;
		m_frame = 0;
		if (m_nCurKey >= m_apMotion[m_Motion].nNumKey)
		{//�L�[�ԍ����ő吔�𒴂����ꍇ���Z�b�g
			if (m_apMotion[m_Motion].bLoop == true)
			{
				m_nCurKey = 0;
			}
		}
		for (int j = 0; j < m_apMotion[m_Motion].aKey[m_nCurKey].nNumCollision; j++)
		{//�_���[�W�t���O����
			m_apMotion[m_Motion].aKey[m_nCurKey].Collision[j]->SetDmg(true);
		}
	}
	m_frame++;
}

//===========================
//���[�V�����Ǘ�
//===========================
void CPlayer::MotionManager()
{
	if (m_MotionOld != m_Motion)
	{//��Ԃ��Ⴄ�ꍇ
		PlayFirstMotion();
	}
		MotionPlayer();		//�v���C���[�̃��[�V����
	m_MotionOld = m_Motion;
}

//===========================
//�ŏ��̃��[�V������ݒ�
//===========================
void CPlayer::PlayFirstMotion()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//�v���C���[�̐���
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_Motion].aKey[0].aKey[i].fPos) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_Motion].aKey[0].aKey[i].fRot) + m_apModel[i]->GetDRot());	//�����̎擾
	}
	m_frame = 0;
	m_nCurKey = 0;
}

//===========================
//�����蔻���\�����邩�ǂ���
//===========================
void CPlayer::DrawCollision()
{
	//�����蔻��̕\��
	for (int i = 0; i < PM_MAX; i++)
	{
		for (int k = 0; k < m_apMotion[i].nNumKey; k++)
		{//�v���C���[1
			//�_���[�W����
			for (int j = 0; j < m_apMotion[i].aKey[k].nNumCollision; j++)
			{//�Ⴄ���[�V�����̓����蔻����I�t�ɂ���

				 //�U������Đݒ�
				if (m_bSide == false)
				{//�G���E���̏ꍇ
					//�v���C���[1
					m_apMotion[i].aKey[k].Collision[j]->SetPos(m_pos + m_apMotion[i].aKey[k].Collision[j]->GetDPos());
				}
				else
				{//�E����

					//�v���C���[1
					m_apMotion[i].aKey[k].Collision[j]->SetPos(D3DXVECTOR3(
						m_pos.x - m_apMotion[i].aKey[k].Collision[j]->GetDPos().x,				//X
						m_pos.y + m_apMotion[i].aKey[k].Collision[j]->GetDPos().y,
						m_pos.z - m_apMotion[i].aKey[k].Collision[j]->GetDPos().z));
				}

				if (m_Motion == i&&m_nCurKey == k&&m_frame >= m_apMotion[i].aKey[k].Collision[j]->GetStartf() && m_frame <= m_apMotion[i].aKey[k].Collision[j]->GetEndf())
				{//�L�[�ƃ��[�V��������v���Ă���ꍇ�̂ݕ\��
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

			//���ꔻ��
			for (int j = 0; j < m_apMotion[i].aKey[k].nNumHurtCol; j++)
			{
				if (m_bSide == true)
				{//�E����(1P)
					m_apMotion[i].aKey[k].HurtCol[j]->SetPos(D3DXVECTOR3(
						m_pos.x - m_apMotion[i].aKey[k].HurtCol[j]->GetDPos().x,
						m_pos.y + m_apMotion[i].aKey[k].HurtCol[j]->GetDPos().y,
						m_pos.z + m_apMotion[i].aKey[k].HurtCol[j]->GetDPos().z));
				}
				else
				{//������(2P)
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
//�����o������
//=====================
void CPlayer::Axis(void)
{
	if (m_pos.y - m_AxisBox->GetWidth().y / 2 <= m_pEne->m_pos.y + m_pEne->m_AxisBox->GetWidth().y 
		&& m_pos.y + m_AxisBox->GetWidth().y / 2 >= m_pEne->m_pos.y - m_pEne->m_AxisBox->GetWidth().y )
	{
			//X��
			//����
			if (m_posold.x + m_AxisBox->GetWidth().x / 2 <= m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x / 2	//�O��̃v���C���[�̈ʒu���u���b�N�̈ʒu���E�ɂ���ꍇ
				&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x / 2 	//���݂̃v���C���[�̈ʒu���u���b�N�̈ʒu��荶�ɂ���(�߂荞��ł���)�ꍇ
				)//�v���C���[�̍��E�Ƀu���b�N�����������ꍇ
			{//�v���C��-�ƃu���b�N������������
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
			}
			//�E��
			if (m_posold.x - m_AxisBox->GetWidth().x / 2 >= m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x / 2 	//�O��̃v���C���[�̈ʒu���u���b�N�̈ʒu���E�ɂ���ꍇ
				&& m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x / 2	//���݂̃v���C���[�̈ʒu���u���b�N�̈ʒu��荶�ɂ���(�߂荞��ł���)�ꍇ
				)//�v���C���[�̍��E�Ƀu���b�N�����������ꍇ
			{//�v���C��-�ƃu���b�N������������
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
			}
	}
	if (m_posold.y + m_pEne->m_AxisBox->GetWidth().y >= m_pEne->m_pos.y + m_pEne->m_AxisBox->GetWidth().y
		&& m_pos.y+ m_pEne->m_AxisBox->GetWidth().y<= m_pEne->m_pos.y + m_pEne->m_AxisBox->GetWidth().y )
	{//�W�����v�������̉��������̔���
		if (m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x / 2
			&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x / 2)
		{
			if (m_pos.x >= m_pEne->m_pos.x)
			{//���S��聨
				m_pos.x = m_pEne->m_pos.x + m_pEne->m_AxisBox->GetWidth().x+1;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = -m_move.x;
			}
			else
			{//���S��聩
				m_pos.x = m_pEne->m_pos.x - m_pEne->m_AxisBox->GetWidth().x-1;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = -m_move.x;
			}
		}
	}
}

//=====================
//�W�����v
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
				m_move.y = INITIAL_VELOCITY;	//������ݒ�
			}
		}
		
		m_nJumpCount++;
		if (m_pos.y < 0.0f)
		{//���n���̐ݒ�
			m_nJumpCount = 0;				//�W�����v�J�E���g������
			m_State = PST_STAND;			//������ԂɕύX
			m_pos.y = 0.0f;					//�ʒu�ݒ�
			m_move.y = 0.0f;				//�ړ��ʂ�0�ɖ߂�
			m_move.x = 0.0f;				//�ړ��ʂ�0�ɖ߂�
			m_bJump = false;				//�W�����v��ԉ���
			m_bAttack = false;				//�U����Ԃ�����
			m_bMotion = false;
			if (m_Motion == PM_JP_HURT)
			{
				m_Motion = PM_DOWN;
			}
			else
			{
				m_Motion = PM_ST_NEUTRAL;		//�j���[�g�������[�V�����ɖ߂�
			}
		}
	}
}

//=====================
//�����U�����
//=====================
void CPlayer::AutoTurn(void)
{
	if (m_State == PST_STAND)
	{//�n�㎞�̂ݑ���̕����ʐU��ނ�
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
//�_���[�W����
//=====================
void CPlayer::Damage()
{
	for (int i = 0; i < m_apMotion[m_Motion].aKey[m_nCurKey].nNumHurtCol; i++)
	{
		for (int j = 0; j < m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].nNumCollision; j++)
		{
			if (m_pEne->m_apMotion[m_pEne->m_Motion].aKey[m_pEne->m_nCurKey].Collision[j]->GetUseDmg() == true)	//�_���[�W����c���Ă邩�ǂ���
			{
				if (ColJudge(i, j)==true)	//��`�̔���
				{
					//�_���[�W����
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
//�K�[�h����
//=====================
bool CPlayer::Guard(CCollision::EDAMAGE_POINT dp)
{
	switch (m_Motion)
	{
	case PM_ST_GUARD:	//�����K�[�h
		if (dp == CCollision::DP_LOW)
		{

			return false;	//�_���[�W���ʂ�
		}
		else
		{//�ړ��ʂ̐ݒ�
			CParticle::Create(m_pos + D3DXVECTOR3{ -20.0f,30.0f,0.0f },			//�ʒu�̐ݒ�
				D3DXVECTOR3{ GUARD_EFFECTSIZ,GUARD_EFFECTSIZ ,GUARD_EFFECTSIZ },		//���a�̑傫���̐ݒ�
				D3DXCOLOR(0.05f, 0.05f, 0.45f, 1.0f),							//���_�J���[�̐ݒ�	
				CParticle::PAR_FIREFLOWER);
			return true;	//�K�[�h����
		}

	case PM_CR_GUARD:
		if (dp == CCollision::DP_MIDDLE)
		{
			return false;	//�_���[�W���ʂ�
		}
		else
		{
			CParticle::Create(m_pos + D3DXVECTOR3{ -20.0f,20.0f,0.0f },			//�ʒu�̐ݒ�
				D3DXVECTOR3{ GUARD_EFFECTSIZ,GUARD_EFFECTSIZ ,GUARD_EFFECTSIZ },		//���a�̑傫���̐ݒ�
				D3DXCOLOR(0.05f, 0.05f, 0.45f, 1.0f),							//���_�J���[�̐ݒ�	
				CParticle::PAR_FIREFLOWER);
			return true;	//�K�[�h����
		}

	case PM_JP_GUARD:
		return true;	//�K�[�h����

	default:

		//�ړ��ʂ̐ݒ�
		CParticle::Create(m_pos + D3DXVECTOR3{ 0.0f,30.0f,0.0f },							//�ʒu�̐ݒ�
			D3DXVECTOR3{ HIT_EFFECTSIZ,HIT_EFFECTSIZ ,HIT_EFFECTSIZ },				//���a�̑傫���̐ݒ�
			D3DXCOLOR(0.35f, 0.16f, 0.0f, 1.0f),				//���_�J���[�̐ݒ�	
			CParticle::PAR_FIREFLOWER);
		return false;	//�_���[�W���ʂ�
	}
}

//=====================
//���쏈��
//=====================
void CPlayer::Command()
{
	if (m_pEne->m_nLife >= 0)
	{
		//�ړ�
		if (m_State != PST_AIR)
		{
			//�j���[�g����
			if ((m_anInput[0] & INPUT5) == INPUT5)
			{
				m_Motion = PM_ST_NEUTRAL;
				m_move.x = 0.0f;
			}

			//��
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
						{//�e�̖h�䏈��
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

			//�E
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
						{//�e�̖h�䏈��
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

			//�E�E
			if (CheckInput(CMD66) == true && m_State == PST_STAND)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_Motion != PM_ST_DASH)
				{//�E�����̏ꍇ
					//�O�_�b�V��
					m_Motion = PM_ST_DASH;
				}
				else
				{//������(2P��)�̏ꍇ
					//�o�b�N�X�e�b�v
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
				{//������(2P��)�̏ꍇ
				 //�O�_�b�V��
					m_Motion = PM_ST_DASH;
				}
				else
				{//�E�����̏ꍇ
				 //�o�b�N�X�e�b�v
					m_Motion = PM_ST_BACKSTEP;
					m_move = { -BACKSTEP_MOVE_X,BACKSTEP_MOVE_Y,0.0f };
					m_State = PST_AIR;
					m_nJumpCount = 1;
					m_bJump = true;
					PlaySound(SOUND_LABEL_SE_BACKSTEP);
				}
			}

			//���E���������Ńj���[�g����
			if ((m_anInput[0] & INPUT6) == INPUT6 && (m_anInput[0] & INPUT4) == INPUT4)
			{
				m_Motion = PM_ST_NEUTRAL;

				m_move.x = 0.0f;
				m_move.z = 0.0f;
			}

			//��(���Ⴊ��)
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
						{//�e�̖h�䏈��
							if (m_pEne->m_pBullet->GetPos().x - m_pos.x <= 100.0f&&m_pEne->m_pBullet->GetPos().x - m_pos.x >= -100.0f)
							{
								m_Motion = PM_CR_GUARD;
								m_move = { 0.0f,0.0f,0.0f };
							}
						}
					}
				}
			}

			//��(�W�����v)
			if ((m_anInput[0] & INPUT8) == INPUT8)
			{
				m_Motion = PM_JP_NEUTRAL;
				m_State = PST_AIR;
				m_bJump = true;

			}

			//�E��
			if ((m_anInput[0] & INPUT9) == INPUT9)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{//1P��
					m_Motion = PM_JP_MOVEFORWARD;
				}
				else
				{//2P��
					m_Motion = PM_JP_MOVEBACK;
				}
			}

			//����
			if ((m_anInput[0] & INPUT7) == INPUT7)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{//1P��
					m_Motion = PM_JP_MOVEBACK;
				}
				else
				{//2P��
					m_Motion = PM_JP_MOVEFORWARD;
				}
			}
		}

		//�U��
		if (m_Motion != PM_ST_BACKSTEP)
		{
			//��U��
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

			//���U��
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

			//���U��
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

			//�g�����E����
			//��
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

			//��
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

			//��
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

			//2P��
			//��
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

			//��
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

			//��
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
		StateManagement();	//��ԊǗ�
	}
}

//==============================================
//�R�}���h�̓��͔���
//==============================================
bool CPlayer::CheckInput(const int * command)
{
	//�R�}���h
	int i = 0, j = 0;
	//�R�}���h�̍Ō�𒲂ׂ�
	while (!(command[i] & INPUT_END))i++;

	//�R�}���h�̓��͎��Ԃ��擾
	int time = command[i] & ~INPUT_END;

	//���͎��Ԃ𒲂ׂĐ������ꍇ��true����ȊO��false
	for (i--; i >= 0; i--)
	{
		int input = command[i];

		//��������R�}���h�̗v�f��T��
		while (j < MAX_KEYMEMORY && ((m_anInput[j] & input) != input)) j++;

		//���͎��Ԃ𒴂�����A�����̖����ɒB������Afalse��Ԃ�
		if (j >= time || j == MAX_KEYMEMORY)return false;
	}

	//�R�}���h�̗v�f������������true
	return true;
}

//==============================================
//�v���C���[�̏�ԊǗ�
//==============================================
void CPlayer::StateManagement()
{
	//�ړ����Ă��Ȃ��ꍇ�̏���
	if (m_Motion == PM_ST_NEUTRAL || m_Motion == PM_CR_NEUTRAL || m_bAttack == true)
	{
		if (m_bJump == false)
		{
			m_move.x = 0;
			m_move.z = 0;
		}
	}

	//�������Ă��Ȃ�&�n�㎞&�U�����Ă��Ȃ��ꍇ
	if ((m_anInput[0] & INPUT_NOT2) == INPUT_NOT2&&m_State != PST_AIR&&m_bAttack == false)
	{//�������Ă��Ȃ��ꍇ������Ԃ�
		m_State = PST_STAND;
	}

	//�_�b�V������
	if (m_MotionOld == PM_ST_DASH&&m_bAttack == false&& m_State == PST_STAND)
	{//�U�����Ă��Ȃ��ꍇ�A�n�㎞�̂݃_�b�V���ێ�
		if (m_pos.x <= m_pEne->m_pos.x && (m_anInput[0] & INPUT6) == INPUT6 && (m_anInput[0] & INPUT_NOT4) == INPUT_NOT4)
		{//�O�������ςȂ��ł������͂Ȃ�
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
//���͐ݒ�
//==============================================
void CPlayer::Input()
{
	CInput* pInput = CApplication::GetInput();
	int Key = 0;
	pInput->PressDevice(KEY_DOWN_RIGHT);
	//���o�[
	{
		//����
		Key |= (pInput->Press(DIK_A) && pInput->Press(DIK_S)) || pInput->Press(JOYPAD_DOWN_LEFT, m_nPlayerNumber) ? INPUT1 : INPUT_NOT1;
		//��
		Key |= pInput->Press(DIK_S) || pInput->Press(JOYPAD_DOWN, m_nPlayerNumber)|| pInput->Press(JOYPAD_DOWN_LEFT, m_nPlayerNumber) || pInput->Press(JOYPAD_DOWN_RIGHT, m_nPlayerNumber) ? INPUT2 : INPUT_NOT2;
		//�E��
		Key |= (pInput->Press(DIK_D) && pInput->Press(DIK_S)) || pInput->Press(JOYPAD_DOWN_RIGHT, m_nPlayerNumber) ? INPUT3 : INPUT_NOT3;
		//��
		Key |= pInput->Press(DIK_A) || pInput->Press(JOYPAD_LEFT, m_nPlayerNumber) || pInput->Press(JOYPAD_DOWN_LEFT, m_nPlayerNumber) || (pInput->Press(JOYPAD_UP_LEFT, m_nPlayerNumber)) ? INPUT4 : INPUT_NOT4;
		//�E
		Key |= pInput->Press(DIK_D) || pInput->Press(JOYPAD_RIGHT, m_nPlayerNumber) || (pInput->Press(JOYPAD_UP_RIGHT, m_nPlayerNumber)) || pInput->Press(JOYPAD_DOWN_RIGHT, m_nPlayerNumber) ? INPUT6 : INPUT_NOT6;
		//����
		Key |= (pInput->Press(DIK_A) && pInput->Press(DIK_SPACE)) || (pInput->Press(JOYPAD_UP_LEFT, m_nPlayerNumber)) ? INPUT7 : INPUT_NOT7;
		//��
		Key |= pInput->Press(DIK_SPACE) || pInput->Press(JOYPAD_UP, m_nPlayerNumber) || (pInput->Press(JOYPAD_UP_RIGHT, m_nPlayerNumber)) || (pInput->Press(JOYPAD_UP_LEFT, m_nPlayerNumber)) ? INPUT8 : INPUT_NOT8;
		//�E��
		Key |= (pInput->Press(DIK_D) && pInput->Press(DIK_SPACE)) || (pInput->Press(JOYPAD_UP_RIGHT, m_nPlayerNumber)) ? INPUT9 : INPUT_NOT9;

		//�j���[�g����
		Key |= (m_anInput[0] & INPUT_NOT6) == INPUT_NOT6 && (m_anInput[0] & INPUT_NOT2) == INPUT_NOT2 && (m_anInput[0] & INPUT_NOT4) == INPUT_NOT4 && (m_anInput[0] & INPUT_NOT8) == INPUT_NOT8 ? INPUT_NOT5 : INPUT5;

		//�U��
		//��
		Key |= pInput->Press(DIK_U) || pInput->Press(JOYPAD_A, m_nPlayerNumber) ? INPUT_LATK : INPUT_NOT_LATK;
		//��
		Key |= pInput->Press(DIK_I) || pInput->Press(JOYPAD_B, m_nPlayerNumber) ? INPUT_MATK : INPUT_NOT_MATK;
		//��
		Key |= pInput->Press(DIK_O) || pInput->Press(JOYPAD_R1, m_nPlayerNumber) ? INPUT_HATK : INPUT_NOT_HATK;
	}

	//�R�}���h����ւ�����
	for (int i = MAX_KEYMEMORY - 1; i > 0; i--)
	{
		m_anInput[i] = m_anInput[i - 1];
	}
	m_anInput[0] = Key;

}

//==============================================
//���W�̍X�V
//==============================================
void CPlayer::Updatepos()
{
	m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
	m_pos += m_move;		//�ʒu�̍X�V
	m_AxisBox->SetPos(m_pos + D3DXVECTOR3(0.0f, m_AxisBox->GetDPos().y, 0.0f));	//���̔���̈ʒu�X�V
}

//==============================================
//�L�����Z��
//==============================================
void CPlayer::Cancel()
{
	if (m_bAttack == true)
	{//�U���L�����Z��
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
			//�g�����E����
			//��
			if (CheckInput(CMD236H) == true && m_State != PST_AIR)
			{
				if (m_pos.x <= m_pEne->m_pos.x&&m_bBullet == false)
				{
					m_NextMotion = PM_236H;
				}
			}

			//��
			if (CheckInput(CMD236M) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{
					m_NextMotion = PM_236M;
				}
			}

			//��
			if (CheckInput(CMD236L) == true && m_State != PST_AIR&&m_bBullet == false)
			{
				if (m_pos.x <= m_pEne->m_pos.x)
				{
					m_NextMotion = PM_236L;
				}
			}

			//2P��
			//��
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

			//��
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

			//��
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
			//�g�����E����
			//��
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

			//��
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

			//��
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

			//2P��
			//��
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

			//��
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

			//��
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
//�����蔻��`�F�b�N
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
//�_�E�����̏���
//==============================================
void CPlayer::Down()
{
	if (m_Motion == PM_DOWN&&m_State != PST_DIE)
	{
		m_nRig--;
		if (m_nRig <= 0)
		{//�|�ꂽ�Ƃ��̏���
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
//��e���̏������̏���
//==============================================
void CPlayer::Damage_Cal(int Damage, CCollision::EDAMAGE_POINT pro,int HitRig,int GuardRig,bool knockback)
{
	if (Guard(pro) == false)
	{//�q�b�g
		m_nLife -= Damage;
		m_bAttack = false;
		m_bMotion = false;

		switch (m_State)
		{//�����Ԃֈڍs
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
			//�q�b�g�d���̒l����
			m_nRig = HitRig;
		}
		
		//��Ō�
		if (m_pEne->m_Motion == PM_JP_LATTACK || m_pEne->m_Motion == PM_ST_LATTACK || m_pEne->m_Motion == PM_CR_LATTACK)
		{
			PlaySound(SOUND_LABEL_SE_LATK_HIT);
		}
		//���Ō�
		if (m_pEne->m_Motion == PM_JP_MATTACK || m_pEne->m_Motion == PM_ST_MATTACK || m_pEne->m_Motion == PM_CR_MATTACK)
		{
			PlaySound(SOUND_LABEL_SE_MATK_HIT);
		}
		//���Ō�
		if (m_pEne->m_Motion == PM_JP_HATTACK || m_pEne->m_Motion == PM_ST_HATTACK || m_pEne->m_Motion == PM_CR_HATTACK)
		{
			PlaySound(SOUND_LABEL_SE_HATK_HIT);
			switch (m_pEne->m_Motion)
			{//�Z�ɉ����Đ�����΂��ʂ�ς���
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

		//�Ȃ񂿂���ăq�b�g�o�b�N
		if (knockback == true)
		{
			if (m_pos.x < m_pEne->m_pos.x)
			{
				m_pos.x -= 8.0f;	//�q�b�g�o�b�N�̂���

				if (m_pos.x - m_AxisBox->GetWidth().x * 2 <= -FIELD_WIDTH)
				{
					m_pos.x += 8.0f;
					m_pEne->m_pos.x += 8.0f;	//�q�b�g�o�b�N�̂���
				}
			}
			else
			{
				m_pos.x += 8.0f;	//�q�b�g�o�b�N�̂���

				if (m_pos.x + m_AxisBox->GetWidth().x * 2 >= FIELD_WIDTH)
				{
					m_pos.x -= 8.0f;
					m_pEne->m_pos.x -= 8.0f;	//�q�b�g�o�b�N�̂���
				}
			}
		}
	}
	else
	{//�K�[�h����
	 //�K�[�h�̉�
		PlaySound(SOUND_LABEL_SE_GUARD);
		if (knockback == true)
		{//�K�[�h�o�b�N
			if (m_pos.x < m_pEne->m_pos.x)
			{
				m_pEne->m_pos.x += 10.0f;	//�q�b�g�o�b�N�̂���
			}
			else
			{
				m_pEne->m_pos.x -= 10.0f;	//�q�b�g�o�b�N�̂���
			}
		}

		//�K�[�h�d���̒l����
		m_nRig = GuardRig;
	}

}

//==============================================
//���S���̏���
//==============================================
void CPlayer::Die()
{
	if (m_nLife <= 0)
	{//�̗͂��S���Ȃ�����
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
		{//�󒆂Ŏ��񂾂Ƃ��̏���
			m_pos.y = 0.0f;
			m_move.y = 0.0f;
			m_move.x = 0.0f;

			m_Motion = PM_DOWN;
			m_State = PST_DIE;
		}
	}
}

//==============================================
//�������Z
//==============================================
void CPlayer::FireBall()
{
	D3DXVECTOR3 move;
	int life = 0;

	if (m_bBullet == false)
	{//�e���˃��[�V�����ɉ����Đ��\��ω�������
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
//�ʏ퓊��
//==============================================
void CPlayer::Slow()
{
}

//==============================================
//�p�x�̐��K��
//==============================================
void CPlayer::Normalization()
{
	//�p�x�̐��K��
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
//�[�̏���
//==============================================
void CPlayer::StageEdge()
{
	//�X�e�[�W�̒[
	if (m_pos.x - m_AxisBox->GetWidth().x * 2 <= -FIELD_WIDTH)
	{
		m_pos.x = -FIELD_WIDTH + m_AxisBox->GetWidth().x * 2;
	}
	else if (m_pos.x + m_AxisBox->GetWidth().x * 2 >= FIELD_WIDTH)
	{
		m_pos.x = FIELD_WIDTH - m_AxisBox->GetWidth().x * 2;
	}
}
