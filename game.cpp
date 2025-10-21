#include	"Manager.h"
#include	"sprite.h"
#include	"Game.h"
#include	"keyboard.h"
#include	"player.h"
#include	"field.h"
#include	"Effect.h"
#include	"score.h"
#include	"Audio.h"
#include "mouse.h"

//=========================================================================================================
// グローバル宣言
//=========================================================================================================
static	int		g_BgmID = NULL;	//サウンド管理ID

//=========================================================================================================
// 初期化処理
//=========================================================================================================
void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	Player_Initialize(pDevice, pContext); // ポリゴンの初期化
	field_Initialize(pDevice, pContext); // フィールドの初期化
	Effect_Initialize(pDevice, pContext);//エフェクト初期化
	Score_Initialize(pDevice, pContext);//スコア初期化

	g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
	PlayAudio(g_BgmID, true);	//再生開始（ループあり）
	//PlayAudio(g_BgmID);			//再生開始（ループなし）
	//PlayAudio(g_BgmID, false);	//再生開始（ループなし）

}

//=========================================================================================================
// 終了処理
//=========================================================================================================
void Game_Finalize()
{
	Player_Finalize();	// ポリゴンの終了処理
	field_Finalize();	// フィールドの終了処理
	Effect_Finalize();
	Score_Finalize();

	UnloadAudio(g_BgmID);//サウンドの解放
}

//=========================================================================================================
// 更新処理
//=========================================================================================================
void Game_Update()
{
	Player_Update();
	Effect_Update();
	Score_Update();

}

//=========================================================================================================
// 描画処理
//=========================================================================================================
void Game_Draw()
{ 
	field_Draw();
	Player_Draw();
	Effect_Draw();
	Score_Draw();

}

