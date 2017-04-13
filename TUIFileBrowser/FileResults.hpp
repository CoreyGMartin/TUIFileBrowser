#pragma once
#include <regex>
#include <memory>
#include <set>
#include <vector>
#include <filesystem>

#include "Observer.hpp"

using namespace std::experimental::filesystem;

class FileResults : public Subject {
	//fields
	std::vector<std::string>	filesMatched;
	std::vector<path>			directories;
	size_t						matchedFileSize;
	size_t						numberOfMatchedFiles;

	//const fields
	std::string const 			rootFolder;

public:
	//Constructor/Destructor
	FileResults(std::string const& rootFolder, bool const& recursiveSearch);
	~FileResults() = default;

	//Methods
	void								Filter(std::string const& regex);
	std::vector<std::string> const&		PreviousFilter();
	size_t const&						FilesFoldersSearched();
	size_t const&						MatchedFileSize();
	size_t const&						NumberOfMatchedFiles();
};