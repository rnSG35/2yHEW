#include "Graphics.h"

#pragma comment(lib,"d3d11.lib")

using Microsoft::WRL::ComPtr;

//---------------------------------------------------------
//シングルトン取得
//---------------------------------------------------------
Graphics& Graphics::GetInstance()
{
	//staticにすることで最初の１回のみ生成
	//ひとつで十分なのでシングルトン設計に
	static Graphics instance;

	return instance;
}

//---------------------------------------------------------
//初期化
//1.Device作成
//2.Context作成
//3.SwapChain作成
//4.RenderTarget作成
//5.DepthBuffer作成
//6.Viewport設定
//---------------------------------------------------------
bool Graphics::Initialize(
	HWND hwnd,
	UINT width,
	UINT height)
{
	//---------------------------------------------------------
	//スワップチェイン設定
	//バックバッファの設定を行う
	//---------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd = {};

	sd.BufferCount = 1;

	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;

	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//RGBA 8bit

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//描画先として使うバッファ

	sd.OutputWindow = hwnd;

	sd.SampleDesc.Count = 1;	//MSAAナシ

	sd.Windowed = true;

	//---------------------------------------------------------
	//DirectX11生成
	//Device	GPU資源を作成
	//context	GPUに命令を送る
	//SwapChain 画面表示を管理
	//	をまとめて生成
	//---------------------------------------------------------
	HRESULT hr =
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
#ifdef _DEBUG
			D3D11_CREATE_DEVICE_DEBUG,
#else
			0,
#endif
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			m_swapChain.GetAddressOf(),
			m_device.GetAddressOf(),
			nullptr,
			m_context.GetAddressOf()
		);
	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//バックバッファ取得
	//SwapChainの描画先を取得
	//GPUに「どこに書くのか」と聞かれたときに答えるためのオブジェクト
	//今回はバックバッファに
	//---------------------------------------------------------
	ComPtr<ID3D11Texture2D> backBuffer;

	hr = m_swapChain->GetBuffer(
		0,
		_uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(
			backBuffer.GetAddressOf()));
	if (FAILED(hr)) { return false; }


	//---------------------------------------------------------
	//RenderTargetView作成
	//このRTVに向かって描画
	//---------------------------------------------------------
	hr = m_device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		m_renderTargetView.GetAddressOf());
	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//DepthBuffer作成
	//奥行き判定に必要
	//---------------------------------------------------------
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = width;
	depthDesc.Height = height;

	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;

	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthDesc.SampleDesc.Count = 1;

	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D>depthBuffer;

	hr = m_device->CreateTexture2D(
		&depthDesc,
		nullptr,
		depthBuffer.GetAddressOf());
	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//DepthStencilView作成
	//震度バッファをGPUから使えるようにする
	// 壁、プレイヤーなどが重なったときに奥にあるものを隠すため
	//---------------------------------------------------------
	hr = m_device->CreateDepthStencilView(
		depthBuffer.Get(),
		nullptr,
		m_depthStencilView.GetAddressOf());
	if (FAILED(hr)) { return false; }

	//---------------------------------------------------------
	//描画先設定
	//RenderTargetとDepthBuferをDirectXに登録
	//---------------------------------------------------------
	m_context->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get());

	//---------------------------------------------------------
	//ビューポート設定
	//画面全体に描画
	//ミニマップや分割画面を作る場合はここを変更
	//---------------------------------------------------------
	D3D11_VIEWPORT viewport = {};

	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_context->RSSetViewports(
		1,
		&viewport);
	return true;
}

//---------------------------------------------------------
//終了処理
//ComPtrはスコープ終了時に自動開放されるが、
//終了順序を明確にしたいため明示的にReleaseする
//---------------------------------------------------------
void Graphics::UnInitialize()
{
	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_context.Reset();
	m_device.Reset();
}

//---------------------------------------------------------
//フレーム開始
//---------------------------------------------------------
void Graphics::BeginFrame()
{
	//---------------------------------------------------------
	//背景色、空白にしておく
	//---------------------------------------------------------
	float clearColor[4] =
	{
		0.4f,
		0.6f,
		0.9f,
		1.0f
	};

	//---------------------------------------------------------
	//前フレームの色を消す
	//消さないと前の描画結果が残る
	//---------------------------------------------------------
	m_context->ClearRenderTargetView(
		m_renderTargetView.Get(),
		clearColor);

	//---------------------------------------------------------
	//深度情報もリセット
	// ここが無いと
	// ・モデルが突然消える
	// ・描画がおかしくなる	というバグが発生
	//---------------------------------------------------------
	m_context->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0);


}

//---------------------------------------------------------
//
//---------------------------------------------------------
void Graphics::EndFrame()
{
	//---------------------------------------------------------
	//バックバッファ表示
	//今書いた内容を実際の画面に出力
	//---------------------------------------------------------
	HRESULT hr = m_swapChain->Present(1, 0);

	if (FAILED(hr)) { OutputDebugStringA("Present失敗\n"); }

	//---------------------------------------------------------
	//Presentがある理由
	//DirectXはバックバッファへ描画する
	//Presentを呼ぶことでバックバッファとフロントバッファを入れ替える
	//---------------------------------------------------------
}