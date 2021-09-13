#pragma once

#include <vector>
#include <filesystem>
#include <mutex>
#include <map>
#include "FileManager.h"

///Calculates hashes from files and makes hash table with this data
class HashManager
{
public:
	explicit HashManager(uint32_t threadsCount, 
		std::map<FileManager::FileHash, std::vector<FileManager>>* hashMap,
		std::vector<FileManager> fileNames);

	///Initalize worker threads
	void generateHashMap();
	std::map<FileManager::FileHash, std::vector<FileManager>>* getHashMap();

private:

	///Takes 1 file, generates hash from it and sets it to Hash Map
	void processNextFile();

	std::mutex m_filesVectorMutex;
	std::vector<FileManager> m_vUnProcessedFiles;
	std::mutex m_hashMapMutex;
	std::map<FileManager::FileHash, std::vector<FileManager>>* m_mpHashMap;
	uint32_t m_uThreadsCount = 0;
};

