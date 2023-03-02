//=================================================
// Content     (ゲームの設定)(Line.h)
// Author     : 有田明玄
//=================================================
#ifndef _LINE_H_		//このマクロ定義がされてなかったら
#define _LINE_H_		//二重インクルード防止のマクロ定義
#include"main.h"
#include"Object.h"

//モデルの構造体
class CCollision :public CObject
{
public:
	enum COLLISION
	{
		COLLI_AXIS = 0,		//軸の判定
		COLLI_DAMAGE,		//ダメージ判定
		COLLI_SLOW,			//投げ判定
		COLLI_HURT,			//やられ判定
		COLLI_HURTSLOW,		//投げのやられ判定
	};

	//攻撃の属性
	enum EDAMAGE_POINT
	{
		DP_HIGH = 0,		//上段
		DP_MIDDLE,			//中段
		DP_LOW,				//下段
		DP_MAX,
	};

	explicit CCollision(int nPriority = 3);
	~CCollision();

	//プロトタイプ宣言
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CCollision* Create(D3DXVECTOR3 pos, COLLISION type);
	//ゲッターとセッター

	//セッター
	void SetPos(D3DXVECTOR3(pos)) { m_pos = pos; };
	void SetSiz(D3DXVECTOR3 size) { m_width = size; };
	void SetUse(bool use) { m_bUse = use; };					//オンオフの切り替え
	void SetStartf(int start) { m_nStart = start; };			//開始フレーム
	void SetEndf(int end) { m_nEnd = end; };					//終了フレーム
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; };				//向き
	void SetDPos(D3DXVECTOR3 pos) { m_defaultpos = pos; };		//初期位置
	void SetDRot(D3DXVECTOR3 rot) { m_defaultrot = rot; };		//初期向き
	void SetDP(EDAMAGE_POINT pro) { m_Property = pro; };		//属性の設定
	void SetDmg(bool dmg) { m_bDmg = dmg; };					//ダメージ処理を無効にするかどうか
	void SetHitRig(int dmgrig) { m_nHitRig = dmgrig; };			//ダメージ硬直の設定
	void SetGuardRig(int dmgrig) { m_nGuardRig = dmgrig; };		//ガード硬直の設定



	//ゲッター
	int				GetStartf() { return m_nStart; };			//開始フレーム
	int				GetEndf() { return m_nEnd; };				//終了フレーム
	D3DXVECTOR3		GetRot() { return m_rot; };					//向き
	D3DXVECTOR3		GetDPos() { return m_defaultpos; };			//初期位置
	D3DXVECTOR3		GetDRot() { return m_defaultrot; };			//初期向き
	int				GetDamage() { return m_nDamage; };			//ダメージ量の取得
	EDAMAGE_POINT	GetProperty() { return m_Property; };		//属性の取得
	bool			GetUse() { return m_bUse; };				//オンオフの切り替え
	bool			GetUseDmg() { return m_bDmg; };				//ダメージ処理を無効にするかどうか
	D3DXVECTOR3		GetWidth() { return m_width; };				//当たり判定の大きさ
	D3DXVECTOR3		GetPos() { return m_pos; };					//当たり判定の位置
	int				GetHitRig() { return m_nHitRig; };			//ダメージ硬直の取得
	int				GetGuardRig() { return m_nGuardRig; };		//ガード硬直の取得

private:
	D3DXVECTOR3				m_pos;			//位置
	D3DXVECTOR3				m_posold;		//位置
	D3DXVECTOR3				m_defaultpos;	//位置

	D3DXVECTOR3				m_width;		//幅
	D3DXVECTOR3				m_rot;			//向き
	D3DXVECTOR3				m_defaultrot;	//向き

	D3DXCOLOR				m_col;			//色
	D3DXMATRIX				m_mtxWorld;		//ワールドマトリックス
	bool					m_bUse;			//使用しているかどうか
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファ
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		//インデックスバッファ
	COLLISION				m_Colltype;		//当たり判定の種類
	int						m_nStart;		//開始フレーム
	int						m_nEnd;			//終了フレーム
	int						m_nDamage;		//ダメージ量
	EDAMAGE_POINT			m_Property;		//攻撃の属性
	bool					m_bDmg;			//ダメージのオンオフ切り替え
	int						m_nHitRig;		//ダメージ硬直
	int						m_nGuardRig;	//ガード硬直
};

#endif



