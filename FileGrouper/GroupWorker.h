#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <filesystem>
#include "FileManager.h"

///Main FileGrouper class: use it to group givrn files by their content
class GroupWorker
{
public:
	explicit GroupWorker();
	explicit GroupWorker(uint32_t threadsCount, std::vector<std::filesystem::path> filePaths);

	///Function that starts reading files, processes and groups them
	void run();

private:
	uint32_t m_uThreadsCount = 0;
	std::vector<std::filesystem::path> m_vFilePaths;

	///Hash table: each file has its hash, but the same hash can have more files
	std::map<FileManager::FileHash, std::vector<FileManager>> m_mHashMap;
};

