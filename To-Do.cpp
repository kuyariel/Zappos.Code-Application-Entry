/***************************************************************
Roman Gabriele Ocampo
5.31.2012
To-Do List

This program is designed to use filestream variables to create, edit, and save
to-do list files. The program begins by prompting the user to create a new file,
or to open an existing one. When the user creates a new file, the program asks for
a filename, the number of items on the to-do list, and then it asks the user
to input every item. A file will be created in the same directory as the program
titled as whatever the user inputted for filename.

When the user decides to open an existing file, the name of the file is prompted.
The program will then locate the file and display its contents to the screen. If
there is no file with that name, a new blank list with that filename will be created.
From this point, the user can decide to add another item, delete an item, edit a particular
item, or sort the items by switching around their positions. Any changes made to the list
will be saved after the user decides to quit by typing in 'Q' at the prompt.
****************************************************************/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAXLIST=64; //Midify this if you want to fit more items in your lists

int prompt(char&);
void createnewlist(string[],int&);
void writethefile(string[],string,int);
void readoldlist(string[],string,int&);
void displaytoscreen(string[],int);
int additem(string[],int&);
int deleteitem(string[],int&);
int edititem(string[],int);
int sortitem(string[],int);

int main()
{
	char quit=' ';
	cout << "To-Do List" << endl << endl;
	while (quit!='Q'&&quit!='q')
	{
	prompt(quit);
	}
	return 0;
}

int prompt(char& quit)
{
	char response=' '; //Reads in the first set of responses, either N, O, or Q
	char response2=' '; //Reads the second set of responses, either A, D, E, S, or Q
	string whitespace; //Used to see in whitespace that needs to be ignored
	string filename; //The name of the file that is being created or edited
	string todolist[MAXLIST]; //Array of the actual to-do list
	int items=0; //Number of items currently in the list being worked on.

	cout << "Would you like to create a (N)ew list? Or would you like to (O)pen a saved one?" << endl;
	cout << "Or did you want to (Q)uit?" << endl;
	while (response!='N'&&response!='n'&&response!='O'&&response!='o')
	{
		cout << "Enter N, O, or Q:" << endl;
		cin >> response;
		getline(cin,whitespace);
		if (response=='Q'||response=='q')
		{
			quit='Q';
			return 0;
		}
		if (response=='N'||response=='n')
		{
			cout << "Enter name of new to-do list file" << endl;
			cin >> filename;
			createnewlist(todolist,items);
			writethefile(todolist,filename,items);
			cout << "File has been created at '" << filename << "'." << endl;
			cout << "Type in O at the prompt and enter the filename to open for reading or editing." << endl << endl;
		}
		else
		{
			if (response=='O'||response=='o')
			{
				cout << "Enter name of existing to-do list file." << endl;
				cout << "NOTICE: If a file doesn't exist at your filename, a blank one will be created." << endl;
				cout << "However, you are better off typing N at the prompt and creating it from there." << endl;
				cin >> filename;
				readoldlist(todolist,filename,items);
				while (response2!='Q'&&response2!='q')
				{
					displaytoscreen(todolist,items);
					cout << endl << "What would you like to do now?" << endl;
					cout << "(A)dd another item" << endl;
					cout << "(D)elete an item" << endl;
					cout << "(E)dit an item" << endl;
					cout << "(S)ort the items" << endl;
					cout << "(Q)uit to main prompt" << endl;
					cin >> response2;
					getline(cin,whitespace);
					if (response2=='A'||response2=='a')
						additem(todolist,items);
					else
					{
						if (response2=='D'||response2=='d')
							deleteitem(todolist,items);
						else
						{
							if (response2=='E'||response2=='e')
								edititem(todolist,items);
							else
							{
								if (response2=='S'||response2=='s')
									sortitem(todolist,items);
								else
								{
									if (response2=='Q'||response2=='q')
									{
										writethefile(todolist,filename,items);
										cout << "Changes have been saved to '" << filename << "'" << endl << endl;
									}
									else
										cout << "Please enter a valid character." << endl;
								}
							}
						}
					}
				}
			}
			else
				cout << "Please enter a valid character" << endl;
		}
	}
}

void createnewlist (string todolist[MAXLIST],int& items)
	//This function creates a new list. The number of items on the list are prompted for,
	//and then the items are read in.
{
	string whitespace;
	while (items<1||items>MAXLIST)
	{
		cout << "Please enter how many items are on your to-do list (Max " << MAXLIST << "):" << endl;
		cin >> items;
		if (items<1)
			cout << "You need to have at least 1 item to continue" << endl;
		if (items>MAXLIST)
		{
			cout << "You cannot have more than " << MAXLIST << " items in this list." << endl;
			cout << "If you would like more, change the MAXLIST constant in the source code." << endl;
		}
	}
	getline(cin,whitespace); //This gets rid of the '/n' after entering the items.
	for (int i=0;i<items;i++)
	{
		cout << "Enter item #" << i+1 << ":" << endl;
		getline(cin,todolist[i]);
	}
}

void writethefile (string todolist[MAXLIST], string filename,int items)
	//Given the current string array todolist, the filename, and the number of items
	//on the list, this function saves the array to filename.
{
	ofstream out;
	out.open(filename.c_str());
	for (int i=0;i<items;i++)
	{
		out << i+1 << ". " << todolist[i] << endl;
	}
	out.close();
}

void readoldlist(string todolist[MAXLIST], string filename, int& items)
	//Given the filename of an existing to-do list, this function reads its contents
	//and stores it in the todolist array.
{
	ifstream in;
	int number; //Number before each of the to-do list entry. Ignored.
	char ignore; //The period and whitespace after the number. Ignored.
	in.open(filename.c_str());
	while (in)
	{
		in >> number;
		in >> ignore;
		ignore = in.get();
		getline(in,todolist[items]);
		items++;
	}
	items--;
}

void displaytoscreen (string todolist[MAXLIST], int items)
	//Displays the contents in the todolist array to the screen in a to-do list fashion.
{
	cout << endl << "To-Do List:" << endl;
	for (int i=0;i<items;i++)
	{
		cout << i+1 << ". " << todolist[i] << endl;
	}
}

int additem (string todolist[],int& items)
	//Adds a string to the end of the todolist array and has the user fill it in.
{
	string whitespace;
	char option=' ';
	while (1)
	{
		option=' ';
		cout << "Enter an item to add to your to-do list:" << endl;
		getline(cin,todolist[items]);
		items++;
		while (option!='Y'&&option!='y'&&option!='N'&&option!='n')
		{
			cout << "Would you like to add another item? (Y/N)" << endl;
			cin >> option;
			getline(cin,whitespace);
			if (option=='Y'||option=='y')
			{}
			else
			{
				if (option=='N'||option=='n')
					return 0;
				else
					cout << "Please input a valid character" << endl;
			}
		}
	}
}

int deleteitem(string todolist[],int& items)
	//This function prompts the user for which item they want deleted.
	//Every item past the identified string is moved one space up in the array,
	//which overwrites the string which the user wanted to be deleted.
{
	string whitespace;
	int itemtodelete;
	cout << "Which item did you want deleted? (1-" << items << ")" << endl;
	cin >> itemtodelete;
	getline(cin,whitespace);
	for (int i=(itemtodelete-1); i<items-1; i++)
	{
		todolist[i]=todolist[i+1];
	}
	items--;
	return 0;
}

int edititem(string todolist[],int items)
	//This function replaces the contents of one item in the array
	//with another string.
{
	string whitespace;
	int itemtoedit;
	cout << "Which item did you want to edit? (1-" << items << ")" << endl;
	cin >> itemtoedit;
	getline(cin,whitespace);
	cout << "Enter what you want to replace it with:" << endl;
	getline(cin,todolist[itemtoedit-1]);
	return 0;
}

int sortitem(string todolist[],int items)
	//This function prompts the user to enter two numbers.
	//The strings in these two positions in the todolist array have their positions swapped.
{
	string temp;
	int firststring;
	int secondstring;
	cout << "Select which two items to switch." << endl;
	cout << "Enter two integers (1-" << items << ") separated by a space." << endl;
	cin >> firststring;
	cin >> secondstring;
	temp=todolist[firststring-1];
	todolist[firststring-1]=todolist[secondstring-1];
	todolist[secondstring-1]=temp;
	cout << "Items " << firststring << " and " << secondstring << " have swapped positions." << endl;
	return 0;
}