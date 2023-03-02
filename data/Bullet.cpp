//=================================================
// Content     (パーティクルの処理)(Particle.cpp)
// Author     : 有田明玄
// Since      : (作成日2021/09/07)
//=================================================
#include"Bullet.h"
#include"effect.h"
#include"Application.h"
#include"renderer.h"
#include"Game.h"
#include"Collision.h"
#include"Player.h"
#include"sound.h"
#include"Camera.h"
#include"Time.h"

CBullet::CBullet(int nPriority)
{
}
CBullet::~CBullet()
{
}
//============================
//パーティクルの初期化処理
//============================
HRESULT CBullet::Init()
{

	m_nNumber = 2;															//一度に放出するパーティクルの数
	m_nSpeed = 2;															//パーティクルの最大速度
	m_nTimer = 0;															//パーティクルの発射感覚
	m_moverot = 1;															//拡散	大きければ大きいほど狭くなる
	m_Collision = CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
	m_Collision->SetPos(m_pos);
	m_Collision->SetSiz(D3DXVECTOR3(50.0f,50.0f,10.0f));
	PlaySound(SOUND_LABEL_SE_FIRE);
	
	return S_OK;
}

//===========================
//パーティクルの終了処理
//===========================
void CBullet::Uninit(void)
{
	CGame::GetPlayer(m_PlayerNumber)->SetBullet(false);
	m_Collision->SetUse(false);
	m_Collision->Uninit();


	CObject::Release();
}

//===========================
//パーティクルの更新処理
//===========================
void CBullet::Update(void)
{
	if (CGame::GetTimer()->GetTimer() < DEFAULT_TIME&& CGame::GetTimer()->GetTimer() > 0)
	{
		m_nTimer++;

		//移動量の設定
		for (int nCnt = 0; nCnt < m_nNumber; nCnt++)
		{
			float fAngle = ((float)(rand() % m_moverot - m_moverot / 2) + KAKUDO) / 100.0f;
			float fSpeed = ((float)(rand() % m_nSpeed)) + 2.0f;
			D3DXVECTOR3 randpos = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), 0.0f);
			D3DXVECTOR3 randmove = D3DXVECTOR3(sinf(fAngle)*fSpeed + 0.0f, cosf(fAngle)*fSpeed + 0.0f, 0.0f);
			float randrot = (float)(rand() % KAKUSANDO) / KAKUSANDOB + 0.0f;	//各酸度

			if (m_pos.x >= CGame::GetPlayer(1 ^ m_PlayerNumber)->GetPos().x)
			{//2P側で向き逆転
				randrot *= -1;
			}

			CEffect::Create(m_pos,							//位置の設定
				m_siz,										//半径の大きさの設定
				m_fRot + randrot,							//画像の角度
				randmove*0.0f,								//移動量の設定
				m_nLife,									//寿命
				m_col,										//頂点カラーの設定	
				0,											//使用するテクスチャの番号
				true);										//加算合成するかどうか
		}

		//位置更新
		m_pos += m_move;
		m_Collision->SetPos(m_pos);
		Hit();

		float Centerpos = (CGame::GetPlayer(1)->GetPos().x - CGame::GetPlayer(0)->GetPos().x) / 2 + CGame::GetPlayer(0)->GetPos().x;

		if (Centerpos + FIELD_WIDTH< m_pos.x || Centerpos - FIELD_WIDTH  > m_pos.x)
		{//画面外に出たときの処理
			Uninit();
			return;
		}
	}
}

void CBullet::Draw(void)
{
}

//===========================
//弾の生成
//===========================
CBullet * CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col,D3DXVECTOR3 move, float rot, int life,int PlayerNumber)
{
	CBullet* pParticle = new CBullet(2);

	pParticle->m_pos = pos;
	pParticle->m_pos.y += siz.x/2;
	pParticle->m_siz = siz;
	pParticle->m_col = col;
	pParticle->m_move = move;
	pParticle->m_fRot = rot;
	pParticle->m_nLife = life;
	pParticle->m_PlayerNumber = PlayerNumber;
	pParticle->Init();
	return pParticle;
}

//===========================
//弾の当たり判定
//===========================
void CBullet::Hit()
{
	CPlayer* pPlayer = CGame::GetPlayer(1 ^ m_PlayerNumber);
	int i = 0;
	D3DXVECTOR3 Hurt, col, HurtWidth, colWidth;

	//敵の弾の取得

	col = m_Collision->GetPos();
	colWidth = m_Collision->GetWidth();

	//相殺判定
	if (pPlayer->GetBullet() ==true)
	{//敵が弾を発射している場合のみ通る
		Hurt = pPlayer->GetBulletState()->m_Collision->GetPos();
		HurtWidth = pPlayer->GetBulletState()->m_Collision->GetWidth();
		if (Hurt.x + HurtWidth.x / 2 >= col.x - colWidth.x / 2 &&
			Hurt.x - HurtWidth.x / 2 <= col.x + colWidth.x / 2 &&
			Hurt.y + HurtWidth.y / 2 >= col.y - colWidth.y / 2 &&
			Hurt.y - HurtWidth.y / 2 <= col.y + colWidth.y / 2)
		{
			Uninit();
			pPlayer->GetBulletState()->Uninit();
			return;
		}
	}
	//敵の当たり判定を参照
	while (pPlayer->GetHurt(i) != nullptr)
	{
		CCollision*pCollision = pPlayer->GetHurt(i);
		Hurt = pCollision->GetPos();
		HurtWidth = pCollision->GetWidth();
		col = m_Collision->GetPos();
		colWidth = m_Collision->GetWidth();

		if (Hurt.x + HurtWidth.x / 2 >= col.x - colWidth.x / 2 &&
			Hurt.x - HurtWidth.x / 2 <= col.x + colWidth.x / 2 &&
			Hurt.y + HurtWidth.y / 2 >= col.y - colWidth.y / 2 &&
			Hurt.y - HurtWidth.y / 2 <= col.y + colWidth.y / 2)
		{
			pPlayer->Damage_Cal(DAMAGE, CCollision::EDAMAGE_POINT::DP_HIGH, 10, 10,false);
			PlaySound(SOUND_LABEL_SE_SPECIAL);
			Uninit();
			return;
		}
		i++;
	}
}
