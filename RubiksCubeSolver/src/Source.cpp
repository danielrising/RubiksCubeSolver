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
			cube.ConsoleRender();
		}

		else if (input == "scramble")
		{
			cube.Scramble(5, 0, true);
		}

		else if (input == "solve") {
			std::vector<char> possibleMoves = { 
				0, 1,	1, 1,	2, 1,	3, 1,	4, 1,	5, 1, 
				0, 2,	1, 2,	2, 2,	3, 2,	4, 2,	5, 2, 
				0, 3,	1, 3,	2, 3,	3, 3,	4, 3,	5, 3 };

			std::vector<char> solution;
			cube = IterativeDeepening(cube, 10, 0, solution, possibleMoves);

			for (int i = 0; i < solution.size(); i += 2) {
				int powIndex = i + 1;
				std::cout << (int)solution[i] << "*" << (int)solution[powIndex] << " - ";
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