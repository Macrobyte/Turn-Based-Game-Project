﻿#include<iostream>
#include<Windows.h>
#include<stdio.h>
#include<cstring> 
#include<time.h>
#include<chrono>
#include<thread>


using namespace std;
void Game();

class Interface
{
public:

	enum Colors
	{
		White = 7,
		Red = 12,
		Blue = 9,
		Yellow = 14,
		Green = 10
	};

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

	int CheckInput(char input[])
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
		cls();
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

	void cls(char fill = ' ') //Clear screen function
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
		this_thread::sleep_for(chrono::seconds(time)); //Pause thread for X seconds.
	}
};

struct Creature
{
	Interface UI;

	const float maxHealth = 100;
	const int maxEnergy = 50;

	const int maxAttackDamage = 10;
	const int minAttackDamage = 1;
	const int maxSpecialDamage = 20;
	const int minSpecialDamage = 5;

	const int specialAttackCost = 50;

	const float attackChance = 80.0f;
	const float specialAttackChance = 50.0f;
	const float rechargeHitChanceDecrease = 10.0f;
	const float dodgeHitChance = 30.0f;

	const int defaultChargeRate = 2;
	const int rechargeChargeRate = 4;
	const int dodgeChargeRate = defaultChargeRate / 2;

	float health = 0;
	int energy = 0;
	int damage = 0;
	
	float hitChance = 0; 
	int energyRechargeRate = 0;

	bool endTurn = false;
	int moveCounter = 0;

	bool hasHealed = false;
	int ammountToHeal = 0;
	const int minEnergy = 0;

	bool hasAttacked = false;
	bool hasSpecialAttacked = false;
	bool hasRecharged = false;
	bool hasDodged = false;

	void Setup()
	{
		health = maxHealth;
		energy = maxEnergy;
		energyRechargeRate = defaultChargeRate;
	}

	void Attack()
	{
		hitChance += attackChance;
		damage = (rand() % (maxAttackDamage - minAttackDamage + 1)) + minAttackDamage; 	

		endTurn = true;
		hasAttacked = true;
	}

	void SpecialAttack()
	{
		if (energy < specialAttackCost)
		{
			UI.Log("Not Enough Energy!",true); 
		}
		else
		{
			hitChance += specialAttackChance;
			damage = (rand() % (maxSpecialDamage - minSpecialDamage + 1)) + minSpecialDamage; 
			energy -= specialAttackCost;

			endTurn = true;
			hasSpecialAttacked = true;
		}
		
	}

	void Recharge(Creature oponent, float* newHitChance)
	{
		energyRechargeRate *= rechargeChargeRate;
		oponent.hitChance -= rechargeHitChanceDecrease;
		*newHitChance = oponent.hitChance;

		endTurn = true;
		hasRecharged = true;
	}

	void Dodge(Creature oponent, float* newHitChance)
	{
		oponent.hitChance -= dodgeHitChance;
		*newHitChance = oponent.hitChance;

		energyRechargeRate = dodgeChargeRate;

		endTurn = true;
		hasDodged = true;
	}

	void Heal()
	{
		if (hasHealed == true)
		{
			UI.Log("Already Healed this turn!", true);
			Sleep(1);
		}
		else
		{
			ammountToHeal = (rand() % ((energy/2) - minEnergy + 1)) + minEnergy;
			health += ammountToHeal;

			if (health > maxHealth)
			{
				health = maxHealth;
				hasHealed = true;
			}
			else
			{
				//health += ammountToHeal;
				energy -= ammountToHeal;

				moveCounter++; 
				hasHealed = true;
			}
		}
	}

	void EnergyCharge()
	{
		energy += energyRechargeRate;
		//float energyIncrease = energy - energyRechargeRate;

		if (energy > maxEnergy)
		{
			energy = maxEnergy;
		}

	}

	void ResetStats()
	{
		hitChance = 0;
		energyRechargeRate = defaultChargeRate;
		damage = 0;
		endTurn = false;
		hasHealed = false;
		moveCounter = 0;
		hasAttacked = false;
		hasSpecialAttacked = false;
		hasRecharged = false;
		hasDodged = false;
	}
};

void HitCheck(Creature playerCreature, Creature computerCreature, float* playerHealth, float* computerHealth)
{
	Interface UI;

	float chanceToHit = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //Assign "n" a random float value between 0.0 and 1.0.
	chanceToHit = chanceToHit * 100; //Get "n" into a range from 0 to 100.

	if (playerCreature.hasAttacked || playerCreature.hasSpecialAttacked)
	{
		//Player Hit Check
		if (chanceToHit < playerCreature.hitChance)
		{
			UI.SetTextColor(UI.Green);
			UI.Log("Player hit enemy!", true);
			UI.SetTextColor(UI.White);

			computerCreature.health -= playerCreature.damage;
			chanceToHit = 0; //Resets "n" so that the next calculation can be done the same way as the first one.
			*computerHealth = computerCreature.health;
		}
		else
		{
			UI.SetTextColor(UI.Yellow);
			UI.Log("Player Missed!", true);
			UI.SetTextColor(UI.White);
			chanceToHit = 0;
		}
	}
	

	cout << endl;
	Sleep(1);

	if (computerCreature.hasAttacked || computerCreature.hasSpecialAttacked)
	{
		//Enemy Hit Check
		chanceToHit += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		chanceToHit = chanceToHit * 100;

		if (chanceToHit < computerCreature.hitChance)
		{
			UI.SetTextColor(UI.Red);
			UI.Log("Enemy hit enemy!", true);
			UI.SetTextColor(UI.White);

			playerCreature.health -= computerCreature.damage;
			chanceToHit = 0;
			*playerHealth = playerCreature.health;
		}
		else
		{
			UI.SetTextColor(UI.Green);
			UI.Log("Enemy Missed!", true);
			UI.SetTextColor(UI.White);
			chanceToHit = 0;
		}
	}
	

	Sleep(1);
}

void GameHeader(Creature playerCreature, Creature computerCreature, int roundCounter)
{
	Interface UI;

	UI.Log("==========================================", true); 

	//Player Info
	UI.Log("Your Health: ",false);
	UI.SetTextColor(UI.Red);
	cout << playerCreature.health << "/100";
	UI.SetTextColor(UI.White);

	UI.Log(" Your Energy: ", false);
	UI.SetTextColor(UI.Blue);
	cout << playerCreature.energy << "/50";;
	UI.SetTextColor(UI.White);
	cout << endl;
	cout << endl;

	//Enemy Info
	UI.Log("Enemy Health: ", false);
	UI.SetTextColor(UI.Red);
	cout << computerCreature.health << "/100";
	UI.SetTextColor(UI.White);
	cout << endl;

	//Turn Info
	UI.Log("Round: ", false);
	UI.SetTextColor(UI.Yellow);
	cout << roundCounter;
	UI.SetTextColor(UI.White);
	cout << endl;

	UI.Log("==========================================", true);
}

struct Menus
{
	enum menuOptions
	{
		Option1 = 1,
		Option2 = 2,
	};

	char userInput[10000] = { 0 };
	int newUserInput = 0;

	Interface UI; 
	
	int ActionMenu()
	{
		UI.EndLine();
		UI.Log("Choose your action!", true);
		UI.Log("1. Attack", true);
		UI.Log("2. Special Attack", true);
		UI.Log("3. Recharge", true);
		UI.Log("4. Dodge", true);
		UI.Log("5. Heal", true);

		cin >> userInput;

		return UI.CheckInput(userInput);
	}

	void MainMenu()
	{
		
		UI.Log("Welcome to my turn-based game project! Select an option to begin.", true);
		UI.Log("1. Start Game", true);
		UI.Log("2. Exit", true);

		cin >> userInput;

		newUserInput = UI.CheckInput(userInput);

		switch (newUserInput)
		{
		case Option1:
			UI.cls();
			Game();
			break;
		case Option2:
			exit(0);
			break;
		default:
			UI.InvalidOption();
			break;
		}

	}
};

void Game()
{
	enum Moves
	{
		Attack = 1,
		SpecialAttack = 2,
		Recharge = 3,
		Dodge = 4, 
		Heal = 5 
	};

	Interface UI;
	Menus menus;
	Creature playerCreature;
	Creature computerCreature;


	const int maxMoves = 2;
	int roundCounter = 1; //Game starts at round 1.

	int computerChoice = 0;
	const int maxComputerChoice = 5;
	const int minComputerChoice = 1;
	

	playerCreature.Setup();
	computerCreature.Setup();

	//Round Loop
	while (playerCreature.health > 0 && computerCreature.health > 0)
	{
		
		GameHeader(playerCreature, computerCreature, roundCounter);
		
		//Player Actions
		while (playerCreature.endTurn == false && playerCreature.moveCounter < maxMoves)
		{
			switch (menus.ActionMenu())
			{

			case Attack:
				cout << "Player used Attack!" << endl;
				playerCreature.Attack();
				break;
			case SpecialAttack:
				cout << "Player used Special Attack!" << endl;
				playerCreature.SpecialAttack();
				break;
			case Recharge:
				cout << "Player used Recharge!" << endl;
				playerCreature.Recharge(computerCreature, &computerCreature.hitChance);
				break;
			case Dodge:
				cout << "Player used Dodge!" << endl;
				playerCreature.Dodge(computerCreature, &computerCreature.hitChance);
				break;
			case Heal:
				cout << "Player used Heal!" << endl;
				playerCreature.Heal();
				break;
			default:
				UI.InvalidOption();
				GameHeader(playerCreature, computerCreature, roundCounter);
				break;
			}
		}
			
		cout << endl;
		UI.Sleep(1);  

		//Computer Actions
		while (computerCreature.endTurn == false && computerCreature.moveCounter < maxMoves)
		{
			
			computerChoice = (rand() % (maxComputerChoice - minComputerChoice + 1)) + minComputerChoice;

			switch (computerChoice)
			{

			case Attack:
				cout << "Enemy used Attack!" << endl;
				computerCreature.Attack();
				break;
			case SpecialAttack:
				cout << "Enemy used Special Attack!" << endl;
				computerCreature.SpecialAttack();
				break;
			case Recharge:
				cout << "Enemy used Recharge!" << endl;
				computerCreature.Recharge(playerCreature, &playerCreature.hitChance);
				break;
			case Dodge:
				cout << "Enemy used Dodge!" << endl;
				computerCreature.Dodge(playerCreature, &playerCreature.hitChance);
				break;
			case Heal:
				cout << "Enemy used Heal!" << endl;
				computerCreature.Heal();
				break;
			default:
				GameHeader(playerCreature, computerCreature,roundCounter);
				UI.InvalidOption();
				break;
			}
		}
		
		cout << endl;
		UI.Sleep(1);

		//Checks if any creature hit each other.
		HitCheck(playerCreature, computerCreature, &playerCreature.health, &computerCreature.health);
			
		//Energy is charged at the end of each round.
		playerCreature.EnergyCharge();
		computerCreature.EnergyCharge();

		//Stats are reset at the end of each round.
		playerCreature.ResetStats();
		computerCreature.ResetStats();

		roundCounter++;

		cout << endl;
		UI.LoadingDots();
		UI.cls();

	}

	if (playerCreature.health <= 0 )
	{
		cout << "Enemy Wins!" << endl;

	}
	else if (computerCreature.health <= 0)
	{
		cout << "Player Wins!" << endl;

	}

	
	
}

int main()
{
	Menus menus;
	srand(unsigned int(time(NULL)));

	menus.MainMenu();

	return 0;
}