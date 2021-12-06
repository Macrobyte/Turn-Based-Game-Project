#include "MacroLibV1.h"


struct Creature
{
	//Set max health and energy here.
	const float maxHealth = 100;
	const int minEnergy = 0;
	const int maxEnergy = 50;

	//Set damage of attacks here.
	const int maxAttackDamage = 10;
	const int minAttackDamage = 1;
	const int maxSpecialDamage = 20;
	const int minSpecialDamage = 5;

	//Set skill cost here.
	const int specialAttackCost = 50;

	//Chances
	const float attackChance = 80.0f;
	const float specialAttackChance = 50.0f;
	const float rechargeHitChanceDecrease = 10.0f;
	const float dodgeHitChance = 30.0f;

	//Charge Rates
	const int defaultChargeRate = 2;
	const int rechargeChargeRate = 4;
	const int dodgeChargeRate = defaultChargeRate / 2;

	//Creature stats
	float health = 0;
	int energy = 0;
	int damage = 0;
	float hitChance = 0; 
	int energyRechargeRate = 0;
	int moveCounter = 0;
	int ammountToHeal = 0;
	bool hasHealed = false;
	bool hasAttacked = false;
	bool hasSpecialAttacked = false;
	bool hasRecharged = false;
	bool hasDodged = false;

	bool endTurn = false;

	//Stats handling
	void Setup()
	{
		health = maxHealth;
		energy = maxEnergy;
		energyRechargeRate = defaultChargeRate;
	}

	void EnergyCharge()
	{
		energy += energyRechargeRate;

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

	//Creature Moves
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
			Log("Not Enough Energy!",true); 
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
			Log("Already Healed this turn!", true);
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


};

void HitCheck(Creature playerCreature, Creature computerCreature, float* playerHealth, float* computerHealth)
{

	float chanceToHit = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //Assign "n" a random float value between 0.0 and 1.0.
	chanceToHit = chanceToHit * 100; //Get "n" into a range from 0 to 100.

	if (playerCreature.hasAttacked || playerCreature.hasSpecialAttacked)
	{
		//Player Hit Check
		if (chanceToHit < playerCreature.hitChance)
		{
			SetTextColor(Green);
			Log("Player hit enemy!", true);
			SetTextColor(White);

			computerCreature.health -= playerCreature.damage;
			chanceToHit = 0; //Resets "n" so that the next calculation can be done the same way as the first one.
			*computerHealth = computerCreature.health;
		}
		else
		{
			SetTextColor(Yellow);
			Log("Player Missed!", true);
			SetTextColor(White);
			chanceToHit = 0;
		}
	}
	

	EndLine();
	Sleep(1);

	if (computerCreature.hasAttacked || computerCreature.hasSpecialAttacked)
	{
		//Enemy Hit Check
		chanceToHit += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		chanceToHit = chanceToHit * 100;

		if (chanceToHit < computerCreature.hitChance)
		{
			SetTextColor(Red);
			Log("Enemy hit Player!", true);
			SetTextColor(White);

			playerCreature.health -= computerCreature.damage;
			chanceToHit = 0;
			*playerHealth = playerCreature.health;
		}
		else
		{
			SetTextColor(Green);
			Log("Enemy Missed!", true);
			SetTextColor(White);
			chanceToHit = 0;
		}
	}
	

	Sleep(1);
}

void GameHeader(Creature playerCreature, Creature computerCreature, int roundCounter)
{

	Log("==========================================", true); 

	//Player Info
	Log("Your Health: ",false);
	SetTextColor(Red);
	std::cout << playerCreature.health << "/100";
	SetTextColor(White);

	Log(" Your Energy: ", false);
	SetTextColor(Blue);
	std::cout << playerCreature.energy << "/50";;
	SetTextColor(White);
	EndLine();
	EndLine();

	//Enemy Info
	Log("Enemy Health: ", false);
	SetTextColor(Red);
	std::cout << computerCreature.health << "/100";
	SetTextColor(White);
	EndLine();

	//Turn Info
	Log("Round: ", false);
	SetTextColor(Yellow);
	std::cout << roundCounter;
	SetTextColor(White);
	EndLine();

	Log("==========================================", true);
}

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
			switch (ActionMenu())
			{

			case Attack:
				Log("Player used Attack!", true);
				playerCreature.Attack();
				break;
			case SpecialAttack:
				Log("Player used Special Attack!", true);
				playerCreature.SpecialAttack();
				break;
			case Recharge:
				Log("Player used Recharge!", true);
				playerCreature.Recharge(computerCreature, &computerCreature.hitChance);
				break;
			case Dodge:
				Log("Player used Dodge!", true);
				playerCreature.Dodge(computerCreature, &computerCreature.hitChance);
				break;
			case Heal:
				Log("Player used Heal!", true);
				playerCreature.Heal();
				break;
			default:
				InvalidOption();
				GameHeader(playerCreature, computerCreature, roundCounter);
				break;
			}
		}
			
		EndLine();
		Sleep(1);  

		//Computer Actions
		while (computerCreature.endTurn == false && computerCreature.moveCounter < maxMoves)
		{
			
			computerChoice = (rand() % (maxComputerChoice - minComputerChoice + 1)) + minComputerChoice;

			switch (computerChoice)
			{

			case Attack:
				Log("Enemy used Attack!", true);
				computerCreature.Attack();
				break;
			case SpecialAttack:
				Log("Enemy used Special Attack!", true);
				computerCreature.SpecialAttack();
				break;
			case Recharge:
				Log("Enemy used Recharge!", true);
				computerCreature.Recharge(playerCreature, &playerCreature.hitChance);
				break;
			case Dodge:
				Log("Enemy used Dodge!", true);
				computerCreature.Dodge(playerCreature, &playerCreature.hitChance);
				break;
			case Heal:
				Log("Enemy used Heal!", true);
				computerCreature.Heal();
				break;
			default:
				GameHeader(playerCreature, computerCreature,roundCounter);
				InvalidOption();
				break;
			}
		}
		
		EndLine();
		Sleep(1);

		//Checks if any creature hit each other.
		HitCheck(playerCreature, computerCreature, &playerCreature.health, &computerCreature.health);
			
		//Energy is charged at the end of each round.
		playerCreature.EnergyCharge();
		computerCreature.EnergyCharge();

		//Stats are reset at the end of each round.
		playerCreature.ResetStats();
		computerCreature.ResetStats();

		roundCounter++;

		EndLine();
		LoadingDots();
		cls(' ');

	}

	if (playerCreature.health <= 0 )
	{
		SetTextColor(Red);
		Log("Enemy Wins", true);
		SetTextColor(White);
		EndLine();
	}
	else if (computerCreature.health <= 0)
	{
		SetTextColor(Green);
		Log("Player Wins", true);
		SetTextColor(White);
		EndLine();
	}

	GameOverMenu();	
}

int main()
{
	ConsoleTitle(TEXT("Turn Based Game"));

	srand(unsigned int(time(NULL)));

	MainMenu();

	return 0;
}