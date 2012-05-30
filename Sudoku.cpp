/*************************************************************************
Roman Gabriele Ocampo
5.31.2012
Sudoku Generator

This program is designed to generate a random 9x9 grid of integers following
the rules of traditional sudoku. First, the program goes through every cell
and determines which one has the least number of possibilities. From there, it
assigns that cell a number between 1-9 following the rules for a valid number.
Then, it eliminates that number's possibility in the cell's row, column, and box.
This process is repeated, selecting each new cell by finding the one with the least
number possible. There are occasions where a 9x9 grid cannot be completed.
When this loop repeats long enough without filling in a cell, it trashes its progress
and starts over with a blank grid.

The second part of this program removes cells from the completed grid to create
the actual puzzle. It picks a cell at random and clears it out. Then, the program
runs through a similar process to the one above to try to solve it. If the solution is
the same as the one generated above, the cleared cell is considered valid and the program
moves on to clear more cells in this fashion. If it creates a solution dissimilar to the one
generated above, the program returns the original values to the cell and repeats
the process until it achieves the correct number of cleared cells.

Currently, this program has issues with creating more difficult puzzles.
The brute force nature of the solving portion causes problems. Namely, for
different values of HOWMANYSQUARESDIDYOUWANTBLANK and MAXPOSSIBILITIES,
the output puzzle may not be solvable by humans. With it's current values of
HOWMANYSQUARESDIDYOUWANTBLANK=35 and MAXPOSSIBILITIES=1, the program often
generates a valid, one solution sudoku, albeit a really simple one to solve.
To improve this, a better solving algorithm needs to be implemented. Rather than
brute force, this algorithm needs to use human strategies to solve sudoku puzzles.

*************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int HOWMANYSQUARESDIDYOUWANTCOMPLETED=81; //Number of cells required for a complete sudoku, do not change.
const int HOWMANYSQUARESDIDYOUWANTBLANK=35;
/*******************************************
HOWMANYSQUARESDIDYOUWANTBLANK determines how many cells are removed by the program
to generate the puzzle. This value can be changed to eliminate more cells to make
the puzzle more difficult. However, the brute force nature of the program usually causes
more blank squares to result in either an impossible to solve sudoku, or a sudoku with
multiple solutions.
*******************************************/
const int MAXPOSSIBILITIES=1;
/******************************************
MAXPOSSIBILITIES is used in the puzzle generating portion in conjuction with
HOWMANYSQUARESDIDYOUWANTBLANK. This number dictates how easily each cell's
value can be derived from every other clue. When eliminating cells to generate
the puzzle, the number of possible values that can fit in each cell is compared
to MAXPOSSIBILITIES. For larger values of HOWMANYSQUARESDIDYOUWANTBLANK, MAXPOSSIBILITES
must also increase. Otherwise, the program may loop indefinately in an attempt
to create a sudoku puzzle.
******************************************/

struct sudoku //Used to create an array for the sudoku grid
{
	int value; //Value of the cell
	bool possible1, //If the value 1 can fit in the cell
		possible2, //Same, but for the number 2.
		possible3, //Same, but for the number 3. ETC.
		possible4,
		possible5,
		possible6,
		possible7,
		possible8,
		possible9; //In hindsight, this could have also been an array within the struct
	int possiblecount; //The number of possibilities for each cell. Sum of 'trues' for the above bool values
	char display; //The character that is displayed to the grid. If 0, it is an underscore.
};

void initializesquares(sudoku[9][9]);
void displayboard(sudoku[9][9]);
void findnextsquare(sudoku[9][9],int&,int&);
int fillinsquare(sudoku[9][9],int,int);
void fillin1(sudoku[9][9],int,int);
void fillin2(sudoku[9][9],int,int);
void fillin3(sudoku[9][9],int,int);
void fillin4(sudoku[9][9],int,int);
void fillin5(sudoku[9][9],int,int);
void fillin6(sudoku[9][9],int,int);
void fillin7(sudoku[9][9],int,int);
void fillin8(sudoku[9][9],int,int);
void fillin9(sudoku[9][9],int,int);
void eliminatepossible1(sudoku[9][9],int,int);
void eliminatepossible2(sudoku[9][9],int,int);
void eliminatepossible3(sudoku[9][9],int,int);
void eliminatepossible4(sudoku[9][9],int,int);
void eliminatepossible5(sudoku[9][9],int,int);
void eliminatepossible6(sudoku[9][9],int,int);
void eliminatepossible7(sudoku[9][9],int,int);
void eliminatepossible8(sudoku[9][9],int,int);
void eliminatepossible9(sudoku[9][9],int,int);
void eliminatepossibilities(sudoku[9][9]);
void countpossibles(sudoku[9][9],bool&);
void setuppuzzle(sudoku[9][9],sudoku[9][9]);
void chooserandomsquare(sudoku[9][9],int&,int&);
void clearsquare(sudoku[9][9],int,int);
bool canbesolved(sudoku[9][9],sudoku[9][9]);
int solvesquare(sudoku[9][9],int,int);
bool checksolution(sudoku[9][9],sudoku[9][9]);
void countnumberofblanks(sudoku[9][9],bool&);
int highestnumberpossible(sudoku[9][9]);

int main()
{
	bool completed=false;
	int nexti=0; //The next value of i, or the column. Used for the coordinate system (i,j) around the grid.
	int nextj=0; //The next value of j, or the row. Begins at coordinate (0,0)
	char response; //Requests an input before displaying the solutin, as well as before exiting.
	sudoku square[9][9]; //square will be the array that will hold the generated solution
	sudoku puzzle[9][9]; //puzzle will be the array that has its values cleared. puzzle will be compared to square to check solutions.
	srand(time(NULL)); //Initializes the random seed for the rand() function.
	initializesquares(square);
	while (!completed)
	{
		findnextsquare(square,nexti,nextj);
		fillinsquare(square,nexti,nextj);
		countpossibles(square,completed); //When the highest possibilities-0, it returns completed=true to end the loop
	}
	setuppuzzle(square,puzzle);
	completed=false; //The loop that generates the puzzle also runs on the completed value.
	int toomucherror=0; //If the loop occurs too long without removing another cell, it starts over again.
	while(!completed)
	{
		if (toomucherror>500)
			setuppuzzle(square,puzzle);
		chooserandomsquare(puzzle,nexti,nextj);
		clearsquare(puzzle,nexti,nextj);
		eliminatepossibilities(puzzle);
		if (!canbesolved(square,puzzle)) //If it can't be solved, the original values are returned to the cell
		{
			puzzle[nextj][nexti]=square[nextj][nexti];
			toomucherror++; //And the error count increases
		}
		if (highestnumberpossible(puzzle)>MAXPOSSIBILITIES) //If the cell has too many possibilities, the original values are returned to the cell
		{
			puzzle[nextj][nexti]=square[nextj][nexti];
			toomucherror++; //And the error count increases
		}
		countnumberofblanks(puzzle,completed); //When enough squares have been cleared, the loop concludes.
	}
	displayboard(puzzle); //Displays the puzzle array, which has had some of its cells cleared.
	cout << "Push any key and hit enter for solution" << endl;
	cin >> response;
	displayboard(square); //Displays the square array, which has the solution.
	cout << "Push any key and hit enter to quit" << endl;
	cin >> response;
}

void initializesquares(sudoku square[9][9])
	//Takes the sudoku array and clears all its cells.
{
	for (int j=0;j<9;j++)
	{
		for (int i=0;i<9;i++)
		{
			square[j][i].value=0;
			square[j][i].possible1=true;
			square[j][i].possible2=true;
			square[j][i].possible3=true;
			square[j][i].possible4=true;
			square[j][i].possible5=true;
			square[j][i].possible6=true;
			square[j][i].possible7=true;
			square[j][i].possible8=true;
			square[j][i].possible9=true;
			square[j][i].possiblecount=9;
			square[j][i].display='_';
		}
	}
}

void findnextsquare(sudoku square[9][9],int& nexti, int& nextj)
	//Goes through every cell to find the one with the least possibilities.
	//The coordinate for the first cell with the fewest possibilities is returned
	//as nexti and nextj.
{
	int lowestpossiblecount=100;
	int currentcount;
	for (int j=0;j<9;j++)
	{
		for (int i=0;i<9;i++)
		{
			currentcount=square[j][i].possiblecount;
			if (currentcount==0) //If there are 0 possibilities for a cell, that means it is solved
			{}                   //We don't need to solve an already solved cell, so it is ignored
			else
			{
				if (currentcount<lowestpossiblecount)
				{
					lowestpossiblecount=currentcount;
					nexti=i;
					nextj=j;
				}
			}
		}
	}
}

int fillinsquare(sudoku square[9][9],int nexti,int nextj)
	//This function guesses a random number and sees if it can fit
	//in the cell. If it can, it calls the function to fill it in properly.
	//If it can't, it tries another random number until it can fit.
	//In the event where no possible number can fit, it restarts the
	//entire solution generating loop.
{
	int guess;
	int errorcount=0;
	while(true)
	{
		if (errorcount>100)
		{
			initializesquares(square); //If it errors too much, it restarts the whole solution.
			return 0;
		}
		guess=rand()%10;
		if (guess==0) //Zero is not a valid number for a sudoku. It is ignored.
		{}
		else
		{
			if (guess==1)
			{
				if (square[nextj][nexti].possible1)
				{
					fillin1(square,nexti,nextj);
					eliminatepossible1(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==2)
			{
				if (square[nextj][nexti].possible2)
				{
					fillin2(square,nexti,nextj);
					eliminatepossible2(square,nexti,nextj);
					return 0;
				}
				else 
					errorcount++;
			}
			if (guess==3)
			{
				if (square[nextj][nexti].possible3)
				{
					fillin3(square,nexti,nextj);
					eliminatepossible3(square,nexti,nextj);
					return 0;
				}
				else
				{
					errorcount++;
				}
			}
			if (guess==4)
			{
				if (square[nextj][nexti].possible4)
				{
					fillin4(square,nexti,nextj);
					eliminatepossible4(square,nexti,nextj);
					return 0;
				}
				else 
				{
					errorcount++;
				}
			}
			if (guess==5)
			{
				if (square[nextj][nexti].possible5)
				{
					fillin5(square,nexti,nextj);
					eliminatepossible5(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==6)
			{
				if (square[nextj][nexti].possible6)
				{
					fillin6(square,nexti,nextj);
					eliminatepossible6(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==7)
			{
				if (square[nextj][nexti].possible7)
				{
					fillin7(square,nexti,nextj);
					eliminatepossible7(square,nexti,nextj);
					return 0;
				}
				else 
					errorcount++;
			}
			if (guess==8)
			{
				if (square[nextj][nexti].possible8)
				{
					fillin8(square,nexti,nextj);
					eliminatepossible8(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==9)
			{
				if (square[nextj][nexti].possible9)
				{
					fillin9(square,nexti,nextj);
					eliminatepossible9(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
		}
	}
}

void fillin1(sudoku square[9][9],int nexti, int nextj)
	//Fills in the value of 1 for the cell.
	//Pretty much self explanitory.
	//Theres a function like this for 1 through 9.
	//I will not comment on all of them.
	//They pretty much do the same exact thing.
{
	square[nextj][nexti].value=1;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='1';
}

void fillin2(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=2;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='2';
}

void fillin3(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=3;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='3';
}

void fillin4(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=4;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='4';
}

void fillin5(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=5;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='5';
}

void fillin6(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=6;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='6';
}

void fillin7(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=7;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='7';
}

void fillin8(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=8;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='8';
}

void fillin9(sudoku square[9][9],int nexti, int nextj)
{
	square[nextj][nexti].value=9;
	square[nextj][nexti].possible1=false;
	square[nextj][nexti].possible2=false;
	square[nextj][nexti].possible3=false;
	square[nextj][nexti].possible4=false;
	square[nextj][nexti].possible5=false;
	square[nextj][nexti].possible6=false;
	square[nextj][nexti].possible7=false;
	square[nextj][nexti].possible8=false;
	square[nextj][nexti].possible9=false;
	square[nextj][nexti].possiblecount=0;
	square[nextj][nexti].display='9';
}

void eliminatepossible1(sudoku square[9][9],int nexti, int nextj)
	//When a cell gets filled in, the cells in its row, column, and box need to
	//have the possibility of that number removed.
	//The following functions remove each possibility from 1 to 9.
	//I won't comment on them because they do the exact same thing for each value.
{
	for(int i=0;i<9;i++) //Removes from row
		square[nextj][i].possible1=false;
	for(int j=0;j<9;j++) //Removes from column
		square[j][nexti].possible1=false;
	if(nextj>=0&&nextj<=2) //Removes from box. A bit more complicated than the previous two.
	{                      //First it determines which box the cell is located, and then eliminates the possibilities.
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible1=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible1=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible1=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible1=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible1=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible1=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible1=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible1=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible1=false;
			}
		}
	}
}

void eliminatepossible2(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible2=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible2=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible2=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible2=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible2=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible2=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible2=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible2=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible2=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible2=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible2=false;
			}
		}
	}
}

void eliminatepossible3(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible3=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible3=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible3=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible3=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible3=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible3=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible3=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible3=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible3=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible3=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible3=false;
			}
		}
	}
}

void eliminatepossible4(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible4=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible4=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible4=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible4=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible4=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible4=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible4=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible4=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible4=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible4=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible4=false;
			}
		}
	}
}

void eliminatepossible5(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible5=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible5=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible5=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible5=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible5=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible5=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible5=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible5=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible5=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible5=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible5=false;
			}
		}
	}
}

void eliminatepossible6(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible6=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible6=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible6=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible6=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible6=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible6=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible6=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible6=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible6=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible6=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible6=false;
			}
		}
	}
}

void eliminatepossible7(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible7=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible7=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible7=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible7=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible7=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible7=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible7=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible7=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible7=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible7=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible7=false;
			}
		}
	}
}

void eliminatepossible8(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible8=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible8=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible8=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible8=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible8=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible8=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible8=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible8=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible8=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible8=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible8=false;
			}
		}
	}
}

void eliminatepossible9(sudoku square[9][9],int nexti, int nextj)
{
	for(int i=0;i<9;i++)
		square[nextj][i].possible9=false;
	for(int j=0;j<9;j++)
		square[j][nexti].possible9=false;
	if(nextj>=0&&nextj<=2)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible9=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible9=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=0;j<3;j++)
					square[j][i].possible9=false;
			}
		}
	}
	if(nextj>=3&&nextj<=5)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible9=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible9=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=3;j<6;j++)
					square[j][i].possible9=false;
			}
		}
	}
	if(nextj>=6&&nextj<=8)
	{
		if(nexti>=0&&nexti<=2)
		{
			for(int i=0;i<3;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible9=false;
			}
		}
		if(nexti>=3&&nexti<=5)
		{
			for(int i=3;i<6;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible9=false;
			}
		}
		if(nexti>=6&&nexti<=8)
		{
			for(int i=6;i<9;i++)
			{
				for(int j=6;j<9;j++)
					square[j][i].possible9=false;
			}
		}
	}
}

void countpossibles(sudoku square[9][9],bool& complete)
	//Counts the number of possibilities for every cell.
	//If the cell has a value in it, this is also counted in completesquares.
	//Once completesquares reaches HOWMANYSQUARESDIDYOUWANTCOMPLETED (which should always be 81)
	//It makes complete=true to end the solution generating loop.
{
	int possibles;
	int completesquares=0;
	for (int j=0;j<9;j++)
	{
		for (int i=0;i<9;i++)
		{
			possibles=0;
			if (square[j][i].possible1)
				possibles++;
			if (square[j][i].possible2)
				possibles++;
			if (square[j][i].possible3)
				possibles++;
			if (square[j][i].possible4)
				possibles++;
			if (square[j][i].possible5)
				possibles++;
			if (square[j][i].possible6)
				possibles++;
			if (square[j][i].possible7)
				possibles++;
			if (square[j][i].possible8)
				possibles++;
			if (square[j][i].possible9)
				possibles++;
			square[j][i].possiblecount=possibles;
			if (square[j][i].value!=0)
				completesquares++;
		}
	}
	if (completesquares==HOWMANYSQUARESDIDYOUWANTCOMPLETED)
		complete=true;
}

void setuppuzzle(sudoku square[9][9],sudoku puzzle[9][9])
	//This function duplicates the array in square to the array puzzle.
	//Remember, these are only the names of parameters.
	//Sometime later, I call setuppuzzle(puzzle,temp). That means puzzle is called
	//square and temp is called puzzle IN THIS FUNCTION ONLY.
{
	for(int j=0;j<9;j++)
	{
		for(int i=0;i<9;i++)
			puzzle[j][i]=square[j][i];
	}
}

void chooserandomsquare(sudoku square[9][9],int& i, int& j)
	//This function chooses a random square for clearing.
{
	int tempi=10;
	int tempj=10;
	while(tempi>8||tempj>8)
	{
		tempi=rand()%10;
		tempj=rand()%10;
		if (tempi==9||tempj==9) //There is no row or column 9.
		{}                      //So it is ignored
		else
		{
			if (square[tempj][tempi].value!=0) //If the value is already zero, we don't need to clear it again.
			{
				i=tempi;
				j=tempj;
			}
		}
	}
}

void clearsquare(sudoku square[9][9],int i, int j)
	//A lot like the initializesquares function, except this one only does it to one cell.
{
	square[j][i].value=0;
	square[j][i].possible1=true;
	square[j][i].possible2=true;
	square[j][i].possible3=true;
	square[j][i].possible4=true;
	square[j][i].possible5=true;
	square[j][i].possible6=true;
	square[j][i].possible7=true;
	square[j][i].possible8=true;
	square[j][i].possible9=true;
	square[j][i].possiblecount=9;
	square[j][i].display='_';
}

void eliminatepossibilities(sudoku square[9][9])
	//Sees in the value for each solved cell and calls the appropriate
	//eliminatepossible function. This is so that each cleared cell can
	//be solved in ordinance with sudoku rules.
{
	int currentvalue;
	for(int j=0;j<9;j++)
	{
		for(int i=0;i<9;i++)
		{
			currentvalue=square[j][i].value;
			if (currentvalue==1)
				eliminatepossible1(square,i,j);
			if (currentvalue==2)
				eliminatepossible2(square,i,j);
			if (currentvalue==3)
				eliminatepossible3(square,i,j);
			if (currentvalue==4)
				eliminatepossible4(square,i,j);
			if (currentvalue==5)
				eliminatepossible5(square,i,j);
			if (currentvalue==6)
				eliminatepossible6(square,i,j);
			if (currentvalue==7)
				eliminatepossible7(square,i,j);
			if (currentvalue==8)
				eliminatepossible8(square,i,j);
			if (currentvalue==9)
				eliminatepossible9(square,i,j);
		}
	}
}

bool canbesolved(sudoku solution[9][9],sudoku square[9][9])
	//This function returns true if the generated puzzle can be solved.
	//It uses a technique identical to how the sudoku was originally generated.
{
	sudoku temp[9][9];
	setuppuzzle(square,temp);
	bool solved=false;
	int nexti=0;
	int nextj=0;
	while (!solved)
	{
		findnextsquare(temp,nexti,nextj); //Finds the coordinates with the least possibilities.
		if (solvesquare(temp,nexti,nextj)==100) //solvesquare uses the same techniques as fillinsquare
			return false;                       //It returns 100 if the error count is too high **See solvesquare below**
		countpossibles(temp,solved); //Checks to see if the entire puzzle is solved.
	}
	if (!checksolution(solution,temp)) //Checks if puzzle solution is identical to generated solution.
		return false;
	else
		return true;
}

int highestnumberpossible(sudoku square[9][9])
	//Finds and returns the highest number of possibilities from any cell.
	//This value is compared to MAXPOSSIBILITIES above.
{
	int possibles;
	int highest=0;
	for (int j=0;j<9;j++)
	{
		for (int i=0;i<9;i++)
		{
			possibles=0;
			if (square[j][i].possible1)
				possibles++;
			if (square[j][i].possible2)
				possibles++;
			if (square[j][i].possible3)
				possibles++;
			if (square[j][i].possible4)
				possibles++;
			if (square[j][i].possible5)
				possibles++;
			if (square[j][i].possible6)
				possibles++;
			if (square[j][i].possible7)
				possibles++;
			if (square[j][i].possible8)
				possibles++;
			if (square[j][i].possible9)
				possibles++;
			square[j][i].possiblecount=possibles;
			if (possibles>highest)
				highest=possibles;
		}
	}
	return highest;
}

int solvesquare(sudoku square[9][9],int nexti,int nextj)
	//Uses identical techniques to fillinsquare.
{
	int guess;
	int errorcount=0;
	while(true)
	{
		if (errorcount>100)
		{
			return 100; //If the errorcount becomes too high, the puzzle is considered insolvable, and makes
		}               //function canbesolved above return false.
		guess=rand()%10;
		if (guess==0)
		{}
		else
		{
			if (guess==1)
			{
				if (square[nextj][nexti].possible1)
				{
					fillin1(square,nexti,nextj);
					eliminatepossible1(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==2)
			{
				if (square[nextj][nexti].possible2)
				{
					fillin2(square,nexti,nextj);
					eliminatepossible2(square,nexti,nextj);
					return 0;
				}
				else 
					errorcount++;
			}
			if (guess==3)
			{
				if (square[nextj][nexti].possible3)
				{
					fillin3(square,nexti,nextj);
					eliminatepossible3(square,nexti,nextj);
					return 0;
				}
				else
				{
					errorcount++;
				}
			}
			if (guess==4)
			{
				if (square[nextj][nexti].possible4)
				{
					fillin4(square,nexti,nextj);
					eliminatepossible4(square,nexti,nextj);
					return 0;
				}
				else 
				{
					errorcount++;
				}
			}
			if (guess==5)
			{
				if (square[nextj][nexti].possible5)
				{
					fillin5(square,nexti,nextj);
					eliminatepossible5(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==6)
			{
				if (square[nextj][nexti].possible6)
				{
					fillin6(square,nexti,nextj);
					eliminatepossible6(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==7)
			{
				if (square[nextj][nexti].possible7)
				{
					fillin7(square,nexti,nextj);
					eliminatepossible7(square,nexti,nextj);
					return 0;
				}
				else 
					errorcount++;
			}
			if (guess==8)
			{
				if (square[nextj][nexti].possible8)
				{
					fillin8(square,nexti,nextj);
					eliminatepossible8(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
			if (guess==9)
			{
				if (square[nextj][nexti].possible9)
				{
					fillin9(square,nexti,nextj);
					eliminatepossible9(square,nexti,nextj);
					return 0;
				}
				else
					errorcount++;
			}
		}
	}
}

bool checksolution(sudoku solution[9][9],sudoku guess[9][9])
	//Compares the solution array to the guess array.
	//If they are the same, it returns true, otherwise it returns false.
{
	int correctsquares=0;
	for (int j=0;j<9;j++)
	{
		for (int i=0;i<9;i++)
		{
			if (guess[j][i].value==solution[j][i].value)
			{
				correctsquares++;
			}
		}
	}
	if (correctsquares==81)
	{
		return true;
	}
	else
		return false;
}

void countnumberofblanks(sudoku puzzle[9][9],bool& completed)
	//Counts the number of cleared out cells. If this value
	//is equal to HOWMANYSQUARESDIDYOUWANTBLANK, it considers the puzzle
	//generator to be done and returns completed=true to end the puzzle
	//generating loop.
{
	int blanks=0;
	for(int j=0;j<9;j++)
	{
		for(int i=0;i<9;i++)
		{
			if (puzzle[j][i].value==0)
				blanks++;
		}
	}
	if (blanks==HOWMANYSQUARESDIDYOUWANTBLANK)
		completed=true;
}

void displayboard(sudoku square[9][9])
	//Displays whatever array is called in sudoku format.
	//It is called twice in main to display the puzzle, and then the solution.
{
	cout << " _____________ _____________ _____________ " << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[0][0].display << "| |" << square[0][1].display << "| |" << square[0][2].display << "| | |" << square[0][3].display << "| |" << square[0][4].display << "| |" << square[0][5].display << "| | |" << square[0][6].display << "| |" << square[0][7].display << "| |" << square[0][8].display << "| |" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[1][0].display << "| |" << square[1][1].display << "| |" << square[1][2].display << "| | |" << square[1][3].display << "| |" << square[1][4].display << "| |" << square[1][5].display << "| | |" << square[1][6].display << "| |" << square[1][7].display << "| |" << square[1][8].display << "| |" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[2][0].display << "| |" << square[2][1].display << "| |" << square[2][2].display << "| | |" << square[2][3].display << "| |" << square[2][4].display << "| |" << square[2][5].display << "| | |" << square[2][6].display << "| |" << square[2][7].display << "| |" << square[2][8].display << "| |" << endl;
	cout << "|_____________|_____________|_____________|" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[3][0].display << "| |" << square[3][1].display << "| |" << square[3][2].display << "| | |" << square[3][3].display << "| |" << square[3][4].display << "| |" << square[3][5].display << "| | |" << square[3][6].display << "| |" << square[3][7].display << "| |" << square[3][8].display << "| |" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[4][0].display << "| |" << square[4][1].display << "| |" << square[4][2].display << "| | |" << square[4][3].display << "| |" << square[4][4].display << "| |" << square[4][5].display << "| | |" << square[4][6].display << "| |" << square[4][7].display << "| |" << square[4][8].display << "| |" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[5][0].display << "| |" << square[5][1].display << "| |" << square[5][2].display << "| | |" << square[5][3].display << "| |" << square[5][4].display << "| |" << square[5][5].display << "| | |" << square[5][6].display << "| |" << square[5][7].display << "| |" << square[5][8].display << "| |" << endl;
	cout << "|_____________|_____________|_____________|" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[6][0].display << "| |" << square[6][1].display << "| |" << square[6][2].display << "| | |" << square[6][3].display << "| |" << square[6][4].display << "| |" << square[6][5].display << "| | |" << square[6][6].display << "| |" << square[6][7].display << "| |" << square[6][8].display << "| |" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[7][0].display << "| |" << square[7][1].display << "| |" << square[7][2].display << "| | |" << square[7][3].display << "| |" << square[7][4].display << "| |" << square[7][5].display << "| | |" << square[7][6].display << "| |" << square[7][7].display << "| |" << square[7][8].display << "| |" << endl;
	cout << "|  _   _   _  |  _   _   _  |  _   _   _  |" << endl;
	cout << "| |" << square[8][0].display << "| |" << square[8][1].display << "| |" << square[8][2].display << "| | |" << square[8][3].display << "| |" << square[8][4].display << "| |" << square[8][5].display << "| | |" << square[8][6].display << "| |" << square[8][7].display << "| |" << square[8][8].display << "| |" << endl;
	cout << "|_____________|_____________|_____________|" << endl;
}