#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define nQSize 100
#define maxIteration 500000
#define parentCount 200

static inline void InitialiseParents(int parents[][nQSize]);
static inline int CalculateFitness(const int parents[]);
static inline void PerformMutation(int array[]);
static inline int TournamentSelection(const int fitnessArray[]);
static inline void ParentSelection(int parents[][nQSize], int child[][nQSize], int fitnessArray[], const int childrenFitness[]);
static inline void NextGeneration(int parents[][nQSize], int fitnessArray[]);
static inline void PrintBoard(const int parents[][nQSize], const int fitnessArray[]);
static inline bool CheckForSolution(const int fitnessArray[]);

int main()
{
    int iteration = 0, parents[parentCount][nQSize], fitnessArray[parentCount];

    InitialiseParents(parents);
    for (int i = 0; i < parentCount; i++) fitnessArray[i] = CalculateFitness(parents[i]);

    clock_t start = clock();
    while (iteration < maxIteration && !CheckForSolution(fitnessArray))
    {
        NextGeneration(parents, fitnessArray);
        iteration++;
    }

    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds.\n", time);
    printf("Number of Iterations: %d\n", iteration);

    PrintBoard(parents, fitnessArray);
    return 0;
}

static inline void InitialiseParents(int parents[][nQSize])
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

        for (int i = 0; i < nQSize; i++) parents[l][i] = numberSet[i];
    }
}

static inline int CalculateFitness(const int parents[])
{
    int fitness = 0;
    for (int i = 0; i < nQSize; i++)
        for (int j = i + 1; j < nQSize; j++)
            if (parents[i] == parents[j] || abs(parents[i] - parents[j]) == abs(i - j)) fitness++;
    return fitness;
}

static inline void PerformMutation(int array[])
{
    int randomIndex1 = rand() % nQSize, randomIndex2 = rand() % nQSize;
    int temp = array[randomIndex1];
    array[randomIndex1] = array[randomIndex2];
    array[randomIndex2] = temp;
}

static inline int TournamentSelection(const int fitnessArray[])
{
    int firstIndex = rand() % parentCount, secondIndex = rand() % parentCount;
    return fitnessArray[firstIndex] < fitnessArray[secondIndex] ? firstIndex : secondIndex;
}

static inline void ParentSelection(int parents[][nQSize], int child[][nQSize], int fitnessArray[], const int childrenFitness[])
{
    for (int i = 0; i < parentCount; i++)
    {
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
            int parentIndex = rand() % 2;
            if (parentIndex == 1 && childrenFitness[i] < fitnessArray[i])
            {
                fitnessArray[i] = childrenFitness[i];
                for (int j = 0; j < nQSize; j++) parents[i][j] = child[i][j];
            }
        }
    }
}

static inline void NextGeneration(int parents[][nQSize], int fitnessArray[])
{
    int child[parentCount][nQSize];
    int childrenFitness[parentCount];
    int firstParentIndex, secondParentIndex;

    for (int i = 0; i < parentCount; i++)
    {
        firstParentIndex = TournamentSelection(fitnessArray);
        secondParentIndex = TournamentSelection(fitnessArray);

        for (int j = 0; j < nQSize; j++) child[i][j] = parents[firstParentIndex][j];

        PerformMutation(child[i]);
        childrenFitness[i] = CalculateFitness(child[i]);
    }

    ParentSelection(parents, child, fitnessArray, childrenFitness);

}

static inline void PrintBoard(const int parents[][nQSize], const int fitnessArray[])
{
    int index = 0, min = fitnessArray[0];
    for (int i = 1; i < parentCount; i++)
        if (fitnessArray[i] < min) {
            min = fitnessArray[i];
            index = i;
        }

    for (int i = 0; i < nQSize; i++) {
        for (int j = 0; j < nQSize; j++) printf(parents[index][i] == j ? "Q " : "• ");
        printf("\n");
    }
}

static inline bool CheckForSolution(const int fitnessArray[])
{
    for (int i = 0; i < parentCount; i++) if (fitnessArray[i] == 0) return true;
    return false;
}