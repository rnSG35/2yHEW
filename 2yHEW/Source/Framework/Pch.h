#pragma once

//プリコンパイルヘッダ
//ほとんど変更しない重いヘッダーだけをまとめている
//自作ヘッダーは入れないこと

//Windows
#define WIN32_LEAN_AND_MEAN	//Windows.hの使わない部分を省いて軽くする
#define NOMINMAX	//min・maxマクロがstd::minなどとぶつかるのを防ぐ
#include <Windows.h>

//DirectX11
#include <d3d11.h>		// DirectX11を使うためのヘッダーファイル
#include <wrl/client.h>
#include <DirectXMath.h>// DirextXの数学関連のヘッダーファイル
#include <d3dcompiler.h>


//標準ライブラリ
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <Xinput.h>		//XInputを使うためのヘッダーファイル
#include <xaudio2.h>
#include <fstream>
#include <source_location>