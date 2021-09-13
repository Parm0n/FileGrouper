#pragma once
#include <vector>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include <mutex>

///Class wich writes logs to the console
class Logger
{
public:
	explicit Logger();
	~Logger();

	void addGroup(std::vector<std::string>  group);

	///To be a singleton
	static Logger& getInstance();

private:
	///Takes groups one by one from the group vector and writes file names to the console
	void worker();

	std::vector<std::vector<std::string>> m_mvFileGroups;
	std::thread* m_pWritter;
	bool m_bReadyToWrite = false;
	bool m_bEnd = false;
	std::mutex m_fileGroupsMutex;
	std::condition_variable m_fileGroupsCV;

};

