#include "pch.h"
#include "AudioDecoder.h"
#include "Shared.h"

namespace UniversalAudioComponent
{
    AudioDecoder::AudioDecoder()
    {
        HRESULT hr = MFStartup(MF_VERSION);

        if (FAILED(hr))
            ref new COMException(hr, "Media foundation startup failure");
    }

    Array<byte>^ AudioDecoder::Decode(IRandomAccessStream^ audioStream)
    {
        ComPtr<IMFByteStream> byteStream = nullptr;
        ComPtr<IMFSourceReader> reader = nullptr;

        HRESULT hr = MFCreateMFByteStreamOnStreamEx((IUnknown*)audioStream, &byteStream);

        if (FAILED(hr))
            ref new COMException(hr, "Could not create byte stream");

        hr = MFCreateSourceReaderFromByteStream(byteStream.Get(), NULL, &reader);

        if (FAILED(hr))
            ref new COMException(hr, "Could not create source reader");

        auto data = DecodeAsWav(reader);

        return data;
    }

    Array<byte>^ AudioDecoder::DecodeAsWav(ComPtr<IMFSourceReader> reader)
    {
        ComPtr<IMFMediaType> audioType = ConfigureAudioStream(reader); // Represents the PCM audio format.
        Vector<byte>^ decodedData = GetDecodedAudioData(reader);
        Array<byte>^ wav = GetWav(decodedData, audioType);

        return wav;
    }

    ComPtr<IMFMediaType> AudioDecoder::ConfigureAudioStream(ComPtr<IMFSourceReader> reader)
    {
        ComPtr<IMFMediaType> uncompressedAudioType = nullptr;
        ComPtr<IMFMediaType> partialType = nullptr;

        HRESULT hr = reader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, FALSE);

        if (FAILED(hr))
            ref new COMException(hr, "Could not set select audio stream");

        hr = reader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, FALSE);

        if (FAILED(hr))
            ref new COMException(hr, "Could not set select first audio stream");

        MFCreateMediaType(&partialType);

        hr = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);

        if (FAILED(hr))
            ref new COMException(hr, "Could not set major audio type");

        hr = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

        if (FAILED(hr))
            ref new COMException(hr, "Could not set sub audio type");

        hr = reader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, partialType.Get());

        if (FAILED(hr))
            ref new COMException(hr, "Could not set current media type");

        hr = reader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &uncompressedAudioType);

        if (FAILED(hr))
            ref new COMException(hr, "Could not get complete audio type");

        hr = reader->SetStreamSelection((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);

        if (FAILED(hr))
            ref new COMException(hr, "Could not select the entire stream");

        return uncompressedAudioType;
    }

    Vector<byte>^ AudioDecoder::GetDecodedAudioData(ComPtr<IMFSourceReader> reader)
    {
        Vector<byte>^ dataBytes = ref new Vector<byte>();

        DWORD cbBuffer = 0;
        ComPtr<IMFSample> sample = nullptr;
        ComPtr<IMFMediaBuffer> buffer = nullptr;
        BYTE *pAudioData = NULL;

        while (true)
        {
            DWORD dwFlags = 0;

            HRESULT hr = reader->ReadSample((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, 0, &dwFlags, NULL, &sample);

            if (FAILED(hr))
                ref new COMException(hr, "Failed to read sample");

            if (dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
            {
                printf("Type change - not supported by WAVE file format.\n");
                break;
            }
            if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM)
            {
                printf("End of input file.\n");
                break;
            }

            if (sample == nullptr)
            {
                continue;
            }

            hr = sample->ConvertToContiguousBuffer(&buffer);

            if (FAILED(hr))
                ref new COMException(hr, "Could not convert buffer");

            hr = buffer->Lock(&pAudioData, NULL, &cbBuffer);

            WriteDataToBuffer(dataBytes, pAudioData, cbBuffer);
            
            hr = buffer->Unlock();
            pAudioData = NULL;

            if (FAILED(hr))
                ref new COMException(hr, "Could not release lock");
        }

        return dataBytes;
    }

    Array<byte>^ AudioDecoder::GetWav(Vector<byte>^ audioBytes, ComPtr<IMFMediaType> audioType)
    {
        WAVEFORMATEX * wavFormat = NULL;
        UINT32 wavFormatBytes = 0;
        
        HRESULT hr = MFCreateWaveFormatExFromMFMediaType(audioType.Get(), &wavFormat, &wavFormatBytes);

        if (FAILED(hr))
            ref new COMException(hr, "Could not get media format");

        int dataChunkBytes = 8 + audioBytes->Size;
        int formatChunkBytes = 8 + wavFormatBytes;
        int totalBytes = 12 + dataChunkBytes + formatChunkBytes;
        auto buffer = ref new Array<byte>(totalBytes);
        byte * bufferPtr = buffer->begin();

        DWORD header[] = {
            FCC('RIFF'), // Riff chunk
            totalBytes - 8,
            FCC('WAVE'),
            FCC('fmt '), // 'fmt ' chunk
            wavFormatBytes
        };

        DWORD dataHeader[] = { 
            FCC('data'),  // data chunk
            audioBytes->Size 
        };
        
        int headerBytes = sizeof(header);
        int dataHeaderBytes = sizeof(dataHeader);

        WriteDataToBuffer(bufferPtr, reinterpret_cast<byte*>(header), headerBytes);
        bufferPtr += headerBytes;

        WriteDataToBuffer(bufferPtr, reinterpret_cast<byte*>(wavFormat), wavFormatBytes);
        bufferPtr += wavFormatBytes;
        
        WriteDataToBuffer(bufferPtr, reinterpret_cast<byte*>(dataHeader), dataHeaderBytes);
        bufferPtr += dataHeaderBytes;

        WriteDataToBuffer(bufferPtr, audioBytes);

        return buffer;
    }
}
