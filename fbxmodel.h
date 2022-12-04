//=============================================================================
//
// Xファイル読み込み
// Author : 有田明玄
//
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//インクルード
#include"main.h"
#include"Object.h"

//前方宣言

class CFBX
{
public:
	 CFBX();
	~CFBX();
	//プロトタイプ宣言
	 int Init();
	 void Uninit(void);
	 void Update(void);
	 void Draw(void);

private:
	FbxManager* m_fbx_manager;
	FbxImporter* m_fbx_importer;
	FbxScene* m_fbx_scene;
};
#endif
