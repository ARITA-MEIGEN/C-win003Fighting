//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
                               
//インクルード
#include"main.h"

#define MAX_TYPE (OBJTYPE_MAX+1)

class CObject
{
public:
	//構造体
	enum EObjType
	{
		OBJTYPE_MAP,
		OBJTYPE_PLANET,
		OBJTYPE_GOAL,
		OBJTYPE_BLOCK,
		OBJTYPE_MODEL,
		OBJTYPE_GIMMICK,
		OBJTYPE_UI,
		OBJTYPE_FADE,
		OBJTYPE_MAX,
	};

	//メンバ関数
	explicit CObject(int nPriority = 3);
	virtual ~CObject();
	virtual HRESULT Init() = 0;	//初期化
	virtual void Uninit() = 0;								//終了
	virtual void Update() = 0;								//更新
	virtual void Draw() = 0;								//描画
	static void ReleaseAll();								//全体の破棄
	static void UpdateAll();								//全体の更新
	static void DrawAll();									//全体の描画
	//static CObject* GetObj(int m_nNumber, int nType);		//オブジェクトの取得　引数はオブジェクトの番号
	//EObjType GetObjType(int m_nNumber, int nType);		//オブジェクトの種類の設定	
	static int NumAll();									//全体の数の取得
	void Release();											//破棄
	//void SetObjType(EObjType Type);						//種類の設定
	//static void test();									//テスト用
	static void Flagcheck();								//死亡フラグの確認
protected:
	int m_frame;											//生成されてからの時間

private:
	//メンバ変数
	static int m_nNumAll;									//全体の数
	int m_nPriority;										//描画の優先順位
	static CObject*m_pTop[MAX_TYPE];						//先頭のオブジェクトへのポインタ
	static CObject*m_pCurrent[MAX_TYPE];					//現在(一番後ろ)のオブジェクトへのポインタ
	CObject*m_pPrev;										//前のオブジェクトへのポインタ
	CObject*m_pNext;										//次のオブジェクトへの
	bool bDead;												//死亡フラグ

};

#endif // !_OBJECT_H_
