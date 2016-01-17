#include "../Zinc/Zinc.hpp"

#include <fstream>

using namespace Zinc;

int main()
{
	auto expr =
		#include "expression"
		;
	auto limit = lim(_x.to(
		#include "limit"
		), expr);
	std::ofstream stream("output");
	if (stream.is_open())
	{
		stream << limit;
		stream.close();
	}
	else
	{
		return -1;
	}
	return 0;
}
