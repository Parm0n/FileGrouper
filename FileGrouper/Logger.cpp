#include "Logger.h"
#include <iostream>

Logger::Logger()
{
	m_pWritter = new std::thread(&Logger::worker, this);
}

Logger::~Logger()
{
	m_bEnd = true;
	m_bReadyToWrite = true;
	m_fileGroupsCV.notify_one();
	m_pWritter->join();
}

///Adding new group to the vector and notifing writing thread
void Logger::addGroup(std::vector<std::string>  group)
{
	{
		std::scoped_lock<std::mutex> lc(m_fileGroupsMutex);

		if (!group.empty())
			m_mvFileGroups.push_back(group);
	}

	if(!m_mvFileGroups.empty())
		m_bReadyToWrite = true;

	m_fileGroupsCV.notify_one();
}

Logger & Logger::getInstance()
{
	static Logger instance;
	return instance;
}

void Logger::worker()
{
	std::vector<std::string> currentGroup;

	while (!m_bEnd || !m_mvFileGroups.empty())
	{
		/// Wait until addGroup sends data
		std::unique_lock<std::mutex> lk(m_fileGroupsMutex);
		m_fileGroupsCV.wait(lk, [&] {return m_bReadyToWrite;});

		///Geting next group and unlock mutex.
		if (!m_mvFileGroups.empty())
		{
			currentGroup = m_mvFileGroups.back();
			m_mvFileGroups.pop_back();
		}

		if (m_mvFileGroups.size() == 0)
		{
			m_bReadyToWrite = false;
		}

		lk.unlock();

		if (!currentGroup.empty())
		{
			for (uint32_t i = 0; i < currentGroup.size(); i++)
			{
				std::cout << currentGroup[i];
				if (!(i == currentGroup.size() - 1))
				{
					std::cout << ",";
				}
				else 
				{
					std::cout << "\n";
				}
			}
			currentGroup.clear();

		}
	}

	std::cout << "---END---";
}
