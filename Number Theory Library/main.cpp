#include "pch.h"


int main() {
	NTL::RR value;
	NTL::RR::SetPrecision(512);
	NTL::RR::SetOutputPrecision(512);

	value = 1.0;

	for (int i = 0; i < 80; i++)
	{
		value *= 2;
		NTL::RR tmp;
		tmp = value * value;
		tmp = 1 / tmp;
		tmp += 1;
		std::cout << i << ": " << tmp << std::endl;
	}

	system("pause");
}