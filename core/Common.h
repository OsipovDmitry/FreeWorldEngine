#ifndef __COMMON__
#define __COMMON__

#include <string>
#include <algorithm>

#include "Types.h"

// ���������� ���������� ����� � ��������� ��������
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

// �������� float � uint32
inline uint32 floatToUint32(float value)
{
	return *(reinterpret_cast<uint32*>(&value));
}

// �������� uint32 � float
inline float uint32ToFloat(uint32 value)
{
	return *(reinterpret_cast<float*>(&value));
}

// ������ ������� ������� � float �� ������� �������
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