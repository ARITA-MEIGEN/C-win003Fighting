//=============================================================================
//
// 入力処理 [inputkeydata.h]
// Author : KOZUNA HIROHITO
// 
//=============================================================================

#ifndef _INPUT_KEY_DATA_H_		//このマクロ定義がされなかったら
#define _INPUT_KEY_DATA_H_		//2重インクルード防止のマクロ定義

//----------------------------------------------------------------------------
//列挙型定義
//----------------------------------------------------------------------------

//総合キー一覧
enum STAN_DART_INPUT_KEY
{						//					キーボード						ジョイパッド
	KEY_UP = 0,			//上			 W & UP							スティック上　＆　十字キー上
	KEY_UP_LEFT,		//左上			(W&A)  &  (UP&Left)				スティック左上　＆　（十字キー上　＆　十字キー左）
	KEY_UP_RIGHT,		//右上			(W&D)  &  (UP&Right)			スティック右上　＆　（十字キー上　＆　十字キー右）
	KEY_DOWN,			//下			 S  & Down						スティック下　＆　十字キー下
	KEY_DOWN_LEFT,		//左下			(S&A)  &  (Down&Left)			スティック左下　＆　（十字キー下　＆　十字キー左）
	KEY_DOWN_RIGHT,		//右下			(S&D)  &  (Down&Right)			スティック右下　＆　（十字キー下　＆　十字キー右）
	KEY_LEFT,			//左			 A  &  Left)					スティック左　＆　十字キー左
	KEY_RIGHT,			//右			 D  &  Right					スティック右　＆　十字キー右
	KEY_DECISION,		//決定			 エンターキー								A
	KEY_SHOT,			//スペース		 スペースキー								R1
	KEY_BACK,			//バック		 バックスペースキー & B						B
	KEY_SHIFT,			//シフト		 シフト左右									L1
	KEY_MOVE,			//移動系すべて
	KEY_PAUSE,			//ポーズボタン	 P											START
	KEY_ALL,			//全てのキー
	KEY_MAX
};

//キー種類(ジョイパッド十字キーのみ)
enum JOYKEY_DIRECT_CROSS
{
	JOYKEY_CROSS_UP = 0,
	JOYKEY_CROSS_UP_RIGHT = 4500,
	JOYKEY_CROSS_RIGHT = 9000,
	JOYKEY_CROSS_DOWN_RIGHT = 13500,
	JOYKEY_CROSS_DOWN = 18000,
	JOYKEY_CROSS_DOWN_LEFT = 22500,
	JOYKEY_CROSS_LEFT = 27000,
	JOYKEY_CROSS_UP_LEFT = 31500,
	JOYKEY_CROSS_MAX
};

//キー種類(ジョイパッド)
enum DirectJoypad
{
	JOYPAD_X = 0,				//Xボタン
	JOYPAD_Y,					//Yボタン
	JOYPAD_A,					//Aボタン
	JOYPAD_B,					//Bボタン
	JOYPAD_L1,					//L1ボタン
	JOYPAD_R1,					//R1ボタン
	JOYPAD_L2,					//L2ボタン
	JOYPAD_R2,					//R2ボタン
	JOYPAD_LT,					//左スティック押し込みボタン
	JOYPAD_RT,					//右スティック押し込みボタン
	JOYPAD_BACK,				//バックボタン
	JOYPAD_START,				//スタートボタン
	JOYPAD_HOME,				//ホームボタン
	JOYPAD_UP,					//上
	JOYPAD_UP_LEFT,				//左上
	JOYPAD_UP_RIGHT,			//右上
	JOYPAD_DOWN,				//下
	JOYPAD_DOWN_LEFT,			//左下
	JOYPAD_DOWN_RIGHT,			//右下
	JOYPAD_LEFT,				//左
	JOYPAD_RIGHT,				//右
	JOYPAD_MAX
};

//キー種類(ジョイパッド)
enum InputType
{
	INPUT_TYPE_KEYBOARD = 0,
	INPUT_TYPE_JOYPAD,
	INPUT_TYPE_MOUSE,
	INPUT_TYPE_MAX
};

//キー種類(マウス)
enum MOUSE_KEY
{
	MOUSE_INPUT_LEFT,			//マウス左ボタン
	MOUSE_INPUT_RIGHT,			//マウス右ボタン
	MOUSE_INPUT_MIDDLE,			//マウス中央ボタン
	MOUSE_INPUT_4,				//マウスボタン4
	MOUSE_INPUT_5,				//マウスボタン5
	MOUSE_INPUT_6,				//マウスボタン6
	MOUSE_INPUT_7,				//マウスボタン7
	MOUSE_INPUT_8,				//マウスボタン8
};

#endif

