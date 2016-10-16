#ifndef __FILE_H__
#define __FILE_H__

#include <string>

#include "UtilitySettings.h"

namespace FreeWorldEngine {

struct FilePrivate;
class UTILITY_DLL File {
public:
	File(const std::string& filename = "");
	~File();

	std::string filename() const;
	void setFilename(const std::string& filename);

	enum OpenMode { OpenMode_BinaryReadOnly, OpenMode_BinaryWriteOnly, OpenMode_BinaryAppend };
	bool open(const OpenMode openMode);
	void close();
	bool isOpened() const;

	std::string fileExtension() const;
	std::string filePath() const;
	std::string fileShortName() const;

private:
	FilePrivate *m;

}; // class File

} // namespace

#endif // __FILE_H__