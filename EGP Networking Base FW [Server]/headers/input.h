#pragma once

//getch and kbhit
#include <conio.h>
#include "../headers/CommonInclude.h"


enum Commands
{
	ENTER = '\n',
	PUSH = 1,
	SHARE,
	COUPLE,
};

char getInput(std::string &s)
{
	if (_kbhit())
	{
		char c = _getch();
		if (c == 13)
		{
			if (s == "p")
			{
				printf("\n");
				return PUSH;
			}
			if (s == "c")
			{
				printf("\n");
				return COUPLE;
			}

			if (s == "s")
			{
				printf("\n");
				return SHARE;
			}
			printf("\n");
			return '\n';
		}
		else if (c == '\b') //backspace
		{
			if (s.length()) //check for content to backspace
			{
				s.pop_back(); //remove last char from buffer
				printf("\b \b"); //change display on screen
			}
		}
		else if (c != NULL)
		{
			s += c;
			printf("%c", c);
		}
		return c;
	}
	return 0;
}
