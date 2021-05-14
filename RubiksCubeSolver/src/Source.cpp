#include "rcspch.h"

#include "Cube\Cube3.h"

bool isNumber(const std::string& str)
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

Cube3 solveBruteForce(Cube3 cube, std::vector<char> table)
{
	// Starting time
	auto start = std::chrono::system_clock::now();

	std::vector<char> possibleMoves = {
		0, 1,	1, 1,	2, 1,	3, 1,	4, 1,	5, 1,
		0, 2,	1, 2,	2, 2,	3, 2,	4, 2,	5, 2,
		0, 3,	1, 3,	2, 3,	3, 3,	4, 3,	5, 3 };

	std::vector<char> solution;

	cube = IterativeDeepening(cube, 10, 0, solution, possibleMoves, table);

	const char faces[FACES] = { 'U', 'L', 'F', 'R', 'B', 'D' };
	const char power[3] = { ' ', '2', 39 };

	for (int i = 0; i < solution.size(); i += 2) {
		int powIndex = i + 1;
		if (solution[i] != -1) {
			std::cout << faces[solution[i]] << power[solution[powIndex] - 1] << " -> ";
		}
	}
	std::cout << std::endl;

	// Elapsed time
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << "Elapsed time: " << elapsedSeconds.count() << "s" << std::endl;

	return cube;
}

int main()
{
	Cube3 cube;
	std::vector<char> table1;
	std::vector<char> table2;
	std::vector<char> table3;

	bool shouldClose = false;
	while (!shouldClose)
	{
		std::string input;
		std::cin >> input;

		if (input == "stop" || input == "Stop")
		{
			shouldClose = true;
		}

		else if (input == "print" || input == "Print")
		{
			cube.ConsoleRender();
			//cube.ConsolePrint();
		}

		else if (input == "scramble")
		{
			cube.Scramble(10, 0, true);
		}

		else if (input == "solve")
		{
			cube = solveBruteForce(cube, table1);
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

		else if (input == "generatetables") {
			tableOne(table1);

			std::ofstream fOut("table-one.PRUNE");
			for (long int i = 0; i < table1.size(); i++) {
				fOut << (int)table1[i];
			}
			fOut.close();

			std::cout << std::endl;

			tableTwo(table2);

			fOut.open("table-two.PRUNE");
			for (long int i = 0; i < table2.size(); i++) {
				fOut << (int)table2[i];
			}
			fOut.close();

			std::cout << std::endl;

			tableThree(table3);

			fOut.open("table-three.PRUNE");
			for (long int i = 0; i < table3.size(); i++) {
				fOut << (int)table3[i];
			}
			fOut.close();

			std::cout << "FINISHED GENERATING TABLES" << std::endl;
		}

		else if (input == "readtables") {
			// Do something smart here
		}
	}
}