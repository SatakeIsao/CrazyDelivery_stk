#pragma once

namespace nsK2EngineLow {

	class Shadow
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="renderObjects"></param>
		void Render(
		RenderContext& rc,
		std::vector<IRenderer* >& renderObjects
		);
		/// <summary>
		/// 影の描画
		/// </summary>
		/// <param name="rc"レンダリングコンテキスト></param>
		void SpriteShadowDraw(RenderContext& rc);

		RenderTarget& GetRenderTarget()
		{
			return shadowMap;
		}

	private:
		/// <summary>
		/// レンダーターゲットの初期化
		/// </summary>
		void InitRenderTarget();
		/// <summary>
		/// 影描画用のカメラの初期化
		/// </summary>
		void InitLightCamera();

		
	private:
		Camera m_lightCamera;
		Vector3 m_lightCameraPos;
		RenderTarget shadowMap;
		Sprite sprite;
	};
}


