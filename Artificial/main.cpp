#include <iostream>
#include "CoreLoader.h"
#include "Trim.h"
#include "JumpExpression.h"

using namespace Commands;
using namespace Memory;
using namespace GUI;

/*
std::string get_image_from(const std::string& path)
{
	std::string image;
	std::ifstream file(path);
	while (!file.eof())
	{
		std::string buf;
		std::getline(file, buf, '\n');
		image += buf + '\n';
	}
	file.close();
	return image;
}
*/

int main()
{
	try
	{
		srand((unsigned int)time(0));
		{ int temp = rand(); }
		
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);

		ConsoleWindow::init();
		Mova::Processor proc;
		Core core;
		CoreLoader::load(core);
		
		return 0;
	}
	catch (const Exception& ex)
	{
		std::cout << "Exception: " << ex.what();
	}
}
