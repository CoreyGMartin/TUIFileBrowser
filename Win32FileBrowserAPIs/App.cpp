#include <crtdbg.h>	// C RunTime DeBuG header (debug builds only, MS only)
#define _CRTDBG_MAP_ALLOC
#include <memory>
#include <crtdbg.h>
#include<App.hpp>
#include<iostream>
#include<exception>
using namespace std;

#if defined(CHECK_MEM_INTESIVE_) && defined(_DEBUG)
#define CHECK_MEMORY_INTEGRITY() assert(_CrtCheckMemory())
#else
#define CHECK_MEMORY_INTEGRITY()
#endif

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

int main(int argc, char*argv[])try {
#if defined(_DEBUG)
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	//_CRTDBG_CHECK_ALWAYS_DF slows down program ALOT!
	//dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;	// check block integrity on every memory call
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF;	// don't remove block on delete
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF;		// check for memory leaks at process termination
	_CrtSetDbgFlag(dbgFlags);
#endif

	return App::main(argc, argv);

	CHECK_MEMORY_INTEGRITY();
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