// ===================================================
// collision.h 当たり判定
// 
// 制作者：田中雅虎				日付：2025
// ===================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <d3d11.h>
#include<DirectXMath.h>
using namespace DirectX;

// ===================================================
// プロトタイプ宣言
// ===================================================
bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB);	// バウンディングボックスの当たり判定
bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB);		// バウンディングサークルの当たり判定
#endif
