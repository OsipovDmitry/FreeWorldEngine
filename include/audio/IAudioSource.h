#ifndef __IAUDIOSOURCE__
#define __IAUDIOSOURCE__

#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/gtc/quaternion.hpp>

#include "../IResource.h"

namespace FreeWorldEngine {

namespace Audio {

class IAudioBuffer;
class IAudioSource : public IResource {
public:
    virtual ~IAudioSource() {}

    virtual glm::vec3 position() const = 0;
    virtual void setPosition(const glm::vec3 &pos) = 0;

    virtual glm::vec3 velocity() const = 0;
    virtual void setVelocity(const glm::vec3 &vel) = 0;

    virtual float gain() const = 0;
    virtual void setGain(float value) = 0;

    virtual float pitch() const = 0;
    virtual void setPitch(float value) = 0;

    virtual glm::quat direction() const = 0;
    virtual void setDirection(const glm::quat &orient) = 0;

    // [0] - inner angle, [1] - outer angle, [2] - outer gain factor
    virtual glm::vec3 coneParameters() const = 0;
    virtual void coneParameters(const glm::vec3 &param) = 0;

    virtual bool looping() const = 0;
    virtual void setLooping(bool value) = 0;

    virtual IAudioBuffer *buffer() const = 0;
    virtual void setBuffer(IAudioBuffer *pBuffer) = 0;

}; // class IAudioBuffer

} // namespace Renderer

} // namespace FreeWorldEngine

#endif // __IAUDIOSOURCE__
