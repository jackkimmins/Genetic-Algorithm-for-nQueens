#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Size of board and number of queens
const int nQSize = 100;

// Maximum number of generations
const int maxIteration = 500000;

// Number of individuals in population
const int parentCount = 200;


// This function initialises the "parents" array with random values by repeatedly
// swapping elements of a "numberSet" array and copying the result into "parents" array.
void InitialiseParents(int parents[][nQSize])
{
    srand(time(NULL));

    int numberSet[nQSize]; 
    for (int i = 0; i < nQSize; i++) numberSet[i] = i;

    for (int l = 0; l < parentCount; l++)
    {
        for (int i = 0; i < nQSize; i++)
        {
            int j = rand() % nQSize;
            int temp = numberSet[i];
            numberSet[i] = numberSet[j];
            numberSet[j] = temp;
        }

        // Copy the shuffled numberSet array into parents
        for (int i = 0; i < nQSize; i++) parents[l][i] = numberSet[i];
    }
}

// Calculates the fitness of a given individual
int CalculateFitness(int parents[])
{
    int fitness = 0;
    for (int i = 0; i < nQSize; i++)
    {
        for (int j = i + 1; j < nQSize; j++)
        {
            if (parents[i] == parents[j]) fitness++;
            else if (abs(parents[i] - parents[j]) == abs(i - j)) fitness++;
        }
    }
    return fitness;
}  

// Crossover Function
// Mutates a given individual by swapping two random elements
void PerformMutation(int array[], float* mutationProbability)
{
    int randomIndex1 = rand() % nQSize, randomIndex2 = rand() % nQSize;

    // Swap the values at randomIndex1 and randomIndex2
    int temp = array[randomIndex1];
    array[randomIndex1] = array[randomIndex2];
    array[randomIndex2] = temp;

    // Increment mutationProbability for each mutation
    *mutationProbability += 0.0001;
}

// Tournament Selection Function
// Selects two parents from the population and returns the fittest one.
void TournamentSelection(int fitnessArray[], int *selectedIndex) 
{
    int firstIndex = rand() % parentCount, secondIndex = rand() % parentCount;

    // Compare the fitness values of the two randomly selected individuals
    if (fitnessArray[firstIndex] < fitnessArray[secondIndex]) *selectedIndex = firstIndex;
    else *selectedIndex = secondIndex;
}

// Parent Selection Function
// Selects parents for the next generation.
void ParentSelection(int parents[][nQSize], int child[][nQSize], int fitnessArray[], int childrenFitness[])
{
    for (int i = 0; i < parentCount; i++)
    {
        // Checks if there is an elite individual in the population
        if (fitnessArray[i] < 3 || childrenFitness[i] < 3)
        {
            if (childrenFitness[i] < fitnessArray[i])
            {
                fitnessArray[i] = childrenFitness[i];
                for (int j = 0; j < nQSize; j++) parents[i][j] = child[i][j];
            }
        }
        else
        {
			// Randomly selects one of the parent or child to be the parent for the next generation.
            int parentIndex = rand() % 2;

            if (parentIndex == 1)
            {
                // If the child is selected and its fitness is better than that
                // of the parent, replace the parent in the next generation.
                if (childrenFitness[i] < fitnessArray[i])
                {
                    fitnessArray[i] = childrenFitness[i];
                    for (int j = 0; j < nQSize; j++) parents[i][j] = child[i][j];
                }
            }
        }
    }
}

// Generation Iteration Function
void NextGeneration(int parents[][nQSize], int fitnessArray[], float *mutationPercentage)
{
    int child[parentCount][nQSize];
    int childrenFitness[parentCount];
    int firstParentIndex, secondParentIndex;

    for (int i = 0; i < parentCount; i++)
    {
        TournamentSelection(fitnessArray, &firstParentIndex);
        TournamentSelection(fitnessArray, &secondParentIndex);

        for (int j = 0; j < nQSize; j++) child[i][j] = parents[firstParentIndex][j];

        PerformMutation(child[i], mutationPercentage);
        childrenFitness[i] = CalculateFitness(child[i]);
    }

    ParentSelection(parents, child, fitnessArray, childrenFitness);
}

// Prints the board to the console
void PrintBoard(int parents[][nQSize], int fitnessArray[])
{
    int index = 0;
    int min = fitnessArray[0];
    for (int i=0; i < parentCount; i++)
    {
        if (fitnessArray[i] < min)
        {
            min = fitnessArray[i];
            index = i;
        }
    }
    
    for (int i = 0; i < nQSize; i++)
    {
        for (int j = 0; j < nQSize; j++)
        {
            if (parents[index][i] == j) printf("Q ");
            else printf("• ");
        }
        printf("\n");
    }
}

// Checks if the solution is found
bool CheckForSolution(int fitnessArray[])
{
    for (int i = 0; i < parentCount; i++) if (fitnessArray[i] == 0) return true; return false;
}

int main()
{
    int iteration = 0;
    int parents[parentCount][nQSize];
    int fitnessArray[parentCount];
    float mutationPercentage = 1 / ((float)parentCount) + 1 / ((float)nQSize);

    InitialiseParents(parents);
    for (int i = 0; i < parentCount; i++) fitnessArray[i] = CalculateFitness(parents[i]);

    clock_t start = clock();

    while (iteration < maxIteration)
    {
        for (int i = 0; i < parentCount; i++) fitnessArray[i] = CalculateFitness(parents[i]);

        if (CheckForSolution(fitnessArray)) break;

        NextGeneration(parents, fitnessArray, &mutationPercentage);
        iteration++;

        printf("Iteration: %d, Fitness: %d, Mutation Percentage: %f\n", iteration, fitnessArray[0], mutationPercentage);
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds.\n", time);
    printf("Mutation Percentage: %f\n", mutationPercentage);
    printf("Number of Iterations: %d\n", iteration);

    PrintBoard(parents, fitnessArray);

    return 0;
}