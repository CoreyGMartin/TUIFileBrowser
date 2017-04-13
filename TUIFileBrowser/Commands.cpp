#include "Commands.hpp"

RenderTopSection::RenderTopSection(TopSection* const& topSection) :	topSection(topSection) {}
void RenderTopSection::execute() {
	ThickAdapter* const thick = ThickAdapter::GetInstance();

	//Fill background
	unsigned long length = topSection->width * topSection->sectionSize;
	thick->fillConsoleOutputAttribute(BACKGROUND_GREEN, length, 0, 0);

	//Starting coordinates
	short column = 1;
	short row = topSection->fileInputRow;

	//Root folder Label and textbox
	thick->WriteTextToConsole(column, row, topSection->rootFolderLabel);
	column += (short)topSection->rootFolderLabel.length();
	thick->WriteTextToConsole(column, row, topSection->rootFolder);
	thick->fillConsoleOutputAttribute(BACKGROUND_GREEN | BACKGROUND_INTENSITY, (short)topSection->textBoxLength, column, row);
	column += topSection->textBoxLength;

	//Search Recursively label and checkbox
	++column;
	thick->WriteTextToConsole(column, row, topSection->searchRecursivelyLabel);
	column += (short)topSection->searchRecursivelyLabel.length();
	length = 1;
	const std::string checked = topSection->recursiveSearch ? "x" : " ";
	thick->WriteTextToConsole(column, row, checked);
	thick->fillConsoleOutputAttribute(BACKGROUND_INTENSITY, length, column, row);

	//Search Button
	column += 3;
	length = topSection->searchLabel.length() + 2; // +2 for space before and after button
	thick->WriteTextToConsole(column, row, topSection->searchLabel);
	thick->fillConsoleOutputAttribute(BACKGROUND_BLUE | BACKGROUND_INTENSITY, length, (column - 1), row);

	//New row and reset column
	column = 1;
	row = topSection->regexInputRow;

	//Filter Label and textbox
	thick->WriteTextToConsole(column, row, topSection->filterLabel);
	column += (short)topSection->filterLabel.length();
	thick->WriteTextToConsole(column, row, topSection->regex);
	thick->fillConsoleOutputAttribute(BACKGROUND_GREEN | BACKGROUND_INTENSITY, topSection->textBoxLength, column, row);
	column += (short)topSection->textBoxLength;

	//Apply Filter Button
	column += 3;
	length = topSection->applyFilterLabel.length() + 2;
	thick->WriteTextToConsole(column, row, topSection->applyFilterLabel);
	thick->fillConsoleOutputAttribute(BACKGROUND_BLUE | BACKGROUND_INTENSITY, length, (column - 1), row);
}


RenderMiddleSection::RenderMiddleSection(std::unique_ptr<FileResults> const& fileResults, MiddleSection* const& middleSection) : middleSection(middleSection), fileResults(fileResults) {}
void RenderMiddleSection::execute() {
	ThickAdapter* const thick = ThickAdapter::GetInstance();

	//Fill Background
	unsigned long const length = middleSection->width * middleSection->sectionSize;
	const short rowOffSet = middleSection->rowOffSet;
	thick->fillConsoleOutputCharacter(' ', length, 0, 4);
	thick->fillConsoleOutputAttribute(BACKGROUND_INTENSITY, length, 0, rowOffSet);

	//Scroll bar
	const unsigned long vertialScrollBarLocation = middleSection->width - 1;
	for (int i = 1; i < (middleSection->sectionSize - 1); ++i)
		thick->WriteTextToConsole(vertialScrollBarLocation, i + rowOffSet, "|");

	//Up arrow and down arrow of scroll bar
	thick->WriteTextToConsole(vertialScrollBarLocation, rowOffSet, "^");
	thick->WriteTextToConsole(vertialScrollBarLocation, (middleSection->sectionSize + rowOffSet) - 1, "v");

	if (fileResults != nullptr) {
		//Figure out where the scroll bar element should be 
		double percentage = (double)middleSection->linePosition / (fileResults->PreviousFilter().size() - middleSection->verticalScrollBarSize);
		unsigned short const position = (unsigned short)(middleSection->verticalScrollBarSize * percentage);
		thick->fillConsoleOutputAttribute(BACKGROUND_BLUE | BACKGROUND_INTENSITY, 1, (middleSection->width - 1), (rowOffSet + position + 1));

		//Draw text to console
		for (int i = 0; i + middleSection->linePosition < (int)fileResults->PreviousFilter().size() && i < (int)middleSection->sectionSize; ++i)
			thick->WriteTextToConsole(0, i + rowOffSet, fileResults->PreviousFilter()[i + middleSection->linePosition]);
	}
}


RenderBottomSection::RenderBottomSection(std::unique_ptr<FileResults> const& fileResults, BottomSection* const& bottomSection)
	:	bottomSection(bottomSection), fileResults(fileResults) {}
void RenderBottomSection::execute() {
	ThickAdapter* const thick = ThickAdapter::GetInstance();

	//Fill Background
	unsigned long charsWritten = 0;
	unsigned long length = bottomSection->width * bottomSection->sectionSize;
	short const rowOffSet = bottomSection->rowOffSet;
	thick->fillConsoleOutputCharacter(' ', length, 0, rowOffSet);
	thick->fillConsoleOutputAttribute(BACKGROUND_GREEN, length, 0, rowOffSet);

	//Starting coordinates
	short column = 1;
	short row = rowOffSet;

	//Total Files/Folders Label and textbox
	thick->WriteTextToConsole(column, row, bottomSection->totalFilesFolderLabel);
	column += (short)(bottomSection->totalFilesFolderLabel.length() + 1);
	thick->WriteTextToConsole(column, row, fileResults == nullptr ? "0" : std::to_string(fileResults->FilesFoldersSearched()));

	//New row and reset column
	++row;
	column = 1;

	//Files Matched Label and texetbox
	thick->WriteTextToConsole(column, row, bottomSection->filesMatchedLabel);
	column += (short)bottomSection->filesMatchedLabel.length() + 1;
	thick->WriteTextToConsole(column, row, fileResults == nullptr ? "0" : std::to_string(fileResults->NumberOfMatchedFiles()));
	column += (short)bottomSection->textBoxLength;

	//Files Matched Size Label and texetbox
	++column;
	thick->WriteTextToConsole(column, row, bottomSection->filesMatchedSizeLabel);
	column += (short)(bottomSection->filesMatchedSizeLabel.length() + 1);
	thick->WriteTextToConsole(column, row, fileResults == nullptr ? "0" : std::to_string(fileResults->MatchedFileSize()));
}


ScrollUp::ScrollUp(int const& amount, std::unique_ptr<FileResults> const& fileResults, MiddleSection* const& middleSection)
	:	middleSection(middleSection), fileResults(fileResults), amount(amount) {}
void ScrollUp::execute() {

	if (middleSection->linePosition != 0) {

		if ((middleSection->linePosition - amount) >= 0) {
			middleSection->linePosition -= amount;
		}else {
			//near the top, pop up 
			middleSection->linePosition = 0;
		}

		std::unique_ptr<Command>(new RenderMiddleSection(fileResults, middleSection))->execute();
	}
}


ScrollDown::ScrollDown(int const& amount, std::unique_ptr<FileResults> const& fileResults, MiddleSection* const& middleSection)
	:	middleSection(middleSection), fileResults(fileResults), amount(amount) {}
void ScrollDown::execute() {
	if ((middleSection->linePosition + (middleSection->verticalScrollBarSize + 1)) < (short)fileResults->PreviousFilter().size()) {

		if ((middleSection->linePosition + (amount * 2)) <= (short)fileResults->PreviousFilter().size()) {
			middleSection->linePosition += amount;
		} else {
			//near the bottom, pop down
			middleSection->linePosition = (fileResults->PreviousFilter().size() - amount);
		}

		std::unique_ptr<Command>(new RenderMiddleSection(fileResults, middleSection))->execute();
	}
}


ChangeRecursiveCheckbox::ChangeRecursiveCheckbox(TopSection* const& topSection) : topSection(topSection) {}
void ChangeRecursiveCheckbox::execute() {
	topSection->recursiveSearch = !topSection->recursiveSearch;
	const std::string checked = topSection->recursiveSearch ? "x" : " ";
	ThickAdapter::GetInstance()->WriteTextToConsole(topSection->searchRecursivelyCheckBoxColumn, topSection->searchRecursivelyCheckBoxRow, checked);
}


PlaceCursorInTextBox::PlaceCursorInTextBox(TopSection* const& topSection, short const& mousePositionX, short const& mousePositionY)
	:	topSection(topSection), mousePositionX(mousePositionX), mousePositionY(mousePositionY) {}
void PlaceCursorInTextBox::execute() {
	//Top or bottom text box
	if (mousePositionY == topSection->fileInputRow) {
		//Find cursor position
		short cursorPositionX = mousePositionX - topSection->fileInputColumnStart;
		const short rootFolderLength = (short)topSection->rootFolder.length();
		if (cursorPositionX > rootFolderLength)
			cursorPositionX = rootFolderLength;

		//Set values
		topSection->cursorPosition = cursorPositionX;
		topSection->fileInputTextBoxSelected = true;
		topSection->regexInputTextBoxSelected = false;

		//Place cursor
		ThickAdapter::GetInstance()->showAndSetCursorPosition(cursorPositionX + topSection->fileInputColumnStart, topSection->fileInputRow);
	} else {
		//Find cursor position
		short cursorPositionX = mousePositionX - topSection->regexInputColumnStart;
		const short regexLength = (short)topSection->regex.length();
		if (cursorPositionX > regexLength)
			cursorPositionX = regexLength;

		//Set values
		topSection->cursorPosition = cursorPositionX;
		topSection->fileInputTextBoxSelected = false;
		topSection->regexInputTextBoxSelected = true;

		//Place cursor
		ThickAdapter::GetInstance()->showAndSetCursorPosition(cursorPositionX + topSection->regexInputColumnStart, topSection->regexInputRow);
	}
}


EditTextBox::EditTextBox(TopSection* const& topSection, KEY_EVENT_RECORD const& ke) : topSection(topSection), ke(ke) {}
void EditTextBox::execute() {

	//Figure out which text box was selected and create some references
	bool const& fileInputTextBoxSelected = topSection->fileInputTextBoxSelected;
	std::string& selectedString = fileInputTextBoxSelected ? topSection->rootFolder : topSection->regex;
	size_t& aperture = fileInputTextBoxSelected ? topSection->fileInputAperture : topSection->regexInputAperature;
	short const& row = fileInputTextBoxSelected ? topSection->fileInputRow : topSection->regexInputRow;
	short const& column = fileInputTextBoxSelected ? topSection->fileInputColumnStart : topSection->regexInputColumnStart;

	//What key was pressed?
	switch (ke.wVirtualKeyCode) {
		case VK_BACK:
			if (0 < topSection->cursorPosition && topSection->cursorPosition <= selectedString.size()) {
				--topSection->cursorPosition;
				selectedString.erase(topSection->cursorPosition, 1);
			}
			break;
		case VK_DELETE:
			if (0 <= topSection->cursorPosition && topSection->cursorPosition < selectedString.size())
				selectedString.erase(topSection->cursorPosition, 1);
			break;
		case VK_LEFT:
			if (topSection->cursorPosition > 0)
				--topSection->cursorPosition;
			break;
		case VK_RIGHT:
			if (topSection->cursorPosition < selectedString.size())
				++topSection->cursorPosition;
			break;
		case VK_END:
			topSection->cursorPosition = selectedString.size();
			break;
		case VK_HOME:
			topSection->cursorPosition = 0;
			break;	
		default:
			char ch = ke.uChar.AsciiChar;
			if (isprint(ch))
				selectedString.insert(topSection->cursorPosition++ + selectedString.begin(), ch);
	}

	//Display the proper string in the proper textbox
	auto practicalSize = selectedString.size() + 1;
	while (topSection->cursorPosition < aperture)
		--aperture;

	while (topSection->cursorPosition - aperture >= topSection->textBoxLength)
		++aperture;

	while (practicalSize - aperture< topSection->textBoxLength && practicalSize > topSection->textBoxLength)
		--aperture;

	std::string display = selectedString.substr(aperture, topSection->textBoxLength);
	display += std::string(topSection->textBoxLength - display.size(), ' ');

	ThickAdapter* const thick = ThickAdapter::GetInstance();
	thick->WriteTextToConsole(column, row, display);
	thick->showAndSetCursorPosition(topSection->cursorPosition - (short)aperture + column, row);
}


Search::Search(std::vector<Observer*> const& observers, std::unique_ptr<FileResults>& fileResults, std::string const& searchFolder, bool const& doRecursion, std::string const& regex)
	: fileResults(fileResults), observers(observers), searchFolder(searchFolder), doRecursion(doRecursion), regex(regex) {}
void Search::execute() {
	//No need to detach. using smart pointers.
	fileResults = std::unique_ptr<FileResults>(new FileResults(searchFolder, doRecursion));

	//Attach observers.
	for (auto& observer : observers)
		fileResults->attach(observer);
	
	//Apply the filter.
	fileResults->Filter(regex);
}


Filter::Filter(std::unique_ptr<FileResults>& fileResults, std::string const& regex) : fileResults(fileResults), regex(regex) {}
void Filter::execute() {
	fileResults->Filter(regex);
}