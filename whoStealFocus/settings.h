// 
// Module			: settings.h
// Author			: bss
// Project			: WhoStealFocus
// State			: 
// Creation Date	: 2014-12-26
// Last Modification: ???
// Description		: main
// 


#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// �������ļ�ʱÿ���ֶε���󳤶�
static const unsigned int MAX_STRING_LENGTH = 256;
// �����ļ�ÿ����󳤶�
static const unsigned int MAX_LINE_LENGTH = 1024;

struct Settings
{
    char logPath[MAX_STRING_LENGTH];
    char targetName[MAX_STRING_LENGTH];
};

// ����һ������
bool processLine(char* line, Settings* settings);

bool LoadSettings(char* filePath, Settings* settings);

#endif
