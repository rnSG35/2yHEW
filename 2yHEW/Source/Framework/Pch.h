#pragma once

//プリコンパイルヘッダ
//ほとんど変更しない重いヘッダーだけをまとめている
//自作ヘッダーは基本的にb入れないこと

//Windows
#define WIN32_LEAN_AND_MEAN	//Windows.hの使わない部分を省いて軽くする
#define NOMINMAX	//min・maxマクロがstd::minなどとぶつかるのを防ぐ
#include <Windows.h>//HWND・MSG・PeekMessageなど、ウィンドウとメッセージの処理(Window・Application)

//DirectX11
#include <d3d11.h>		//ID3D11Deviceなど          DirectX11を使うためのヘッダーファイル(Graphics・SpriteRenderer・Texture)
#include <wrl/client.h>	//Microsoft::WRL::ComPtr   DirectXの部品を自動で解放するスマートポインタ
#include <DirectXMath.h>//XMFLOAT2・XMMATRIXなど    DirectXの数学関連のファイル(MathTypes・Camera・SpriteRenderer)
#include <d3dcompiler.h>//D3DCompileFromFileなど    シェーダー(CSO・HLSL)の読み込み(SpriteRenderer)

//入力・音声
#include <Xinput.h>		//XInputGetStateなど   XInputを使うため(Input)
#include <xaudio2.h>	//IXAudio2など         XAudioを使うため(AudioManager)

//標準ライブラリ
#include <memory>		//std::shared_ptr・std::unique_ptr・std::weak_ptr   自動で解放されるポインタ
#include <string>		//std::string               ファイルパスなどに使う文字列	
#include <string_view>	//std::string_view          コピーせずに文字列の一部を扱う(DebugLogの相対パス変換用)	
#include <vector>		//std::vector               数が変わる配列(m_gameObjects・m_components)	
#include <unordered_map>//キーで値を探す表(TextureManagerの読み込み画像)
#include <concepts>		//std::derived_from         テンプレートに渡せる型の制限(AddComponent・AddGameObject)	

#include <fstream>			//std::ofstream                         ファイルへの書き込み(DebugLogのlog.txt)	
#include <source_location>	//std::source_location                  ログを書いたファイル名と行番号の取得(DebugLog)	
#include <cmath>			//std::sqrt・std::sin・std::cos         平方根や三角関数(MathTypesの正規化・SpriteRenderer)	
#include <numbers>			//std::numbers::pi_v                    円周率(度からラジアンへの変換)		
#include <algorithm>	//std::stable_sort・std::clamp・std::max	    並べ替えや値の範囲の制限(SceneBase・Camera)

//ほとんど変更する予定がない自作ヘッダファイル
#include "../Framework/Math/MathTypes.h"   //Float2・Float3・Float4など、フレームワーク共通の数学の型