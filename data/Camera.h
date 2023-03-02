//=================================================
// Content     (ゲームの設定)(camera.h)
// Author     : 有田明玄
//=================================================
#ifndef _CAMERA_H_		//このマクロ定義がされてなかったら
#define _CAMERA_H_		//二重インクルード防止のマクロ定義
#include"main.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	//プロトタイプ宣言
	void Init(void);				//初期化
	void Uninit(void);				//終了
	void Update(void);				//更新
	void Set(void);					//設定
	void RadianPosR(void);			//注視点の旋回
	void RadianPosV(void);			//視点の旋回
	void NormalizeRadian(void);		//角度の正規化
	void ControlPos(void);			//カメラの位置の操作
	void DestPos(void);				//カメラの追従処理
	//void LeavingCamera(void);		//カメラの放置処理
	D3DXVECTOR3 Getrot(void);		//向き取得

	static CCamera* Create(void);	//生成

private:
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posVDest;		//視点の目的地
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_posRDest;		//注視点の目的地
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_rotDest;		//向きの目的の角度
	float m_fDistance;			//距離
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			//ビューマトリックス

};

#endif
