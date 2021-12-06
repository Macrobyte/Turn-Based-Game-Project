#include "MacroLibV1.h"


	enum menuOptions
	{
		Option1 = 1,
		Option2 = 2,
	};

	char userInput[10000] = { 0 };
	int newUserInput = 0;
	
	bool choiceMade = false;

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
		while (choiceMade == false)
		{
			Log("Welcome to my turn-based game project! Select an option to begin.", true);
			Log("1. Start Game", true);
			Log("2. Exit", true);

			std::cin >> userInput;
			newUserInput = CheckInput(userInput);

			switch (newUserInput)
			{
			case Option1:
				choiceMade == true; 
				cls(' ');
				Game();
				break;
			case Option2:
				exit(0);
				break;
			default:
				InvalidOption();
				choiceMade == false;
				break;
			}
		}

		

	}

	void GameOverMenu()
	{
		while (choiceMade == false)
		{
			Log("=========================", true);
			Log("        GAME OVER        ", true);
			Log("    Choose an option!    ", true);
			Log("        1. Exit          ", true);
			Log("        2. Play Again!   ", true);
			Log("=========================", true);

			std::cin >> userInput;
			newUserInput = CheckInput(userInput);

			switch (newUserInput)
			{
			case Option1:
				exit(0);
				break;
			case Option2:
				cls(' ');
				Game();
				break;
			default:
				InvalidOption();
				break;
			}
		}
	}