#pragma once

#include <vector>
#include <filesystem>
#include <iostream>


/// A class that represents a file with basic functions that can be done with it
class FileManager
{
public:
	explicit FileManager();
	explicit FileManager(std::filesystem::path filePath);

	/// Object of a single file hash 
	struct FileHash
	{
		size_t data = 0U;
		bool operator<(FileHash x) const {
			return data < x.data;
		}
	};

	int setFilePath(std::filesystem::path filePath);

	///Gets file name(not path)
	std::string getFileName();

	///Reads whole file and generates hash of it(this hash will be stored in private variable)
	void generateHash();
	FileHash getCurrentHash();

	static bool equal(const FileManager *f1, const FileManager *f2);

	///Utility function that initialize buf by '\0'
	static void initCharBuf(char *buf, uint32_t bufSize);
private:
	std::filesystem::path m_filePath;
	FileHash m_sCurrentHash;
};

