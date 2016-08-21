#ifndef __COMMON__
#define __COMMON__

#include <string>
#include <algorithm>

#include "Types.h"

// Возвращает расширение файла в маленьком регистре
inline std::string fileExtension(const std::string& filename)
{
	const std::string::size_type dotPos = filename.find_last_of(".");
	if (dotPos != std::string::npos) {
		std::string ext = filename.substr(dotPos+1);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		return ext;
	}
    return "";
}

// Упаковка float в uint32
inline uint32 floatToUint32(float value)
{
	return *(reinterpret_cast<uint32*>(&value));
}

// Упаковка uint32 в float
inline float uint32ToFloat(uint32 value)
{
	return *(reinterpret_cast<float*>(&value));
}

// Расчет размера вершины в float по формату вершины
inline uint32 calcVertexSize(const VertexFormat& vertexFormat)
{
	return vertexFormat.positionSize +
		vertexFormat.tangentSize +
		vertexFormat.binormalSize +
		vertexFormat.normalSize +
		vertexFormat.texCoord0Size +
		vertexFormat.texCoord1Size +
		vertexFormat.texCoord2Size +
		vertexFormat.texCoord3Size;
}

#endif // __COMMON__