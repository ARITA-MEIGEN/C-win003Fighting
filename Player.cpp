//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
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

//�ÓI�����o�ϐ�
int CPlayer::m_nPlayer = 0;

//===========================
//�R���X�g���N�^
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	m_nPlayerNumber = m_nPlayer;
	m_nPlayer++;
}

//===========================
//�f�X�g���N�^
//===========================
CPlayer::~CPlayer()
{
	m_nPlayer--;
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

	m_AxisBox = CCollision::Create(m_pos,CCollision::COLLI_AXIS);						//�����o������(�v���C���[�̎�)
	
	//���f���ƃ��[�V�����̓ǂݍ���
	ReadMotion();

	for (int i = 0; i < NUM_PARTS; i++)
	{
		//�v���C���[�̐���
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fPosX,
			m_apMotion[0].aModelKey[0].aKey[i].fPosY,
			m_apMotion[0].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fRotX,
			m_apMotion[0].aModelKey[0].aKey[i].fRotY,
			m_apMotion[0].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}

	//�f�o�b�O�p�ݒ��Ńt�@�C�����o�͂ł���悤�ɂ���
	m_nJump = 50;
	m_nLife = 1000;

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

	CObject::Release();
}

//===========================
//�X�V����
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

	m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
	m_pos += m_move;		//�ʒu�̍X�V
	m_AxisBox->SetPos(m_pos + D3DXVECTOR3(0.0f,m_AxisBox->GetDPos().y,0.0f));
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̍X�V

	ControlPlayer();		//����
	Jump();					//�W�����v
	Axis();					//���̉����o������
	MotionManager();
	AutoTurn();
	//m_pos.y = CGame::GetMesh()->Collision(m_pos);
	DrawCollision();
	SetHitBox();

	CDebugProc::Print("���݂̃v���C���[�̍��W:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("���݂̃��[�V����:%d ", (int)m_Motion);
	if (m_nLife <= 0)
	{//�̗͂��S���Ȃ�����

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

#ifdef _DEBUG
	if (CGame::GetDebugCamera() == true)
	{
		return;
	}
#endif
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//�ړ�
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
			m_State = PST_GROUND;	//������Ԃɖ߂�
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
//�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CPlayer::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//���[�V�����ǂݍ���
//===========================
void CPlayer::ReadMotion()
{
	const int lenLine = 2048;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	char Read[lenLine];			//�ǂݎ��p
	int	m_pEnemynumber = 0;		//���f���̔ԍ�
	int motionnumber = 0;		//���[�V�����̔ԍ�
	int key = 0;
	int m_pEnemy = 0;
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

						m_apModel[m_pEnemynumber]->SetModel(&m_nModelpass[0]);
						m_pEnemynumber++;
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
															m_pEnemy = 0;	//�ԍ����Z�b�g
															break;
														}
														else if (strcmp(&strLine[0], "FRAME") == 0)
														{//�L�[�̍Đ����Ԃ̐ݒ�
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[key].nFrame);	//�Đ����Ԃ̐ݒ�
														}
														else if (strcmp(&strLine[0], "COLLISIONSET") == 0)
														{
															m_apMotion[motionnumber].aModelKey[key].Collision[m_apMotion[motionnumber].aModelKey[key].nNumCollision]
																= CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																											//������̕���
																sscanf(Read, "%s", &strLine);

																//key�̓��f���̃L�[�̔ԍ�
																if (strcmp(&strLine[0], "END_COLLISIONSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumCollision++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//����̊J�n����
																	int start, select;
																	select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &start);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//����̊J�n����
																	int end;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &end);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	D3DXVECTOR3 dpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &dpos.x, &dpos.y, &dpos.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetDPos(dpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  dsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %f%f%f", &strLine, &dsiz.x, &dsiz.y, &dsiz.z);	//�L�[�̑���
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
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																//������̕���
																sscanf(Read, "%s", &strLine);

																//key�̓��f���̃L�[�̔ԍ�
																if (strcmp(&strLine[0], "END_HURTSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumHurtCol++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//����̊J�n����
																	int start;
																	sscanf(Read, "%s = %d", &strLine, &start);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//����̊J�n����
																	int end;
																	sscanf(Read, "%s = %d", &strLine, &end);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	D3DXVECTOR3 hpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &hpos.x, &hpos.y, &hpos.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetDPos(hpos);
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]->SetPos(hpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  hsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %f%f%f", &strLine, &hsiz.x, &hsiz.y, &hsiz.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetSiz(hsiz);
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
																	m_pEnemy++;
																	break;
																}
																else if (strcmp(&strLine[0], "POS") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosX,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosY,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosZ);	//�Đ����Ԃ̐ݒ�
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
void CPlayer::MotionPlayer(int nNumber)
{
	D3DXVECTOR3 RelaPos, RelaRot;		//1�t���[�����Ƃ̈ړ���
	D3DXVECTOR3 pos, rot, DiffPos, DiffRot;
	CDebugProc::Print("���݂̃t���[��:%d", m_frame);

	//�J�E���^�[�X�V
	if (m_nCurrentKey == m_apMotion[m_Motion].nNumKey&& m_apMotion[m_Motion].bLoop == false)
	{
		//�G�f�B�b�g����Ȃ��Ƃ����[�v���[�V�������I�������j���[�g�����ɂ���
		m_bMotion = false;
		m_bAttack = false;
		PlayFirstMotion();
		return;
	}
	else
	{
		for (int i = 0; i < m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nNumCollision; i++)
		{//�_���[�W���肪�o�����邩�ǂ���(1�`4�t���[���܂Ń_���[�W������o���݂�����)
			if (m_frame >= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->GetStartf())
			{//�J�n�t���[���ȏォ�ǂ���
				if (m_frame <= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].Collision[i]->GetEndf())
				{//�I���t���[���ȉ����ǂ���
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
		{//�p�[�c�S���̃��[�V�����Đ�
			if (m_apModel[i] != nullptr)
			{
				if (m_nCurrentKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == true)
				{//���[�v����ꍇ�ŏ��̃��[�V�����Ɉڍs����
					DiffPos = D3DXVECTOR3(
						m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosZ - m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	//�����̎擾

					DiffRot = D3DXVECTOR3(
						m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	//�����̎擾
				}
				else if (m_nCurrentKey == m_apMotion[m_Motion].nNumKey - 1 && m_apMotion[m_Motion].bLoop == false)
				{//���[�v���Ȃ��ꍇ�j���[�g�������[�V�����ɖ߂�
					DiffPos = D3DXVECTOR3(//���W�����̎擾
						m_apMotion[0].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[0].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[0].aModelKey[0].aKey[i].fPosZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//���������̎擾
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
					{//�ʒu���W�̐ݒ肵�Ȃ���
						PlayFirstMotion();
					}
					DiffPos = D3DXVECTOR3(//���W�����̎擾
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fPosX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fPosY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fPosZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//���������̎擾
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fRotX -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fRotY -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[m_Motion].aModelKey[m_nCurrentKey + 1].aKey[i].fRotZ -
						m_apMotion[m_Motion].aModelKey[m_nCurrentKey].aKey[i].fRotZ);
				}
			}

			//�ʒu
			RelaPos = DiffPos / m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nFrame;		//���Βl
			RelaRot = DiffRot / m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nFrame;

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
	
	if (m_frame >= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nFrame)
	{//�L�[�ԍ��̍X�V�ƃJ�E���^�[�̃��Z�b�g
		m_nCurrentKey++;
		m_frame = 0;
		if (m_nCurrentKey >= m_apMotion[m_Motion].nNumKey)
		{//�L�[�ԍ����ő吔�𒴂����ꍇ���Z�b�g
			if (m_apMotion[m_Motion].bLoop == true)
			{
				m_nCurrentKey = 0;
			}
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
		MotionPlayer(m_Motion);		//�v���C���[�̃��[�V����
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
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_Motion].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}
	m_frame = 0;
	m_nCurrentKey = 0;
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
		{
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumCollision; j++)
			{//�Ⴄ���[�V�����̓����蔻����I�t�ɂ���
				if (m_Motion == i&&m_nCurrentKey == k)
				{//�L�[�ƃ��[�V��������v���Ă���ꍇ�̂ݕ\��
					if (m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
					{
						m_apMotion[i].aModelKey[k].Collision[j]->SetUse(true);
						if ( m_pos.x >= m_pEnemy->m_pos.x)
						{//�G���E���̏ꍇ
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
			{//���ꔻ��
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
//�����o������
//=====================
void CPlayer::Axis(void)
{
	if (m_pos.y - m_AxisBox->GetWidth().y / 2 <= m_pEnemy->m_pos.y + m_pEnemy->m_AxisBox->GetWidth().y 
		&& m_pos.y + m_AxisBox->GetWidth().y / 2 >= m_pEnemy->m_pos.y - m_pEnemy->m_AxisBox->GetWidth().y )
	{
			//X��
			//����
			if (m_posold.x + m_AxisBox->GetWidth().x / 2 <= m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x / 2	//�O��̃v���C���[�̈ʒu���u���b�N�̈ʒu���E�ɂ���ꍇ
				&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x / 2 	//���݂̃v���C���[�̈ʒu���u���b�N�̈ʒu��荶�ɂ���(�߂荞��ł���)�ꍇ
				)//�v���C���[�̍��E�Ƀu���b�N�����������ꍇ
			{//�v���C��-�ƃu���b�N������������
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = 0;
			}
			//�E��
			if (m_posold.x - m_AxisBox->GetWidth().x / 2 >= m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x / 2 	//�O��̃v���C���[�̈ʒu���u���b�N�̈ʒu���E�ɂ���ꍇ
				&& m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x / 2	//���݂̃v���C���[�̈ʒu���u���b�N�̈ʒu��荶�ɂ���(�߂荞��ł���)�ꍇ
				)//�v���C���[�̍��E�Ƀu���b�N�����������ꍇ
			{//�v���C��-�ƃu���b�N������������
				m_pos.x -= m_move.x;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = 0;
			}
	}
	if (m_posold.y + m_pEnemy->m_AxisBox->GetWidth().y >= m_pEnemy->m_pos.y + m_pEnemy->m_AxisBox->GetWidth().y
		&& m_pos.y+ m_pEnemy->m_AxisBox->GetWidth().y<= m_pEnemy->m_pos.y + m_pEnemy->m_AxisBox->GetWidth().y )
	{//�W�����v�������̉��������̔���
		if (m_pos.x - m_AxisBox->GetWidth().x / 2 <= m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x / 2
			&& m_pos.x + m_AxisBox->GetWidth().x / 2 >= m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x / 2)
		{
			if (m_pos.x >= m_pEnemy->m_pos.x)
			{//���S��聨
				m_pos.x = m_pEnemy->m_pos.x + m_pEnemy->m_AxisBox->GetWidth().x+1;
				m_AxisBox->SetPos(m_pos + m_AxisBox->GetDPos());
				m_move.x = -m_move.x;
			}
			else
			{//���S��聩
				m_pos.x = m_pEnemy->m_pos.x - m_pEnemy->m_AxisBox->GetWidth().x-1;
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
//�����U�����
//=====================
void CPlayer::AutoTurn(void)
{
	if (m_State == PST_GROUND)
	{//�n�㎞�̂ݑ���̕����ʐU��ނ�
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
//���ꔻ��̐ݒ�
//===========================
void CPlayer::SetHitBox()
{
	for (int i = 0; i < m_apMotion[m_Motion].aModelKey[m_nCurrentKey].nNumHurtCol; i++)
	{//�_���[�W���肪�o�����邩�ǂ���(1�`4�t���[���܂Ń_���[�W������o���݂�����)
		if (m_frame >= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->GetStartf())
		{//�J�n�t���[���ȏォ�ǂ���
			if (m_frame <= m_apMotion[m_Motion].aModelKey[m_nCurrentKey].HurtCol[i]->GetEndf())
			{//�I���t���[���ȉ����ǂ���
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
