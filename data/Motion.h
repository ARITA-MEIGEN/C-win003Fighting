//=============================================================================
//
// モーション読み込み
// Author : 有田明玄
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_
                               
//インクルード

#define MAX_PARTS	(14)	//パーツの数

class CMotion
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int MAX_MOTION = 320;
	static const int MAX_KEY = 240;
	static const int MAX_MOTION_ALL = 160;

public:
	//構造体
	struct KEY
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	//キー情報
	struct KEY_SET 
	{
		int nFrame;				//次のモーションまでの時間
		KEY aKey[MAX_PARTS];
	};

	//メンバ関数
	 CMotion(int nPriority = 3);
	~CMotion();
	HRESULT Init();						//初期化
	void Uninit();						//終了
	void Update();						//更新
	void Read(char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);			//読み込み
	KEY_SET m_MotionKey[MAX_MOTION];	//登録した動きの情報
protected:
private:
	int m_NowKey;	//現在のキー

	//メンバ変数

};

#endif // !_OBJECT_H_

