#include "Image.h"

namespace FreeWorldEngine {

Image::Image(const std::string& name) :
	m_name(name),
	m_width(0), m_height(0), m_depth(0),
	m_numComponents(0),
	m_type(TYPE_SIGNED_INT_8),
	m_data(0)
{
}

Image::~Image()
{
	if (m_data)
		delete [] m_data;
}

std::string Image::name() const
{
	return m_name;
}

uint32 Image::width() const
{
	return m_width;
}

uint32 Image::height() const
{
	return m_height;
}

uint32 Image::depth() const
{
	return m_depth;
}

uint32 Image::numComponents() const
{
	return m_numComponents;
}

Type Image::type() const
{
	return m_type;
}

void *Image::data() const
{
	return m_data;
}

void Image::setWidth(const uint32 width)
{
	m_width = width;
}

void Image::setHeight(const uint32 height)
{
	m_height = height;
}

void Image::setDepth(const uint32 depth)
{
	m_depth = depth;
}

void Image::setNumComponents(const uint32 numComponents)
{
	m_numComponents = numComponents;
}

void Image::setType(const Type type)
{
	m_type = type;
}

void Image::setData(void *data)
{
	m_data = data;
}

} // namespace