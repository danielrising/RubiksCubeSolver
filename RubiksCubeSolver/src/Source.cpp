#include "rcspch.h"

#include "Cube\Cube3.h"

bool IsNumber(const std::string& str)
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

void WriteToFile(std::vector<char>& table, std::string fileName) {
	std::ofstream fOut(fileName);
	for (long int i = 0; i < table.size(); i++) {
		fOut << (int)table[i];
	}
	fOut.close();
}

void ReadFromFile(std::vector<char>& table, std::string fileName) {
	std::ifstream fIn(fileName);
	char c;
	while (fIn.get(c)) {
		table.push_back(c);
	}

	std::cout << "Finished reading " << fileName << std::endl;
}

Cube3 SolveBruteForce(Cube3 cube, std::vector<char> table)
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
	std::vector<char> tableOne;
	std::vector<char> tableTwo;
	std::vector<char> tableThree;

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
			// cube = SolveBruteForce(cube, tableOne);
		}

		else if (input == "rotate")
		{
			std::string move, power;
			std::cin >> move >> power;
			if (IsNumber(move) && IsNumber(power))
			{
				cube.Rotate(stoi(move), stoi(power));
			}
		}

		else if (input == "generatetables") {
			TableOne(tableOne);
			WriteToFile(tableOne, "table-one.prun");

			TableTwo(tableTwo);
			WriteToFile(tableOne, "table-two.prun");

			TableThree(tableThree);
			WriteToFile(tableThree, "table-three.prun");
		}

		else if (input == "readtables") {
			ReadFromFile(tableOne, "table-one.prun");
			ReadFromFile(tableTwo, "table-two.prun");
			ReadFromFile(tableThree, "table-three.prun");
		}
	}
}