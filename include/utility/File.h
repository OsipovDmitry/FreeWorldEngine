#ifndef __FILE_H__
#define __FILE_H__

#include <string>

#include "utility/UtilitySettings.h"
#include "Types.h"

namespace FreeWorldEngine {

namespace Utility {

struct FilePrivate;
class UTILITY_EXPORT File {
public:
	enum OpenMode { OpenMode_ReadOnly, OpenMode_WriteOnly, OpenMode_Append };

	File(const std::string& filename = "");
	~File();

	std::string filename() const;
	void setFilename(const std::string& filename);

	bool open(const OpenMode openMode) const;
	void close() const;
	bool isOpened() const;

	int64 size() const;
	int64 readAll(const int64 maxLength, void *pData) const;
	int64 readData(const int64 length, void *pData, const int64 offset = 0) const;
	void writeData(const int64 length, void *pData, const int64 offset = 0);

	std::string fileExtension() const;
	std::string filePath() const;
	std::string fileShortName() const;

private:
	FilePrivate *m;

}; // class File

} // namespace
} // namespace

#endif // __FILE_H__
