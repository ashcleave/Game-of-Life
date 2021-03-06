// GameOfLife.cpp : Defines the entry point for the console application.
//

/* 
Assumptions:
Game of life grid is always square or rectangular.
The code will be run in a Windows environment.
The grid dimensions will not be set smaller than 5 x 5.
Only single characters will be entered into the user interface.
The #include "stdafx.h" line may need to be removed should the
	development environment the code is run in not be visual studio.
For all but two scenarios regarding a dead cell , the outcome is not specified.
	Therefore, it has been assumed the cell will remain dead
*/

#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <time.h> 

using namespace std;

/* Defining game of life grid size */
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;

/* Function declarations */
bool userInterfece(int arr[][GRID_HEIGHT]);
void gridInit(int arr[][GRID_HEIGHT]);
void gridInit(int arr[][GRID_HEIGHT], float threshold);
void gridInitTest(int arr[][GRID_HEIGHT]);
void gridIterate(int arr[][GRID_HEIGHT]);
int calculateScenario(int cell, int neighbour);
void printGrid(int arr[][GRID_HEIGHT], int iteration);

int main()
{
	/* Defining program variables */
	int gameOfLife[GRID_WIDTH][GRID_HEIGHT];	// Defining game of life grid


	/* Initialise grid cells - launches user interface, runs until user input is accepted */
	while (userInterfece(gameOfLife));
	system("cls");	// Clears console to allow for only the game to be visualised 


	/* Display game of life grid */
	int iteration = 0;							// Variable to count the iteration of the game of life
	printGrid(gameOfLife, iteration);

	
	/* Iterating the game */
	double const timeInterval = 0.5;			// Time intervals between iterations 
	time_t lastTime, currentTime;				// Variables for iterating the game of life
	lastTime = time(nullptr);

	while (true)
	{
		currentTime = time(nullptr);	

		if ((currentTime - lastTime) > timeInterval)	// Calculation of elapsed time
		{
			system("cls");								// Clearing console (Windows specific command)
			++iteration;

			gridIterate(gameOfLife);					// Iterate/update grid
			
			printGrid(gameOfLife, iteration);			// Display game of life grid

			lastTime = currentTime;						// Update time variables 
		}
	}

    return 0;
}

/* User interface to initialise the game of life. This is used because
there are three different function definitions to initialise the grid with */
bool userInterfece(int arr[][GRID_HEIGHT])
{
	char userSelection;
	float threshold;

	// Printing instructions to the user
	cout << "Please enter either 1, 2 or 3 to select an initial state for the game of life grid:"
		<< endl
		<< "1 - Randomly distribute 0's and 1's"
		<< endl
		<< "2 - Randomly distribute 0's and 1's with a bias towards either 0's or 1's"
		<< endl
		<< "3 - Initialise the grid with the example state in the BBC instruction document"
		<< endl
		<< "Enter choice: ";

	cin >> userSelection;		// Acquiring user input

	// Running code relevant to the user input
	switch (userSelection)
	{
	// Randomly initialise grid
	case '1':
		gridInit(arr);
		return false;

	// Rendomly initialise grid with a potential bias towards 0's ot 1's
	case '2':
		// Printing further instructions to the user
		cout << endl
			<< "Please enter a number between 0 and 1. \nThe closer to 1, the more 0's will be present"
			<< endl
			<< "Enter choice: ";

		cin >> threshold;		// Acquiring user input
		gridInit(arr, threshold);
		return false;

	// Initialise grid to demonstrate example conditions
	case '3':
		gridInitTest(arr);
		return false;

	// Default condition. Function doesn't break loop in main() so user asked to 
	//   input something again
	default:
		// Informing the user that the input was not valid
		cout << "Input not valid." << endl << endl;
		break;
	}
}

/* Initialises each grid value randomly with a 1 or 0 */
void gridInit(int arr[][GRID_HEIGHT])
{
	int randNo;

	// Randomise random number sequence
	srand(time(nullptr));

	// Loops through each game of life grid element, randomly initialising a 0 or 1
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			randNo = rand() % 2;	// Returns a 0 or 1
			arr[j][i] = randNo;
		}
	}
}

/* Overloaded function that additionally takes a decimal value between 0 and 1,
larger value means more 0's */
void gridInit(int arr[][GRID_HEIGHT], float threshold)
{
	float randNo;

	// Randomise random number sequence
	srand(time(nullptr));

	// Loops through each game of life grid element, randomly initialising a 0 or 1
	//	 however with biased odds of either a 0 or 1
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			// Generating random numbers <= 1
			randNo = (rand() / double(RAND_MAX));

			// Assigning 0 or 1 depending on the random number returned 
			(randNo <= threshold) ? arr[j][i] = 0 : arr[j][i] = 1;
		}
	}
}

/* Makes a string of three 1's, horizontally, at the (x,y) position specified */
void gridInitTest(int arr[][GRID_HEIGHT])
{
	// Position of the test alive cells
	int xPos = 2;
	int yPos = 1;

	// Looping through the game of life grid, initialising 0's bar the three positions identified
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			// Checking the current x and y grid coordinate against the three specified to be 1's
			if ((i == xPos) && (j >= yPos) && (j < (yPos + 3)))
			{
				arr[j][i] = 1;
			}
			else
			{
				arr[j][i] = 0;
			}
		}
	}
}

/* Looping through each grid cell, calculating the new value based on surrounding cells */
void gridIterate(int arr[][GRID_HEIGHT])
{
	// Accumulator used to sum the alive cells around the current cell
	int accumulator;
	int tempGrid[GRID_WIDTH][GRID_HEIGHT];		// Temporary game of life grid

	// Looping through each element of the game of life grid
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			accumulator = 0;	// Reset accumulator

			// Loop selecting all 8 grid locations around the cell of interest
			for (int a = (i - 1); a <= (i + 1); a++)
			{
				for (int b = (j - 1); b <= (j + 1); b++)
				{
					// If cell is within bounds and not including the center cell
					if (!((a < 0) || (a >= GRID_HEIGHT) || (b < 0) || (b >= GRID_HEIGHT) || ((a == i) && (b == j))))
					{
						if (arr[b][a] == 1)
						{
							accumulator++;	// A count of how many live cells are neighbouring the current cell
						}
					}
				}
			}
			
			// Calculate new array value and store in temporary game of life grid
			tempGrid[j][i] = calculateScenario(arr[j][i], accumulator);
		}
	}

	// Update game of life grid with new values in temp grid
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			arr[j][i] = tempGrid[j][i];
		}
	}
}

/* Depending on the current cell value, the new value is calculated based on neighbouring cells */
int calculateScenario(int cell, int neighbour)
{
	if (cell == 0)
	{
		// For the current cell state being dead, return new cell state based on number of neighbours
		if (neighbour == 0)
		{
			return 0;	// Satisfies scenario 0
		}
		else if (neighbour == 3)
		{
			return 1;	// Satisfies scenario 4
		}
		else
		{
			return 0;	// Scenario not specified so this is the assumed outcome
		}
	}
	else
	{
		// For the current cell state being alive, return new cell state based on number of neighbours
		if (neighbour < 2)
		{
			return 0;	// Satisfies scenario 1
		}
		else if (neighbour > 3)
		{
			return 0;	// Satisfies scenario 2
		}
		else if ((neighbour == 2) || (neighbour == 3))
		{
			return 1;	// Satisfies scenario 3
		}
		else
		{
			// Enters a -1 into the grid to display an error, should one have somehow occured
			cout << "Error - Unspecified game scenario occured" << endl;	// Print error message to console
			return -1;
		}
	}
}

/* Displays game of life grid and iteration value */
void printGrid(int arr[][GRID_HEIGHT], int iteration)
{
	// Looping through the game of life grid, printing each row to the console
	for (int i = 0; i < GRID_HEIGHT; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
		{
			cout << arr[j][i] << " ";
		}
		cout << endl;
	}
	// Printing the current game iteration under the game of life grid
	cout << endl << "Iteration count: " << iteration;
}