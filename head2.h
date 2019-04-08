#pragma once
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<iostream>
#include<string>
#include<vector>
#include<sys/wait.h>
#include<glob.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

using namespace std;
void pars(vector<vector<string> >&, string); //разбирает строку
void vf(vector<string>&); //вызывает нужную функцию
void pwd();
void cd(const string scd);
void shellf(vector<string>&);
int zam(vector<vector<string> >&, vector<string>&); // заменяет * и ? на всевозможные файлы
void mytime(vector<string>&);
void sh1(int signum);

