//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================

//インクルード
#include "Object.h"
#include"Application.h"
#include"renderer.h"
#include"InputKeyBoard.h"

//静的メンバ変数宣言
 int CObject::m_nNumAll=0;
 CObject*CObject::m_pTop[MAX_TYPE] = {nullptr};
 CObject*CObject::m_pCurrent[MAX_TYPE] = { nullptr };
//=============================================================================
// コンストラクタ
//=============================================================================
 CObject::CObject(int nPriority /*=3*/)
 {
	 CObject*pObject;
	 pObject = this;

	 //自身を代入
	 if (m_pTop[nPriority] == nullptr)
	 {//最初の一回目の場合
		 m_pTop[nPriority] = pObject;
	 }

	 if (m_pCurrent[nPriority] != nullptr)
	 {//最後尾の設定
		 m_pCurrent[nPriority]->m_pNext = pObject;
		 pObject->m_pPrev = m_pCurrent[nPriority];
	 }
	 pObject->bDead = false;
	 m_pCurrent[nPriority] = pObject;			//最後尾に自分を設定
	 m_nNumAll++;
 }

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{
	m_nNumAll--;
}

//=============================================================================
//全ての終了
//=============================================================================
void CObject::ReleaseAll()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		if (!m_pTop[i])
		{//Topがnullなら
			continue;
		}

		CObject*pObject = m_pTop[i];

		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			pObject->Uninit();
			pObject = pObjectNext;
		}
	}
	Flagcheck();
}

//=============================================================================
//全ての更新
//=============================================================================
void CObject::UpdateAll()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		if (!m_pTop[i])
		{//Topがnullなら
			continue;
		}

		CObject*pObject = m_pTop[i];

		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			if (pObject->bDead == false)
			{
				pObject->Update();
			}
			pObject = pObjectNext;
		}
	}

	Flagcheck();
}

//=============================================================================
// 全ての描画
//=============================================================================
void CObject::DrawAll()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		CObject*pObject = m_pTop[i];
		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			pObject->Draw();
			pObject = pObjectNext;
		}
	}
}

////=============================================================================
//// オブジェクトの取得
////=============================================================================
//CObject* CObject::GetObj(int m_nNumber,int nType)
//{
//	return m_pObject[m_nNumber];
//}

////=============================================================================
//// オブジェクトの種類の取得
////=============================================================================
//CObject::EObjType CObject::GetObjType(int m_nNumber, int nType)
//{
//	return m_pObject[m_nNumber]->m_ObjType;
//}

//=============================================================================
//全体の取得
//=============================================================================
int CObject::NumAll()
{
	return m_nNumAll;
}

//=============================================================================
// 単体の終了処理
//=============================================================================
void CObject::Release()
{
	CObject*pObject = this;
	if (pObject != nullptr)
	{
		pObject->bDead = true;
	}
}

//=============================================================================
// 死亡フラグの確認
//=============================================================================
void CObject::Flagcheck()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		CObject*pObject = m_pTop[i];
		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			if (pObject->bDead == true)
			{//死亡フラグがついてる場合
				//自分のオブジェクトの前後を連結させる
				if (m_pTop[i] == m_pCurrent[i])
				{
					m_pTop[i] = nullptr;
					m_pCurrent[i] = nullptr;
				}
				else if (m_pTop[i] == pObject)
				{//トップを削除する場合
					m_pTop[i] = pObjectNext;
					m_pTop[i]->m_pPrev = nullptr;
				}
				else if (m_pCurrent[i] == pObject)
				{//最後尾のオブジェクトを削除する場合
					m_pCurrent[i] = pObject->m_pPrev;
					pObject->m_pPrev->m_pNext = nullptr;
					pObject->m_pPrev = nullptr;
				}
				else
				{//リストから破棄
					pObject->m_pPrev->m_pNext = pObject->m_pNext;
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;
				}
				//自身をを削除
				delete pObject;
				pObject = nullptr;
			}
			//次のオブジェクトへ移行
			pObject = pObjectNext;
		}
	}
}

//
////=============================================================================
//// 種類の設定
////=============================================================================
//void CObject::SetObjType(EObjType Type)
//{
//	m_ObjType = Type;
//}
