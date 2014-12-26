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
// �������� char*�ַ���
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


// ����һ������
bool processLine(char* line, Settings* settings)
{
    // һ��
    char line_t[MAX_LINE_LENGTH];
    // �г�
    int lineLength = 0;
    // ������
    char variableName[MAX_LINE_LENGTH];
    // ����ֵ
    char* variableValue = nullptr;

    strcpy(line_t, line);
    variableValue = nullptr;

    lineLength = strlen(line);
    // ���л�ע����
    if ((lineLength <= 0) || ('#' == line_t[0]))
    {
        return true;
    }
    bool lineIsValid = false;
    // �� '='
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
    // �޵Ⱥŵ��У���Ϊע��
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

    // һ��
    char line[MAX_LINE_LENGTH];
    // �к�
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
