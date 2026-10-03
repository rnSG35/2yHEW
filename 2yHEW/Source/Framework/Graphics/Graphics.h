#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Graphics
{
public:
	//---------------------------------------------------------
	//シングルトン取得
	//Deviceを複数作るとVRAMが無駄に消費されるため、シングルトンで１つだけ生成する
	//---------------------------------------------------------
	static Graphics& GetInstance();

	//---------------------------------------------------------
	//初期化
	// hwnd : 描画先ウィンドウ
	// width : 画面横幅
	// height : 画面縦幅
	//---------------------------------------------------------
	bool Initialize(
		HWND hwnd,
		UINT width,
		UINT height
	);

	//---------------------------------------------------------
	//終了処理
	//---------------------------------------------------------
	void UnInitialize();

	//---------------------------------------------------------
	//フレーム開始、毎フレーム最初に呼ぶ
	//---------------------------------------------------------
	void BeginFrame();

	//---------------------------------------------------------
	//フレーム終了、描画結果を画面に表示
	//---------------------------------------------------------
	void EndFrame();

	//---------------------------------------------------------
	//Device取得
	// TextureやShader作成時に使用
	//---------------------------------------------------------
	ID3D11Device* GetDevice()
	{
		return m_device.Get();
	}

	//---------------------------------------------------------
	//Context取得
	// 描画命令発行に使用
	//---------------------------------------------------------
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
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
};
