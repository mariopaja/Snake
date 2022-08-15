

#include <iostream>
#include "receiver.h"


int main()
{
	displaySetup();
	while (true)
	{
		receiveGameData();
	}
	clearDisplay();
}

