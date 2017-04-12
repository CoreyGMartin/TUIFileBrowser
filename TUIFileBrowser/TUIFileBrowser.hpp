#pragma once
#include <memory>
#include <filesystem>
#include <algorithm>

#include <App.hpp>
#include "Observer.hpp"
#include "Component.hpp"
#include "ThickAdapter.hpp"
#include "FileResults.hpp"

using namespace std::experimental::filesystem;

class TUIFileBrowser : public App {

	std::unique_ptr<ThickAdapter>				thick;
	std::unique_ptr<FileResults>				fileResults;
	unsigned short const						height = 30;
	unsigned short const						width = 120;
	unsigned long const							textBoxLength = (unsigned long)(width * .25); //Each textbox will be 25% of the console screen width

	//Top Section
	std::string									regex;
	std::string									rootFolder;
	bool										recursiveSearch;
	size_t										cursorPosition;
	size_t										fileInputAperture;
	size_t										regexInputAperature;
	bool										fileInputTextBoxSelected;
	bool										regexInputTextBoxSelected;

	unsigned short const						topSectionRowSize					= 4;
	std::string const							rootFolderLabel						= "Root Folder:";
	std::string const							searchRecursivelyLabel				= "Search Recursively:";
	std::string const							searchLabel							= "Search";
	std::string const							filterLabel							= "Filter:";
	std::string const							applyFilterLabel					= "Apply Filter";

	short const									fileInputRow						= 0;
	short const									fileInputColumnStart				= (short)rootFolderLabel.length() + 1;
	short const									fileInputColumnEnd					= fileInputColumnStart + (short)textBoxLength;
	short const									searchRecursivelyCheckBoxRow		= fileInputRow;
	short const									searchRecursivelyCheckBoxColumn		= fileInputColumnEnd + (short)searchRecursivelyLabel.length() + 1;
	short const									searchButtonRow						= fileInputRow;
	short const									searchButtonColumnStart				= searchRecursivelyCheckBoxColumn + 2; //add an extra for button padding left
	short const									searchButtonColumnEnd				= searchButtonColumnStart + (short)searchLabel.length() + 1; //add an extra for button padding right

	short const									regexInputRow						= 2;
	short const									regexInputColumnStart				= (short)filterLabel.length() + 1;
	short const									regexInputColumnEnd					= regexInputColumnStart + (short)textBoxLength;
	short const									applyFilterButtonRow				= regexInputRow;
	short const									applyFilterButtonColumnStart		= regexInputColumnEnd + 2; //add an extra for button padding left
	short const									applyFilterButtonColumnEnd			= applyFilterButtonColumnStart + (short)applyFilterLabel.length() + 1; //add an extra for button padding right 

	//Bottom Section
	unsigned short const						bottomSectionSize					= 2;
	std::string const							totalFilesFolderLabel				= "Total Files/Folders Searched:";
	std::string const							filesMatchedLabel					= "Files Matched:";
	std::string const							filesMatchedSizeLabel				= "Size of Files Matched:";

	//Middle Section
	int											linePosition;
	unsigned short const						fileSectionSize						= height - topSectionRowSize - bottomSectionSize; //top and bottom section will always be the same, but file display section will change
	unsigned short const						verticalScrollBarSize				= fileSectionSize - 2; //one above for up arrow, one below for down arrow
	unsigned short const						pageSize							= verticalScrollBarSize + 1; 
	short const									scrollerXLocation					= width - 1;
	short const									topScrollerYLocation				= bottomSectionSize;
	short const									bottomScrollerYLocation				= height - 3;

	//Utility Methods
	void	processMouseEvent(MOUSE_EVENT_RECORD const& mer);
	void	processKeyEvent(KEY_EVENT_RECORD const& ker);

	void Search();
	void ApplyFilter();

public:
	TUIFileBrowser();
	~TUIFileBrowser() = default;

	int		execute() override;

} paintApp;