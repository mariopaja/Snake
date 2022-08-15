#ifndef SNAKEDATA
#define SNAKEDATA
#include <chrono>
#include <vector>
#include <conio.h>
#include <iostream>


//declare display size
int displaySize;

//declare food position
int xFood, yFood;

//declare map
std::vector<std::vector<int>> map;

//declare chrono it will be used to check if 5 seconds is already passed
std::chrono::steady_clock::time_point startTime; bool newGame = true;

//declaring a struct snake that contains
//the location of the body x,y
//pointer address to the next body
//2 pointer global variables that can be accessed
struct snakeBody {
	int x;
	int y;
	snakeBody* next;
}*head, * tail;

//this function creates a new body element of the snake
//it takes as input the location of the body
void createSnakeBody(int x, int y)
{
	//create a struct element that allocates a snakebody
	////returns a pointer to the first snakebody address
	snakeBody* body = (snakeBody*)malloc(sizeof(snakeBody));

	//assign the body location x, y
	body->x = x;
	body->y = y;

	//if initial body part and there is no head yet
	//head is equal to tail and body
	if (!head) {
		head = tail = body;
	}
	else {
		// new body is added at the end of the tail
		tail->next = body;
		tail = body;
	}
	//last pointer of the tail becomes NULL
	tail->next = NULL;
}


//create map by filling the edges with 1 (#)
void createMap() {
	for (int i = 0; i < displaySize; i++) {
		for (int j = 0; j < displaySize; j++) {
			if (i == 0 || i == displaySize - 1 || j == 0 || j == displaySize - 1) map[i][j] = 1;
		}
	}
}

//ask User to set the Size of the game
//read from the console
//assign the size to the map
void setDisplaySize()
{
	printf("Please enter the game size: ");
	std::cin >> displaySize;
	map.resize(displaySize, std::vector<int>(displaySize, 0));
}


//creates food in a random position of the map
//if the position is not 0 it generates another position
//else declare that position as food
void createFood() {
	srand(time(NULL)); //allow  computer's internal clock to control the choice of the seed

	do {
		xFood = rand() % (displaySize - 1) + 1;
		yFood = rand() % (displaySize - 1) + 1;
	} while (map[yFood][xFood] != 0);
	map[yFood][xFood] = 3;
}

//it takes as input the direction of the snake ->up(0,-1) /left (-1,0) /right (1,0)/ down(0,1)
bool runGame(int x, int y)
{
	snakeBody* currentBody = head; //create a snakebody instance that is equal to snake head
	int tempX_1, tempY_1; //declare temp locations that will be used to migrate the location data
	int tempX_2, tempY_2;

	if (newGame) //check is it is a new game
	{

		int newX = tail->x - x; //create two instances that assign the new body part location at the end of the body
		int newY = tail->y - y;
		createSnakeBody(newX, newY);//creates the head and one body element
		createSnakeBody(newX, newY);//creates a second body element
		createFood(); //create the first food
		startTime = std::chrono::steady_clock::now(); //start the timer for the next food
		newGame = false; //it is not anymore a new game, all initial game components are already set
	}

	//check it it is already 5s since the timer started
	//if yes create a new food and reset the timer
	if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - startTime).count() == 5)
	{
		createFood();
		startTime = std::chrono::steady_clock::now();
	}

	//start a while loop from head
	bool foodEaten = false;
	while (currentBody) {

		//if the curren body is head
		if (currentBody == head) {

			//create a snake instance that is equal to two elements before head
			snakeBody* previousBody = head->next->next;


			//check if any of the previous body elements is overlapping with he head
			//if yes gameover
			while (previousBody) {
				if (previousBody->x == head->x && previousBody->y == head->y) {
					map[head->y][head->x] = 6;
					return false;
				}
				previousBody = previousBody->next;
			}

			//check if head is touching the border
			//if yes gameover
			if (head->x + x < 1 || head->x + x > displaySize - 2 || head->y + y < 1 || head->y + y > displaySize - 2) {
				return false;
			}

			//save the location of the head in the temporary instance
			tempX_1 = currentBody->x;
			tempY_1 = currentBody->y;

			//update the head location with the new instructions
			currentBody->x = currentBody->x + x;
			currentBody->y = currentBody->y + y;

			//print the head on the map


			//check the map value of the current body
			//if food add body else just update the map
			if (map[currentBody->y][currentBody->x] == 3) {
				foodEaten = true;
				int newX = tail->x - x;
				int newY = tail->y - y;
				createSnakeBody(newX, newY);
				createFood();
			}

			//update the map as a head element
			map[currentBody->y][currentBody->x] = 6;

		}
		else { //if the current body part is not the head


			//save the temporaty location data of the firs element into a second temporary location 
			tempX_2 = tempX_1;
			tempY_2 = tempY_1;

			//update the first temporary location data with the current body information
			tempX_1 = currentBody->x;
			tempY_1 = currentBody->y;

			//update the current body information with the body information from the previous one
			currentBody->x = tempX_2;
			currentBody->y = tempY_2;

			//update the map value as a tail element
			map[currentBody->y][currentBody->x] = 2;
		}

		//check the map value of the current body
		//if food add body else just update the map


		//if the current body is head asign it as 6 to make it easier for the printing process
		if (currentBody == head)map[currentBody->y][currentBody->x] = 6;

		//it will not change the last element to 0 if the snake had food
		//this makes the process smoother and the tail w/o interruptions
		if (currentBody->next == NULL && !foodEaten)map[tempY_1][tempX_1] = 0;
		//move to the next body part

		currentBody = currentBody->next;

	}
	//return true when the process has ended
	return true;
}


//print the map
//X only if there is a problem with the map
void printMap()
{
	for (int i = 0; i < displaySize; i++) {
		for (int j = 0; j < displaySize; j++) {
			if (map[i][j] == 1) printf("#");
			else if (map[i][j] == 3) printf("*");
			else if (map[i][j] == 2) printf("s");
			else if (map[i][j] == 6) printf("h");
			else if (map[i][j] == 0) printf(" ");
			else printf("X");
		}
		puts(""); //prints a newline
	}
}





void playSnake()
{

	int direction = 4; //declare the innitial direction
	bool nextMove = true; //declare that moving is allowed

	system("cls"); //clear the console

	while (nextMove) {

		//check if there is a keyboard hit
		//check if it is a direction
		//check if it is the right direction
		//print the selected direction
		//set the direction
		if (_kbhit()) {
			switch (_getch()) {
			case 'w':
				if (direction != 3)
				{
					puts("UP");
					direction = 1;
				}
				break;
			case 'a':
				if (direction != 4)
				{
					puts("LEFT");
					direction = 2;
				}
				break;
			case 's':
				if (direction != 1)
				{
					puts("DOWN");
					direction = 3;
				}
				break;
			case 'd':
				if (direction != 2)
				{
					puts("RIGHT");
					direction = 4;
				}
				break;
			}
		}

		//check the direction
		//nextMove calls runGame function and defines the input parameters of this function
		//nextMove or game over depends on runGame
		switch (direction) {
		case 1:
			nextMove = runGame(0, -1);
			break;
		case 2:
			nextMove = runGame(-1, 0);
			break;
		case 3:
			nextMove = runGame(0, 1);
			break;
		case 4:
			nextMove = runGame(1, 0);
			break;
		}

		//send the map to the display
		sendGameData(displaySize, map);

		//delay the next iteration by 0.5s
		Sleep(500);
	}

}
#endif