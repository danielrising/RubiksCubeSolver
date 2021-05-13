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

void MemTest() {
	std::cout << "Initializing..." << std::endl;
	auto start = std::chrono::system_clock::now();

	std::vector<char> testVector;
	testVector.resize(3 * pow(10, 9), 1);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;
	std::cout << "Elapsed time: " << elapsedSeconds.count() << "s" << std::endl;

	std::cout << "Calculating sum using range based for-loop..." << std::endl;
	start = std::chrono::system_clock::now();

	long long int sum = 0;

	for (char elem : testVector) {
		sum += elem;
	}

	end = std::chrono::system_clock::now();
	elapsedSeconds = end - start;
	std::cout << "Elapsed time: " << elapsedSeconds.count() << "s" << std::endl;

	std::cout << sum << std::endl;

	// For loop
	std::cout << "Calculating sum using regular for-loop..." << std::endl;
	start = std::chrono::system_clock::now();

	sum = 0;

	for (long long int i = 0; i < testVector.size(); ++i) {
		sum += testVector[i];
	}

	end = std::chrono::system_clock::now();
	elapsedSeconds = end - start;
	std::cout << "Elapsed time: " << elapsedSeconds.count() << "s" << std::endl;

	std::cout << sum << std::endl;
}

int main()
{
	Cube3 cube;
	std::vector<char> table1;

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
				std::cout << cube.UDSliceCombinationIndex() << std::endl;
			}
		}

		else if (input == "tabletest") {
			// Starting time
			auto start = std::chrono::system_clock::now();

			generatePruneTableOne(4, table1);

			// Elapsed time
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << "Elapsed time (Generation): " << elapsedSeconds.count() << "s" << std::endl;

			// Starting time
			start = std::chrono::system_clock::now();

			std::ofstream fOut("PruningTest.PRUNE");
			for (long int i = 0; i < table1.size(); i++) {
				fOut << (int)table1[i];
			}
			fOut.close();

			// Elapsed time
			end = std::chrono::system_clock::now();
			elapsedSeconds = end - start;
			std::cout << "Elapsed time (File output): " << elapsedSeconds.count() << "s" << std::endl;
		}

		else if (input == "memtest")
		{	
			std::vector<char> mem(3000000000, 2);
			auto start = std::chrono::system_clock::now();

			int sum = 0;
			srand(1234);
			for (int i = 0; i < 1000000; i++) {
				int random = rand() % 3000000000;
				sum += mem[random];
			}
			std::cout << sum;

			// Elapsed time
			auto end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsedSeconds = end - start;
			std::cout << "Elapsed time: " << elapsedSeconds.count() << "s" << std::endl;

			//MemTest();
		}

		else if (input == "test") {
			std::cout << Choose(12, 4) << std::endl;
		}
	}
}