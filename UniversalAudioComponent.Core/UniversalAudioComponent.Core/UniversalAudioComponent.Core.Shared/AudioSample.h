#pragma once

using namespace Platform;
using namespace Windows::Storage::Streams;

namespace UniversalAudioComponent
{
    public ref class AudioSample sealed
    {

    private:
        IBuffer^ buffer;
        String^ name;

    public:
        AudioSample(String^ name, IBuffer^ buffer)
        {
            this->buffer = buffer;
            this->name = name;
        }

        property String^ Name
        {
            String^ get() { return this->name; }
        }

        property IBuffer^ Buffer
        {
            IBuffer^ get() { return this->buffer; }
        }
    };
}
