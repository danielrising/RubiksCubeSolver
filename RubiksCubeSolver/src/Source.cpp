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
			cube.ConsolePrint();
			cube.ConsoleRender();
			std::cout << cube.IsSolved(1) << std::endl;
		}

		else if (input == "scramble")
		{
			cube.Scramble40();
		}

		else if (input == "solve") {
			std::vector<short int> solution;
			IterativeDeepening(cube, 10, 1, solution);

			for (int i = 0; i < solution.size(); i++) {
				std::cout << solution[i] << " - ";
			}
		}

		else if ((input == "help") || (input == "h"))
		{
			std::cout << "I FJELL OF THE SHJIP" << std::endl;
		}

		else if (input == "rotate")
		{
			std::string move, power;
			std::cin >> move >> power;
			if (isNumber(move) && isNumber(power))
			{
				cube.Rotate(stoi(move), stoi(power));
			}
		}
	}
}