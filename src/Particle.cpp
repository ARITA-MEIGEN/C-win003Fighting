//=================================================
// Content     (パーティクルの処理)(Particle.cpp)
// Author     : 有田明玄
// Since      : (作成日2021/09/07)
//=================================================
#include"Particle.h"
#include"effect.h"
#include"Application.h"
#include"renderer.h"
#include"Game.h"
#include"Player.h"

CParticle::CParticle(int nPriority):CObject(nPriority)
{
}
CParticle::~CParticle()
{
}
//============================
//パーティクルの初期化処理
//============================
HRESULT CParticle::Init()
{
	m_nSpeed = 1;															//パーティクルの最大速度
	m_nTimer = 0;															//パーティクルの発射感覚
	m_moverot = 628;														//拡散	大きければ大きいほど狭くなる

	nStartRandPosX = 20;	//初期位置の乱数
	nStartRandPosY = 20;	//初期位置の乱数
	nMovewidthMin = 0;
	return S_OK;
}

//===========================
//パーティクルの終了処理
//===========================
void CParticle::Uninit(void)
{
	CObject::Release();
}

//===========================
//パーティクルの更新処理
//===========================
void CParticle::Update(void)
{
	//移動量の設定
	switch (m_ParticleType)
	{
	case CParticle::PAR_FIREFLOWER:
			m_nLife = 10;
			m_nNumber = 100;
			m_fRot = D3DX_PI*BULLET_ANGLE;

		break;
	case CParticle::MAX_PARTICLE:
		break;
	default:
		break;
	}

	for (int i = 0; i < m_nNumber; i++)
	{
		float fAngle = ((float)(rand() % m_moverot - m_moverot / 2) + nMovewidthMin) / 100.0f;
		float fSpeed = ((float)(rand() % m_nSpeed)) + 2.0f;
		D3DXVECTOR3 randpos = D3DXVECTOR3((float)(rand() % nStartRandPosX - (nStartRandPosX / 2)), (float)(rand() % nStartRandPosY - (nStartRandPosY / 2)), 0.0f);			//乱数の初期位置
		D3DXVECTOR3 randmove = D3DXVECTOR3(sinf(fAngle)*fSpeed + 0.0f, cosf(fAngle)*fSpeed + 0.0f, 0.0f);
		float randrot = (float)(rand() % 314) / 100 + 0.0f;	//拡散度

		CEffect::Create(m_pos + randpos,				//位置の設定
			m_siz,										//半径の大きさの設定
			m_fRot+ randrot,							//画像の角度
			randmove,									//ランダムな移動量
			m_nLife,									//寿命
			m_col,										//頂点カラーの設定	
			1,
			true);
	}
	

	Uninit();
	return;
}

void CParticle::Draw(void)
{
}

//===========================
//弾の生成
//===========================
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, Particle type)
{
	CParticle* pParticle = new CParticle(2);
	pParticle->Init();
	pParticle->m_pos = pos;
	pParticle->m_pos.y += siz.x / 2;
	pParticle->m_siz = siz;
	pParticle->m_col = col;
	pParticle->m_ParticleType = type;
	return pParticle;
}
