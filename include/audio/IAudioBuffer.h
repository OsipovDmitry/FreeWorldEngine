#ifndef __IAUDIOBUFFER__
#define __IAUDIOBUFFER__

namespace FreeWorldEngine {

struct SoundData;

namespace Audio {

class IAudioBuffer  {
public:
    virtual ~IAudioBuffer() {}

//    virtual void getData(SoundData *pResData) const = 0;
    virtual void setData(SoundData *pData) = 0;

}; // class IAudioBuffer

} // namespace Renderer

} // namespace FreeWorldEngine

#endif // __IAUDIOBUFFER__
