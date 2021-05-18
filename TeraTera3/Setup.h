/**
 * @file Setup.h
 * @author jupiter ()
 * @brief 広範囲に影響するマクロなどを記入するヘッダー
 * @n デバッグ時のフラグもここで建てる
 */

#pragma once

///フラグをつける際の注意事項
///DebugLogに描画系は描画数（ほぼ生成するオブジェクト数）に応じてFPSかなり落ちます(マルチスレッドではないので)
///

#ifdef _DEBUG

/**
 * @brief デバッグ用のログを出力するフラグ
 * @details 基本上の_DEBUGがあり これがあるとデバッグ時のみ機能
 */
#define FLAG_DEBUGLOG

#endif

/**
 * @brief デバッグ時にFPSを表示するかどうか しないならコメントアウト
 */
#define FLAG_FPS

/**
 * @brief デバッグ時に表示しているオブジェクトの数を表示するフラグ
 * @n フラスタムカリングを適用し描画したオブジェクト数も表示
 */
#define FLAG_OBJECTNUM

/**
 * @brief 衝突判定の処理をのぞくフラグ(処理計測用) 基本コメントアウト
 */
//#define FLAG_STOP_COLLISION

//DEBUGLOG系

#ifdef FLAG_DEBUGLOG

/**
 * @brief DebugLogにObjectGeneratorのUpdateにかかった時間を表示するフラグです
 */
#define FLAG_DEBUGLOG_UPDATETIME

/**
 * @brief DebugLogにObjectGeneratorのDrawにかかった時間を表示するフラグです
 */
#define FLAG_DEBUGLOG_DRAWTIME

/**
 * @brief DebugLogに当たり判定にかかった時間を表示するフラグ
 */
#define FLAG_DEBUGLOG_COLLISIONTIME

/**
 * @brief 当たり判定の上限(MAX_COLLISIONNUM)に達したオブジェクトを表示するフラグ
 */
//#define FLAG_DEBUGLOG_COLLISIONLIMITOBJECT

/**
 * @brief  DebugLogにフラスタムカリングの対象になっていないObjectのnameをレイヤーごとに一括で表示するフラグ
 * @n たくさん出るので注意(FPSかなり落ちます)
 */
//#define FLAG_DEBUGLOG_CHECKNOTFRUSTUMTYPE
#endif

//

//!@brief	ウインドウのクラス名
//!@todo	追記必要
#define CLASS_NAME TEXT("AppClass")

//!@brief	ウインドウのところに出る名前
#define WINDOW_NAME TEXT("DX11 Template")

//!@brief	フルスクリーンにするかどうか
#define FULLSCREEN false

//!@brief	ウインドウの横の幅
#define SCREEN_WIDTH (1000)

//!@brief	ウインドウの高さ
#define SCREEN_HEIGHT (600)

/**
 * @brief これでこのフレームワークの最大のFPSを決めれる
 */
constexpr int MAX_FRAME_RATE = 100;

/**
 * @brief 1つのオブジェクトにつき何回まで当たり判定を行うか
 * @n FPSの減少対策の１つ
 * @details ゲーム内容により適時変更すること
 */
constexpr int MAX_COLLISIONNUM = 20;

//!@brief	色の情報を持たせる際に必要
typedef struct _tagCOLOR
{
	int red = 255;	   //!<赤色
	int green = 255;   //!<緑色
	int blue = 255;	   //!<青色
	int opacity = 255; //!<不透明度
} tagCOLOR_DATA;

//!@brief	色の指定に使用
enum class E_COLOR_INFO
{
	BLACK = 0,	 //!<黒
	DARK_BLUE,	 //!<暗い青
	DARK_GREEN,	 //!<暗い緑
	DARK_CYAN,	 //!<暗いシアン
	DARK_RED,	 //!<暗い赤色
	DARK_VIOLET, //!<暗い紫色
	DARK_YELLOW, //!<暗い黄色
	GRAY,		 //!<灰色
	LIGHT_GRAY,	 //!<明るい灰色
	BLUE,		 //!<青色
	GREEN,		 //!<緑色
	CYAN,		 //!<シアン
	RED,		 //!<赤色
	VIOLET,		 //!<紫
	YELLOW,		 //!<黄色
	WHITE,		 //!<白
};