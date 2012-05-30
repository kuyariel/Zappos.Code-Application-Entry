/***********************************************************************************
Roman Gabriele Ocampo
5.1.2012
Ninja Book Sorter

     This program is designed to read in a list of books and output them in alphabetical
order sideways as if they were in a book shelf. First, the program prompts the user
for the number of books in their bookshelf. Currently, it reads a maximum of 64 books,
but this can be changed by modifying the MAXBOOKS constant below. Then, the user enters
the titles of their books in any desired order. The program reads these strings and
fixes them so each word in the title is capitalized with the rest of the letters
lowercased. The program them sorts them in alphabetical order and displays them sideways
as if they were in a bookshelf.

***********************************************************************************/

#include <iostream>;
#include <string>;
#include <iomanip>;
#include <cctype>;
using namespace std;

const int MAXBOOKS=64; //Modify this if you want the possibility to fit more books.

void blankoutlist(string[],int);
void displayshelf(string[],int,int);
void fillinlist(string[],int,int&);
void fixstring(string&,int);
void alphasort(string[],int);
void switchstring(string&,string&);
void centerstrings(string[],int,int);

int main()
{
	string booklist[MAXBOOKS];
	int bookcount=0; //The number of books inputted by the user
	char quit; //This is used to exit the program
	string whitespace; //This reads the /n after the user enters the first integer **See below**
	int lengthoflongestword=0; //As the name states, it is the length of the longest word. Used for display purposes

	cout << "Ninja Book List" << endl << endl;
	cout << "A ninja worth his salt would have many books stored in his bookshelf. This bookshelf of course" << endl;
	cout << "should be completely invisible. Sometimes, even the best of us forget which order we had our books" << endl;
	cout << "in, and this is where this program comes to help. If you had forgotten how your books stack up, enter" << endl;
	cout << "their titles below and they will be displayed in alphabetical order as if they were in your shelf at home!" << endl;
	cout << "If your bookshelf was not in alphabetical order the last time you checked, then this program won't do you" << endl;
	cout << "much good and you have brought dishonor to you and your family. You should be ashamed." << endl;
	cout << endl;
	while (bookcount<1||bookcount>MAXBOOKS)
	{
		cout << "How many books are in your bookshelf? (Max " << MAXBOOKS << ")" << endl;
		cin >> bookcount;
		if (bookcount<1)
			cout << "You need to have at least 1 book to continue" << endl;
		if (bookcount>MAXBOOKS)
		{
			cout << "You cannot have more than " << MAXBOOKS << " in this bookshelf." << endl;
			cout << "If you would like more, change the MAXBOOKS constant in the source code." << endl;
		}
	}
	getline(cin,whitespace); //This gets rid of the '/n' after entering the bookcount.
	blankoutlist(booklist,bookcount);
	fillinlist(booklist,bookcount,lengthoflongestword);
	alphasort(booklist,bookcount);
	centerstrings(booklist,bookcount,lengthoflongestword);
	displayshelf(booklist,bookcount,lengthoflongestword);
	cout << "This is what your bookcase should look like." << endl << "Input anything to quit." <<endl;
	cin >> quit;
	return 0;
}

void blankoutlist(string booklist[MAXBOOKS],int n)
	//This function removes any previous data stored in booklist and replaces it with a blank string.
{
	for (int i=0;i<n;i++)
		booklist[i]="";
}

void displayshelf(string booklist[MAXBOOKS],int n,int size)
	//This function is responsible for displaying the bookshelf with the books sideways.
{
	int widthofshelf=(2*n)+1;
	int heightofshelf=size+2;
	for (int i=0;i<widthofshelf;i++) //Loop for the upper boarder of the shelf
		cout << "-";
	cout << endl;
	for (int i=0;i<size;i++)
	{
		for (int j=0;j<n;j++)
		{
			if (n==1) //Makes sure that it displays correctly if only 1 book is entered.
				cout << "|" << booklist[j][i] << "|" << endl;
			else
			{
				if (j==0)
					cout << "|" << booklist[j][i] << " ";
				else
				{
					if (j==n-1)
						cout << booklist[j][i] << "|" << endl;
					else
						cout << booklist[j][i] << " ";
				}
			}
		}
	}
	for (int i=0;i<widthofshelf;i++) //Loop for the bottom boarder of the shelf
		cout << "-";
	cout << endl;
}

void fillinlist(string booklist[MAXBOOKS],int n, int& lengthoflongestword)
	//This function reads in the title of each book. It also identifies the length of the longest string
	//and calls for the fixstring function.
{
	int currsize; //current length of string
	string temp;
	for (int i=0;i<n;i++)
	{
		cout << "Enter title of book #" << i+1 << ":" << endl;
		getline(cin,temp);
		currsize=temp.length();
		if (currsize>lengthoflongestword)
			lengthoflongestword=currsize;
		fixstring(temp,currsize);
		booklist[i]=temp;
	}
}

void fixstring(string& title,int size)
	//Makes sure the first letter of every word is capitalized, and all other letters are lowercase.
{
	bool capitalizenext=true;
	for (int i=0;i<size;i++)
	{
		if (capitalizenext)
		{
			title[i]=toupper(title[i]);
			capitalizenext=false;
		}
		else
		{
			if (title[i]==' ')
				capitalizenext=true;
			else
				title[i]=tolower(title[i]);
		}
	}
}

void alphasort(string booklist[MAXBOOKS], int n)
	//This function sorts the array in alphabetical order by comparing each string.
	//It calls the switchstring function if the two strings' positions need to be swapped.
{
	for (int i=0;i<n-1;i++)
	{
		for (int j=0;j<n-1;j++)
		{
			if (booklist[j]>booklist[j+1])
				switchstring(booklist[j],booklist[j+1]);
		}
	}
}

void switchstring(string& string1, string& string2)
	//This function swaps the position of two strings.
{
	string temp;
	temp=string1;
	string1=string2;
	string2=temp;
}

void centerstrings(string booklist[MAXBOOKS],int n, int size)
	//This function uses the length of the longest string to center the other strings for proper
	//display. It takes the difference between each string's length and the length of the longest
	//string and uses it as whitespace before and after the shorter strings to create a centered
	//look for the final display.
{
	string temp;
	double whitespaceneeded=0;
	int numberofleftspaces=0;
	int numberofrightspaces=0;
	for (int i=0;i<n;i++)
	{
		whitespaceneeded=size-booklist[i].length();
		whitespaceneeded=whitespaceneeded/2;
		numberofleftspaces=floor(whitespaceneeded);
		numberofrightspaces=ceil(whitespaceneeded);
		temp=booklist[i];
		booklist[i]="";
		for (int j=1;j<=numberofleftspaces;j++)
			booklist[i]=booklist[i]+" ";
		booklist[i]=booklist[i]+temp;
		for (int j=1;j<=numberofrightspaces;j++)
			booklist[i]=booklist[i]+" ";
	}
}