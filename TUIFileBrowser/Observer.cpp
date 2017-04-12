#include "Observer.hpp"

void Subject::attach(std::shared_ptr<Observer> p) {
	observers.insert(p);
}

void Subject::detach(std::shared_ptr<Observer> p) {
	observers.erase(p);
}

void Subject::notify() {
	for (auto& observer : observers)
		observer->update();
}

