#pragma once

namespace UniversalAudioComponent
{
    namespace Core
    {
        private struct RiffChunk
        {
            uint32 tag;
            uint32 size;
        };

        private struct ChunkInfo
        {
            uint32 id;
            uint32 size;
            byte* dataOffset;
        };

        public ref class UniversalAudioPlayer sealed
        {
        private:
            ~UniversalAudioPlayer()
            {

            }


        public:
            UniversalAudioPlayer()
            {

            }
        };
    }
}

