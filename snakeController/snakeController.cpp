#include "transmitter.h"
#include "snakeData.h"

using namespace std;



int main()
{

	controllerSetup(); //setsup the socket
	setDisplaySize(); //input the map size
	createMap(); //create map
	createSnakeBody(3, 1); //create snake body at this position
	playSnake(); //play game
	closeGame(); //close socket



}

