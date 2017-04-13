#pragma once
#include <memory>

#include "ThickAdapter.hpp"
#include "Observer.hpp"
#include "View.hpp"
#include "FileResults.hpp"

class Command {
public:
	virtual void execute() = 0;
};

class RenderTopSection : public Command {
	TopSection* const&					topSection;

public:
	//Constructors/Destructor
	RenderTopSection(TopSection* const& topSection);
	~RenderTopSection() = default;

	//Method
	void execute() override;
};

class RenderMiddleSection : public Command {
	MiddleSection* const&						middleSection;
	std::unique_ptr<FileResults> const&			fileResults;

public:
	//Constructors/Destructor
	RenderMiddleSection(std::unique_ptr<FileResults> const& fileResults, MiddleSection* const& middleSection);
	~RenderMiddleSection() = default;

	//Method
	void execute() override;
};

class RenderBottomSection : public Command {
	BottomSection* const&						bottomSection;
	std::unique_ptr<FileResults> const&			fileResults;

public:
	//Constructors/Destructor
	RenderBottomSection(std::unique_ptr<FileResults> const& fileResults, BottomSection* const& bottomSection);
	~RenderBottomSection() = default;

	//Method
	void execute() override;
};

class ScrollUp : public Command {
	MiddleSection* const&						middleSection;
	std::unique_ptr<FileResults> const&			fileResults;
	int const&									amount;

public:
	//Constructor/Destructor
	ScrollUp(int const& amount, std::unique_ptr<FileResults> const& fileResults, MiddleSection* const& middleSection);
	~ScrollUp() = default;

	//Method
	void execute() override;
};

class ScrollDown : public Command {
	MiddleSection* const&						middleSection;
	std::unique_ptr<FileResults> const&			fileResults;
	int const&									amount;

public:
	//Constructor/Destructor
	ScrollDown(int const& amount, std::unique_ptr<FileResults> const& fileResults, MiddleSection* const& middleSection);
	~ScrollDown() = default;

	//Method
	void execute() override;
};

class ChangeRecursiveCheckbox : public Command {
	TopSection* const&			topSection;

public:
	//Constructor/Destructor
	ChangeRecursiveCheckbox(TopSection* const& topSection);
	~ChangeRecursiveCheckbox() = default;

	//Method
	void execute() override;
};

class PlaceCursorInTextBox : public Command {
	TopSection* const&							topSection;
	short const&								mousePositionX;
	short const&								mousePositionY;

public:
	//Constructor/Destructor
	PlaceCursorInTextBox(TopSection* const& topSection, short const& mousePoisitonX, short const& mousePoisitonY);
	~PlaceCursorInTextBox() = default;

	//Method
	void execute() override;
};

class EditTextBox : public Command {
	TopSection* const&							topSection;
	KEY_EVENT_RECORD const&						ke;

public:
	//Constructor/Destructor
	EditTextBox(TopSection* const& topSection, KEY_EVENT_RECORD const& ke);
	~EditTextBox() = default;

	//Method
	void execute() override;
};

class Search : public Command {
	std::unique_ptr<FileResults>&				fileResults;
	std::vector<Observer*> const&				observers;
	std::string const&							searchFolder;
	bool const&									doRecursion;
	std::string const&							regex;

public:
	//Constructor/Destructor
	Search(std::vector<Observer*> const& observers, std::unique_ptr<FileResults>& fileResults, std::string const& searchFolder, bool const& doRecursion, std::string const& regex);
	~Search() = default;

	//Method
	void execute() override;
};

class Filter : public Command {
	std::unique_ptr<FileResults>&				fileResults;
	std::string const&							regex;

public:
	//Constructor/Destructor
	Filter(std::unique_ptr<FileResults>& fileResults, std::string const& regex);
	~Filter() = default;

	//Method
	void execute() override;
};