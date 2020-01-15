#include <iostream>
#include <conio.h> 

using namespace std;
bool gameOver;//boolean variable that stops the game from ending until it is true.
int x, y, fruitPosX, fruitPosY, score;//x and y = position coordinates, fruitPosX and fruitPosY = fruit coordinates, the score is the score
const int width = 20;//parameters of the game border
const int height = 17;
int wall = 219;//int values of Extended Ascii characters
int fruit = 184;
int tailX[100], tailY[100]; //Arrays to store the snakes tail segments
int tailLength;//Length of tail
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN }; //The users controls
eDirecton dir;

int getInput()//simple function that gets a users input and stores it in a variable. Used for the menus
{
	int input;
	cin >> input;
	return input;
}

void mainMenu()//Main menu for the game, this is the first thing that is displayed to the user.
{
	cout << "Main Menu\n";
	cout << "1 - Start game\n";
	cout << "2 - Help Options\n";
	cout << "3 - Quit\n";
	cout << "Please enter the number associated\n with the menu option to continue.";
}

void optionsMenu()//Options menu, displays the options to the user
{
	cout << "Options Menu\n";
	cout << "1 - How to Play\n";
	cout << "2 - Controls\n";
	cout << "3 - Game info and help options\n";
	cout << "4 - Credits\n";
	cout << "5 - Back\n";
}

void optionsOutput()//This function contains all the outputs for the options menu.
{
	int input = 0;//Variable made for the switch.
	do
	{
		optionsMenu();
		input = getInput();//Changes the contents of the input to whatever option the user inputs.
		switch (input)//The switch displays an output that is equal to the users input that is now stored in choice.
		{
		case 1:
			cout << "You are a snake and you are very hungry." << endl << 
				"Your goal is to collect cubes of food whilst not running into your own tail." << endl <<
				"Your tail grows indefinitely longer every time you eat a fruit." << endl <<
				"You can pass through the border walls, taking you to the oposite side of the border\n";
			break;
		case 2:
			cout << "Left = a" << endl <<
				"Right = d" << endl <<
				"Up = w" << endl <<
				"Down = s\n";
			break;
		case 3:
			cout << "Game info and help options\n";
			cout << "Version number:1.0\n";
			cout << "Programmed in Visual Studio 2019 Community Edition using the language c++\n";
			cout << "Please refer to the game developer for help. Phone number - 07920 063677\n";
			cout << "Url to my source code is : \n https://github.com/Jazzmandu/JaysCode/upload/master   \n and is named main.cpp\n";
            cout << "Error codes that could return:\n Once you have played the game once, you need to exit the program to play again\n";
			break;
		case 4:
			cout << "Game Developer:" << endl << "Jay Howard" << endl << "Advisors:" << endl << "Andy Cuffe" << endl << "William Barnes\n";
			break;
		case 5:
			break;
		default:
			break;
		}
	} while (input != 5);/*The switch is inside a dowhile loop,
						  which keeps on displaying the options menu until case 5 is chosen.
						  After case 5 is chosen, the loop is exited and you are now back in
						  the mainmenu dowhile loop*/
}

void Setup() {
	gameOver = false;
	dir = STOP;//This means that the snake is not moving when you enter the game.
	x = width / 2;//Positions the snake at the start of the game to be in the middle of the border.
	y = height / 2;
	fruitPosX = rand() % width; //Places the fruit in a random location at the start of the game.
	fruitPosY = rand() % height; 
	score = 0;
}
void border() {
	system("cls");/*Clears the screen. Without this in the gameloop the game would have a new game 
				  border for each move the snake makes*/
	for (int i = 0; i < width + 2; i++)
		cout << (char)wall;//This prints out the topside of the border using a forloop.
	cout << endl;
	for (int i = 0; i < height; i++) {//sets the parameters for where the chars can be printed
		for (int j = 0; j < width; j++) {
			if (j == 0)
				cout << (char)wall; //prints out the left wall 
			if (i == y && j == x)
				cout << "O"; //This prints out the snakes head. Variables x and y always correspond to the snake heads position
			else if (i == fruitPosY && j == fruitPosX)
				cout << (char)fruit; //This elif prints out the fruit.
			else {
				bool print = false;//I used a boolean variable so that the snakes tail doesn't print until it is true
				for (int k = 0; k < tailLength; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						cout << (char)254; print = true;//Prints out all the tail segments stored in the tail arrays
					}
				}
				if (!print) cout << " ";//If the coordinates dont match any of the other elifs, a space is printed
			}
			if (j == width - 1)//This prints out the right side border
				cout << (char)wall;
		}
		cout << endl;
	}
	for (int i = 0; i < width + 2; i++)//This prints the bottom side border
		cout << (char)wall;

	cout << endl;
	cout << "Score:" << score << endl;//Prints out the users score below the border
}
void userInput()//This function uses a switch to bind the keys to a direction.
{
	if (_kbhit()) {
		switch (_getch()) {//the _getch function is used to get the users character key press.
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 'w':
			dir = UP;
			break;
		case 's':
			dir = DOWN;
			break;
		}
	}
}
void snakeLogic()/*This function creates the logic needed to create the snakes tail, gives the enum variables
				 there directions, allows the snakes to go through borders (and end up on the opposite side), 
				 ends the game when you run into the tail and the logic of what happens when you eat a fruit
				 (increased score, another fruit and increased tail)*/
{
	int prev2X, prev2Y;
	int prevX = tailX[0];
	int prevY = tailY[0];
	tailX[0] = x;//Variables prevX and prevY are made to equal value[0] of their respective array, and then that array is made to equal the respective x or y coord.
	tailY[0] = y;//What happens is that prevX+Y variables store the previous value in the array, and then the current position of the snake is store where we just took the previous position value from. 
	for (int i = 1; i < tailLength; i++) {//i is equal to 1 so that we dont erase the position value we just got.
		prev2X = tailX[i];//Two more variables are needed to store the snake segments values
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch (dir) {//This switch function makes it so that the directional words that we linked to a keyboard press now go in said direction
	case LEFT:
		x--;//Changing the x coord moves the snake because the x variable is linked directly to the snake heads position 
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default:
		break;
	}
	if (x >= width) x = 0; else if (x < 0) x = width - 1;/*This code allows for the snake to go into the left side of the border
														 and then come out on the right side and vicaversa. This is possible 
														 because the snakes head position is tied to its x and y axis, so you can just
														 change this value to 'teleport' the snake to the other side*/
	if (y >= height) y = 0; else if (y < 0) y = height - 1;//Same concept as the previous line of code, just in the y axis.
	for (int i = 0; i < tailLength; i++)
		if (tailX[i] == x && tailY[i] == y)/*if the snakes tail coordinates match the snakes head coordinates,
										   then the gameover variable is changed to true and the game is ended
										   , taking you back to the dowhile main menu loop*/
			gameOver = true;
	if (x == fruitPosX && y == fruitPosY) {//If the snakes head and the fruits corrdinate match, the score is incresed and another fruit is spawned into the map
		score += 1;
		fruitPosX = rand() % width;
		fruitPosY = rand() % height;
		tailLength++;//The tail is also increased
	}
}
int main()
{
	int input = 0;
	do//Another dowhile loop is used to manage the main menu 
	{
		mainMenu();
		input = getInput();
		switch (input) {
		case 1://This is the main game loop
			system("cls");
			Setup();
			while (!gameOver) {//while the game is not finished, the gameOver will stay as false and the game will continue. This is the game loop
				border();
				userInput();
				snakeLogic();
			}
			break;
		case 2:
			optionsOutput();//This is the options menu
			break;
		case 3:
			cout << "I hope you enjoyed the game!";//This is the exit option
			break;

		default:
			break;
		}
	} while (input != 3);
	system("PAUSE");//Cheatsy way of stopping the program from doing background tasks.
	return EXIT_SUCCESS;//Command to exit the program
};