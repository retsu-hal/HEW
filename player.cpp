#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "shader.h"
#include "debug_ostream.h"
#include "sprite.h"
#include "keyboard.h"
#include "fade.h"
#include "player.h"
#include "mouse.h"
using namespace DirectX;

//=========================================================================================================
// �}�N����`
//=========================================================================================================
#define NUM_VERTEX (100)

//=========================================================================================================
// �\���̒�`�E��`
//=========================================================================================================
static Vertex vdata[NUM_VERTEX]
{
	//���_�O�@LEFT-TOP
	{
		XMFLOAT3(-0.5f,0.5f,0.0f),			//���㒸�_���W
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//���㒸�_�J���[
		XMFLOAT2(0.0f,0.0f)					    //����e�N�X�`�����W
	},

	//���_1�@RIGHT-TOP
	{
		XMFLOAT3(0.5f,0.5f,0.0f),				//�E�㒸�_���W
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�E�㒸�_�J���[
		XMFLOAT2(1.0f,0.0f)					    //�E��e�N�X�`�����W
	},

	//���_2�@FEFT-BOTTOM
	{
		XMFLOAT3(-0.5f,-0.5f,0.0f),			//�������_���W
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�������_�J���[
		XMFLOAT2(0.0f,1.0f)					    //�����e�N�X�`�����W
	},


	//���_0�@RIGHT-TOP
	{
		XMFLOAT3(0.5f,0.5f,0.0f),				//�E��_���W
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�E�㒸�_�J���[
		XMFLOAT2(1.0f,0.0f)					    //�E��e�N�X�`�����W
	},

	//���_1�@RIGHT-BOTTOM
	{
		XMFLOAT3(0.5f,-0.5f,0.0f),			//�E�����_���W
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�E�����_�J���[
		XMFLOAT2(1.0f,1.0f)					    //�E���e�N�X�`�����W
	},
	//���_2�@FEFT-BOTTOM
	{
		XMFLOAT3(-0.5f,-0.5f,0.0f),			//�������_���W
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�������_�J���[
		XMFLOAT2(0.0f,1.0f)					    //�����e�N�X�`�����W
	},
};

static Mouse_State ms{};

//=========================================================================================================
// �O���[�o���錾
//=========================================================================================================
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
static ID3D11ShaderResourceView* g_Texture;		//�e�N�X�`���ϐ�
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_�o�b�t�@
static XMFLOAT3 Position;		//�|���S�����W
static XMFLOAT3 Rotation;		//�|���S����]�p�x
static XMFLOAT3 Scaling;		//�|���S���g�嗦

//=========================================================================================================
// ����������
//=========================================================================================================
void Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));	//0�ŃN���A
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;	//�i�[�ł��钸�_��*���_�T�C�Y
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\texture.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	g_pDevice = pDevice;
	g_pContext = pContext;
}

//=========================================================================================================
// �I������
//=========================================================================================================
void Player_Finalize(void)
{
	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}

//=========================================================================================================
// �X�V����
//=========================================================================================================
void Player_Update()
{
	Mouse_GetState(&ms);
	float sensitivity = 0.01f;	//�}�E�X�����x
	static bool relativeMode = false;	//���[�h�ؑցitrue:����/false:��΁j

	bool suppressDelta = false;

	static bool prevRight = false;
	if (ms.rightButton && !prevRight) {
		relativeMode = !relativeMode;
		Mouse_SetMode(relativeMode ? MOUSE_POSITION_MODE_RELATIVE
			: MOUSE_POSITION_MODE_ABSOLUTE);

		suppressDelta = true;
	}

	prevRight = ms.rightButton;

	if (ms.positionMode == MOUSE_POSITION_MODE_RELATIVE)
	{
		if (!suppressDelta)
		{
			Position.x += ms.x * sensitivity;
			Position.y -= ms.y * sensitivity;
		}
	}

	

	if (Keyboard_IsKeyDown(KK_W))
	{
		Position.z += 0.5f;
	}
	if (Keyboard_IsKeyDown(KK_S))
	{
		Position.z -= 0.5f;
	}
	if (Keyboard_IsKeyDown(KK_A))
	{
		Position.x -= 0.5f;
	}
	if (Keyboard_IsKeyDown(KK_D))
	{
		Position.x += 0.5f;
	}
}

//=========================================================================================================
// �`�揈��
//=========================================================================================================
void Player_Draw(void)
{
	// ���[���h�s��̍쐬
		//�X�P�[�����O�s��̍쐬
	XMMATRIX ScalingMatrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	//���s�ړ��s��̍쐬
	XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//��]�s��̍쐬
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));

	//�v�Z�̏��ԁu�X�P�[��*��]*���s�ړ��v
	XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

	//�v���W�F�N�V�����s��쐬
	float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	XMMATRIX Projection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(45.0f),			//����p
		SCREEN_WIDTH / SCREEN_HEIGHT,	//�A�X�y�N�g��
		1.0f,													//�߃N���b�v����
		10.0f													//���ʃN���b�v����	
	);

	//�r���[�s��쐬
	XMVECTOR VPos = XMVectorSet(0.0f, 0.0f, -4.0f, 0.0f);	//�J�����̍��W
	XMVECTOR vAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);		//�J�����̒����_
	XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		//������x�N�g��
	XMMATRIX View = XMMatrixLookAtLH(VPos, vAt, vUp);	//�r���[�s��

	//�ŏI�I�ȕϊ��s����쐬	���Ԃɒ��ӁI�I
	XMMATRIX WVP = WorldMatrix * View * Projection;

	//�ϊ��s��𒸓_�V�F�[�_�փZ�b�g
	Shader_SetMatrix(WVP);

	//�V�F�[�_�[��`��p�C�v���C���֐ݒ�
	Shader_Begin();

	//���_�f�[�^�𒸓_�o�b�t�@�փR�s�[����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* vertex = (Vertex*)msr.pData;
	//���_�f�[�^�R�s�[
	CopyMemory(&vertex[0], &vdata[0], sizeof(Vertex) * NUM_VERTEX);
	g_pContext->Unmap(g_VertexBuffer, 0);

	//�e�N�X�`���[�Z�b�g
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);

	//���_�o�b�t�@���Z�b�g
	UINT stride = sizeof(Vertex); //1���_������̃f�[�^�T�C�Y
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//�`�悷��|���S���̎�ނ��Z�b�g
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�`�惊�N�G�X�g
	g_pContext->Draw(NUM_VERTEX, 0);
}

