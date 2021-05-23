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

void clearConsole() {
	system("cls");
}

void consoleColor() {
	HANDLE hConsole;
	int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// you can loop k higher to see more color choices
	for (k = 1; k < 255; k++)
	{
		// pick the colorattribute k you want
		SetConsoleTextAttribute(hConsole, k);
		std::cout << k << " I want to be nice today!" << std::endl;
	}

	std::cin.get(); // wait
}

int main()
{
	Cube3 cube;
	std::vector<char>tablePhaseOneEC;
	std::vector<char> tablePhaseOneEUD;
	std::vector<char> tablePhaseOneCUD;
	std::vector<char> tablePhaseTwo;

	bool shouldClose = false;

	std::cout << "Daniel Rising's -Rubik's Cube Solver- 1.0." << std::endl
		<< "Type [Help] for program instructions." << std::endl;

	while (!shouldClose)
	{
		std::string input;
		std::cin >> input;
		clearConsole();

		if (input == "stop" || input == "Stop")
		{
			shouldClose = true;
		}

		else if (input == "help" || input == "Help")
		{
			std::cout << "[Stop] - Exits the program." << std::endl
				<< "[Help] - Prints this message." << std::endl
				<< "[Print] - Prints the current state of the cube, both graphically as a 2D-visualization of each side of the cube and as a list of positions and rotations." << std::endl
				<< "[Rotate] x y - Rotate side x (0-Up, 1-Left, 2-Front, 3-Right, 4-Back, 5-Down) with a power of y (1-Quarter Clockwise, 2-Halfturn, 3-Quarter Counter Clockwise)." << std::endl
				<< "[Scramble] - Applies a 25-move scramble to the cube." << std::endl
				<< "[Generate] - Generates/Reads/Initalizes pruning tables for solving algorithm." << std::endl
				<< "[Solve] - (Runs [Generate]) Finds first Kociemba two phase algorithm solution, then applies it to the current cube state." << std::endl
				<< "[Benchmark] - Solve 100 individual scrambles after one another, outputs results to benchmark_results.txt" << std::endl;
		}

		else if (input == "print" || input == "Print")
		{
			cube.ConsoleRender();
			std::cout << std::endl;
			cube.ConsolePrint();
		}

		else if (input == "rotate" || input == "Rotate")
		{
			std::string move, power;
			std::cin >> move >> power;
			if (IsNumber(move) && IsNumber(power))
			{
				cube.Rotate(stoi(move), stoi(power));
			}
			cube.ConsoleRender();
		}

		else if (input == "scramble" || input == "Scramble")
		{
			int seed = std::chrono::system_clock::now().time_since_epoch().count();
			cube.Scramble(30, seed, true);
			std::cout << std::endl;
			cube.ConsoleRender();
		}

		else if (input == "generate" || input == "Generate") {
			GenerateTables(true, tablePhaseOneEC, tablePhaseOneEUD, tablePhaseOneCUD, tablePhaseTwo);
		}

		else if (input == "solve" || input == "Solve")
		{			
			// Prepare tables
			GenerateTables(false, tablePhaseOneEC, tablePhaseOneEUD, tablePhaseOneCUD, tablePhaseTwo);

			// Initialize containers
			std::vector<char> solution;
			std::vector<char> indexIdsOne = { PRUNE_EDGECORNERTWIST, PRUNE_EDGETWIST_UDCOMB, PRUNE_CORNERTWIST_UDCOMB };
			std::vector<std::vector<char>*> pruneTablesOne = { &tablePhaseOneEC, &tablePhaseOneEUD, &tablePhaseOneCUD};
			std::vector<char> indexIdsTwo = { PRUNE_PHASETWO };
			std::vector<std::vector<char>*> pruneTablesTwo = { &tablePhaseTwo };

			// Solve
			cube = KociembaAlgorithm(cube, solution, indexIdsOne, pruneTablesOne, indexIdsTwo, pruneTablesTwo);
			printFormatted(solution);
			cube.ConsoleRender();
		}
		else if (input == "input" || input == "Input") {
			cube.setStateFromCin();
		}
		else if (input == "test" || input == "Test") {
			consoleColor();
		}
		else if (input == "benchmark" || input == "Benchmark") {
			// Prepare tables
			GenerateTables(true, tablePhaseOneEC, tablePhaseOneEUD, tablePhaseOneCUD, tablePhaseTwo);

			// Initialize containers
			std::vector<char> solution;
			std::vector<char> indexIdsOne = { PRUNE_EDGECORNERTWIST, PRUNE_EDGETWIST_UDCOMB, PRUNE_CORNERTWIST_UDCOMB };
			std::vector<std::vector<char>*> pruneTablesOne = { &tablePhaseOneEC, &tablePhaseOneEUD, &tablePhaseOneCUD };
			std::vector<char> indexIdsTwo = { PRUNE_PHASETWO };
			std::vector<std::vector<char>*> pruneTablesTwo = { &tablePhaseTwo };

			// Open file output
			std::ofstream results("benchmark_results.txt");
			results << "[Case] [stdlib.h random seed] [Amount of moves] [Time]" << std::endl;

			// loop test cases
			for (int i = 0; i < 100; i++) {
				// Set random seed
				int seed = std::chrono::system_clock::now().time_since_epoch().count();
				cube.Scramble(30, seed, false);
				std::cout << i << ". Random seed: " << seed << std::endl;
				cube.ConsoleRender();

				auto start = std::chrono::system_clock::now(); // STARTING TIME

				cube = KociembaAlgorithm(cube, solution, indexIdsOne, pruneTablesOne, indexIdsTwo, pruneTablesTwo);

				std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now() - start; // CLOSING TIME

				int counter = 0;
				for (int j = 0; j < solution.size() - 1; j += MOVE_STRIDE) {
					if (solution[j] != -1) {
						counter++;
					}
				}

				results << i << " " << seed << " " << counter << " " << elapsedSeconds.count() << std::endl;
			}
		}
	}
}