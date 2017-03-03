#ifndef __IAUDIOENGINE__
#define __IAUDIOENGINE__

#include <string>

namespace FreeWorldEngine {

struct SoundData;

namespace Audio {

class IAudioBuffer;
class IAudioSource;
class IAudioListener;

class IAudioEngine {
public:
    virtual ~IAudioEngine() {}

    virtual IAudioBuffer *createBuffer(SoundData *pData = nullptr) = 0;
    virtual void destroyBuffer(IAudioBuffer *pBuffer) = 0;

    virtual IAudioSource *createSource(const std::string &name = "@utoname") = 0;
    virtual void destroySource(IAudioSource *pSource) = 0;

    virtual void playSource(IAudioSource *pSource) const = 0;
    virtual void pauseSource(IAudioSource *pSource) const = 0;
    virtual void stopSource(IAudioSource *pSource) const = 0;

    virtual IAudioListener *listener() const = 0;

}; // class IAudioEngine

} // namespace Renderer

} // namespace FreeWorldEngine

#endif // __IAUDIOENGINE__
