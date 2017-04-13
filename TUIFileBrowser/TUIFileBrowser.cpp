#include "TUIFileBrowser.hpp"

static bool done = false;

bool handleCtrlC(unsigned long const ctrlType) {
	switch (ctrlType) {
	case CTRL_C_EVENT:
		done = true;
		return true;
	}
	return false;
}

TUIFileBrowser::TUIFileBrowser() 
	:	thick(ThickAdapter::GetInstance()),
		height(30),
		width(120),
		textBoxLength((unsigned long)(width * .25)), //Each textbox will be 25% of the console screen width
		topSection(std::unique_ptr<TopSection>(new TopSection(textBoxLength, width, fileResults))),
		bottomSection(std::unique_ptr<BottomSection>(new BottomSection(textBoxLength, width, fileResults, height - 2))),
		middleSection(std::unique_ptr<MiddleSection>(new MiddleSection(height, width, topSection->sectionSize, bottomSection->sectionSize, fileResults)))
{
	//Group observers
	observers.push_back(topSection.get());
	observers.push_back(middleSection.get());
	observers.push_back(bottomSection.get());
}

int TUIFileBrowser::execute() {
	//Save console state
	thick->saveConsoleState();
	
	//Set defaults
	topSection->regex = "";
	topSection->rootFolder = "";
	topSection->recursiveSearch = false;

	//Process arguments
	std::vector<std::string> args = GetArgs();
	if (args.size() != 0) {
		for (size_t i = 0; i < args.size(); ++i) {
			if (args[i] == "-r")
				topSection->recursiveSearch = true;
			else if (args[i].find(R"x(:\)x") != std::string::npos)
				topSection->rootFolder = args[i];
			else if (args[i].find(R"x(.)x") != std::string::npos)
				topSection->regex = args[i];
		}
	}

	//Setup
	thick->setTitle("TUI File Browser");
	thick->setSize(width, height);
	thick->setConsoleMode(ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	thick->setConsoleCtrlHandler(handleCtrlC, true);
	thick->hideCursor();
	thick->FillAndSetConsoleBackgroundColour(BACKGROUND_INTENSITY, ' ');

	//Render views
	std::unique_ptr<Command>(new RenderTopSection(topSection.get()))->execute();
	std::unique_ptr<Command>(new RenderMiddleSection(fileResults, middleSection.get()))->execute();
	std::unique_ptr<Command>(new RenderBottomSection(fileResults, bottomSection.get()))->execute();

	//Perform the search
	if (topSection->rootFolder != "")
		std::unique_ptr<Command>(new Search(observers, fileResults, topSection->rootFolder, topSection->recursiveSearch, topSection->regex))->execute();	

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

void TUIFileBrowser::processMouseEvent(MOUSE_EVENT_RECORD const& me) {
	switch (me.dwEventFlags) {
		case 0: {
			int const leftClickUp = !me.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED;

			if (leftClickUp) {
				short const mousePositionX = me.dwMousePosition.X;
				short const mousePositionY = me.dwMousePosition.Y;

				//Place Cursor in texetbox
				if ((mousePositionY == topSection->fileInputRow && mousePositionX >= topSection->fileInputColumnStart && mousePositionX < topSection->fileInputColumnEnd) ||
					(mousePositionY == topSection->regexInputRow && mousePositionX >= topSection->regexInputColumnStart && mousePositionX < topSection->regexInputColumnEnd)) {
					std::unique_ptr<Command>(new PlaceCursorInTextBox(topSection.get(), mousePositionX, mousePositionY))->execute();
				}
				//Check for Scroll Up
				else if (middleSection->scrollerXLocation == mousePositionX && middleSection->topScrollerYLocation == mousePositionY) {
					std::unique_ptr<Command>(new ScrollUp(1, fileResults, middleSection.get()))->execute();
				}
				//Check for Scroll Down
				else if (middleSection->scrollerXLocation == mousePositionX && middleSection->bottomScrollerYLocation == mousePositionY) {
					std::unique_ptr<Command>(new ScrollDown(1, fileResults, middleSection.get()))->execute();
				}
				//Check for Recursive checkbox hit
				else if (topSection->searchRecursivelyCheckBoxColumn == mousePositionX && topSection->searchRecursivelyCheckBoxRow == mousePositionY) {
					std::unique_ptr<Command>(new ChangeRecursiveCheckbox(topSection.get()))->execute();
				}
				//Check for Search button hit
				else if (mousePositionX >= topSection->searchButtonColumnStart && mousePositionX <= topSection->searchButtonColumnEnd && topSection->searchButtonRow == mousePositionY) {
					std::unique_ptr<Command>(new Search(observers, fileResults, topSection->rootFolder, topSection->recursiveSearch, topSection->regex))->execute();
				}
				//Check for Apply Filter button hit
				else if (mousePositionX >= topSection->applyFilterButtonColumnStart && mousePositionX <= topSection->applyFilterButtonColumnEnd && topSection->applyFilterButtonRow == mousePositionY) {
					std::unique_ptr<Command>(new Filter(fileResults, topSection->regex))->execute();
				} else {
					thick->hideCursor();
					topSection->regexInputTextBoxSelected = false;
					topSection->fileInputTextBoxSelected = false;
				}
			}
		}
	} 
}

void TUIFileBrowser::processKeyEvent(KEY_EVENT_RECORD const& ke) {

	if (ke.bKeyDown) {
		if (!(topSection->fileInputTextBoxSelected || topSection->regexInputTextBoxSelected)) {
		
			switch (ke.wVirtualKeyCode) {
				case VK_UP:
					std::unique_ptr<Command>(new ScrollUp(1, fileResults, middleSection.get()))->execute();
					break;
				case VK_DOWN:
					std::unique_ptr<Command>(new ScrollDown(1, fileResults, middleSection.get()))->execute();
					break;
				case VK_PRIOR:
					std::unique_ptr<Command>(new ScrollUp(middleSection->pageSize, fileResults, middleSection.get()))->execute();
					break;
				case VK_NEXT:
					std::unique_ptr<Command>(new ScrollDown(middleSection->pageSize, fileResults, middleSection.get()))->execute();
					break;
			}
		} else  {
			if (ke.wVirtualKeyCode == VK_RETURN)
				if (topSection->fileInputTextBoxSelected)
					std::unique_ptr<Command>(new Search(observers, fileResults, topSection->rootFolder, topSection->recursiveSearch, topSection->regex))->execute();
				else
					std::unique_ptr<Command>(new Filter(fileResults, topSection->regex));
			else
				std::unique_ptr<Command>(new EditTextBox(topSection.get(), ke))->execute();
		}
	}
}