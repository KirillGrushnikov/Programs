#include <iostream>
#include <string>
#include "Calculate.h"

void Test()
{
	std::cout << "\ntest 1: 5+2 = 7 \n";
	std::cout << "result: " << Calculate("5+2") << std::endl;
	std::cout << "test 2: 5-2 = 3 \n";
	std::cout << "result: " << Calculate("5-2") << std::endl;
	std::cout << "test 3: 5*2 = 10 \n";
	std::cout << "result: " << Calculate("5*2") << std::endl;
	std::cout << "test 4: 5/2 = 2.5 \n";
	std::cout << "result: " << Calculate("5/2") << std::endl;
	std::cout << "test 5: 5^2 = 25\n";
	std::cout << "result: " << Calculate("5^2") << std::endl;
	std::cout << "test 6: 5%2 = 1\n";
	std::cout << "result: " << Calculate("5%2") << std::endl;
	std::cout << "test 7: 5*2+1 = 11\n";
	std::cout << "result: " << Calculate("5*2+1") << std::endl;
	std::cout << "test 8: 5-2+3 = 6\n";
	std::cout << "result: " << Calculate("5-2+3") << std::endl;
	std::cout << "test 9: 53+11-24 = 40\n";
	std::cout << "result: " << Calculate("53+11-24") << std::endl;
	std::cout << "test 10: -2+5 = 3\n";
	std::cout << "result: " << Calculate("-2+5") << std::endl;
	std::cout << "test 11: (5+10)*2 = 30\n";
	std::cout << "result: " << Calculate("(5+10)*2") << std::endl;
	std::cout << "test 12: 2+(5+10) = 17\n";
	std::cout << "result: " << Calculate("2+(5+10)") << std::endl;
	std::cout << "test 13: -1-(13+12) = -26\n";
	std::cout << "result: " << Calculate("-1-(13+12)") << std::endl;
	std::cout << "test 14: 2+(10+5+(-7+5)) = 15\n";
	std::cout << "result: " << Calculate("2+(10+5+(-7+5))") << std::endl;
	std::cout << "test 15: 2+(10+5+(-7+5))*2 = 28\n";
	std::cout << "result: " << Calculate("2+(10+5+(-7+5))*2") << std::endl;
	std::cout << "test 16: 2/0 = Error\n";
	std::cout << "result: " << Calculate("2/0") << std::endl;
	std::cout << "test 17: 2+(1+1)) = Error\n";
	std::cout << "result: " << Calculate("2+(1+1))") << std::endl;
	std::cout << "test 18: 2+(1+1 = Error\n";
	std::cout << "result: " << Calculate("2+(1+1") << std::endl;
	std::cout << "test 19: 2(10) = Error\n";
	std::cout << "result: " << Calculate("2(10)") << std::endl;
	std::cout << "test 20: (10)() = Error\n";
	std::cout << "result: " << Calculate("(10)()") << std::endl;
	std::cout << std::endl;
}

int main()
{
	int prec = 2;
Again:
	std::cout << "Enter \"test\" to test programm\n";
	std::cout << "Enter \"exit\" to out of programm\n";
	std::cout << "Enter \"cls\" to clear cmd\n\n";
	std::cout << "Enter \"prec\" to set precision\n\n";

	while (1)
	{
		char* c_user_example = new char[150];
		std::cout << "Enter example: ";
		std::cin.getline(c_user_example, 150);
		std::string user_example(c_user_example);
		if (user_example == "exit" || user_example == "Exit") break;
		if (user_example == "cls" || user_example == "clear") { system("cls"); goto Again; }
		if (user_example == "test" || user_example == "Test") { Test(); continue; }
		if (user_example == "prec") { std::cin >> prec; continue; }
		std::cout << "Result: " << Calculate(user_example, prec) << std::endl;
	}
	return 0;
}