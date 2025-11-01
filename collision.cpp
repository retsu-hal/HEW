// ===================================================
// collision.cpp �����蔻��
// 
// ����ҁF�c�����			���t�F2025
// ===================================================
#include "collision.h"

// ===================================================
// �o�E���f�B���O�{�b�N�X�̓����蔻��
// 
// ����:
// 	��`�`�̒��S���W
// 	��`�a�̒��S���W
// 	��`�`�̃T�C�Y
// 	��`�a�̃T�C�Y
//
// �߂�l
//  true�F�������Ă���
// 	false�F�������Ă��Ȃ�
// ===================================================
bool CheckBoxCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, XMFLOAT2 SizeA, XMFLOAT2 SizeB)
{
	float ATop = PosA.y - SizeA.y / 2;	// A�̏�[
	float ABottom = PosA.y + SizeA.y / 2;	// A�̉��[
	float ARight = PosA.x + SizeA.x / 2;	// A�̉E�[
	float ALeft = PosA.x - SizeA.x / 2;	// A�̍��[

	float BTop = PosB.y - SizeB.x / 2;	// B�̏�[
	float BBottom = PosB.y + SizeB.x / 2;	// B�̉��[
	float BRight = PosB.x + SizeB.y / 2;	// B�̉E�[
	float BLeft = PosB.x - SizeB.y / 2;	// B�̍��[

	if ((ARight >= BLeft) &&		// A�̉E�[ > B�̍��[
		(ALeft <= BRight) &&		// A�̍��[ < B�̉E�[
		(ABottom >= BTop) &&		// A�̉��[ > B�̏�[
		(ATop <= BBottom))			// A�̏�[ < B�̉��[
	{
		// �������Ă���
		return true;
	}

	// �������Ă��Ȃ�
	return false;
}

// ===================================================
// �o�E���f�B���O�T�[�N���̓����蔻��
// 
// ����:
// 	�~�P�̒��S���W
// 	�~�Q�̒��S���W
// 	�~�P�̔��a
// 	�~�Q�̔��a
//
// �߂�l
//  true�F�������Ă���
// 	false�F�������Ă��Ȃ�
// ===================================================
bool CheckCircleCollider(XMFLOAT2 PosA, XMFLOAT2 PosB, float rA, float rB)
{
	// (�~A�̒��S���WX - �~B�̒��S���WX)��2�� + (�~A�̒��S���WY - �~B�̒��S���WY)��2��
	float distance = (PosA.x - PosB.x) * (PosA.x - PosB.x) + (PosA.y - PosB.y) * (PosA.y - PosB.y);

	// (�~1�̔��a+�~2�̔��a)��2��
	float rSum = (rA + rB) * (rA + rB);

	// 2�̉~�̋��������a�̍��v�����������
	if (distance <= rSum)
	{
		// �������Ă���
		return true;
	}

	// �������Ă��Ȃ�
	return false;
}