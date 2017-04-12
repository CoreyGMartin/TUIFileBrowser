#include<App.hpp>
#include<iostream>
#include<exception>
using namespace std;

int main(int argc, char*argv[])try {
	return App::main(argc, argv);
}
catch (char const * msg) {
	cerr << "exception string: " << msg << endl;
}
catch (exception const& e) {
	cerr << "std::exception: " << e.what() << endl;
}
catch (...) {
	cerr << "Error: an exception has been caught...\n";
	return EXIT_FAILURE;
}

int App::main(int argc, char*argv[]) {
	for (int i = 1; i < argc; ++i)
		thisApp->_args.push_back(argv[i]);
	return thisApp->execute();
}

App* App::thisApp = nullptr;

App::App() {
	if (thisApp)
		throw "Error: Already initialized!";
	thisApp = this;
}

int App::execute() {
	cout << "App console framework, (c) us, 2017\n";
	return EXIT_SUCCESS;
}