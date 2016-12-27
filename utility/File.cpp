#include <cstdlib>
#include <algorithm>

#include <utility/File.h>

namespace FreeWorldEngine {

namespace Utility {

struct FilePrivate {
	std::string filename;
	FILE *pHandle;

	FilePrivate(const std::string fname = "") : filename(fname), pHandle(0) {}
};

File::File(const std::string& filename) :
	m(new FilePrivate(filename))
{
}

File::~File()
{
	close();
	delete m;
}

std::string File::filename() const
{
	return m->filename;
}

void File::setFilename(const std::string& filename)
{
	close();
	m->filename = filename;
}

bool File::open(const File::OpenMode openMode) const
{
	std::string modeStr = "";

	switch (openMode) {
	case OpenMode_ReadOnly: { modeStr = "rb"; break; }
	case OpenMode_WriteOnly: { modeStr = "wb"; break; }
	case OpenMode_Append: { modeStr = "ab"; break; }
	default: break;
	};

	m->pHandle = fopen(m->filename.c_str(), modeStr.c_str());
	return m->pHandle != 0;
}

void File::close() const
{
	if (m->pHandle) {
		fclose(m->pHandle);
		m->pHandle = 0;
	}
}

bool File::isOpened() const
{
	return m->pHandle != 0;
}

int64 File::size() const
{
	fseek(m->pHandle, 0, SEEK_END);
	return ftell(m->pHandle);
}

int64 File::readAll(const int64 maxLength, void *pData) const
{
	int64 sz = size();
	if (sz > maxLength)
		sz = maxLength;
	return readData(sz, pData);
}

int64 File::readData(const int64 length, void *pData, const int64 offset) const
{
	fseek(m->pHandle, offset, SEEK_SET);
	return fread(pData, length, 1, m->pHandle) * length;
}

void File::writeData(const int64 length, void *pData, const int64 offset)
{
	fseek(m->pHandle, offset, SEEK_SET);
	fwrite(pData, length, 1, m->pHandle);
}

std::string File::fileExtension() const
{
	const std::string::size_type dotPos = m->filename.find_last_of(".");
	if (dotPos != std::string::npos) {
		std::string ext = m->filename.substr(dotPos+1);
		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);
		return ext;
	}
    return "";
}

std::string File::filePath() const
{
	const std::string::size_type slashPos = m->filename.find_last_of("/\\");
	return (slashPos != std::string::npos) ? m->filename.substr(0, slashPos) : "";
}

std::string File::fileShortName() const
{
	const std::string::size_type dotPos = m->filename.find_last_of(".");
	const std::string::size_type slashPos = m->filename.find_last_of("/\\");

	if (dotPos == std::string::npos && slashPos == std::string::npos)
		return "";
	else if (dotPos == std::string::npos)
		return m->filename.substr(slashPos+1);
	else if (slashPos == std::string::npos)
		return m->filename.substr(0, dotPos);
	else
		return (slashPos >= dotPos) ? "" : m->filename.substr(slashPos+1, dotPos-slashPos-1);
}

} // namesapce
} // namespace