// ===================================================
// collision.h �����蔻��
// 
// ����ҁF�c�����				���t�F2025
// ===================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <d3d11.h>
#include<DirectXMath.h>
using namespace DirectX;

// ===================================================
// �v���g�^�C�v�錾
// ===================================================
bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB);	// �o�E���f�B���O�{�b�N�X�̓����蔻��
bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB);		// �o�E���f�B���O�T�[�N���̓����蔻��
#endif
