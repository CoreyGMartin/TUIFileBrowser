#include "Observer.hpp"

void Subject::attach(Observer* const& observer) {
	observers.insert(observer);
}

void Subject::detach(Observer* const& observer) {
	observers.erase(observer);
}

void Subject::notify() {
	for (auto& observer : observers)
		observer->update();
}

