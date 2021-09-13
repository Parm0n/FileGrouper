#include "HashManager.h"
#include <vector>


HashManager::HashManager(uint32_t threadsCount, 
	std::map<FileManager::FileHash, std::vector<FileManager>>* hashMap,
	std::vector<FileManager> fileNames)
{
	m_uThreadsCount = threadsCount;
	m_vUnProcessedFiles = fileNames;
	m_mpHashMap = hashMap;
}

///Creating worker threads
void HashManager::generateHashMap()
{
	std::vector<std::thread> vThreads; 

	for (uint32_t i = 0; i < m_uThreadsCount; i++)
	{
		vThreads.emplace_back(&HashManager::processNextFile, this);
	}

	for (auto& i : vThreads) {
		i.join();
	}

}

std::map<FileManager::FileHash, std::vector<FileManager>>* HashManager::getHashMap()
{
	return m_mpHashMap;
}

void HashManager::processNextFile()
{
	///Works till unprocessed vector is not empty
	while (true)
	{
		FileManager currentFilePath;

		///Thread safety getting file and removing it from the vector
		{
			std::scoped_lock<std::mutex> lc(m_filesVectorMutex);

			if (m_vUnProcessedFiles.size() == 0)
				return;

			currentFilePath = m_vUnProcessedFiles.back();
			m_vUnProcessedFiles.pop_back();
		}

		currentFilePath.generateHash();

		{
			std::scoped_lock<std::mutex> lc(m_hashMapMutex);
			///Set file with it hash to the Hash Table
			(*m_mpHashMap)[currentFilePath.getCurrentHash()].push_back(currentFilePath);
		}
	}
}