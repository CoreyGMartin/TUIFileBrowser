#pragma once
#include <memory>

#include "ThickAdapter.hpp"
#include "FileResults.hpp"

class Command {
public:
	virtual void execute() = 0;
};

//class SearchCommand : public Command {
//
//};

class ScrollDown : public Command {

public:
	ScrollDown(int& linePosition, FileResults const& fileResults);


	void execute() override;
};