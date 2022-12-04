//=============================================================================
//
// モーション読み込み
// Author : 有田明玄
//
//=============================================================================

#include"Motion.h"

CMotion::CMotion(int nPriority)
{
}

CMotion::~CMotion()
{
}

HRESULT CMotion::Init()
{
	return E_NOTIMPL;
}

void CMotion::Uninit()
{
}

void CMotion::Update()
{
	
}

void CMotion::Read(char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int Frame;
	int DiffF;
	m_NowKey;
	m_MotionKey[m_NowKey];

	//ファイル読み込み

	for (int i = 0; i < MAX_PARTS; i++)
	{//モデルパーツ数分ループ
		if (m_MotionKey[i].aKey != nullptr)
		{
			//モデルの位置を設定
			//モデルの向きを設定
		}
	}

	{
		{//パーツ数分用意する
			DiffF = Frame / m_MotionKey[0].nFrame;	//現在のフレーム(再生時間)/全体フレーム=差分

			{
				D3DXVECTOR3 Diffpos = pos - m_MotionKey[m_NowKey].aKey[0].pos;//現在地と目的地の差分を生成
				D3DXVECTOR3 Diffrot = rot - m_MotionKey[m_NowKey].aKey[0].rot;//現在地と目的地の差分を生成

				Diffpos /= Frame;
				Diffrot /= Frame;
			}		//現在地
			{
				m_MotionKey[m_NowKey].aKey[0].pos;
				m_MotionKey[m_NowKey].aKey[0].rot;	//目的地
			}
		}

		{
			//フレーム数、
			{
				pos; rot;
			}		//パーツ数分用意する
		}


	}
}
