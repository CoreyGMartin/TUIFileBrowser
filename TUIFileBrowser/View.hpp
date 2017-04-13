#pragma once
#include <string>

#include "Observer.hpp"
#include "FileResults.hpp"

class TopSection : public Observer {
public:
	//Constructor/Destructor
	TopSection(unsigned long const& textBoxLength, unsigned short const& width, std::unique_ptr<FileResults> const& fileResults);
	~TopSection() = default;

	//fields
	std::string									regex;
	std::string									rootFolder;
	bool										recursiveSearch;
	size_t										cursorPosition;
	size_t										fileInputAperture;
	size_t										regexInputAperature;
	bool										fileInputTextBoxSelected;
	bool										regexInputTextBoxSelected;

	//const fields
	std::unique_ptr<FileResults> const&			fileResults;
	unsigned short const						width;
	unsigned long const&						textBoxLength;
	unsigned short const						sectionSize;
	std::string const							rootFolderLabel;
	std::string const							searchRecursivelyLabel;
	std::string const							searchLabel;
	std::string const							filterLabel;
	std::string const							applyFilterLabel;

	short const									fileInputRow;
	short const									fileInputColumnStart;
	short const									fileInputColumnEnd;
	short const									searchRecursivelyCheckBoxRow;
	short const									searchRecursivelyCheckBoxColumn;
	short const									searchButtonRow;
	short const									searchButtonColumnStart;
	short const									searchButtonColumnEnd;

	short const									regexInputRow;
	short const									regexInputColumnStart;
	short const									regexInputColumnEnd;
	short const									applyFilterButtonRow;
	short const									applyFilterButtonColumnStart;
	short const									applyFilterButtonColumnEnd;

	//Method
	void update() override;
};

class MiddleSection : public Observer {
public:
	//Constructor/Destructor
	MiddleSection(short const& height, short const& width, unsigned short const& topSectionSize, unsigned short const& bottomSectionSize, std::unique_ptr<FileResults> const& fileResults);
	~MiddleSection() = default;

	//fields
	int											linePosition;

	//const fields
	std::unique_ptr<FileResults> const&			fileResults;
	unsigned short const						width;
	unsigned short const						sectionSize;
	short const									rowOffSet;
	unsigned short const						verticalScrollBarSize;
	unsigned short const						pageSize;
	short const									scrollerXLocation;
	short const									topScrollerYLocation;
	short const									bottomScrollerYLocation;

	//Method
	void update() override;
};

class BottomSection : public Observer {
public:
	//Constructor/Destructor
	BottomSection(unsigned long const& textBoxLength, unsigned short const& width, std::unique_ptr<FileResults> const& fileResults, short const& rowOffSet);
	~BottomSection() = default;

	//const fields
	std::unique_ptr<FileResults> const&			fileResults;
	unsigned short const						width;
	unsigned long const&						textBoxLength;
	unsigned short const						sectionSize;
	short const									rowOffSet;
	std::string const							totalFilesFolderLabel;
	std::string const							filesMatchedLabel;
	std::string const							filesMatchedSizeLabel;

	//Method
	void update() override;
};