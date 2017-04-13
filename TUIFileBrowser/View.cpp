#include "View.hpp"
#include "Commands.hpp"

TopSection::TopSection(unsigned long const& textBoxLength, unsigned short const& width, std::unique_ptr<FileResults> const& fileResults)
	:	fileResults(fileResults),
		width(width),
		textBoxLength(textBoxLength),
		sectionSize(4),
		rootFolderLabel("Root Folder:"),
		searchRecursivelyLabel("Search Recursively:"),
		searchLabel("Search"),
		filterLabel("Filter:"),
		applyFilterLabel("Apply Filter"),
		fileInputRow(0),
		fileInputColumnStart(rootFolderLabel.length() + 1),
		fileInputColumnEnd(fileInputColumnStart + (short)textBoxLength),
		searchRecursivelyCheckBoxRow(fileInputRow),
		searchRecursivelyCheckBoxColumn(fileInputColumnEnd + (short)searchRecursivelyLabel.length() + 1),
		searchButtonRow(fileInputRow),
		searchButtonColumnStart(searchRecursivelyCheckBoxColumn + 2), //add an extra for button padding left
		searchButtonColumnEnd(searchButtonColumnStart + (short)searchLabel.length() + 1), //add an extra for button padding right
		regexInputRow(2),
		regexInputColumnStart((short)filterLabel.length() + 1),
		regexInputColumnEnd(regexInputColumnStart + (short)textBoxLength),
		applyFilterButtonRow(regexInputRow),
		applyFilterButtonColumnStart(regexInputColumnEnd + 2), //add an extra for button padding left
		applyFilterButtonColumnEnd(applyFilterButtonColumnStart + (short)applyFilterLabel.length() + 1) //add an extra for button padding right
{
	fileInputTextBoxSelected = false;
	regexInputTextBoxSelected = false;
	fileInputAperture = 0;
	regexInputAperature = 0;
}

void TopSection::update() {
	std::unique_ptr<Command>(new RenderTopSection(this))->execute();
}

MiddleSection::MiddleSection(short const& height, short const& width, unsigned short const& topSectionSize, unsigned short const& bottomSectionSize, std::unique_ptr<FileResults> const& fileResults)
	:	fileResults(fileResults), 
		width(width),
		sectionSize(height - topSectionSize - bottomSectionSize), //top and bottom section will always be the same, but file display section will change
		verticalScrollBarSize(sectionSize - 2), //one above for up arrow, one below for down arrow
		pageSize(verticalScrollBarSize + 1),
		scrollerXLocation(width - 1),
		topScrollerYLocation(topSectionSize),
		bottomScrollerYLocation(height - 3),
		rowOffSet(topSectionSize)
{
	linePosition = 0;
}

void MiddleSection::update() {
	std::unique_ptr<Command>(new RenderMiddleSection(this->fileResults, this))->execute();
}

BottomSection::BottomSection(unsigned long const& textBoxLength, unsigned short const& width, std::unique_ptr<FileResults> const& fileResults, short const& rowOffSet)
	:	fileResults(fileResults),
		width(width),
		textBoxLength(textBoxLength),
		sectionSize(2),
		totalFilesFolderLabel("Total Files/Folders Searched:"),
		filesMatchedLabel("Files Matched:"),
		filesMatchedSizeLabel("Size of Files Matched:"),
		rowOffSet(rowOffSet)
{}

void BottomSection::update() {
	std::unique_ptr<Command>(new RenderBottomSection(this->fileResults, this))->execute();
}