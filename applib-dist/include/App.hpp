#pragma once
#include<string>
#include<vector>

#if defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt-gd.lib")
#elif defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-sgd.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib,"applib-mt.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib,"applib-mt-s.lib")
#endif

class App {
	using Args = std::vector<std::string>;
	static App* thisApp;
	friend int main(int argc, char*argv[]);
	static int main(int argc, char*argv[]);
	Args _args;
protected:
	App();
	virtual ~App() {}
	virtual int execute();
	Args const& GetArgs() const { return _args; }
};