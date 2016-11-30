#ifndef __TYPES__
#define __TYPES__

#include <map> 
#include <string>

#include "CoreSettings.h"

typedef signed char            int8;
typedef signed short int       int16;
typedef signed int             int32;
typedef signed long long int   int64;
typedef unsigned char          uint8;
typedef unsigned short int     uint16;
typedef unsigned int           uint32;
typedef unsigned long long int uint64;

enum Type {
	TYPE_SIGNED_INT_8,
	TYPE_SIGNED_INT_16,
	TYPE_SIGNED_INT_32,
	TYPE_UNSIGNED_INT_8,
	TYPE_UNSIGNED_INT_16,
	TYPE_UNSIGNED_INT_32,
    TYPE_FLOAT
}; // enum Type

enum PrimitiveFormat {
    PrimitiveFormat_Points,
    PrimitiveFormat_Lines,
    PrimitiveFormat_LineStrip,
    PrimitiveFormat_LineLoop,
    PrimitiveFormat_Trangles,
    PrimitiveFormat_TriangleStrip,
    PrimitiveFormat_TrangleFan
}; //enum PrimitiveFormat

struct Raster {
    uint32 size[3]; // width, height, depth
    uint32 numComponents;
    Type type;
    void *pData;

    Raster() : numComponents(0), type(TYPE_SIGNED_INT_8), pData(0) { size[0] = 0; size[1] = size[2] = 1; }
}; // struct Raster

/*struct VertexFormatPrivate;
class CORE_FUNC_DLL VertexFormat {
public:
    enum class AttributeType {
        Position,
        Tangent,
        Binormal,
        Normal,
        TexCoord0,
        TexCoord1,
        TexCoord2,
        TexCoord3,
	};

    class Attribute {
    public:
        uint8 size{0};
        uint8 offset{0};

        Attribute() = default;
        Attribute(uint8 size, uint8 offset);
        Attribute(std::initializer_list<uint8>);

        bool isNull() const;
	};
        Attribute(const Attribute&) = default;

    VertexFormat();
    VertexFormat(const VertexFormat&);
    VertexFormat(std::initializer_list<std::pair<AttributeType, Attribute>>);

    void addAttribute(const std::pair<AttributeType, Attribute>&);
    void addAttribute(AttributeType type, const Attribute& attrib);
    Attribute attribute(AttributeType type) const;
	void clear();

    static VertexFormat V3();
    static VertexFormat T2V3();

private:
    VertexFormatPrivate *m;
};

struct Mesh {
    float *pVertexData{nullptr};
    uint32 *pIndicesData{nullptr};
    uint32 numVertices{0};
    uint32 numIndices{0};
	VertexFormat vertexFormat;
    PrimitiveFormat primitiveFormat{PrimitiveFormat::None};
};*/

#endif // __TYPES__
