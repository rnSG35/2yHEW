#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Graphics
{
public:
	/// <summary>
	/// シングルトン取得
	/// </summary>
	/// <returns></returns>
	/// <remarks>Deviceを複数作るとVRAMが無駄に消費されるため、シングルトンで１つだけ生成する</remarks>
	static Graphics& GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="hwnd">描画先ウィンドウ</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <returns></returns>
	bool Initialize(
		HWND hwnd,
		UINT width,
		UINT height
	);

	/// <summary>
	/// 終了処理
	/// </summary>
	void UnInitialize();

	/// <summary>
	/// フレーム開始、毎フレーム最初に呼ぶ
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// フレーム終了、描画結果を画面に表示
	/// </summary>
	void EndFrame();

	/// <summary>
	/// Presentが成功したかどうか
	/// </summary>
	/// <returns>true:成功　false:失敗</returns>
	bool IsLastPresentSucceeded() const
	{
		return m_isLastPresentSucceeded;
	}

	//Device取得
	// TextureやShader作成時に使用
	ID3D11Device* GetDevice()
	{
		return m_device.Get();
	}

	//Context取得
	// 描画命令発行に使用
	ID3D11DeviceContext* GetContext()
	{
		return m_context.Get();
	}

private:
	Graphics() = default;
	~Graphics() = default;

	//シングルトンをコピーできると、Applicationが複数存在してしまう
	//それをコンパイル時に防ぐため、コピー処理を削除する
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;
	Graphics(const Graphics&&) = delete;
	Graphics& operator= (const Graphics&&) = delete;

private:
	//GPU本体
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	//GPUへの命令送信役
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	//画面バッファ管理
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	//描画先
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	//深度情報を保存するバッファ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthBuffer;
	//深度バッファを描画処理から使うためのビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	//直前のPresent(画面表示)が成功したかどうか
	//一度も表示していない場合は「成功していない」ので初期値false
	bool m_isLastPresentSucceeded = false;

	bool CreateDeviceAndSwapChain(HWND hwnd, UINT width, UINT height);
	bool CreateRenderTargetView(UINT width, UINT height);
	bool CreateDepthBuffer(UINT width, UINT height);
	bool CreateDepthStencilView();
	bool SetRenderTargets();
	bool SetViewPort(UINT width, UINT height);
};