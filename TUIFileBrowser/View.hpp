#pragma once
#include <string>

class View {

};

class TopSection {
public:
	//Constructor/Destructor
	TopSection(unsigned long const& textBoxLength);
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
	unsigned short const						topSectionRowSize;
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
};

class MiddleSection {
public:
	//Constructor/Destructor
	MiddleSection(short const& height, short const& width, unsigned short const& topSectionSize, unsigned short const& bottomSectionSize);
	~MiddleSection() = default;

	//fields
	int											linePosition;

	//const fields
	unsigned short const						fileSectionSize;
	unsigned short const						verticalScrollBarSize;
	unsigned short const						pageSize;
	short const									scrollerXLocation;
	short const									topScrollerYLocation;
	short const									bottomScrollerYLocation;
};

class BottomSection {
public:
	//Constructor/Destructor
	BottomSection();
	~BottomSection() = default;

	//const fields
	unsigned short const						bottomSectionSize;
	std::string const							totalFilesFolderLabel;
	std::string const							filesMatchedLabel;
	std::string const							filesMatchedSizeLabel;
};