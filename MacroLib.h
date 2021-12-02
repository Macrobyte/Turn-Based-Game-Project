#pragma once

//Interface.cpp
enum Colors
{
	White = 7,
	Red = 12,
	Blue = 9,
	Yellow = 14,
	Green = 10
};

void Log(const char* message, bool endLine);
void EndLine();
int CheckInput(char input[]);
void InvalidOption();
void LoadingDots();
void cls(char fill);
void SetTextColor(int color);
void Sleep(int time);

//Menus.cpp
int ActionMenu();
void MainMenu();
void GameOverMenu();

//Game.cpp
void Game();