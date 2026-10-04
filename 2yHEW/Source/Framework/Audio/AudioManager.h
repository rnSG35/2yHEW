#pragma once
#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	eSOUND_BGM,
	eSOUND_DROP,
	eSOUND_LINE_CLEAR,

	eSOUND_LABEL_MAX,
} SOUND_LABEL;

class AudioManager {
private:
	// パラメータ構造体
	typedef struct
	{
		LPCSTR filename;	// 音声ファイルまでのパスを設定
		bool bLoop;			// trueでループ。通常BGMはture、SEはfalse。
	} PARAM;

	PARAM m_param[eSOUND_LABEL_MAX] =
	{
		{"AppData/Sound/BGM/Reflect.wav", true},//(ループさせるのでtrue設定)
		{"AppData/Sound/SE/drop.wav",false},
		{"AppData/Sound/SE/deleteline.wav",false},


	};

	IXAudio2* m_pXAudio2 = NULL;
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;
	IXAudio2SourceVoice* m_pSourceVoice[eSOUND_LABEL_MAX];
	WAVEFORMATEXTENSIBLE m_wfx[eSOUND_LABEL_MAX]; // WAVフォーマット
	XAUDIO2_BUFFER m_buffer[eSOUND_LABEL_MAX];
	BYTE* m_DataBuffer[eSOUND_LABEL_MAX];

	HRESULT FindChunk(HANDLE, DWORD, DWORD&, DWORD&);
	HRESULT ReadChunkData(HANDLE, void*, DWORD, DWORD);

public:
	// ゲームループ開始前に呼び出すサウンドの初期化処理
	HRESULT Initialize(void);

	// ゲームループ終了後に呼び出すサウンドの解放処理
	void UnInit(void);

	// 引数で指定したサウンドを再生する
	void Play(SOUND_LABEL label);

	// 引数で指定したサウンドを停止する
	void Stop(SOUND_LABEL label);

	// 引数で指定したサウンドの再生を再開する
	void Resume(SOUND_LABEL label);

	void SetMasterVolume(float volume);

};