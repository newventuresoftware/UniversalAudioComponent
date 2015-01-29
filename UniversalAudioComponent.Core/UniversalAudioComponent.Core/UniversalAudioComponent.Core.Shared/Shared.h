#pragma once

#include "pch.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Streams;

namespace UniversalAudioComponent
{
    byte* GetBufferByteAccess(IBuffer^ buffer);
}