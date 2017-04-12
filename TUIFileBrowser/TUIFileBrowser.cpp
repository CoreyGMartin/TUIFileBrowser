#include "TUIFileBrowser.hpp"

static bool done = false;

bool handleCtrlC(unsigned long ctrlType) {
	switch (ctrlType) {
	case CTRL_C_EVENT:
		done = true;
		return true;
	}
	return false;
}

TUIFileBrowser::TUIFileBrowser() {
	thick = ThickAdapter::GetInstance();

	//Top Section defaults
	fileInputAperture = 0;
	fileInputTextBoxSelected = false;
	regexInputTextBoxSelected = false;

	//Middle Section defaults
	linePosition = 0;
	
	//Bottom Section defaults


	//TODO: Remove below if not needed
	//short x = 1, y = 0;

	//inputSectionComponents.push_back(std::unique_ptr<Component>(new Component(x, y, rootFolderLabel, rootFolderLabel.length(), 0, 0)));
	//x += (short)rootFolderLabel.length();

	//inputSectionComponents.push_back(std::unique_ptr<Component>(new Component(1, 0, rootFolderLabel, rootFolderLabel.length(), 0, 0)));
}

int TUIFileBrowser::execute() {
	//Save console state
	thick->saveConsoleState();
	
	//Set defaults
	regex = R"x(.*)x";
	rootFolder = "C:\\Java";
	//rootFolder = "C:\\$c++Project3-4";
	recursiveSearch = false;

	//Process arguments
	std::vector<std::string> args = GetArgs();
	if (args.size() != 0) {
		for (size_t i = 0; i < args.size(); ++i) {
			if (args[i] == "-r")
				recursiveSearch = true;
			else if (args[i].find(R"x(:\)x") != std::string::npos)
				rootFolder = args[i];
			else if (args[i].find("\"") != std::string::npos)
				regex = args[i];
		}
	}

	//Setup
	thick->setTitle("TUI File Browser");
	thick->setSize(width, height);
	thick->setConsoleMode(ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	thick->setConsoleCtrlHandler(handleCtrlC, true);
	thick->hideCursor();
	thick->FillAndSetConsoleBackgroundColour(BACKGROUND_INTENSITY, ' ');

	//Draw views
	thick->DrawInputSection(recursiveSearch, regex, rootFolder, applyFilterLabel, filterLabel, searchLabel, rootFolderLabel, searchRecursivelyLabel, width, topSectionRowSize, textBoxLength);
	thick->DrawStatsSection(0, 0, 0, filesMatchedSizeLabel, filesMatchedLabel, totalFilesFolderLabel, topSectionRowSize + fileSectionSize, width, bottomSectionSize, textBoxLength);

	//Perform the search
	Search();

	//Listen for events
	std::vector<INPUT_RECORD> inBuffer(128);
	while (!done) {
		unsigned long numEvents;
		thick->readConsoleInput(inBuffer, (unsigned long)inBuffer.size(), numEvents);
		if (numEvents) {
			for (size_t iEvent = 0; iEvent < numEvents; ++iEvent) {
				switch (inBuffer[iEvent].EventType) {
				case MOUSE_EVENT:
					processMouseEvent(inBuffer[iEvent].Event.MouseEvent);
					break;
				case KEY_EVENT:
					processKeyEvent(inBuffer[iEvent].Event.KeyEvent);
					break;
				}
			}
		}
	}

	//Restore Console state
	thick->restoreConsoleState();

	return EXIT_SUCCESS;
}

void TUIFileBrowser::Search() {
	//File Search
	fileResults = std::unique_ptr<FileResults>(new FileResults(rootFolder, recursiveSearch));
	std::vector<std::string> lines = fileResults->Filter(regex);

	//Draw File Section
	thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, lines);

	//Draw stats section
	thick->DrawStatsSection(fileResults->MatchedFileSize(), fileResults->NumberOfMatchedFiles(), fileResults->FilesFoldersSearched(), filesMatchedSizeLabel, filesMatchedLabel, totalFilesFolderLabel, topSectionRowSize + fileSectionSize, width, bottomSectionSize, textBoxLength);
}

void TUIFileBrowser::ApplyFilter() {
	std::vector<std::string> lines = fileResults->Filter(regex);

	//Draw File Section
	thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, lines);

	//Draw stats section
	thick->DrawStatsSection(fileResults->MatchedFileSize(), fileResults->NumberOfMatchedFiles(), fileResults->FilesFoldersSearched(), filesMatchedSizeLabel, filesMatchedLabel, totalFilesFolderLabel, topSectionRowSize + fileSectionSize, width, bottomSectionSize, textBoxLength);
}

void TUIFileBrowser::processMouseEvent(MOUSE_EVENT_RECORD const& me) {
	switch (me.dwEventFlags) {
		//case MOUSE_MOVED: {

		//} break;
	case 0: {
		int const leftClickUp = !me.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;

		if (leftClickUp) {
			short const mousePositionX = me.dwMousePosition.X;
			short const mousePositionY = me.dwMousePosition.Y;

			//File input section hit
			if (mousePositionY == fileInputRow && mousePositionX >= fileInputColumnStart && mousePositionX < fileInputColumnEnd) {
				//Find cursor position
				short cursorPositionX = mousePositionX - fileInputColumnStart;
				const short rootFolderLength = (short)rootFolder.length();
				if (cursorPositionX > rootFolderLength)
					cursorPositionX = rootFolderLength;

				//Place cursor
				thick->showAndSetCursorPosition(cursorPositionX + fileInputColumnStart, fileInputRow);
				cursorPosition = cursorPositionX;
				fileInputTextBoxSelected = true;
				regexInputTextBoxSelected = false;
			}
			//Regex input section hit
			else if (mousePositionY == regexInputRow && mousePositionX >= regexInputColumnStart && mousePositionX < regexInputColumnEnd) {
				//Find cursor position
				short cursorPositionX = mousePositionX - regexInputColumnStart;
				const short regexLength = (short)regex.length();
				if (cursorPositionX > regexLength)
					cursorPositionX = regexLength;

				//Place cursor
				thick->showAndSetCursorPosition(cursorPositionX + regexInputColumnStart, regexInputRow);
				cursorPosition = cursorPositionX;
				regexInputTextBoxSelected = true;
				fileInputTextBoxSelected = false;
			}
			//Check for Scroll Up
			else if (scrollerXLocation == mousePositionX && topScrollerYLocation == mousePositionY) {
				if (linePosition != 0) {
					--linePosition;
					thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, fileResults->PreviousFilter());
				}
			}
			//Check for Scroll Down
			else if (scrollerXLocation == mousePositionX && bottomScrollerYLocation == mousePositionY) {
				if ((linePosition + (verticalScrollBarSize + 1)) < (short)fileResults->PreviousFilter().size()) {
					++linePosition;
					thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, fileResults->PreviousFilter());
				}
			}
			//Check for Recursive checkbox hit
			else if (searchRecursivelyCheckBoxColumn == mousePositionX && searchRecursivelyCheckBoxRow == mousePositionY) {
				recursiveSearch = !recursiveSearch;
				//TODO: Remove change certain location
				thick->DrawInputSection(recursiveSearch, "", rootFolder, applyFilterLabel, filterLabel, searchLabel, rootFolderLabel, searchRecursivelyLabel, width, topSectionRowSize, textBoxLength);
			}
			//Check for Search button hit
			else if (mousePositionX >= searchButtonColumnStart && mousePositionX <= searchButtonColumnEnd && searchButtonRow == mousePositionY) {
				Search();
			}
			//Check for Apply Filter button hit
			else if (mousePositionX >= applyFilterButtonColumnStart && mousePositionX <= applyFilterButtonColumnEnd && applyFilterButtonRow == mousePositionY) {
				ApplyFilter();
			} else {
				thick->hideCursor();
				regexInputTextBoxSelected = false;
				fileInputTextBoxSelected = false;
			}
		}
	}
	} 
}

void TUIFileBrowser::processKeyEvent(KEY_EVENT_RECORD const& ke) {
	unsigned short const upKeyVKCode = 38;
	unsigned short const downKeyVKCode = 40;
	unsigned short const pageUpKeyVKCode = 33;
	unsigned short const pageDownKeyVKCode = 34;

	if (!(fileInputTextBoxSelected || regexInputTextBoxSelected)) {
		//Up Key
		if (ke.bKeyDown && ke.wVirtualKeyCode == upKeyVKCode) {
			if (linePosition != 0) {
				--linePosition;
				thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, fileResults->PreviousFilter());
			}
		}
		//Down key
		else if (ke.bKeyDown && ke.wVirtualKeyCode == downKeyVKCode) {
			if ((linePosition + (verticalScrollBarSize + 1)) < (short)fileResults->PreviousFilter().size()) {
				++linePosition;
				thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, fileResults->PreviousFilter());
			}
		}
		//Page up Key
		else if (ke.bKeyDown && ke.wVirtualKeyCode == pageUpKeyVKCode) {
			if ((linePosition - pageSize) >= 0) {
				linePosition -= pageSize;
			}
			else {
				//near the top, pop up 
				linePosition = 0;
			}
			thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, fileResults->PreviousFilter());
		}
		//Page down key
		else if (ke.bKeyDown && ke.wVirtualKeyCode == pageDownKeyVKCode) {
			if ((linePosition + (pageSize * 2)) <= (short)fileResults->PreviousFilter().size()) {
				linePosition += pageSize;
			}
			else {
				//near the bottom, pop down
				linePosition = (fileResults->PreviousFilter().size() - pageSize);
			}
			thick->DrawFileSection(width, verticalScrollBarSize, topSectionRowSize, fileSectionSize, linePosition, fileResults->PreviousFilter());
		}
	}
	else if (fileInputTextBoxSelected && ke.bKeyDown) {
		switch (ke.wVirtualKeyCode) {
		case VK_BACK:
			if (0 < cursorPosition && cursorPosition <= rootFolder.size()) {
				--cursorPosition;
				rootFolder.erase(cursorPosition, 1);
			}
			break;
		case VK_DELETE:
			if (0 <= cursorPosition && cursorPosition < rootFolder.size())
				rootFolder.erase(cursorPosition, 1);
			break;
		case VK_LEFT:
			if (cursorPosition > 0)
				--cursorPosition;
			break;
		case VK_RIGHT:
			if (cursorPosition < rootFolder.size())
				++cursorPosition;
			break;
		case VK_END:
			cursorPosition = rootFolder.size();
			break;
		case VK_HOME:
			cursorPosition = 0;
			break;
		//case VK_RETURN:
		//	OutputString(2, TITLE_SECTION_START + 1, editControlString, TITLE_SECTION_ATTR);
		//	break;
		default:
			char ch = ke.uChar.AsciiChar;
			if (isprint(ch)) {
				rootFolder.insert(cursorPosition++ + rootFolder.begin(), ch);
			}
		}
		
		// show the string in the control
		auto practicalSize = rootFolder.size() + 1;
		while (cursorPosition < fileInputAperture)
			--fileInputAperture;

		while (cursorPosition - fileInputAperture >= textBoxLength)
			++fileInputAperture;

		while (practicalSize - fileInputAperture<textBoxLength && practicalSize > textBoxLength)
			--fileInputAperture;

		auto s = rootFolder.substr(fileInputAperture, textBoxLength);
		s += std::string(textBoxLength - s.size(), ' ');

		//Show string
		thick->DrawInputSection(recursiveSearch, regex, s, applyFilterLabel, filterLabel, searchLabel, rootFolderLabel, searchRecursivelyLabel, width, topSectionRowSize, textBoxLength);

		// place cursor in the control
		thick->showAndSetCursorPosition(cursorPosition - fileInputAperture + (size_t)fileInputColumnStart, fileInputRow);
	}
	else if (regexInputTextBoxSelected && ke.bKeyDown) {
		switch (ke.wVirtualKeyCode) {
		case VK_BACK:
			if (0 < cursorPosition && cursorPosition <= regex.size()) {
				--cursorPosition;
				regex.erase(cursorPosition, 1);
			}
			break;
		case VK_DELETE:
			if (0 <= cursorPosition && cursorPosition < regex.size())
				regex.erase(cursorPosition, 1);
			break;
		case VK_LEFT:
			if (cursorPosition > 0)
				--cursorPosition;
			break;
		case VK_RIGHT:
			if (cursorPosition < regex.size())
				++cursorPosition;
			break;
		case VK_END:
			cursorPosition = regex.size();
			break;
		case VK_HOME:
			cursorPosition = 0;
			break;
			//case VK_RETURN:
			//	OutputString(2, TITLE_SECTION_START + 1, editControlString, TITLE_SECTION_ATTR);
			//	break;
		default:
			char ch = ke.uChar.AsciiChar;
			if (isprint(ch)) {
				regex.insert(cursorPosition++ + regex.begin(), ch);
			}
		}

		// show the string in the control
		auto practicalSize = regex.size() + 1;
		while (cursorPosition < regexInputAperature)
			--regexInputAperature;

		while (cursorPosition - regexInputAperature >= textBoxLength)
			++regexInputAperature;

		while (practicalSize - regexInputAperature < textBoxLength && practicalSize > textBoxLength)
			--regexInputAperature;

		auto s = regex.substr(regexInputAperature, textBoxLength);
		s += std::string(textBoxLength - s.size(), ' ');

		//Show string
		thick->DrawInputSection(recursiveSearch, s, rootFolder, applyFilterLabel, filterLabel, searchLabel, rootFolderLabel, searchRecursivelyLabel, width, topSectionRowSize, textBoxLength);

		// place cursor in the control
		thick->showAndSetCursorPosition(cursorPosition - regexInputAperature + (size_t)regexInputColumnStart, regexInputRow);
	}
}