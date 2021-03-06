﻿/***********************************************************************************
**
** Walker.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef WALKER_H
#define WALKER_H
//----------------------------------------------------------------------------------
#include "FastWalk.h"
//----------------------------------------------------------------------------------
//Класс для обработки шагов игрока
class CWalker
{
private:
	uchar m_Sequence; //Текущий шаг

	uchar m_Direction[256]; //Направления

public:
	CWalker();
	~CWalker() {}

	CFastWalkStack m_FastWalkStack; //Указатель на Fast Walk Stack
	
	void IncSequence(); //Инкремент счетчика шагов
	void SetSequence(const uchar &seq, const uchar &dir); //Установить значение Direction для указанного шага

	uchar GetSequence() const { return m_Sequence; } //Получить номер шага
	uchar GetDirection(const uchar &seq) const { return m_Direction[seq]; } //Получить значение Direction для указанного шага
};
//----------------------------------------------------------------------------------
extern CWalker *g_Walker;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
