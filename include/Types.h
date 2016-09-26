#ifndef __TYPES__
#define __TYPES__

#include <map> 
#include <string>

typedef signed char            int8;
typedef signed short int       int16;
typedef signed int             int32;
typedef signed long long int   int64;
typedef unsigned char          uint8;
typedef unsigned short int     uint16;
typedef unsigned int           uint32;
typedef unsigned long long int uint64;

enum Type {
	TYPE_SIGNED_INT_8 = 0,
	TYPE_SIGNED_INT_16,
	TYPE_SIGNED_INT_32,
	TYPE_UNSIGNED_INT_8,
	TYPE_UNSIGNED_INT_16,
	TYPE_UNSIGNED_INT_32,
	TYPE_FLOAT
}; // enum Type

struct Time {
	Time(uint8 h = 0, uint8 m = 0, uint8 s = 0, uint16 ms = 0) : hours(h), minutes(m), seconds(s), millisecond(ms) {}
	std::string toString() const { return std::to_string((_ULonglong)hours) + ":" + std::to_string((_ULonglong)minutes) + ":" + std::to_string((_ULonglong)seconds) + ":" + std::to_string((_ULonglong)millisecond); }

	uint16 millisecond;
	uint8 hours, minutes, seconds;
};

struct Date {
	enum Month { Jan, Feb, Mar, Apr, May, June, July, Aug, Sept, Oct, Nov, Dec };

	// d >= 1, m = [Jan..Dec], y >= 0
	Date(uint8 d = 29, Month m = Nov, uint16 y = 1992) : day(d-1), month(m), year(y) {}
	std::string toString() const { return std::to_string((_ULonglong)(day+1)) + "." + std::to_string((_ULonglong)(month-Jan+1)) + "." + std::to_string((_ULonglong)year); }

	uint16 year;
	Month month;
	uint8 day;
};

struct DateTime {
	DateTime(const Time t = Time(), const Date& d = Date()) : date(d), time(t) {}
	std::string toString() const { return date.toString() + " " + time.toString(); }

	Date date;
	Time time;
};

/*struct VertexFormat { // Все значения указываются в штуках float. Если size == 0, то соответсвующего атрибута нет.
	uint8 positionSize, positionStride;
	uint8 tangentSize, tangentStride;
	uint8 binormalSize, binormalStride;
	uint8 normalSize, normalStride;
	uint8 texCoord0Size, texCoord0Stride;
	uint8 texCoord1Size, texCoord1Stride;
	uint8 texCoord2Size, texCoord2Stride;
	uint8 texCoord3Size, texCoord3Stride;

	VertexFormat() : positionSize(0), positionStride(0),
					 tangentSize(0), tangentStride(0),
					 binormalSize(0), binormalStride(0),
					 normalSize(0), normalStride(0), 
					 texCoord0Size(0), texCoord0Stride(0), 
					 texCoord1Size(0), texCoord1Stride(0),
					 texCoord2Size(0), texCoord2Stride(0),
					 texCoord3Size(0), texCoord3Stride(0) {}

}; // struct VertexFormat

struct TriMesh {
	float *pVertexData;
	uint32 *pIndexData;
	uint32 numVertices;
	uint32 numIndices;
	VertexFormat vertexFormat;

	TriMesh() : pVertexData(0), pIndexData(0), numVertices(0), numIndices(0), vertexFormat() {} 

}; // struct TriMesh
*/

/*class VertexFormat {
public:
	enum AttributeType {
		AttributeType_Position,
		AttributeType_Tangent,
		AttributeType_Binormal,
		AttributeType_Normal,
		AttributeType_Tangent,
		AttributeType_TexCoord0,
		AttributeType_TexCoord1,
		AttributeType_TexCoord2,
		AttributeType_TexCoord3,
	};
	struct Attribute {
		uint8 size, offset;
		Attribute(const uint8 theSize, const uint8 theOffset) : size(theSize), offset(theOffset) {}
	};

	VertexFormat() : m_attributes() {}
	void addAttribute(const AttributeType type, const Attribute attrib) { m_attributes[type] = attrib; }
	Attribute attribute(const AttributeType type) const;
	void clear();

	static VertexFormat V3() { 
		VertexFormat vf;
		vf.addAttribute(AttributeType_Position, Attribute(3, 0));
		return vf;
	}

	static VertexFormat T2V3() { 
		VertexFormat vf;
		vf.addAttribute(AttributeType_TexCoord0, Attribute(2, 0));
		vf.addAttribute(AttributeType_Position, Attribute(3, 2));
		return vf;
	}

private:
	std::map<AttributeType, Attribute> m_attributes;

};

struct Mesh {
	enum PrimitiveFormat {
		PrimitiveFormat_None,
		PrimitiveFormat_Points,
		PrimitiveFormat_Lines,
		PrimitiveFormat_LineStrip,
		PrimitiveFormat_LineLoop,
		PrimitiveFormat_Trangles,
		PrimitiveFormat_TriangleStrip,
		PrimitiveFormat_TrangleFan
	};

	float *pVertexData;
	uint32 *pIndicesData;
	uint32 numVertices, numIndices;
	VertexFormat vertexFormat;
	PrimitiveFormat primitiveFormat;

	Mesh() : pVertices(0), pIndicesData(0), numVertices(0), numIndices(0), vertexFormat(0), primitiveFormat(PrimitiveFormat_None) {}
};*/

#endif // __TYPES__