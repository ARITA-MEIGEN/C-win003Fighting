//=============================================================================
//
// X�t�@�C���ǂݍ���
// Author : �L�c����
//
//=============================================================================

#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"

//�O���錾

class CFBX
{
public:
	 CFBX();
	~CFBX();
	//�v���g�^�C�v�錾
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
