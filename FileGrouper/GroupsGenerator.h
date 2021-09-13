#pragma once

#include <mutex>
#include <map>
#include "HashManager.h"
#include "Logger.h"

/// A class that sorts a table of hashes and files, as a result generating groups of files
class GroupsGenerator
{
public:
	explicit GroupsGenerator(uint32_t threadsCount,
		HashManager* hashManager);

	///Initalize worker threads
	void generateGroups();

	static std::vector<std::string> convertVectorToFileNames(std::vector<FileManager> vFileManagers);
private:

	///Processes all files with same hash and makes groups of them
	void worker();

	std::mutex m_hashMapMutex;
	std::map<FileManager::FileHash, std::vector<FileManager>>* m_mpHashMap;
	uint32_t m_uThreadsCount = 0;
	Logger *logger = nullptr;
};

