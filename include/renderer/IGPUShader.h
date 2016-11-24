#ifndef __IGPUSHADER__
#define __IGPUSHADER__

#include <string>
#include "Types.h"

namespace FreeWorldEngine {

class File;

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
    virtual void loadFromFile(const File& file) = 0;

    virtual bool compile() const = 0;

}; // class IGPUShader


class IGPUProgram {
public:
    virtual ~IGPUProgram() {}

    virtual void attachShader(IGPUShader *pShader) = 0;
    virtual void detachShader(IGPUShader *pShader) = 0;

    virtual bool link() const = 0;

    virtual int32 attributeLocationByName(const std::string& name) = 0;
    virtual int32 uniformLocationByName(const std::string& name) = 0;

}; // class IGPUProgram

} // namespace

#endif
