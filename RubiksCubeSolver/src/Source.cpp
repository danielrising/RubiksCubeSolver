#include "rcspch.h"
#include "Cube\Cube3.h"

bool isNumber(const std::string &str)
{
	for (int i = 0; i < str.length(); ++i)
	{
		if (!((47 < str[i]) && (str[i] < 58)))
		{
			return false;
		}
	}
	return true;
}

int main()
{
	Cube3 cube;
	
	bool shouldClose = false;
	while (!shouldClose)
	{
		std::string input;
		std::cin >> input;

		if (input == "stop")
		{
			shouldClose = true;
		}

		else if (input == "print")
		{
			std::cout << cube << std::endl << std::endl;
		}

		else if ((input == "help") || (input == "h"))
		{
			std::cout << "I FJELL OF THE SHJIP" << std::endl;
		}

		else if (input == "rotate")
		{
			std::string move;
			std::cin >> move;
			if (isNumber(move))
			{
				std::cout << stoi(move) << std::endl;
			}
		}
	}
}