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
class CCollision;


//マクロ定義
#define MAX_MOTION		(5)		//モーションの数
#define MAX_KEY			(60)	//キーの総数
#define MAX_FRAME		(120)	//フレームの最大数
#define NUM_PARTS		(14)	//パーツの数
#define MAX_COLLISION	(20)	//1つのパーツにつき設定できる判定の数
#define JUMP_HEIGHT		(150.0f)	//ジャンプの高さ
#define PLAYER_SPEED	(2.0f)	//移動速度






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
		KEY aKey[NUM_PARTS];		//モデルの数分座標を持つ
		int nFrame;					//再生時間
		CCollision* Collision[MAX_COLLISION];	//当たり判定
		int nNumCollision;						//1つのキーの当たり判定の数
	};

	struct MOTION_SET
	{
		KEY_SET		aModelKey[NUM_PARTS];		//キーの総数分持つ
		int			nNumKey;					//キーの総数(ファイルで読み込む)
		bool		bLoop;						//ループするかどうか
	};

	enum PLAYER_MOTION
	{
		//地上
		PM_ST_NEUTRAL,		//ニュートラル
		PM_ST_MOVE,			//移動(しゃがみだけ無し)
		PM_ST_DASH,			//前ダッシュ
		PM_ST_BACKSTEP,		//バックステップ
		PM_ST_GUARD,		//ガード
		PM_ST_HIT,			//被弾
		PM_ST_LATTACK,		//弱攻撃
		PM_ST_MATTACK,		//中攻撃
		PM_ST_HATTACK,		//強攻撃
		//空中
		PM_JP_NEUTRAL,		//垂直ジャンプ
		PM_JP_MOVELEFT	,	//移動(しゃがみだけ無し)
		PM_JP_MOVERIGHT,	//移動(しゃがみだけ無し)
		PM_JP_DASH,			//前ダッシュ
		PM_JP_GUARD,		//ガード
		PM_JP_HIT,			//被弾
		PM_JP_LATTACK,		//弱攻撃
		PM_JP_MATTACK,		//中攻撃
		PM_JP_HATTACK,		//強攻撃
		//しゃがみ
		PM_CR_NEUTRAL,		//ニュートラル
		PM_CR_MOVE,			//移動(しゃがみだけ無し)
		PM_CR_GUARD,		//ガード
		PM_CR_HIT,			//被弾
		PM_CR_LATTACK,		//弱攻撃
		PM_CR_MATTACK,		//中攻撃
		PM_CR_HATTACK,		//強攻撃
		PM_MAX
	};

	enum PLAYER_STATE
	{
		PST_GROUND,	//地上
		PST_AIR,		//空中
		PST_DAMAGE,	//被弾状態
		PST_MAX
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//プロトタイプ宣言
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	void			ControlPlayer(void);			//プレイヤーの操作
	static CPlayer*	Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	D3DXMATRIX		GetMtx();
	void			ReadMotion();
	void			MotionPlayer(int nNumber);		//モーションの再生　引数は再生するモーションの番号
	void			MotionManager();				//状態に合わせてモーション再生する
	void			PlayFirstMotion();				//前と状態が違う場合のみ最初のモーションを設定する
	void			DrawCollision();				//当たり判定の設定
	void			Axis(void);						//軸の押し出し判定
	void			Jump(void);						//ジャンプ
	void			AutoTurn(void);					//自動振りむき

	//セッター
	void			SetPos(D3DXVECTOR3 pos) { m_pos = pos; };						//位置の設定
	void			SetRot(D3DXVECTOR3 rot) { m_rot = rot; };						//向きの設定
	void			SetEnemy(CPlayer* ene) { m_pEnemy = ene; };						//向きの設定

private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//モデルのインスタンス
	MOTION_SET		m_apMotion[PM_MAX];				//モーションの数だけ生成モーションの数->キーの総数->モデルの数
	D3DXMATRIX		m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3		m_pos;							//位置
	D3DXVECTOR3		m_rot;							//向き
	D3DXVECTOR3		m_move;							//移動量
	D3DXVECTOR3		m_posold;						//前回の位置
	//CShadow*		m_Shadow;						//影
	D3DXVECTOR3		m_rotDest;						//目的の角度の保存
	int				m_MotionCnt;					//モーションカウンター
	int				m_nNumKey;						//キーの総数
	int				m_nCurrentKey;					//現在のキー番号
	D3DXMATRIX		m_mtxRot;						//回転マトリックス(保存用)
	D3DXQUATERNION	m_quat;							//クォータニオン
	D3DXVECTOR3		m_vecAxis;						//回転軸
	float			m_fRolling;						//回転量　(回転角)
	int				m_nNumModel;
	char			m_nModelpass[255];
	D3DXVECTOR3		m_movepos;
	D3DXVECTOR3		m_moverot;
	PLAYER_MOTION	m_Motion;
	PLAYER_MOTION	m_MotionOld;					//ひとつ前のモーション
	CCollision* 	m_AxisBox;						//押し出し判定(プレイヤーの軸)
	bool			m_bMotion;						//モーション再生中かどうか
	static int		m_nPlayer;						//プレイヤー番号
	int				m_nPlayerNumber;				//プレイヤー番号
	int				m_nLife;						//体力
	CPlayer*		m_pEnemy;						//対戦相手のポインタ
	PLAYER_STATE	m_State;						//プレイヤーの状態
	bool			m_bAttack;						//攻撃中かどうか

	//ジャンプ関係
	int				m_nJump;						//ジャンプの全体フレーム
	int				m_nJumpCount;					//ジャンプカウンター
	int				m_nJumpRigor;					//ジャンプの着地硬直


};

#endif
