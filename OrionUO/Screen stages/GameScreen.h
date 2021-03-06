﻿/***********************************************************************************
**
** GameScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMESCREEN_H
#define GAMESCREEN_H
//----------------------------------------------------------------------------------
#include "BaseScreen.h"
#include "../Gumps/GumpScreenGame.h"
//----------------------------------------------------------------------------------
//!Структура с данными о параметрах и возможностях игрового окна
struct RENDER_VARIABLES_FOR_GAME_WINDOW
{
	//!Положение окна
	int GameWindowPosX;
	int GameWindowPosY;

	//!Габариты окна
	int GameWindowWidth;
	int GameWindowHeight;

	//!Габариты масштабированного окна
	int GameWindowScaledWidth;
	int GameWindowScaledHeight;

	//!Габариты окна
	int GameWindowScaledOffsetX;
	int GameWindowScaledOffsetY;

	//!Центр окна
	int GameWindowCenterX;
	int GameWindowCenterY;

	//!Дистанции отображения
	int RealMinRangeX;
	int RealMaxRangeX;
	int RealMinRangeY;
	int RealMaxRangeY;

	//!Индексы блоков для отображения
	int MinBlockX;
	int MinBlockY;
	int MaxBlockX;
	int MaxBlockY;

	//!Координаты пикселей для отображения
	int MinPixelsX;
	int MaxPixelsX;
	int MinPixelsY;
	int MaxPixelsY;

	//!Координаты игрока
	int PlayerX;
	int PlayerY;
	int PlayerZ;
};
//----------------------------------------------------------------------------------
//!Структура данных о источнике света
struct LIGHT_DATA
{
	//!Индекс света
	uchar ID;

	//!Цвет света
	ushort Color;

	//!Экранные координаты
	int DrawX;
	int DrawY;
};
//----------------------------------------------------------------------------------
//!Структура данных с информацией для рендера
struct RENDER_OBJECT_DATA
{
	//!Ссылка на объект рендера
	class CRenderWorldObject *Object;

	//!Цвет объекта, вышедшего за рамки видимости клиента (если включена опция)
	ushort GrayColor;

	//!Экранные координаты объекта
	int X;
	int Y;
};
//----------------------------------------------------------------------------------
//!Структура данных с информацией для рендера
struct OBJECT_HITS_INFO
{
	//!Экранные координаты
	int X;
	int Y;

	//!Цвет в зависимости от злобности персонажа
	ushort Color;

	//!Ширина линии/Ссылка на текстуру
	int Width;
};
//----------------------------------------------------------------------------------
//!Структура данных с информацией для гампа с именем объекта (Object Handles)
struct OBJECT_HANDLES_DATA
{
	//!Ссылка на объект рендера
	class CGameObject *Obj;

	//!Экранные координаты объекта
	int X;
	int Y;
};
//----------------------------------------------------------------------------------
class CGameScreen : public CBaseScreen
{
	//!Использовать ли освещение при перерисовке текущего кадра
	SETGET(bool, UseLight);
	//!Флаг, определяющий инициализацию списка рендера
	SETGET(bool, RenderListInitalized);

private:
	//!Список объектов для вывода
	OBJECT_HANDLES_DATA m_ObjectHandlesList[MAX_OBJECT_HANDLES];

	//!Количество объектов для вывода
	int m_ObjectHandlesCount;

	//!Список объектов для отображения
	RENDER_OBJECT_DATA *m_RenderList;

	//!Размер списка объектов рендера
	int m_RenderListSize;

	//!Количество объектов в списке
	int m_RenderListCount;

	//!Список источников света
	LIGHT_DATA m_Light[MAX_LIGHT_SOURCES];

	//!Количество источников света
	int m_LightCount;

	//!Максимальное значение отображения по Z координате
	int m_MaxDrawZ;

	uint  m_ProcessAlphaTimer;

	bool m_CanProcessAlpha;

	/*!
	Вычисление значений Z координаты для рендера
	@param [__out] noDrawRoof Не рисовать крыши и верхние этажи строений
	@param [__out] maxGroundZ Максимальная Z координата ландшафта
	@return Максимальная Z координата отрисовки
	*/
	int GetMaxDrawZ(bool &noDrawRoof, char &maxGroundZ);

	/*!
	Рисование игрового окна
	@param [__in] mode true - отрисовка, false - выбор
	@return 
	*/
	void DrawGameWindow(const bool &mode);

	/*!
	Отображение источников света
	@return 
	*/
	void DrawGameWindowLight();

	/*!
	Отображение текста над объектами мира
	@param [__in] mode true - отрисовка, false - выбор
	@return 
	*/
	void DrawGameWindowText(const bool &mode);

	/*!
	Применение прозрачности крон деревьев в указанных координатах
	@param [__in] graphic ндекс картинки дерева
	@param [__in] x Координата X дерева
	@param [__in] y Координата Y дерева
	@param [__in] z Координата Z дерева
	@return 
	*/
	void ApplyTransparentFoliageToUnion(const ushort &graphic, const int &x, const int &y, const int &z);

	/*!
	Проверка принадлежности кроны к группе крон (с последующим применением прозрачности всей группе)
	@param [__in] graphic Индекс картинки дерева
	@param [__in] x Координата X дерева
	@param [__in] y Координата Y дерева
	@param [__in] z Координата Z дерева
	@return 
	*/
	void CheckFoliageUnion(ushort graphic, int x, int y, int z);

	/*!
	Функция увеличения размера списка рендера
	@return 
	*/
	void IncreaseRenderList();

	void AddTileToRenderList(class CRenderWorldObject *obj, const int &drawX, const int &drawY, const int &worldX, const int &worldY, const uchar &renderIndex, const bool &useObjectHandles, const int &objectHandlesOffsetX, const int &maxZ = 150);

	void AddOffsetCharacterTileToRenderList(class CGameObject *obj, int drawX, int drawY, const uchar &renderIndex, const bool &useObjectHandles, const int &objectHandlesOffsetX);

	class CGumpScreenGame m_GameScreenGump;

	vector<OBJECT_HITS_INFO> m_HitsStack;

public:
	CGameScreen();
	virtual ~CGameScreen();

	//Идентификаторы событий для плавного перехода
	static const uchar ID_SMOOTH_GS_LOGOUT = 1;

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	virtual void ProcessSmoothAction(uchar action = 0xFF);

	/*!
	Вычисление параметров игрового окна
	@return 
	*/
	void CalculateGameWindowBounds();

	/*!
	Расчет списка объектов рендера, вычисление прозрачности крон деревьев (в т.ч. составных)
	@return 
	*/
	void CalculateRenderList();

	/*!
	Инициализация
	@return 
	*/
	virtual void Init();

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	virtual void InitToolTip();

	/*!
	Функция добавления источника света
	@param [__in] rwo Верхний объект, источающий свет
	@param [__in] lightObject Объект, источающий свет
	@param [__in] x Экранная координата X
	@param [__in] y Экранная координата Y
	@return 
	*/
	void AddLight(class CRenderWorldObject *rwo, class CRenderWorldObject *lightObject, const int &x, const int &y);

	virtual void PrepareContent();

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	virtual void Render(const bool &mode);



	virtual void OnLeftMouseButtonDown();
	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnRightMouseButtonDown();
	virtual void OnRightMouseButtonUp();
	virtual bool OnRightMouseButtonDoubleClick();
	virtual void OnMidMouseButtonDown() {}
	virtual void OnMidMouseButtonUp() {}
	virtual bool OnMidMouseButtonDoubleClick() { return false; }
	virtual void OnMidMouseButtonScroll(const bool &up);
	virtual void OnDragging();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyUp(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
//!Ссылка на экран
extern CGameScreen g_GameScreen;

//!Структура с данными о параметрах и возможностях игрового окна
extern RENDER_VARIABLES_FOR_GAME_WINDOW g_RenderBounds;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
