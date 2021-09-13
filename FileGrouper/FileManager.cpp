#include "FileManager.h"
#include <fstream>
#include <string_view>
#include <cstring>

FileManager::FileManager()
{
}


FileManager::FileManager(std::filesystem::path filePath)
{
	m_filePath = filePath;
}

int FileManager::setFilePath(std::filesystem::path filePath)
{
	m_filePath = filePath;
	return 0;
}

std::string FileManager::getFileName()
{
	return m_filePath.filename().u8string();
}

void FileManager::generateHash()
{
	FileHash hash;

	std::ifstream file(m_filePath, std::ifstream::binary | std::ifstream::in);

	///Make data block by wich file will be readed
	constexpr size_t bufferSize = 1024 * 1024;
	std::unique_ptr<char[]> buffer(new char[bufferSize]);

	while (file)
	{
		///nullify data buffer
		initCharBuf(buffer.get(), bufferSize);

		file.read(buffer.get(), bufferSize);

		///Convert char* to string_view to be corresponded with std::hash
		std::string_view stringViewTmp = std::string_view(buffer.get(), bufferSize);

		if (!hash.data)
		{
			///set generated hash as a new
			hash.data = std::hash<std::string_view>()(stringViewTmp);
		}
		else
		{
			///Merge generated hash with existing one
			hash.data ^= std::hash<std::string_view>()(stringViewTmp);
		}
	}

	m_sCurrentHash = hash;
}

FileManager::FileHash FileManager::getCurrentHash()
{
	return m_sCurrentHash;
}


bool FileManager::equal(const FileManager *f1, const FileManager *f2)
{
	std::ifstream file1(f1->m_filePath, std::ifstream::binary | std::ifstream::in);
	std::ifstream file2(f2->m_filePath, std::ifstream::binary | std::ifstream::in);

	///Make data blocks by wich files will be readed
	constexpr size_t bufferSizeFile1 = 1024 * 1024;
	constexpr size_t bufferSizeFile2 = 1024 * 1024;

	std::unique_ptr<char[]> bufferFile1(new char[bufferSizeFile1]);
	std::unique_ptr<char[]> bufferFile2(new char[bufferSizeFile2]);


	while (file1 && file2)
	{
		///nullify data buffers
		initCharBuf(bufferFile1.get(), bufferSizeFile1);
		initCharBuf(bufferFile2.get(), bufferSizeFile2);

		file1.read(bufferFile1.get(), bufferSizeFile1);
		file2.read(bufferFile2.get(), bufferSizeFile2);

		///Compare buffers
		if (strcmp(bufferFile1.get(), bufferFile2.get()) != 0)
			return false;

	}

	/// If files have different sizes
	if (file1 || file2)
		return false;

	return true;
}


void FileManager::initCharBuf(char *buf, uint32_t bufSize)
{
	for (uint32_t i = 0; i < bufSize; i++)
	{
		buf[i] = '\0';
	}
}