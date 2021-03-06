﻿/***********************************************************************************
**
** GumpSkills.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpSkills.h"
#include "../SkillGroup.h"
#include "../Managers/SkillGroupManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/MouseManager.h"
#include "../TextEngine/GameConsole.h"
#include "../Game objects/GamePlayer.h"
#include "../OrionUO.h"
#include "../PressedObject.h"
#include "../SelectedObject.h"
#include "../ToolTip.h"
#include "GumpSkill.h"
#include "../Managers/GumpManager.h"
//----------------------------------------------------------------------------------
CGumpSkills::CGumpSkills(uint serial, short x, short y, bool minimized, int height)
: CGumpBaseScroll(GT_SKILLS, serial, 0x1F40, height, x, y, true, 0, true, 15),
m_ShowReal(false), m_ShowCap(false)
{
	m_Locker.Serial = ID_GS_LOCK_MOVING;

	if (minimized)
	{
		m_Page = 1;
		m_Minimized = minimized;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}
	else
		m_Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x0839, 0, 0));

	Add(new CGUIPage(2));
	Add(new CGUIGumppic(0x0834, 82, 34)); //Skills text gump

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 180, 33));
	text->CreateTextureA(1, "Show:   Real    Cap");

	m_CheckboxShowReal = (CGUICheckbox*)Add(new CGUICheckbox(ID_GS_SHOW_REAL, 0x0938, 0x0939, 0x0938, 226, 34));
	m_CheckboxShowReal->Checked = m_ShowReal;
	m_CheckboxShowCap = (CGUICheckbox*)Add(new CGUICheckbox(ID_GS_SHOW_CAP, 0x0938, 0x0939, 0x0938, 280, 34));
	m_CheckboxShowCap->Checked = m_ShowCap;

	Add(new CGUIGumppic(0x082B, 30, 60)); //Top line
	m_BottomLine = (CGUIGumppic*)Add(new CGUIGumppic(0x082B, 31, m_Height - 48)); //Bottom line
	m_Comment = (CGUIGumppic*)Add(new CGUIGumppic(0x0836, 30, m_Height - 35));//Skills comment gump
	m_CreateGroup = (CGUIButton*)Add(new CGUIButton(ID_GS_BUTTON_NEW_GROUP, 0x083A, 0x083A, 0x083A, 60, m_Height - 3)); //New Group gump

	m_SkillSum = (CGUIText*)Add(new CGUIText(0x0065, 235, m_Height - 6));
	UpdateSkillsSum();

	//Если игрок присутствует
	if (g_Player != NULL)
	{
		int currentIndex = 0;
		int currentY = 0;

		QFOR(group, g_SkillGroupManager.m_Groups, CSkillGroupObject*)
		{
			CGUISkillGroup *skillGroup = (CGUISkillGroup*)m_HTMLGump->Add(new CGUISkillGroup(ID_GS_GROUP + currentIndex, ID_GS_GROUP_MINIMIZE + currentIndex, group, 0, currentY));
			skillGroup->Minimized = true;

			int count = group->GetCount();

			IFOR(i, 0, count)
			{
				uchar index = group->GetItem(i); //Получаем индекс скилла по порядковому номеру

				if (index < g_SkillsCount) //Он валиден
					skillGroup->Add(new CGUISkillItem(ID_GS_SKILL + index, ID_GS_SKILL_BUTTON + index, ID_GS_SKILL_STATE + index, index, 0, i * 17));
			}

			currentY += 19;

			if (!skillGroup->Minimized)
				currentY += count * 17;

			currentIndex++;
		}
	}

	m_HTMLGump->CalculateDataSize();
}
//----------------------------------------------------------------------------------
CGumpSkills::~CGumpSkills()
{
}
//---------------------------------------------------------------------------
void CGumpSkills::InitToolTip()
{
	uint id = g_SelectedObject.Serial;

	if (!m_Minimized)
	{
		switch (id)
		{
			case ID_GS_SHOW_REAL:
			{
				g_ToolTip.Set(L"Show/hide real skill values");
				break;
			}
			case ID_GS_SHOW_CAP:
			{
				g_ToolTip.Set(L"Show/hide cap skill values");
				break;
			}
			case ID_GS_BUTTON_NEW_GROUP:
			{
				g_ToolTip.Set(L"Create the new skills group");
				break;
			}
			case ID_GBS_BUTTON_MINIMIZE:
			{
				g_ToolTip.Set(L"Minimize the skills gump");
				break;
			}
			case ID_GBS_BUTTON_RESIZE:
			{
				g_ToolTip.Set(L"Start resizing for skills gump");
				break;
			}
			case ID_GS_LOCK_MOVING:
			{
				g_ToolTip.Set(L"Lock moving/closing the skills gump");
				break;
			}
			default:
			{
				if (id >= ID_GS_GROUP_MINIMIZE && id < ID_GS_GROUP)
					g_ToolTip.Set(L"Show/hide skills in this group");
				else if (id >= ID_GS_SKILL_BUTTON && id < ID_GS_SKILL)
					g_ToolTip.Set(L"Use the skill");
				else if (id >= ID_GS_SKILL_STATE)
					g_ToolTip.Set(L"Change skill state");
				break;
			}
		}
	}
	else
		g_ToolTip.Set(L"Double click to maximize skills gump");
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateHeight()
{
	CGumpBaseScroll::UpdateHeight();

	m_BottomLine->Y = m_Height - 48; //Bottom line
	m_Comment->Y = m_Height - 35;//Skills comment gump
	m_CreateGroup->Y = m_Height - 3; //New Group gump
	m_SkillSum->Y = m_Height - 6;
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateGroupPositions()
{
	int index = 0;
	int currentY = 0;

	QFOR(group, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (group->Type == GOT_SKILLGROUP)
		{
			CGUISkillGroup *skillGroup = (CGUISkillGroup*)group;
			skillGroup->Y = currentY;
			skillGroup->Serial = ID_GS_GROUP + index;
			skillGroup->m_Minimizer->Serial = ID_GS_GROUP_MINIMIZE + index;
			skillGroup->m_Name->Serial = ID_GS_GROUP + index;

			currentY += 19;

			if (!skillGroup->Minimized)
				currentY += group->GetItemsCount() * 17;

			index++;
		}
	}
}
//----------------------------------------------------------------------------------
CGUISkillGroup *CGumpSkills::GetSkillGroup(const int &index)
{
	int currentIndex = 0;

	QFOR(group, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (group->Type == GOT_SKILLGROUP)
		{
			if (currentIndex == index)
				return (CGUISkillGroup*)group;

			currentIndex++;
		}
	}

	return NULL;
}
//----------------------------------------------------------------------------------
CGUISkillItem *CGumpSkills::GetSkill(const int &index)
{
	QFOR(group, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (group->Type == GOT_SKILLGROUP)
		{
			QFOR(item, group->m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_SKILLITEM && ((CGUISkillItem*)item)->Index == index)
					return (CGUISkillItem*)item;
			}
		}
	}

	return NULL;
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateSkillValue(const int &index)
{
	QFOR(group, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (group->Type == GOT_SKILLGROUP)
		{
			QFOR(item, group->m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_SKILLITEM && ((CGUISkillItem*)item)->Index == index)
				{
					((CGUISkillItem*)item)->CreateValueText(m_ShowReal, m_ShowCap);
					return;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateSkillValues()
{
	QFOR(group, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (group->Type == GOT_SKILLGROUP)
		{
			QFOR(item, group->m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_SKILLITEM)
					((CGUISkillItem*)item)->CreateValueText(m_ShowReal, m_ShowCap);
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateSkillsSum()
{
	char str[20] = { 0 };
	sprintf(str, "%.1f", g_SkillsTotal);
	m_SkillSum->CreateTextureA(3, str);
}
//----------------------------------------------------------------------------------
void CGumpSkills::Init()
{
	//Свернем все доступные группы
	QFOR(group, g_SkillGroupManager.m_Groups, CSkillGroupObject*)
		group->Maximized = false;
}
//----------------------------------------------------------------------------------
void CGumpSkills::CalculateGumpState()
{
	CGump::CalculateGumpState();

	if (g_PressedObject.LeftGump() == this && g_PressedObject.LeftSerial >= ID_GS_SKILL && g_PressedObject.LeftSerial < ID_GS_SKILL_STATE)
	{
		g_GumpMovingOffset.Reset();

		if (m_Minimized)
		{
			g_GumpTranslate.X = (float)m_MinimizedX;
			g_GumpTranslate.Y = (float)m_MinimizedY;
		}
		else
		{
			g_GumpTranslate.X = (float)m_X;
			g_GumpTranslate.Y = (float)m_Y;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::PrepareContent()
{
	uint serial = g_PressedObject.LeftSerial;

	if (g_PressedObject.LeftGump() == this && serial >= ID_GS_SKILL && serial < ID_GS_SKILL_STATE)
	{
		int y = g_MouseManager.Position.Y;
		int testY = m_Y + m_HTMLGump->Y;

		if (y < testY)
		{
			m_HTMLGump->Scroll(false, (SCROLL_LISTING_DELAY / 3));
			m_WantRedraw = true;
		}
		else if (y > testY + m_HTMLGump->Height)
		{
			m_HTMLGump->Scroll(true, (SCROLL_LISTING_DELAY / 3));
			m_WantRedraw = true;
		}
		else if (g_PressedObject.LeftObject() != NULL && ((CBaseGUI*)g_PressedObject.LeftObject())->Type == GOT_SKILLITEM)
		{
			int index = 0;
			CSkillGroupObject *groupObject = GetGroupUnderCursor(index);

			if (groupObject != NULL)
			{
				CSkillGroupObject *currentGroupObject = NULL;
				CSkillGroupObject *groupPtr = g_SkillGroupManager.m_Groups;
				CGUISkillGroup *groupUnderCursor = NULL;
				CGUISkillGroup *currentGroup = NULL;
				int currentIndex = 0;

				QFOR(group, m_HTMLGump->m_Items, CBaseGUI*)
				{
					if (group->Type == GOT_SKILLGROUP)
					{
						if (currentGroup == NULL)
						{
							QFOR(item, group->m_Items, CBaseGUI*)
							{
								if (item->Serial == serial)
								{
									currentGroup = (CGUISkillGroup*)group;
									currentGroupObject = groupPtr;
									break;
								}
							}
						}

						if (index == currentIndex)
							groupUnderCursor = (CGUISkillGroup*)group;

						if (groupPtr != NULL)
							groupPtr = groupPtr->m_Next;

						currentIndex++;
					}
				}

				int skillIndex = serial - ID_GS_SKILL;

				if (groupUnderCursor != NULL && currentGroup != NULL && currentGroupObject != NULL && currentGroup != groupUnderCursor && currentGroupObject != groupObject && !groupObject->Contains(skillIndex))
				{
					currentGroupObject->Remove(skillIndex);
					groupObject->AddSorted(skillIndex);

					CGUISkillItem *skillItem = (CGUISkillItem*)g_PressedObject.LeftObject();

					currentGroup->Unlink(skillItem);

					if (groupObject->GetItem(0) == skillIndex)
						groupUnderCursor->Insert(NULL, skillItem);
					else
					{
						CGUISkillGroup *skillInsertElement = (CGUISkillGroup*)groupUnderCursor->m_Items;
						int itemsCount = groupObject->GetCount();

						IFOR(i, 1, itemsCount)
						{
							if (groupObject->GetItem(i) == skillIndex)
							{
								groupUnderCursor->Insert(skillInsertElement, skillItem);
								break;
							}

							if (skillInsertElement != NULL)
								skillInsertElement = (CGUISkillGroup*)skillInsertElement->m_Next;
						}
					}

					currentGroup->UpdateDataPositions();
					groupUnderCursor->UpdateDataPositions();

					UpdateGroupPositions();

					m_WantRedraw = true;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
CSkillGroupObject *CGumpSkills::GetGroupUnderCursor(int &index)
{
	index = 0;

	//Получить группу под курсором
	int mouseY = g_MouseManager.Position.Y;

	//mouse.X -= m_X + m_HTMLGump->X;
	mouseY -= m_Y + m_HTMLGump->Y;

	//Если вышли за пределы гампа по оси X
	//if (mouse.X < 0 || mouse.X > m_HTMLGump->Width)
	//	return NULL; //Exit from bounds on X

	//Если назодимся в пределах гампа по оси Y
	if (mouseY >= 0 && mouseY < m_HTMLGump->Height) //Bounds of Y
	{
		int drawY = m_HTMLGump->DataOffset.Y - m_HTMLGump->CurrentOffset.Y;
		CSkillGroupObject *group = g_SkillGroupManager.m_Groups;

		QFOR(item, m_HTMLGump->m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_SKILLGROUP)
			{
				int height = ((CGUISkillGroup*)item)->GetSize().Height;

				if (mouseY >= drawY && mouseY < drawY + height)
					return group;

				drawY += height;

				index++;

				if (group != NULL)
					group = group->m_Next;
			}
		}
	}

	//Ничего не нашлось
	return NULL;
}
//----------------------------------------------------------------------------------
void CGumpSkills::UpdateGroupText()
{
	QFOR(item, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_SKILLGROUP)
		{
			CGUISkillGroup *group = (CGUISkillGroup*)item;

			if (group->m_Name != g_SelectedObject.Object())
			{
				if (g_EntryPointer == &group->m_Name->m_Entry)
				{
					SetGroupTextFromEntry();

					if (g_ConfigManager.ConsoleNeedEnter)
						g_EntryPointer = NULL;
					else
						g_EntryPointer = &g_GameConsole;
				}

				group->m_Name->Focused = false;

				m_WantRedraw = true;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::SetGroupTextFromEntry()
{
	int index = 0;
	CSkillGroupObject *groupItem = g_SkillGroupManager.m_Groups;

	QFOR(item, m_HTMLGump->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_SKILLGROUP)
		{
			CGUISkillGroup *group = (CGUISkillGroup*)item;

			if (group->m_Name->Focused && g_EntryPointer == &group->m_Name->m_Entry && groupItem != NULL)
			{
				group->m_Name->Focused = false;

				if (!g_EntryPointer->Length())
					g_EntryPointer->SetText("NoNameGroup");

				groupItem->Name = g_EntryPointer->c_str();

				break;
			}

			group->m_Name->Focused = false;

			index++;

			if (groupItem != NULL)
				groupItem = groupItem->m_Next;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::OnLeftMouseButtonUp()
{
	CGump::OnLeftMouseButtonUp();

	if (g_PressedObject.LeftGump() == this && g_PressedObject.LeftSerial >= ID_GS_SKILL && g_PressedObject.LeftSerial < ID_GS_SKILL_STATE)
	{
		m_WantRedraw = true;

		if (g_SelectedObject.Gump() != this)
		{
			WISP_GEOMETRY::CPoint2Di pos = g_MouseManager.Position;

			g_GumpManager.AddGump(new CGumpSkill(g_PressedObject.LeftSerial - ID_GS_SKILL, pos.X - 70, pos.Y - 10));
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GBS_BUTTON_MINIMIZE) //Сворачиваем гамп
	{
		m_Minimized = true;
		m_Page = 1;
		m_WantRedraw = true;
	}
	else if (serial == ID_GS_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
	else if (serial == ID_GS_BUTTON_NEW_GROUP) //Создание новой группы
	{
		CSkillGroupObject *group = new CSkillGroupObject();
		group->Name = "New Group";
		g_SkillGroupManager.Add(group);

		CGUISkillGroup *skillGroup = (CGUISkillGroup*)m_HTMLGump->Add(new CGUISkillGroup(ID_GS_GROUP, ID_GS_GROUP_MINIMIZE, group, 0, 0));
		skillGroup->Minimized = !group->Maximized;

		UpdateGroupPositions();
	}
	else if (serial >= ID_GS_GROUP_MINIMIZE) //Операции со скиллами
	{
		if (serial >= ID_GS_SKILL_STATE) //Изменение статуса
		{
			int index = serial - ID_GS_SKILL_STATE;

			if (index < 0 || index >= g_SkillsCount)
				return;

			if (g_Player == NULL)
				return;

			uchar status = g_Player->GetSkillStatus(index);

			if (status < 2)
				status++;
			else
				status = 0;

			g_Orion.SkillStatusChange(index, status);
			g_Player->SetSkillStatus(index, status);

			CGUISkillItem *skill = GetSkill(index);

			if (skill != NULL)
				skill->Status = status;
		}
		else if (serial >= ID_GS_SKILL_BUTTON) //Выбор кнопки для использования скилла
		{
			int index = serial - ID_GS_SKILL_BUTTON;

			if (index < 0 || index >= g_SkillsCount)
				return;

			g_Orion.UseSkill(index);
		}
		else if (serial >= ID_GS_GROUP_MINIMIZE) //Скрыть/раскрыть группу
		{
			int index = serial - ID_GS_GROUP_MINIMIZE;
			int currentIndex = 0;

			QFOR(group, g_SkillGroupManager.m_Groups, CSkillGroupObject*)
			{
				if (index == currentIndex)
				{
					group->Maximized = !group->Maximized;

					CGUISkillGroup *skillGroup = GetSkillGroup(index);

					if (skillGroup != NULL)
					{
						skillGroup->Minimized = !group->Maximized;
						UpdateGroupPositions();

						m_HTMLGump->CalculateDataSize();
					}

					break;
				}

				currentIndex++;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::GUMP_CHECKBOX_EVENT_C
{
	if (serial == ID_GS_SHOW_REAL) //Показать реальное значение
	{
		m_ShowReal = state;
		m_ShowCap = false;
		m_CheckboxShowCap->Checked = false;
		UpdateSkillValues();
	}
	else if (serial == ID_GS_SHOW_CAP) //Показать доступный предел прокачки
	{
		m_ShowCap = state;
		m_ShowReal = false;
		m_CheckboxShowReal->Checked = false;
		UpdateSkillValues();
	}
}
//----------------------------------------------------------------------------------
void CGumpSkills::GUMP_TEXT_ENTRY_EVENT_C
{
	CGUISkillGroup * group = GetSkillGroup(serial - ID_GS_GROUP);

	if (group != NULL)
	{
		if (group->m_Name->Focused)
		{
			int x = g_MouseManager.Position.X - group->X;
			int y = g_MouseManager.Position.Y - group->Y;

			group->m_Name->OnClick(this, x, y);
		}
		else
			group->m_Name->Focused = true;
	}
}
//----------------------------------------------------------------------------------
bool CGumpSkills::OnLeftMouseButtonDoubleClick()
{
	if (m_Minimized) //При даблклике по мини-гампу - раскрываем его
	{
		m_Minimized = false;
		m_Page = 2;
		m_WantRedraw = true;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpSkills::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	g_EntryPointer->Insert(wParam);

	int val = g_FontManager.GetWidthA(6, g_EntryPointer->c_str(), g_EntryPointer->Length());

	if (val > 170)
		g_EntryPointer->Remove(true);
	else
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpSkills::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	if (!EntryPointerHere())
	{
		if (wParam == VK_DELETE)
		{
			int index = 0;
			CSkillGroupObject *groupItem = g_SkillGroupManager.m_Groups;
			CGUISkillGroup *first = NULL;

			QFOR(item, m_HTMLGump->m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_SKILLGROUP)
				{
					CGUISkillGroup *group = (CGUISkillGroup*)item;

					if (group->m_Name->Focused)
					{
						if (g_EntryPointer == &group->m_Name->m_Entry)
							break;

						if (g_SkillGroupManager.Remove(groupItem))
						{
							m_HTMLGump->Delete(item);

							if (first != NULL)
							{
								first->Clear();

								groupItem = g_SkillGroupManager.m_Groups;
								int count = groupItem->GetCount();

								IFOR(i, 0, count)
								{
									uchar index = groupItem->GetItem(i); //Получаем индекс скилла по порядковому номеру

									if (index < g_SkillsCount) //Он валиден
										first->Add(new CGUISkillItem(ID_GS_SKILL + index, ID_GS_SKILL_BUTTON + index, ID_GS_SKILL_STATE + index, index, 0, i * 17));
								}
							}

							UpdateGroupPositions();

							m_HTMLGump->CalculateDataSize();
						}

						m_WantRedraw = true;

						break;
					}

					if (!index)
						first = group;

					index++;

					if (groupItem != NULL)
						groupItem = groupItem->m_Next;
				}
			}
		}

		return;
	}

	//Обработчик нажатия клавишь
	switch (wParam)
	{
		case VK_RETURN:
		{
			SetGroupTextFromEntry();

			if (g_ConfigManager.ConsoleNeedEnter)
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;

			m_WantRedraw = true;

			break;
		}
		case VK_HOME:
		case VK_END:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_BACK:
		case VK_DELETE:
		{
			g_EntryPointer->OnKey(this, wParam);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
