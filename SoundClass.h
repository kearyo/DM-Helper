///////////////////////////////////////////////////////////////////////////////
// Filename: soundclass.h
///////////////////////////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_
//The following libraries and headers are required for DirectSound to compile properly.

#if USE_DX_SOUND

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////
// Class name: SoundClass
///////////////////////////////////////////////////////////////////////////////

#define PSOUNDBUFFER IDirectSoundBuffer8*
typedef CTypedPtrMap <CMapStringToPtr, CString, PSOUNDBUFFER> PDXSOUNDBUFFERMAP;

class SoundClass
{
private:
	//The WaveHeaderType structure used here is for the.wav file format.When loading in.wav files I first read in the header to determine the required information for loading in the.wav audio data.If you are using a different format you will want to replace this header with the one required for your audio format.

	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	PDXSOUNDBUFFERMAP m_DXSoundBufferMap;

public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();
	//Initialize and Shutdown will handle everything needed for this tutorial.The Initialize function will initialize DirectSound and load in the.wav audio file and then play it once.Shutdown will release the.wav file and shutdown DirectSound.

	bool Initialize(HWND);
	IDirectSoundBuffer8* InitSoundBuffer(CString szWAVFileName);
	bool PlaySoundBuffer(IDirectSoundBuffer8* pBuffer);
	bool PlayDXSoundFromFile(CString szPath);
	int GetBufferedSoundCount();
	void ShutdownWaveFile(IDirectSoundBuffer8**);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();
	
	bool LoadWaveFile(char*, IDirectSoundBuffer8**);

	

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	
	//Note that I only have one secondary buffer as this tutorial only loads in one sound.
	//IDirectSoundBuffer8* m_secondaryBuffer1;
};

#endif
#endif
