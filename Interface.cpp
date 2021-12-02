#include<iostream>
#include<Windows.h>
#include<stdio.h>
#include<time.h>
#include<chrono>
#include<thread>

#include"MacroLib.h"

	void Log(const char* message, bool endLine)
	{
		if (endLine == true)
		{
			std::cout << message << std::endl;

		}
		else
		{
			std::cout << message;
		}
	}

	void EndLine()
	{
		std::cout << std::endl;
	}

	int CheckInput(char input[])//Turns char input to int and makes sure it's not longer than one digit in the decimal system
	{
		int i, output;
		output = 0;

		for (i = 0; isdigit(input[i]); ++i)
		{
			output = 10 * output + (input[i] - '0');
		}

		return output;
	}

	void InvalidOption()
	{
		cls(' ');
		SetTextColor(Red);
		Log("Select a valid option!", true);
		SetTextColor(White);

	}

	void LoadingDots()
	{
		Sleep(1);
		Log(".", false);
		Sleep(1);
		Log(".", false);
		Sleep(1);
		Log(".", false);
		Sleep(1);

	}

	void cls(char fill = ' ') //Clears the screen
	{
		COORD tl = { 0,0 };
		CONSOLE_SCREEN_BUFFER_INFO s;
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(console, &s);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;
		FillConsoleOutputCharacter(console, fill, cells, tl, &written);
		FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
		SetConsoleCursorPosition(console, tl);
	}

	void SetTextColor(int color) //Set text color function
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	}

	void Sleep(int time)
	{
		std::this_thread::sleep_for(std::chrono::seconds(time)); //Pause thread for X seconds.
	}