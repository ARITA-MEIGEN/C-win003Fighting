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

CParticle::CParticle(int nPriority)
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
	m_nSpeed = 2;															//パーティクルの最大速度
	m_nTimer = 0;															//パーティクルの発射感覚
	m_moverot = 314;															//拡散	大きければ大きいほど狭くなる

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
	for (int nCnt = 0; nCnt < m_nNumber; nCnt++)
	{
		float fAngle = ((float)(rand() % m_moverot - m_moverot / 2) + nMovewidthMin) / 100.0f;
		float fSpeed = ((float)(rand() % m_nSpeed)) + 2.0f;
		D3DXVECTOR3 randpos = D3DXVECTOR3((float)(rand() % nStartRandPosX - (nStartRandPosX / 2)), (float)(rand() % nStartRandPosY - (nStartRandPosY / 2)), 0.0f);			//乱数の初期位置
		D3DXVECTOR3 randmove = D3DXVECTOR3(sinf(fAngle)*fSpeed + 0.0f, cosf(fAngle)*fSpeed + 0.0f, 0.0f);
		float randrot = (float)(rand() % 314) / 100 + 0.0f;	//各酸度
		//if (m_pos.x >= CGame::GetPlayer(1 ^ m_PlayerNumber)->GetPos().x)
		//{//2P側で向き逆転
		//	randrot *= -1;
		//}

		CEffect::Create(m_pos+ randpos,					//位置の設定
			m_siz,										//半径の大きさの設定
			m_fRot+ randrot,							//画像の角度
			randmove,									//移動量の設定
			m_nLife,									//寿命
			m_col,										//頂点カラーの設定	
			2,											//使用するテクスチャの番号
			true);										//加算合成するかどうか
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
CParticle * CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, float rot, int life, int numeffect)
{
	CParticle* pParticle = new CParticle(2);

	pParticle->m_pos = pos;
	pParticle->m_pos.y += siz.x / 2;
	pParticle->m_siz = siz;
	pParticle->m_col = col;
	pParticle->m_fRot = rot;
	pParticle->m_nLife = life;
	pParticle->m_nNumber = numeffect;															//一度に放出するパーティクルの数
	pParticle->Init();
	return pParticle;
}
