//=============================================================================
//
// Xファイル読み込み
// Author : 有田明玄
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//インクルード
#include"main.h"
#include"Object.h"
#include"ObjectX.h"
#include"Model.h"

//前方宣言
class CShadow;
class CModel;

class CPlayer :public CObject
{
public:
	//キー要素
	struct KEY
	{
		float fPosX;
		float fPosY;
		float fPosZ;

		float fRotX;
		float fRotY;
		float fRotZ;
	};

	//キー情報
	struct KEY_SET 
	{
		int nFrame;				//次のモーションまでの時間
		KEY aKey[NUM_PLAYERPARTS];
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//プロトタイプ宣言
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	void			ControlPlayer(void);				//プレイヤーの操作
	static CPlayer*	Create(D3DXVECTOR3 pos, int Priority);
	D3DXMATRIX		GetMtx();
	void			MotionPlayer();
	void			rolling();
private:
	CModel*			m_apModel[NUM_PLAYERPARTS];	//モデルのインスタンス
	D3DXMATRIX		m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3		m_pos;						//位置
	D3DXVECTOR3		m_rot;						//向き
	D3DXVECTOR3		m_move;						//移動量
	D3DXVECTOR3		m_posold;					//前回の位置
	//CShadow*		m_Shadow;					//影
	D3DXVECTOR3		m_rotDest;					//目的の角度の保存
	int				m_MotionCnt;				//モーションカウンター
	int				m_nNumKey;					//キーの総数
	int				m_nCurrentKey;				//現在のキー番号
	D3DXMATRIX		m_mtxRot;					//回転マトリックス(保存用)
	D3DXQUATERNION	m_quat;						//クォータニオン
	D3DXVECTOR3		m_vecAxis;					//回転軸
	float			m_fRolling;					//回転量　(回転角)
};

#endif
