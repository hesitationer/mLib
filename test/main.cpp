
#include "main.h"

class App
{
public:
	void go();

private:
	TestUtility m_utility;
	TestMath m_math;
	TestANN m_ANN;
	TestLodePNG m_lodePNG;
	TestBinaryStream m_binaryStream;
};

void App::go()
{
	ml::Console::openLogFile("console.txt");
	m_utility.run();
	m_math.run();
	m_ANN.run();
	m_lodePNG.run();
	m_binaryStream.run();

	ml::Console::log("All tests completed");
	std::cin.get();
}
  
int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//ml::ColorImageR8G8B8 test;
	//ml::FreeImageWrapper::loadImage("test1.png", test);
	//ml::FreeImageWrapper::saveImage("out1.jpg", test);

	App a;
	a.go();

	getchar();
	return 0;
}
