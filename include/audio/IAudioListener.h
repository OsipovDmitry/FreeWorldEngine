#ifndef __IAUDIOLISTENER__
#define __IAUDIOLISTENER__

#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/gtc/quaternion.hpp>

namespace FreeWorldEngine {

namespace Audio {

class IAudioListener {
public:
    virtual ~IAudioListener() {}

    virtual glm::vec3 position() const = 0;
    virtual void setPosition(const glm::vec3 &pos) = 0;

    virtual glm::vec3 velocity() const = 0;
    virtual void setVelocity(const glm::vec3 &vel) = 0;

    virtual glm::quat orientation() const = 0;
    virtual void setOrientation(const glm::quat &orient) = 0;

    virtual float gain() const = 0;
    virtual void setGain(float value) = 0;

}; // class IAudioListener

} // namespace Renderer

} // namespace FreeWorldEngine

#endif // __IAUDIOLISTENER__
