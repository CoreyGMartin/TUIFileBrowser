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
	std::set<std::shared_ptr<Observer>> observers;

public:
	void attach(std::shared_ptr<Observer> p);
	void detach(std::shared_ptr<Observer> p);
	void notify();
};