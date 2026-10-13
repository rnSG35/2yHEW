#include "Graphics.h"
#include "../DebugSystem/DebugLog.h"
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


	//Device・Context・SwapChain作成
	//ここがないと以降の処理ができない
	if (CreateDeviceAndSwapChain(hwnd, width, height))
	{
		DebugLogWrite("Device・Context・SwapChain作成失敗 / Failed to create Device, Context, and SwapChain");
		return false;
	}

	
	
	
	//bool Graphics::CreateDeviceAndSwapChain(HWND hwnd, UINT width, UINT height)

	
//	//---------------------------------------------------------
//	//スワップチェイン設定
//	//バックバッファの設定を行う
//	//---------------------------------------------------------
//	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//	swapChainDesc.BufferCount = 1;
//	swapChainDesc.BufferDesc.Width = width;
//	swapChainDesc.BufferDesc.Height = height;
//	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//RGBA 8bit
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//描画先として使うバッファ
//	swapChainDesc.OutputWindow = hwnd;
//	swapChainDesc.SampleDesc.Count = 1;	//MSAAナシ
//	swapChainDesc.Windowed = true;
//	//---------------------------------------------------------
//	//DirectX11生成
//	//Device	GPU資源を作成
//	//context	GPUに命令を送る
//	//SwapChain 画面表示を管理
//	//	をまとめて生成
//	//---------------------------------------------------------
//	HRESULT createDeviceResult =
//		D3D11CreateDeviceAndSwapChain(
//			nullptr,
//			D3D_DRIVER_TYPE_HARDWARE,
//			nullptr,
//#ifdef _DEBUG
//			D3D11_CREATE_DEVICE_DEBUG,
//#else
//			0,
//#endif
//			nullptr,
//			0,
//			D3D11_SDK_VERSION,
//			&swapChainDesc,
//			m_swapChain.GetAddressOf(),
//			m_device.GetAddressOf(),
//			nullptr,
//			m_context.GetAddressOf());
//	if (FAILED(createDeviceResult)) { return false; }




	CreateRenderTargetView(width, height);
	
	////---------------------------------------------------------
	////バックバッファ取得
	////SwapChainの描画先を取得
	////GPUに「どこに書くのか」と聞かれたときに答えるためのオブジェクト
	////今回はバックバッファに
	////---------------------------------------------------------
	//ComPtr<ID3D11Texture2D> backBuffer;
	//HRESULT getBackBUfferResult = m_swapChain->GetBuffer(
	//	0,
	//	_uuidof(ID3D11Texture2D),
	//	reinterpret_cast<void**>(
	//		backBuffer.GetAddressOf()));
	//if (FAILED(getBackBUfferResult)) { return false; }
	////---------------------------------------------------------
	////RenderTargetView作成
	////このRTVに向かって描画
	////---------------------------------------------------------
	//HRESULT createRenderTargetViewResult = m_device->CreateRenderTargetView(
	//	backBuffer.Get(),
	//	nullptr,
	//	m_renderTargetView.GetAddressOf());
	//if (FAILED(createRenderTargetViewResult)) { return false; }



	CreateDepthBuffer(width, height);
	
	////---------------------------------------------------------
	////DepthBuffer作成
	////奥行き判定に必要
	////---------------------------------------------------------
	//D3D11_TEXTURE2D_DESC depthTextureDesc = {};
	//depthTextureDesc.Width = width;
	//depthTextureDesc.Height = height;
	//depthTextureDesc.MipLevels = 1;
	//depthTextureDesc.ArraySize = 1;
	//depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthTextureDesc.SampleDesc.Count = 1;
	//depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//ComPtr<ID3D11Texture2D>depthBuffer;
	//HRESULT createDepthBufferResult = m_device->CreateTexture2D(
	//	&depthTextureDesc,
	//	nullptr,
	//	depthBuffer.GetAddressOf());
	//if (FAILED(createDepthBufferResult)) { return false; }



	CreateDepthStencilView();
	
	////---------------------------------------------------------
	////DepthStencilView作成
	////震度バッファをGPUから使えるようにする
	//// 壁、プレイヤーなどが重なったときに奥にあるものを隠すため
	////---------------------------------------------------------
	//HRESULT createDepthStencilViewResult = m_device->CreateDepthStencilView(
	//	depthBuffer.Get(),
	//	nullptr,
	//	m_depthStencilView.GetAddressOf());
	//if (FAILED(createDepthStencilViewResult)) { return false; }



	SetRenderTargets();

	////---------------------------------------------------------
	////描画先設定
	////RenderTargetとDepthBuferをDirectXに登録
	////---------------------------------------------------------
	//m_context->OMSetRenderTargets(
	//	1,
	//	m_renderTargetView.GetAddressOf(),
	//	m_depthStencilView.Get());



	SetViewPort(width, height);
	
	////---------------------------------------------------------
	////ビューポート設定
	////画面全体に描画
	////ミニマップや分割画面を作る場合はここを変更
	////---------------------------------------------------------
	//D3D11_VIEWPORT mainViewport = {};

	//mainViewport.Width = static_cast<float>(width);
	//mainViewport.Height = static_cast<float>(height);
	//mainViewport.MinDepth = 0.0f;
	//mainViewport.MaxDepth = 1.0f;

	//m_context->RSSetViewports(
	//	1,
	//	&mainViewport);

	DebugLogWrite("Graphics初期化成功 / Graphics Initialization Successful");
	return true;
}

//---------------------------------------------------------
//終了処理
//ComPtrはスコープ終了時に自動開放されるが、
//終了順序を明確にしたいため明示的にReleaseする
//---------------------------------------------------------
void Graphics::UnInitialize()
{
	//Contextがの凍っている場合は描画先を解除する
	if (m_context)
	{
		m_context->OMSetRenderTargets(
			0,
			nullptr,
			nullptr);
		m_context->ClearState();
		m_context->Flush();
	}

	//作成した順番の逆に解放
	m_depthStencilView.Reset();
	m_depthBuffer.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_context.Reset();
	m_device.Reset();

	DebugLogWrite("Graphics終了処理完了 / Graphics Cleanup Complete");
}

//---------------------------------------------------------
//フレーム開始
//---------------------------------------------------------
void Graphics::BeginFrame()
{
	if (!m_context)
	{
		DebugLogWrite("BeginFrame時にm_contextがnullptr");
		return;
	}

	if (!m_renderTargetView)
	{
		DebugLogWrite("BeginFrame時にm_renderTargetViewがnullptr");
		return;
	}
	if (!m_depthStencilView)
	{
		DebugLogWrite("BeginFrame時にm_depthStencilViewがnullptr");
		return;
	}

	//---------------------------------------------------------
	//背景色、空色にしておく
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
//フレーム終了
//---------------------------------------------------------
void Graphics::EndFrame()
{
	if (!m_swapChain)
	{
		DebugLogWrite("EndFrame時にm_swapChainがnullptr");
		return;
	}

	//---------------------------------------------------------
	//バックバッファ表示
	//今書いた内容を実際の画面に出力
	//---------------------------------------------------------
	HRESULT presentResult = m_swapChain->Present(1, 0);

	if (FAILED(presentResult)) { DebugLogWrite("Present失敗"); }

	//---------------------------------------------------------
	//Presentがある理由
	//DirectXはバックバッファへ描画する
	//Presentを呼ぶことでバックバッファとフロントバッファを入れ替える
	//---------------------------------------------------------
}

bool Graphics::CreateDeviceAndSwapChain(HWND hwnd, UINT width, UINT height)
{
	//渡されたウィンドウハンドルを確認する
	if (hwnd == nullptr)
	{
		DebugLogWrite("ウィンドウハンドルがnullptr");
		return false;
	}

	//---------------------------------------------------------
	//スワップチェイン設定
	//バックバッファの設定を行う
	//---------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//RGBA 8bit

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//描画先として使うバッファ

	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SampleDesc.Count = 1;	//MSAAナシ

	swapChainDesc.Windowed = true;

	//Device・Context・SwapChainをまとめて作成
	HRESULT createDeviceResult =
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
			&swapChainDesc,
			m_swapChain.GetAddressOf(),
			m_device.GetAddressOf(),
			nullptr,
			m_context.GetAddressOf()
		);
	if (FAILED(createDeviceResult))
	{
		DebugLogWrite("DiD11CreateDeviceAndSwapChain失敗");
		return false;
	}

	//念のためポインタを確認
	if (!m_swapChain)
	{
		DebugLogWrite("m_swapChainがnullptr");
		return false;
	}
	if (!m_device)
	{
		DebugLogWrite("m_deviceがnullptr");
		return false;
	}
	if (!m_context)
	{
		DebugLogWrite("m_contextがnullptr");
		return false;
	}
}

bool Graphics::CreateRenderTargetView(UINT width, UINT height)
{
	//SwapChainが作られているか確認
	if (!m_swapChain)
	{
		DebugLogWrite("CreateRenderTargetView時にm_swapChainがnullptr");
		return false;
	}
	//Deviceが作られているか確認
	if (!m_device)
	{
		DebugLogWrite("CreateRenderTargetView時にm_deviceがnullptr");
		return false;
	}

	//SwapChainからバックバッファ取得
	ComPtr<ID3D11Texture2D> backBuffer;

	HRESULT getBackBUfferResult = m_swapChain->GetBuffer(
		0,
		_uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(
			backBuffer.GetAddressOf()));
	if (FAILED(getBackBUfferResult))
	{
		DebugLogWrite("CreateRenderTargetView時にバックバッファ取得失敗");
		return false;
	}
	if (!backBuffer)
	{
		DebugLogWrite("CreateRenderTargetView時にバックバッファがnullptr");
		return false;
	}
	//RenderTargetView作成
	HRESULT createRenderTargetViewResult = m_device->CreateRenderTargetView(
		backBuffer.Get(),
		nullptr,
		m_renderTargetView.GetAddressOf());
	if (FAILED(createRenderTargetViewResult))
	{
		DebugLogWrite("CreateRenderTargetView失敗");
		return false;
	}
	return true;
}

//---------------------------------------------------------
//DepthBuffer作成
//奥行き判定に必要
//---------------------------------------------------------
bool Graphics::CreateDepthBuffer(UINT width, UINT height)
{
	if (!m_device)
	{
		DebugLogWrite("CreateDepthBuffer時にm_deviceがnullptr");
		return false;
	}

	//深度バッファ作成
	D3D11_TEXTURE2D_DESC depthTextureDesc = {};

	depthTextureDesc.Width = width;
	depthTextureDesc.Height = height;

	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.ArraySize = 1;

	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.SampleDesc.Quality = 0;

	depthTextureDesc.CPUAccessFlags = 0;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;


	//深度バッファ作成
	//ローカル変数ではなく、メンバ変数(m_depthBuffer)に保存する
	HRESULT createDepthBufferResult = m_device->CreateTexture2D(
		&depthTextureDesc,
		nullptr,
		m_depthBuffer.GetAddressOf());
	if (FAILED(createDepthBufferResult))
	{
		DebugLogWrite("DepthBufferのTexture2D作成失敗");
		return false;
	}

}

//---------------------------------------------------------
//DepthStencilView作成
//震度バッファをGPUから使えるようにする
// 壁、プレイヤーなどが重なったときに奥にあるものを隠すため
//---------------------------------------------------------
bool Graphics::CreateDepthStencilView()
{
	if (!m_device)
	{
		DebugLogWrite("CreateDepthStencilView時にm_deviceがnullptr");
		return false;
	}

	//CreateDepthBufferで作ったものがあるか確認
	if (!m_depthBuffer)
	{
		DebugLogWrite("m_depthBufferがnullptr");
		return false;
	}

	//DepthStencilView作成
	HRESULT createDepthStencilViewResult = m_device->CreateDepthStencilView(
		m_depthBuffer.Get(),
		nullptr,
		m_depthStencilView.GetAddressOf());
	if (FAILED(createDepthStencilViewResult))
	{
		DebugLogWrite("CreateDepthStencilView作成失敗");
		return false;
	}


}

//---------------------------------------------------------
//描画先設定
//RenderTargetとDepthBuferをDirectXに登録
//---------------------------------------------------------
bool Graphics::SetRenderTargets()
{
	if (!m_context)
	{
		DebugLogWrite("SetRenderTargets時にm_contextがnullptr");
		return false;
	}
	if (!m_renderTargetView)
	{
		DebugLogWrite("SetRenderTargets時にm_renderTargetViewがnullptr");
		return false;
	}
	if (!m_depthStencilView)
	{
		DebugLogWrite("SetRenderTargets時にm_depthStencilViewがnullptr");
		return false;
	}

	//色の描画先と深度情報の保存先を設定する
	m_context->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get());
	return false;
}

//---------------------------------------------------------
//ビューポート設定
//画面全体に描画
//ミニマップや分割画面を作る場合はここを変更
//---------------------------------------------------------
bool Graphics::SetViewPort(UINT width, UINT height)
{
	if (!m_context)
	{
		DebugLogWrite("SetViewPort時にm_contextがnullptr");
		return false;
	}

	if (width == 0 || height == 0)
	{
		DebugLogWrite("ビューポートのサイズが0");
		return false;
	}

	//画面全体を描画範囲にする
	D3D11_VIEWPORT mainViewport = {};

	mainViewport.TopLeftX = 0.0f;
	mainViewport.TopLeftY = 0.0f;

	mainViewport.Width = static_cast<float>(width);
	mainViewport.Height = static_cast<float>(height);
	mainViewport.MinDepth = 0.0f;
	mainViewport.MaxDepth = 1.0f;

	m_context->RSSetViewports(
		1,
		&mainViewport);
}
