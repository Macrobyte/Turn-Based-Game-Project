#include<iostream>

#include "MacroLib.h"


	enum menuOptions
	{
		Option1 = 1,
		Option2 = 2,
	};

	char userInput[10000] = { 0 };
	int newUserInput = 0;

	int ActionMenu()
	{
		EndLine();
		Log("Choose your action!", true);
		Log("1. Attack", true);
		Log("2. Special Attack", true);
		Log("3. Recharge", true);
		Log("4. Dodge", true);
		Log("5. Heal", true);

		std::cin >> userInput;

		return CheckInput(userInput);
	}

	void MainMenu()
	{

		Log("Welcome to my turn-based game project! Select an option to begin.", true);
		Log("1. Start Game", true);
		Log("2. Exit", true);

		std::cin >> userInput;

		newUserInput = CheckInput(userInput);

		switch (newUserInput)
		{
		case Option1:
			cls(' ');
			Game();
			break;
		case Option2:
			exit(0);
			break;
		default:
			InvalidOption();
			break;
		}

	}