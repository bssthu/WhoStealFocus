// 
// Module			: settings.cpp
// Author			: bss
// Project			: WhoStealFocus
// State			: 
// Creation Date	: 2014-12-26
// Last Modification: ???
// Description		: main
// 

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include "settings.h"


#ifndef DYNAMIC_LOADSETTING_STRING
// 载入设置 char*字符串
#define DYNAMIC_LOADSETTING_STRING(x)	\
    if (strcmp(#x, variableName) == 0)	\
    {	\
        if (sscanf(variableValue, "%s", x) != 1)	\
        {	\
            return false;	\
        }	\
        return true;	\
    }
#endif


// 处理一行设置
bool processLine(char* line, Settings* settings)
{
    // 一行
    char line_t[MAX_LINE_LENGTH];
    // 行长
    int lineLength = 0;
    // 变量名
    char variableName[MAX_LINE_LENGTH];
    // 变量值
    char* variableValue = nullptr;

    strcpy(line_t, line);
    variableValue = nullptr;

    lineLength = strlen(line);
    // 空行或注释行
    if ((lineLength <= 0) || ('#' == line_t[0]))
    {
        return true;
    }
    bool lineIsValid = false;
    // 找 '='
    int i = 0;
    for (i = 0; i < lineLength; i++)
    {
        if ('=' == line_t[i])
        {
            line_t[i] = ' ';
            lineIsValid = true;
            break;
        }
    }
    // 无等号的行，视为注释
    if (!lineIsValid)
    {
        return true;
    }

    sscanf(line_t, "%s", variableName);
    variableValue = line_t + i + 1;

    DYNAMIC_LOADSETTING_STRING(settings->logPath);

    return true;
}

bool LoadSettings(char* filePath, Settings* settings)
{
    // default value
    strcpy(settings->logPath, "hook.log");
    strcpy(settings->targetName, "whoStealFocus.exe");

    // open file
    FILE* fp;
    fopen_s(&fp, filePath, "r");
    if (nullptr == fp)
    {
        return false;
    }

    // 一行
    char line[MAX_LINE_LENGTH];
    // 行号
    int lineNum = 0;

    while (fgets(line, MAX_STRING_LENGTH, fp) != nullptr)
    {
        lineNum++;
        if (!processLine(line, settings))
        {
            fclose(fp);
        }
    }

    fclose(fp);
    return true;
}

#undef DYNAMIC_LOADSETTING_STRING
