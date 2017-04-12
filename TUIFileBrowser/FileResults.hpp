#pragma once
#include <regex>
#include <memory>
#include <set>
#include <vector>
#include <filesystem>

using namespace std::experimental::filesystem;

class FileResults {
	//fields
	std::string const 			rootFolder;
	bool const					recursiveSearch;
	std::vector<std::string>	filesMatched;
	std::vector<path>			directories;
	size_t						matchedFileSize;
	size_t						numberOfMatchedFiles;

public:
	//Constructor/Destructor
	FileResults(std::string const& rootFolder, bool const& recursiveSearch);
	~FileResults() = default;

	//Methods
	std::vector<std::string>	Filter(std::string const& regex);
	std::vector<std::string>	PreviousFilter();
	size_t						FilesFoldersSearched();
	size_t						MatchedFileSize();
	size_t						NumberOfMatchedFiles();
};