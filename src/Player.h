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
#include"Command.h"
#include"Collision.h"

//前方宣言
class CShadow;
class CModel;
class CBullet;

//マクロ定義
#define MAX_MOTION			(5)				//モーションの数
#define MAX_KEY				(60)			//キーの総数
#define MAX_FRAME			(120)			//フレームの最大数
#define NUM_PARTS			(14)			//パーツの数
#define MAX_COLLISION		(20)			//1つのパーツにつき設定できる判定の数
#define JUMP_HEIGHT			(130.0f)		//ジャンプの高さ
#define PLAYER_SPEED		(2.0f)			//移動速度
#define FIELD_WIDTH			(380.0f)		//端端の距離
#define JUMP_FACTOR_X		(1.7f)			//ジャンプの移動の係数
#define JUMP_FACTOR_Y		(40.0f)			//ジャンプの移動の係数
#define MAX_SPEED			(10.f)			//ジャンプの最高速度
#define INITIAL_VELOCITY	(7.0f)			//ジャンプの初速
#define MAX_KEYMEMORY		(60)			//記憶するキーの数
#define	DASH_SPEED			(5.0f)			//ダッシュ速度
#define BULLET_ANGLE		(1.0f)			//弾の角度
#define	BACKSTEP_MOVE_X		(5.0f)			//バックステップの移動量
#define	BACKSTEP_MOVE_Y		(2.0f)			//バックステップの移動量
#define HIT_EFFECTSIZ		(25.0f)			//ヒットエフェクトの大きさ
#define GUARD_EFFECTSIZ		(30.0f)			//ガードエフェクトの大きさ



class CPlayer :public CObject
{
public:
	//キー要素
	struct KEY
	{
		D3DXVECTOR3 fPos;
		D3DXVECTOR3 fRot;
	};

	//キー情報
	struct KEY_SET 
	{
		KEY aKey[NUM_PARTS];					//モデルの数分座標を持つ
		int nFrame;								//再生時間
		CCollision* Collision[MAX_COLLISION];	//当たり判定
		int nNumCollision;						//1つのキーの当たり判定の数
		CCollision* HurtCol[MAX_COLLISION];		//やられ判定
		int nNumHurtCol;						//1つのキーの当たり判定の数
	};

	struct MOTION_SET
	{
		KEY_SET		aKey[NUM_PARTS];		//キーの総数分持つ
		int			nNumKey;					//キーの総数(ファイルで読み込む)
		bool		bLoop;						//ループするかどうか
		int			nHitStopTimer;				//ヒットストップの時間
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
		PM_ST_HURT,			//やられモーション

		//空中
		PM_JP_NEUTRAL,		//垂直ジャンプ
		PM_JP_MOVEFORWARD,	//前ジャンプ
		PM_JP_MOVEBACK,		//バックジャンプ
		PM_JP_DASH,			//前ダッシュ
		PM_JP_GUARD,		//ガード
		PM_JP_HIT,			//被弾
		PM_JP_LATTACK,		//弱攻撃
		PM_JP_MATTACK,		//中攻撃
		PM_JP_HATTACK,		//強攻撃
		PM_JP_HURT,			//やられモーション

		//しゃがみ
		PM_CR_NEUTRAL,		//ニュートラル
		PM_CR_MOVE,			//移動(しゃがみだけ無し)
		PM_CR_GUARD,		//ガード
		PM_CR_HIT,			//被弾
		PM_CR_LATTACK,		//弱攻撃
		PM_CR_MATTACK,		//中攻撃
		PM_CR_HATTACK,		//強攻撃
		PM_CR_HURT,			//やられモーション

		//必殺技
		PM_236L,			//弱波動
		PM_236M,			//中波動
		PM_236H,			//強波動

		PM_214L,			//弱竜巻
		PM_214M,			//中竜巻
		PM_214H,			//強竜巻

		PM_623L,			//弱昇竜
		PM_623M,			//中昇竜
		PM_623H,			//強昇竜

		//死亡
		PM_ST_DIE,			//死
		PM_CR_DIE,			//死
		PM_JP_DIE,			//死

		PM_SLOW,			//投げ
		PM_DOWN,			//死亡&ダウン時モーション
		PM_STANDUP,			//起き上がり
		PM_MAX
	};

	enum PLAYER_STATE
	{
		PST_STAND,	//立ち
		PST_CROUCH,	//しゃがみ
		PST_AIR,	//空中
		PST_DIE,	//被弾状態
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
	D3DXMATRIX		GetMtx() { return m_mtxWorld; };		//マトリックスの取得
	void			ReadMotion();
	void			MotionPlayer();							//モーションの再生　引数は再生するモーションの番号
	void			MotionManager();						//状態に合わせてモーション再生する
	void			PlayFirstMotion();						//前と状態が違う場合のみ最初のモーションを設定する
	void			DrawCollision();						//当たり判定の設定
	void			Axis(void);								//軸の押し出し判定
	void			Jump(void);								//ジャンプ
	void			AutoTurn(void);							//自動振りむき
	void			Damage();								//ダメージ処理
	bool			Guard(CCollision::EDAMAGE_POINT dp);	//ガード処理
	void			Command();								//コマンド処理	
	bool			CheckInput(const int *command);			//コマンドの入力判定
	void			StateManagement();						//プレイヤーの状態管理
	void			Input();								//入力処理
	void			Updatepos();							//座標の更新
	void			Cancel();								//攻撃キャンセル
	bool			ColJudge(int hurtnumber,int colnum);	//当たり判定チェック
	void			Down();									//着地設定
	void			Damage_Cal(int Damage, CCollision::EDAMAGE_POINT pro, int HitRig, int GuardRig, bool knockback);							//ダメージ設定
	void			Die();
	void			FireBall();								//遠距離技
	void			Slow();									//投げ
	void			Normalization();						//正規化
	void			StageEdge();							//ステージの端の処理

	//セッター
	void			SetPos(D3DXVECTOR3 pos) { m_pos = pos; };						//位置の設定
	void			SetRot(D3DXVECTOR3 rot) { m_rot = rot; };						//向きの設定
	void			SetEnemy(CPlayer* ene) { m_pEne = ene; };						//敵のポインタ
	void			SetBullet(bool bullet) { m_bBullet = bullet; };					//弾を使用しているかどうか
	void			SetAttack(bool atk) { m_bAttack = atk; };						//弾を使用しているかどうか


	//ゲッター
	D3DXVECTOR3		GetPos() { return m_pos; };
	int				GetLife() { return m_nLife; };	//体力
	bool			GetBullet() { return m_bBullet ; };					//弾を使用しているかどうか
	CBullet*		GetBulletState() { return m_pBullet; };	//弾の情報取得
	PLAYER_MOTION	GetNowMotion() { return m_Motion; };

	//当たり判定の取得
	CCollision*		GetCollision(int number) { return m_apMotion[m_Motion].aKey[m_nCurKey].Collision[number]; };

	//やられ判定の取得
	CCollision*		GetHurt(int number) { return m_apMotion[m_Motion].aKey[m_nCurKey].HurtCol[number]; };


private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//モデルのインスタンス
	MOTION_SET		m_apMotion[PM_MAX];				//モーションの数だけ生成モーションの数->キーの総数->モデルの数
	D3DXMATRIX		m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3		m_pos;							//位置
	D3DXVECTOR3		m_rot;							//向き
	D3DXVECTOR3		m_move;							//移動量
	D3DXVECTOR3		m_posold;						//前回の位置
	D3DXVECTOR3		m_rotDest;						//目的の角度の保存
	int				m_MotionCnt;					//モーションカウンター
	int				m_nNumKey;						//キーの総数
	int				m_nCurKey;						//現在のキー番号
	int				m_nNumModel;					//読み込むモデルの数
	char			m_nModelpass[255];				//読み込むモデルのパス
	PLAYER_MOTION	m_Motion;						//現在のモーション
	PLAYER_MOTION	m_MotionOld;					//ひとつ前のモーション
	PLAYER_MOTION	m_NextMotion;					//キャンセルで発動する技
	bool			m_bMotion;						//モーション再生中かどうか
	static int		m_nNumPlayer;					//プレイヤーの数
	int				m_nPlayerNumber;				//自分のプレイヤー番号
	CPlayer*		m_pEne;							//対戦相手のポインタ
	PLAYER_STATE	m_State;						//プレイヤーの状態
	bool			m_bAttack;						//攻撃中かどうか
	int				m_nLife;						//体力
	bool			m_bSide;						//どっちを向いてるか(trueなら←)
	int				m_anInput[MAX_KEYMEMORY];		//コマンド認識用
	int				m_nNowKey;						//キー保存用
	int				m_nHitStop;						//ヒットストップの時間
	bool			m_bBullet;						//飛び道具を使用しているか？
	CBullet*		m_pBullet;						//飛び道具
	CShadow*		m_pShadow;						//影
	bool			m_bDamage;						//ダメージフラグ

	//押し出し判定関連
	CCollision* 	m_AxisBox;						//押し出し判定(プレイヤーの軸)
	D3DXVECTOR3		m_aAxisSiz[PST_MAX];			//押し出し判定の大きさ

	//ジャンプ関係
	int				m_nJump;						//ジャンプの全体フレーム
	int				m_nJumpCount;					//ジャンプカウンター
	int				m_nJumpRigor;					//ジャンプの着地硬直
	bool			m_bJump;						//ジャンプ中かどうか
	int				m_nJumpTransTime;				//ジャンプ移行フレーム
	//ガード関係
	int				m_nRig;							//硬直時間
};

#endif
