#pragma once
#include <memory>
#include <filesystem>
#include <algorithm>

#include <App.hpp>
#include "Observer.hpp"
#include "Commands.hpp"
#include "View.hpp"
#include "ThickAdapter.hpp"
#include "FileResults.hpp"

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std::experimental::filesystem;

class TUIFileBrowser : public App {
	//fields
	std::unique_ptr<FileResults>								fileResults;
	std::vector<Observer*>										observers;

	//const fields
	ThickAdapter* const											thick;
	unsigned short const										height;
	unsigned short const										width;
	unsigned long const											textBoxLength;
	std::unique_ptr<TopSection>	const							topSection;
	std::unique_ptr<BottomSection> const						bottomSection;
	std::unique_ptr<MiddleSection> const						middleSection;

	//Utility Methods
	void										processMouseEvent(MOUSE_EVENT_RECORD const& mer);
	void										processKeyEvent(KEY_EVENT_RECORD const& ker);

public:
	TUIFileBrowser();
	~TUIFileBrowser() = default;

	int		execute() override;

} paintApp;