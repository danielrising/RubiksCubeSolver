#include "rcspch.h"

#include "Cube\Cube3.h"

const char faces[FACES] = { 'U', 'L', 'F', 'R', 'B', 'D' };

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
		table.push_back(c - 48);
	}

	std::cout << "Finished reading " << fileName << std::endl;
}

void ReadTables(std::vector<char>& tableOne, std::vector<char>& tableTwo, std::vector<char>& tableThree) {
	ReadFromFile(tableOne, "table-one.prun");
	ReadFromFile(tableTwo, "table-two.prun");
	ReadFromFile(tableThree, "table-three.prun");
}

void GenerateTables(std::vector<char>& tableOne, std::vector<char>& tableTwo, std::vector<char>& tableThree) {
	TableOne(tableOne);
	WriteToFile(tableOne, "table-one.prun");

	TableTwo(tableTwo);
	WriteToFile(tableTwo, "table-two.prun");

	TableThree(tableThree);
	WriteToFile(tableThree, "table-three.prun");
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
			// cube.ConsoleRender();
			cube.ConsolePrint();
		}

		else if (input == "scramble" || input == "Scramble")
		{
			int seed = std::chrono::system_clock::now().time_since_epoch().count();
			cube.Scramble(40, 0, true);
		}

		else if (input == "solve" || input == "Solve")
		{
			auto start = std::chrono::system_clock::now(); // Starting time

			// Prepare tables
			if (tableOne.empty()) { // Already prepared?
				std::ifstream fileOne("table-one.prun");
				if (fileOne.good()) { // Already generated?
					ReadTables(tableOne, tableTwo, tableThree);
				}
				else {
					GenerateTables(tableOne, tableTwo, tableThree);
				}
			}

			std::vector<char> solution;

			std::vector<char> indexIdsOne = { PRUNE_EDGECORNERTWIST, PRUNE_EDGETWIST_UDCOMB, PRUNE_CORNERTWIST_UDCOMB };
			std::vector<std::vector<char>*> pruneTablesOne = {&tableOne, &tableTwo, &tableThree};
			std::vector<char> indexIdsTwo = {};
			std::vector<std::vector<char>*> pruneTablesTwo = {};

			cube = KociembaAlgorithm(cube, solution, indexIdsOne, pruneTablesOne, indexIdsTwo, pruneTablesTwo);


			const char faces[FACES] = { 'U', 'L', 'F', 'R', 'B', 'D' };
			const char power[3] = { ' ', '2', 39 };

			for (int i = 0; i < solution.size(); i += 2) {
				int powIndex = i + 1;
				if (solution[i] != -1) {
					std::cout << faces[solution[i]] << power[solution[powIndex] - 1] << " -> ";
				}
			}
			std::cout << std::endl;

			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << "Time Elapsed " << elapsedSeconds.count() << "s" << std::endl;
		}

		else if (input == "rotate" || input == "Rotate")
		{
			std::string move, power;
			std::cin >> move >> power;
			if (IsNumber(move) && IsNumber(power))
			{
				cube.Rotate(stoi(move), stoi(power));
			}
		}

		else if (input == "generate" || input == "Generate") {
			GenerateTables(tableOne, tableTwo, tableThree);
		}

		else if (input == "read" || input == "Read") {
			ReadTables(tableOne, tableTwo, tableThree);
		}
	}
}