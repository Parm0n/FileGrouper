#include <iostream>
#include <cassert>
#include <filesystem>
#include <string>
#include <vector>
#include <GroupWorker.h>

int main(int argc, char **argv)
{
	//auto start = std::chrono::system_clock::now();
	if (argc != 3)
	{
		assert(false);
		return -1;
	}

	uint32_t i32ThreadsCount = atoi(argv[1]);
	std::string path = argv[2];// "C:/Users/iparm/Documents/HID/FileGrouper/Debug/files";

	if (i32ThreadsCount < 1 || i32ThreadsCount > 255)
	{
		assert(false);
		return -1;
	}

	if (!std::filesystem::exists(path))
	{
		assert(false);
		return -1;
	}

	std::vector<std::filesystem::path> vFileNames;
	for (const auto & entry : std::filesystem::directory_iterator(path))
		vFileNames.push_back(entry.path());

	GroupWorker worker((uint32_t)i32ThreadsCount, vFileNames);

	worker.run();

	//auto end = std::chrono::system_clock::now();
	//auto elapsed = end - start;
	//std::cout << '\n' << elapsed.count() << '\n';

	return 1;

}