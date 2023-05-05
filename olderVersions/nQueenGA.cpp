#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>

class Board
{
private:
    int _size;
    std::vector<int> _board;
public:
    // Constructor
    Board(std::vector<int> board, int &size)
    {
        _board = board;
        _size = size;
    }

    // Getter for board
    std::vector<int> &GetBoard()
    {
        return _board;
    }

    // Calculate the number of conflicts with other queens.
    int Fitness()
    {
        int conflicts = 0;
        for (int i = 0; i < _size; i++)
        {
            for (int j = i + 1; j < _size; j++)
            {
                if (_board[i] == _board[j]) conflicts++;
                else if (abs(_board[i] - _board[j]) == abs(i - j)) conflicts++;
            }
        }
        return conflicts;
    }

    // Generate a random ordering of size numbers.
    static Board RandomChromosome(int &size)
    {
        std::vector<int> board;
        for (int i = 0; i < size; i++) board.push_back(i);
        for (int i = 0; i < size; i++)
        {
            int j = rand() % size;
            int temp = board[i];
            board[i] = board[j];
            board[j] = temp;
        }
        return Board(board, size);
    }

    // Select a parent using tournament selection.
    static Board TournamentSelection(std::vector<Board> &population, int &tournamentSize)
    {
        std::vector<Board> tournament;
        for (int i = 0; i < tournamentSize; i++) tournament.push_back(population[rand() % population.size()]);
        std::sort(tournament.begin(), tournament.end(), [](Board &a, Board &b) { return a.Fitness() < b.Fitness(); });
        return tournament[0];
    }

    // Perform multi-point crossover to create a new chromosome from two existing ones.
    Board MultiPointCrossover(Board &board, int &n)
    {
        std::vector<int> newBoard;
        int i = rand() % n;
        int j = rand() % n;
        int start = std::min(i, j);
        int end = std::max(i, j);

        for (int k = 0; k < n; k++)
        {
            if (k >= start && k <= end) newBoard.push_back(_board[k]);
            else newBoard.push_back(board._board[k]);
        }
        
        return Board(newBoard, _size);
    }

    // Perform adaptive mutation on the chromosome.
    void AdaptiveMutation(int &mutationRate)
    {
        for (int i = 0; i < _size; i++)
        {
            if (rand() % 100 < mutationRate)
            {
                int j = rand() % _size;
                int temp = _board[i];
                _board[i] = _board[j];
                _board[j] = temp;
            }
        }
    }
};

class GeneticAlgorithm
{
private:
    int _size;
    int _populationSize;
    int _tournamentSize;
    int _mutationRate;
    int _generations;
    int _genCount = 0;
    std::vector<Board> _population;
public:
    // Constructor
    GeneticAlgorithm(int &size, int &populationSize, int &tournamentSize, int &mutationRate, int &generations)
    {
        _size = size;
        _populationSize = populationSize;
        _tournamentSize = tournamentSize;
        _mutationRate = mutationRate;
        _generations = generations;
    }

    // Generate new offspring by crossover and mutation.
    void GenerateOffspring(std::vector<Board> &parents)
    {
        std::vector<Board> offspring;
        while (offspring.size() < _populationSize)
        {
            Board parent1 = Board::TournamentSelection(parents, _tournamentSize);
            Board parent2 = Board::TournamentSelection(parents, _tournamentSize);
            Board child = parent1.MultiPointCrossover(parent2, _size);
            child.AdaptiveMutation(_mutationRate);
            offspring.push_back(child);
        }
        _population = offspring;
    }

    // Using multi-threading, call the generate offspring function.
    void GenerateOffspringThread(std::vector<Board> &parents)
    {
        const int numThreads = 8;

        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; i++) threads.push_back(std::thread(&GeneticAlgorithm::GenerateOffspring, this, std::ref(parents)));
        for (int i = 0; i < numThreads; i++) threads[i].join();
    }

    // Evaluate the fitness of each board in the population.
    void EvaluateFitness()
    {
        std::sort(_population.begin(), _population.end(), [](Board &a, Board &b) { return a.Fitness() < b.Fitness(); });
    }

    // Use a genetic algorithm to find a solution to the n-queens problem.
    double Solve()
    {
        clock_t start = clock();

        // Initialize the population.
        for (int i = 0; i < _populationSize; i++)
        {
            _population.push_back(Board::RandomChromosome(_size));
        }

        // Iterate through the generations.
        for (int i = 0; i < _generations; i++)
        {
            // Evaluate the fitness of each board in the population.
            EvaluateFitness();

            // Check if a solution has been found.
            if (_population[0].Fitness() == 0) break;

            // Generate new offspring by crossover and mutation.
            GenerateOffspringThread(_population);
            _genCount++;
        }

        return (clock() - start) / (double)CLOCKS_PER_SEC;
    }

    // Print the board to the console.
    void PrintSolution()
    {
        std::cout << "Solution found in " << _genCount << " generations:" << std::endl;
        for (int i = 0; i < _size; i++)
        {
            for (int j = 0; j < _size; j++)
            {
                if (_population[0].GetBoard()[i] == j) std::cout << "Q ";
                else std::cout << "• ";
            }
            std::cout << std::endl;
        }
    }
};

// n-Queen with Genetic Algorithm
int main()
{
    int size = 100;
    int populationSize = 2500;
    int tournamentSize = 1000;
    int mutationRate = 10;
    int generations = 1000;

    GeneticAlgorithm geneticAlgorithm = GeneticAlgorithm(size, populationSize, tournamentSize, mutationRate, generations);

    double timeTaken = geneticAlgorithm.Solve();

    std::cout << "Operation completed in " << timeTaken << " seconds." << std::endl;

    geneticAlgorithm.PrintSolution();

    return 0;
}