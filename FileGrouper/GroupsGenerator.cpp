#include "GroupsGenerator.h"
#include <algorithm>

GroupsGenerator::GroupsGenerator(uint32_t threadsCount, HashManager * hashManager)
{
	m_uThreadsCount = threadsCount;
	m_mpHashMap = hashManager->getHashMap();
	logger = &(Logger::getInstance());
}

///Start threads
void GroupsGenerator::generateGroups()
{
	std::vector<std::thread> vThreads;

	for (uint32_t i = 0; i < m_uThreadsCount; i++)
	{
		vThreads.emplace_back(&GroupsGenerator::worker, this);
	}

	for (auto& i : vThreads) {
		i.join();
	}
}

std::vector<std::string> GroupsGenerator::convertVectorToFileNames(std::vector<FileManager> vFileManagers)
{
	std::vector<std::string> vGroupString;

	for (auto &file : vFileManagers)
	{
		vGroupString.push_back(file.getFileName());
	}

	return vGroupString;
}

void GroupsGenerator::worker()
{
	std::vector<FileManager> vGroupCandidates;

	while (true)
	{
		///Thread safety getting and removing of HashTable element
		{
			std::scoped_lock<std::mutex> lc(m_hashMapMutex);
			if (m_mpHashMap->empty())
				return;
			vGroupCandidates = m_mpHashMap->begin()->second;
			m_mpHashMap->erase(m_mpHashMap->begin());
		}

		if (vGroupCandidates.size() == 1) {
			///Just write single-file group to the console
			logger->addGroup(convertVectorToFileNames(vGroupCandidates));
			vGroupCandidates.clear();
		}
		else
		{
			std::vector<FileManager> vGroup;

			///Process all files corresponding to one general hash
			while (vGroupCandidates.size() > 0)
			{
				FileManager currentFile = vGroupCandidates[0];
				vGroup.push_back(currentFile);
				vGroupCandidates.erase(vGroupCandidates.begin());

				///Needs to swap vectors
				std::vector<FileManager> tmp;

				///Find files wich are equal to the currentFile 
				for (auto &file : vGroupCandidates)
				{
					if (FileManager::equal(&currentFile, &file))
					{
						vGroup.push_back(file);
					}
					else
					{
						tmp.push_back(file);
					}
				}

				///tmp - vector wich contains only elements with undefined(yet) group
				vGroupCandidates = tmp;

				///Write to the console
				logger->addGroup(convertVectorToFileNames(vGroup));

				vGroup.clear();
			}
		}
	}
}