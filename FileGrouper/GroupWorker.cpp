#include "GroupWorker.h"
#include "HashManager.h"
#include "GroupsGenerator.h"

GroupWorker::GroupWorker()
{
}

GroupWorker::GroupWorker(uint32_t threadsCount, std::vector<std::filesystem::path> filePaths)
{
	m_uThreadsCount = (threadsCount);
	m_vFilePaths = (filePaths);
}

void GroupWorker::run()
{
	///File names -> FileManagers
	std::vector<FileManager> vFileManagers;
	for (const auto &it : m_vFilePaths)
	{
		vFileManagers.emplace_back(it);
	}

	HashManager hashManager(m_uThreadsCount, &m_mHashMap, vFileManagers);

	/**
	* First, create hashes from all input files.
	* After creating these hashes, the function will structure the files and hashes so that 
	* each hash in the map corresponds to a vector of files from which this hash was generated.
	*/
	hashManager.generateHashMap();

	GroupsGenerator groupsGenerator(m_uThreadsCount, &hashManager);

	/**
	* From the previously obtained hash table, a pass is made through each hash in the map.
	* For each vector and associated hash, the following is done:
	*	1. If a vector has one file, then it is written as an independent group
	*	2. Otherwise, the files in the vector are compared to those that are equal are written as a group
	*/
	groupsGenerator.generateGroups();
}