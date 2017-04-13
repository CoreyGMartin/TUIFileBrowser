#include "FileResults.hpp"

FileResults::FileResults(std::string const& rootFolder, bool const& recursiveSearch) : rootFolder(rootFolder) /*, recursiveSearch(recursiveSearch)*/ {
	if (recursiveSearch) {
		//Recursive
		for (recursive_directory_iterator dir(rootFolder), dirEnd; dir != dirEnd; ++dir)
			directories.push_back(dir->path());
	}
	else {
		//Non-recursive
		for (directory_iterator dir(rootFolder), dirEnd; dir != dirEnd; ++dir)
			directories.push_back(dir->path());
	}
}

void FileResults::Filter(std::string const& regex) {
	matchedFileSize = 0;
	numberOfMatchedFiles = 0;
	filesMatched.clear();

	filesMatched.push_back(rootFolder.substr(rootFolder.find_last_of("\\")));
	for (auto& path : directories) {

		std::string line = path.string();
		std::string spaces(std::count(line.begin(), line.end(), '\\'), ' ');

		if (is_directory(path)) {
			filesMatched.push_back(spaces + line.substr(line.find_last_of("\\")));
		}
		else if (is_regular_file(path) && std::regex_match(path.extension().string(), std::regex(regex))) {
			++numberOfMatchedFiles;
			matchedFileSize += (size_t)file_size(path);
			filesMatched.push_back(spaces + line.substr(line.find_last_of("\\") + 1));
		}
	}

	notify();
}

std::vector<std::string> const& FileResults::PreviousFilter() {
	return filesMatched;
}

size_t const& FileResults::FilesFoldersSearched() {
	return directories.size();
}

size_t const& FileResults::MatchedFileSize() {
	return matchedFileSize;
}

size_t const& FileResults::NumberOfMatchedFiles() {
	return numberOfMatchedFiles;
}