﻿/***********************************************************************************
**
** TargetGump.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "TargetGump.h"
#include "OrionUO.h"
#include "Managers/ConfigManager.h"
//----------------------------------------------------------------------------------
CTargetGump g_TargetGump;
CTargetGump g_AttackTargetGump;
CNewTargetSystem g_NewTargetSystem;
//----------------------------------------------------------------------------------
//-------------------------------------CTargetGump----------------------------------
//----------------------------------------------------------------------------------
CTargetGump::CTargetGump()
: m_X(0), m_Y(0), m_Hits(0), m_Color(0)
{
}
//----------------------------------------------------------------------------------
CTargetGump::~CTargetGump()
{
}
//----------------------------------------------------------------------------------
void CTargetGump::Draw()
{
	if (m_Color != 0)
	{
		g_Orion.DrawGump(0x1068, m_Color, m_X, m_Y);

		if (m_Hits > 0)
			g_Orion.DrawGump(0x1069, 0x005A, m_X, m_Y, m_Hits, 0);
	}
}
//----------------------------------------------------------------------------------
//----------------------------------CNewTargetSystem--------------------------------
//----------------------------------------------------------------------------------
CNewTargetSystem::CNewTargetSystem()
: m_Serial(0), m_X(0), m_TopY(0), m_BottomY(0), m_ColorGump(0), m_GumpTop(0),
m_GumpBottom(0), m_GumpX(20), m_GumpY(20), m_Hits(0)
{
}
//----------------------------------------------------------------------------------
CNewTargetSystem::~CNewTargetSystem()
{
}
//----------------------------------------------------------------------------------
void CNewTargetSystem::Draw()
{
	if (!g_ConfigManager.DisableNewTargetSystem && m_ColorGump != 0)
	{
		CIndexObject &top = g_Orion.m_GumpDataIndex[m_GumpTop];

		int x = m_X - (top.Width / 2);

		g_Orion.DrawGump(m_GumpTop, 0, x, m_TopY - top.Height);
		g_Orion.DrawGump(m_ColorGump, 0, x, m_TopY - top.Height);
		g_Orion.DrawGump(m_GumpBottom, 0, x, m_BottomY);

		if (m_Hits > 0)
			g_Orion.DrawGump(0x1069, 0x005A, m_X - 16, m_BottomY + 15, m_Hits, 0);
	}
}
//----------------------------------------------------------------------------------
