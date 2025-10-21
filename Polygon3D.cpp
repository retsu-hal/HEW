#include"d3d11.h"
#include"DirectXMath.h"
#include"direct3d.h"
#include"shader.h"
#include"keyboard.h"
#include"sprite.h"
using namespace DirectX;

//=========================================================================================================
// マクロ定義
//=========================================================================================================
#define NUM_VERTEX (100)

//=========================================================================================================
// 構造体定義
//=========================================================================================================
static Vertex vdata[NUM_VERTEX]
{
	//頂点０　LEFT-TOP
	{
		XMFLOAT3(-0.5f,0.5f,0.0f),			//左上頂点座標
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//左上頂点カラー
		XMFLOAT2(0.0f,0.0f)					    //左上テクスチャ座標
	},

	//頂点1　RIGHT-TOP
	{
		XMFLOAT3(0.5f,0.5f,0.0f),				//右上頂点座標
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//右上頂点カラー
		XMFLOAT2(1.0f,0.0f)					    //右上テクスチャ座標
	},

	//頂点2　FEFT-BOTTOM
	{
		XMFLOAT3(-0.5f,-0.5f,0.0f),			//左下頂点座標
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//左下頂点カラー
		XMFLOAT2(0.0f,1.0f)					    //左下テクスチャ座標
	},
};

//=========================================================================================================
// グローバル変数
//=========================================================================================================
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
static ID3D11ShaderResourceView* g_Texture;		//テクスチャ変数
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点バッファ
static XMFLOAT3 Position;		//ポリゴン座標
static XMFLOAT3 Rotation;		//ポリゴン回転角度
static XMFLOAT3 Scaling;		//ポリゴン拡大率

//=========================================================================================================
// 初期化処理
//=========================================================================================================
void Polygon3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	//頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));	//0でクリア
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;	//格納できる頂点数*頂点サイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// テクスチャ読み込み
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
// 終了処理
//=========================================================================================================
void Polygon3D_Finalize()
{
	if(g_VertexBuffer!=NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}

//=========================================================================================================
// 更新処理
//=========================================================================================================
void Polygon3D_Update()
{
	//移動
	if (Keyboard_IsKeyDown(KK_W))
	{
		Position.z += 0.5f;
	}
	if (Keyboard_IsKeyDown(KK_S))
	{
		Position.z -= 0.5f;
	}

	//回転
	if (Keyboard_IsKeyDown(KK_A))
	{
		Rotation.x += 2.0f;
	}	
	if (Keyboard_IsKeyDown(KK_D))
	{
		Rotation.x -= 2.0f;
	}
}

//=========================================================================================================
// 描画処理
//=========================================================================================================
void Polygon3D_Draw()
{
	// ワールド行列の作成
	//スケーリング行列の作成
	XMMATRIX ScalingMatrix = XMMatrixScaling(Scaling.x, Scaling.y, Scaling.z);
	//平行移動行列の作成
	XMMATRIX TranslationMatrix = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//回転行列の作成
	XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));

	//計算の順番「スケール*回転*平行移動」
	XMMATRIX WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

	//プロジェクション行列作成
	float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	XMMATRIX Projection = XMMatrixPerspectiveFovLH
	(
		XMConvertToRadians(45.0f),			//視野角
		SCREEN_WIDTH / SCREEN_HEIGHT,	//アスペクト比
		1.0f,													//近クリップ距離
		10.0f													//遠面クリップ距離	
	);

	//ビュー行列作成
	XMVECTOR VPos = XMVectorSet(0.0f, 0.0f, -4.0f, 0.0f);	//カメラの座標
	XMVECTOR vAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);		//カメラの注視点
	XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);		//上方向ベクトル
	XMMATRIX View = XMMatrixLookAtLH(VPos, vAt, vUp);	//ビュー行列

	//最終的な変換行列を作成	順番に注意！！
	XMMATRIX WVP = WorldMatrix * View * Projection;

	//変換行列を頂点シェーダへセット
	Shader_SetMatrix(WVP);

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();

	//頂点データを頂点バッファへコピーする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* vertex = (Vertex*)msr.pData;
	//頂点データコピー
	CopyMemory(&vertex[0], &vdata[0], sizeof(Vertex) * NUM_VERTEX);
	g_pContext->Unmap(g_VertexBuffer, 0);

	//テクスチャーセット
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);

	//頂点バッファをセット
	UINT stride = sizeof(Vertex); //1頂点あたりのデータサイズ
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//描画するポリゴンの種類をセット
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画リクエスト
	g_pContext->Draw(NUM_VERTEX, 0);
}