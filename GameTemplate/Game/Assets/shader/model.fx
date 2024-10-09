/*!
 * @brief	シンプルなモデルシェーダー。
 */

//定数
static const int NUM_DIRECTIONAL_LIGHT = 4; //ディレクションライトの数
static const int MAX_POINT_LIGHT = 32; //ポイントライトの最大数
static const int MAX_SPOT_LIGHT = 32; //スポットライトの最大数

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float3 normal : NORMAL; //法線。
	
    float3 tangent : TANGENT; //接ベクトル。
    float4 biNormal : BINORMAL; //従ベクトル。
	
    float2 uv : TEXCOORD0; //UV座標。
	
    SSkinVSIn skinVert; //スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float3 normal : NORMAL; //法線。
	
    float3 tangent : TANGENT; //接ベクトル。
    float3 biNormal : BINORMAL; //従ベクトル。
	
    float2 uv : TEXCOORD0; //UV座標
    float3 worldPos : TEXCOORD1; //ワールド空間座標。
};

// ディレクションライト
struct DirectionalLight
{
    float3 direction; //ライトの方向
    int castShadow; //影をキャストするか？
    float3 color; //ライトのカラー
};

////////////////////////////////////////////////
// 定数バッファ
////////////////////////////////////////////////
// モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer LightCb : register(b1)
{
    DirectionalLight directionalLights[NUM_DIRECTIONAL_LIGHT]; //ディレクションライト。
    float3 ambientLight; //環境光
	//追加予定
}

////////////////////////////////////////////////
// シェーダーリソース
////////////////////////////////////////////////
// モデルテクスチャ
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
//texture2D<float4>g_normalMap : register(t1);

StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。

sampler g_sampler : register(s0); //サンプラステート。

////////////////////////////////////////////////
// 関数宣言。
////////////////////////////////////////////////

// Lamber拡散反射光の計算
float3 CalcLamberDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcLigFromDirectionLight(SPSIn psIn, DirectionalLight dirlig);



/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float3 finalLig = float3(0.0f, 0.0f, 0.0f);
    
    for (int dirligNo = 0; dirligNo < NUM_DIRECTIONAL_LIGHT; dirligNo++)
    {
        finalLig += CalcLigFromDirectionLight(psIn, directionalLights[dirligNo]);
    }
    
    finalLig += ambientLight;
    
   
    
    float4 finalColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //テクスチャカラーに求めた光を乗算して最終出力カラーを求める
    finalColor.xyz *= finalLig;
    
    return finalColor;
}

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

// Lamber拡散反射光の計算
float3 CalcLamberDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    //ピクセルの法線とライトの方向の内積を計算する。
    float t = dot(normal, lightDirection) * -1.0f;
    
    //内積の結果が０より小さいときは０にする
    t = max(0.0f, t);
    
    //拡散反射光を計算する
    return lightColor * t;
}

//ディレクションライトによる反射光を計算
float3 CalcLigFromDirectionLight(SPSIn psIn, DirectionalLight dirLig)
{
    //ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLamberDiffuse(dirLig.direction, dirLig.color, psIn.normal);
    
    //ディレクションライトの最終的な反射光を返す
    return diffDirection;

}
