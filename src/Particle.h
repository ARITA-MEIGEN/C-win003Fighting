#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include"main.h"
#include"Object2D.h"
#include"Object.h"

class CParticle :public CObject
{
public:

	enum Particle
	{
		PAR_FIREFLOWER = 0,	//火花型
		MAX_PARTICLE
	};
	explicit CParticle(int nPriority = 0);
	~CParticle();
	//プロトタイプ宣言
	HRESULT Init(void);		//パーティクルの初期化処理
	void Uninit(void);		//パーティクルの終了処理
	void Update(void);		//パーティクルの更新処理
	void Draw(void);		//パーティクルの更新処理
	static CParticle * Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, D3DXCOLOR col, Particle type);	//パーティクルの情報の取得

	//ゲッター
	D3DXVECTOR3	GetPos() { return m_pos; };

private:
	D3DXVECTOR3 m_pos;		//位置
	D3DXCOLOR m_col;		//頂点カラー
	int m_nTimer;			//タイマー(エフェクトの発射間隔)
	int  m_nSpeed;			//エフェクトの速度
	int  m_nNumber;			//エフェクトの数
	D3DXVECTOR3  m_siz;		//大きさ
	int  m_nLife;			//寿命
	float m_fRot;			//角度
	int m_moverot;
	int nMovewidthMin;		//最低値
	int nStartRandPosX;	//初期位置の乱数
	int nStartRandPosY;	//初期位置の乱数
	Particle m_ParticleType;
};


#endif 

#pragma once

