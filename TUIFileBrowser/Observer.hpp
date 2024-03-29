#pragma once
#include <regex>
#include <memory>
#include <set>
#include <vector>
#include <filesystem>

using namespace std::experimental::filesystem;

class Observer {
public:
	virtual void update() = 0;
};

class Subject {
	std::set<Observer*>			observers;

public:
	void attach(Observer* const& observer);
	void detach(Observer* const& observer);
	void notify();
};