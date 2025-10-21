#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include "direct3d.h"

//=========================================================================================================
// 構造体宣言
//=========================================================================================================
class EFFECT
{
	public:
		bool		Enable;
		XMFLOAT3	Position;
		int			FrameCount;	//アニメーションカウンター

};

//=========================================================================================================
// プロトタイプ宣言
//=========================================================================================================
void Effect_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void Effect_Finalize();
void Effect_Update();
void Effect_Draw();
void CreateEffect(XMFLOAT2 Position);
