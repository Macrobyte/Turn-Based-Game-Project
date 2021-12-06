#pragma once

#include<Windows.h>
#include<iostream>
#include<stdio.h>
#include<time.h>
#include<chrono>
#include<thread>
#include<string>

//Colors for use with SetTextColor()
enum Colors
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Yellow = 6,
	White = 7,
	Gray = 8,
	Bright_Blue = 9,
	Bright_Green = 10,
	Bright_Cyan = 11,
	Bright_Red = 12,
	Bright_Magenta = 13,
	Bright_Yellow = 14,
	Bright_White = 15
};

//Interface.cpp
void Log(const char* message, bool endLine);
void EndLine();
int CheckInput(char input[]);
void InvalidOption();
void LoadingDots();
void cls(char fill);
void SetTextColor(int color);
void Sleep(int time);
void ConsoleTitle(std::wstring title);

//Menus.cpp
int ActionMenu();
void MainMenu();
void GameOverMenu();

//Game.cpp
void Game();