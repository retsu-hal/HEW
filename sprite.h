#pragma once

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;

//=========================================================================================================
// プロトタイプ宣言
//=========================================================================================================
void	DrawSprite(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col);
void	DrawSpriteEx(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col, int bno, int wc, int hc);
void	InitializeSprite();	
void	FinalizeSprite();
void	DrawSpriteScroll(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col,XMFLOAT2 texcoord);
void	DrawSpriteExRotation(XMFLOAT2 pos, XMFLOAT2 size, XMFLOAT4 col, int bno, int wc, int hc, float radian);
void	DrawSprite(XMFLOAT2 size, XMFLOAT4 col, int bno, int wc, int hc);

//=========================================================================================================
// 構造体定義
//=========================================================================================================
struct Vertex
{
	XMFLOAT3 position; // 頂点座標  //XMFLOAT3へ変更
	XMFLOAT4 color;		//頂点カラー（R,G,B,A）
	XMFLOAT2 texCoord;	//テクスチャ座標
};
