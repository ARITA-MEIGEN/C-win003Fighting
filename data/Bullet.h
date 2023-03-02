#ifndef _BULLET_H_
#define _BULLET_H_

#include"main.h"
#include"Object2D.h"
#include"Object.h"

#define KAKUDO	(-100)			//移動方向
#define KAKUSANDO (20)			//幅の角度
#define KAKUSANDOB (10)			//幅の角度
#define	POS_ADJUSTMENT	(30.0f)	//微調整用
#define	DAMAGE	(70)	//微調整用

class CCollision;

class CBullet:public CObject
{
public:
	explicit CBullet(int nPriority = 0);
	~CBullet();
	//プロトタイプ宣言
	HRESULT Init(void);		//パーティクルの初期化処理
	void Uninit(void);		//パーティクルの終了処理
	void Update(void);		//パーティクルの更新処理
	void Draw(void);		//パーティクルの更新処理

	static CBullet * Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, D3DXVECTOR3 move,float rot,int life, int PlayerNumber);	//パーティクルの情報の取得

	void Hit();	//パーティクルの情報の取得

	//ゲッター
	D3DXVECTOR3	GetPos() { return m_pos; };

private:
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3  m_move;	//移動量
	D3DXCOLOR m_col;		//頂点カラー
	int m_nTimer;			//タイマー(エフェクトの発射間隔)
	int  m_nSpeed;			//エフェクトの速度
	int  m_nNumber;			//エフェクトの数
	D3DXVECTOR3  m_siz;		//大きさ
	int  m_nLife;			//寿命
	float m_fRot;			//角度
	int m_moverot;			
	CCollision *	m_Collision;	//当たり判定
	int m_PlayerNumber;		//プレイヤー番号
};


#endif 

#pragma once
