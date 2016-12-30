#ifndef __IGPUSHADER__
#define __IGPUSHADER__

#include <3rdparty/glm/vec2.hpp>
#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/vec4.hpp>
#include <3rdparty/glm/mat2x2.hpp>
#include <3rdparty/glm/mat3x3.hpp>
#include <3rdparty/glm/mat4x4.hpp>

#include <string>
#include <Types.h>

namespace FreeWorldEngine {

namespace Utility {
class File;
}

namespace Renderer {

class IGPUShader {
public:
    enum IGPUShaderType {
        IGPUShaderType_None,
        IGPUShaderType_Vertex,
        IGPUShaderType_Geometry,
        IGPUShaderType_Fragment
    };

    virtual ~IGPUShader() {}

    virtual IGPUShaderType type() const = 0;

    virtual void loadFromData(const std::string& data) = 0;
    virtual void loadFromFile(const Utility::File& file) = 0;

    virtual bool compile(std::string *pLogString = 0) const = 0;

}; // class IGPUShader


class IGPUProgram {
public:
    virtual ~IGPUProgram() {}

    virtual void attachShader(IGPUShader *pShader) = 0;
    virtual void detachShader(IGPUShader *pShader) = 0;

    virtual bool link(std::string *pLogString = 0) const = 0;

    virtual int32 attributeLocationByName(const std::string& name) const = 0;
    virtual int32 uniformLocationByName(const std::string& name) const = 0;
	virtual int32 uniformBlockIndexByName(const std::string& name) const = 0; 

	virtual void setUniform(const int32 location, const float value) const = 0;
	virtual void setUniform(const int32 location, const int32 value) const = 0;
	virtual void setUniform(const int32 location, const uint32 value) const = 0;
	virtual void setUniform(const int32 location, const glm::vec2& value) const = 0;
	virtual void setUniform(const int32 location, const glm::ivec2& value) const = 0;
	virtual void setUniform(const int32 location, const glm::uvec2& value) const = 0;
	virtual void setUniform(const int32 location, const glm::vec3& value) const = 0;
	virtual void setUniform(const int32 location, const glm::ivec3& value) const = 0;
	virtual void setUniform(const int32 location, const glm::uvec3& value) const = 0;
	virtual void setUniform(const int32 location, const glm::vec4& value) const = 0;
	virtual void setUniform(const int32 location, const glm::ivec4& value) const = 0;
	virtual void setUniform(const int32 location, const glm::uvec4& value) const = 0;
	virtual void setUniform(const int32 location, const glm::mat2& value, const bool transpose = false) const = 0;
	virtual void setUniform(const int32 location, const glm::mat3& value, const bool transpose = false) const = 0;
	virtual void setUniform(const int32 location, const glm::mat4& value, const bool transpose = false) const = 0;

	struct UniformBlockInfo {
		std::string name;
		int32 index;
		uint32 dataSize;
		uint32 numUniforms;
		std::vector<std::string> uniformsNames;
		std::vector<int32> uniformsIndices;
		std::vector<int32> uniformsOffsets;
		std::vector<int32> uniformsSizes;
	};
	virtual UniformBlockInfo uniformBlockInfo(const int32 index, const bool writeNames = true) const = 0;
	virtual void setUniformBlockBindingPoint(const int32 index, const uint32 bindingPoint) const = 0;

}; // class IGPUProgram

} // namespace

} // namespace

#endif
