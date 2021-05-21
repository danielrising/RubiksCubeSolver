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
		fOut << table[i];
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

void ReadTables(std::vector<char>& tableOne, std::vector<char>& tableTwo, std::vector<char>& tableThree, std::vector<char>& tableFour, std::vector<char>& tableFive, std::vector<char>& tableSix, std::vector<char>& tableSeven) {
	tableOne.reserve(EDGE_TWIST_INDEX_SIZE* CORNER_TWIST_INDEX_SIZE);
	ReadFromFile(tableOne, "table-one.prun");
	tableTwo.reserve(UDSLICE_COMBINATION_INDEX_SIZE * EDGE_TWIST_INDEX_SIZE);
	ReadFromFile(tableTwo, "table-two.prun");
	tableThree.reserve(UDSLICE_COMBINATION_INDEX_SIZE * CORNER_TWIST_INDEX_SIZE);
	ReadFromFile(tableThree, "table-three.prun");
	tableFour.reserve(CORNER_PERM_INDEX_SIZE);
	ReadFromFile(tableFour, "table-four.prun");
	tableFive.reserve(UDEDGE_PERM_INDEX_SIZE);
	ReadFromFile(tableFive, "table-five.prun");
	tableSix.reserve(CORNER_PERM_INDEX_SIZE * UDEDGE_PERM_INDEX_SIZE);
	ReadFromFile(tableSix, "table-six.prun");
	//ReadFromFile(tableSeven, "table-seven.prun");
}

void GenerateTables(std::vector<char>& tableOne, std::vector<char>& tableTwo, std::vector<char>& tableThree, std::vector<char>& tableFour, std::vector<char>& tableFive, std::vector<char>& tableSix, std::vector<char>& tableSeven) {
	TableOne(tableOne);
	WriteToFile(tableOne, "table-one.prun");
	TableTwo(tableTwo);
	WriteToFile(tableTwo, "table-two.prun");
	TableThree(tableThree);
	WriteToFile(tableThree, "table-three.prun");
	TableFour(tableFour);
	WriteToFile(tableFour, "table-four.prun");
	TableFive(tableFive);
	WriteToFile(tableFive, "table-five.prun");
	TableSix(tableSix);
	WriteToFile(tableSix, "table-six.prun");
	/*
	TableSeven(tableSeven);
	WriteToFile(tableSeven, "table-seven.prun");*/
}

int main()
{
	Cube3 cube;
	std::vector<char> tableOne;
	std::vector<char> tableTwo;
	std::vector<char> tableThree;
	std::vector<char> tableFour;
	std::vector<char> tableFive;
	std::vector<char> tableSix;
	std::vector<char> tableSeven;

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
			// cube.ConsolePrint();
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
					ReadTables(tableOne, tableTwo, tableThree, tableFour, tableFive, tableSix, tableSeven);
				}
				else {
					GenerateTables(tableOne, tableTwo, tableThree, tableFour, tableFive, tableSix, tableSeven);
				}
			}

			start = std::chrono::system_clock::now(); // Starting time

			std::vector<char> solution;

			std::vector<char> indexIdsOne = { PRUNE_EDGECORNERTWIST, PRUNE_EDGETWIST_UDCOMB, PRUNE_CORNERTWIST_UDCOMB };
			std::vector<std::vector<char>*> pruneTablesOne = { &tableOne, &tableTwo, &tableThree};
			std::vector<char> indexIdsTwo = { PRUNE_PHASETWO };
			std::vector<std::vector<char>*> pruneTablesTwo = { &tableSix };

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
			GenerateTables(tableOne, tableTwo, tableThree, tableFour, tableFive, tableSix, tableSeven);
		}

		else if (input == "read" || input == "Read") {
			ReadTables(tableOne, tableTwo, tableThree, tableFour, tableFive, tableSix, tableSeven);
		}

		else if (input == "test" || input == "Test") {
			for (int i = 0; i < 10000; i++) {
				std::cout << (int)tableSix[i] << ", ";
			}
		}
	}
}