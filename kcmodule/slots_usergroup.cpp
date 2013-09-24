/***************************************************************************
 *   Copyright (C) 2009 by Rene Landert <rene.landert@bluewin.ch>          *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <KConfigDialog>
#include <QModelIndex>

#include "GeneralPageS.h"
#include "UserGroupPageS.h"

#include "KchildlockKCM.h"
#include "kchildlock_settings.h"


// User or Group Slots ===========================================================================

void UserGroupPageS::emitChanged_enabled_usergroup(int value)
{
   m_UserGroupSettings->setEnabled(value);
   fillUi();
   emit changed(true);
}


void UserGroupPageS::emitChanged_sameRestrictLT_usergroup(bool value)
{
//   kDebug() << "Value is" << value;
   if (initial_fill) return;
   m_UserGroupSettings->setSameRestrictLT(value);
   m_UserGroupSettings->setDiffRestrictLT(!value);
   m_ui_usergroup->kcfg_sameRestrictLT->setChecked(value);
   m_ui_usergroup->kcfg_diffRestrictLT->setChecked(!value);
 
   setDifferentSettingsLT(TRUE, !value);
   setSameSettingsLT(TRUE, value);
   emit changed(true);
}

void UserGroupPageS::emitChanged_sameRestrictTD_usergroup(bool value)
{
   if (initial_fill) return;
   m_UserGroupSettings->setSameRestrictTD(value);
   m_UserGroupSettings->setDiffRestrictTD(!value);
   m_ui_usergroup->kcfg_sameRestrictTD->setChecked(value);
   m_ui_usergroup->kcfg_diffRestrictTD->setChecked(!value);
 
   setDifferentSettingsTD(TRUE, !value);
   setSameSettingsTD(TRUE, value);
   emit changed(true);
}

void UserGroupPageS::emitChanged_diffRestrictLT_usergroup(bool value)
{
//   kDebug() << "Value is" << value;
   if (initial_fill) return;
   m_UserGroupSettings->setSameRestrictLT(!value);
   m_UserGroupSettings->setDiffRestrictLT(value);
   m_ui_usergroup->kcfg_sameRestrictLT->setChecked(!value);
   m_ui_usergroup->kcfg_diffRestrictLT->setChecked(value);
   
   setDifferentSettingsLT(TRUE, value);
   setSameSettingsLT(TRUE, !value);
   emit changed(true);
}

void UserGroupPageS::emitChanged_diffRestrictTD_usergroup(bool value)
{
   if (initial_fill) return;
   m_UserGroupSettings->setSameRestrictTD(!value);
   m_UserGroupSettings->setDiffRestrictTD(value);
   m_ui_usergroup->kcfg_sameRestrictTD->setChecked(!value);
   m_ui_usergroup->kcfg_diffRestrictTD->setChecked(value);
   
   setDifferentSettingsTD(TRUE, value);
   setSameSettingsTD(TRUE, !value);
   emit changed(true);
}


void UserGroupPageS::emitChanged_DAYenabledLT_usergroup(int value)
{
  if (initial_fill) return;
  
  m_UserGroupSettings->setMONenabledLT(m_ui_usergroup->kcfg_MONenabledLT->isChecked());
  m_UserGroupSettings->setTUEenabledLT(m_ui_usergroup->kcfg_TUEenabledLT->isChecked());
  m_UserGroupSettings->setWEDenabledLT(m_ui_usergroup->kcfg_WEDenabledLT->isChecked());
  m_UserGroupSettings->setTHUenabledLT(m_ui_usergroup->kcfg_THUenabledLT->isChecked());
  m_UserGroupSettings->setFRIenabledLT(m_ui_usergroup->kcfg_FRIenabledLT->isChecked());
  m_UserGroupSettings->setSATenabledLT(m_ui_usergroup->kcfg_SATenabledLT->isChecked());
  m_UserGroupSettings->setSUNenabledLT(m_ui_usergroup->kcfg_SUNenabledLT->isChecked());
  fillUi_LT();
  emit changed(true);
}


void UserGroupPageS::emitChanged_DAYenabledTD_usergroup(int value)
{
  if (initial_fill) return;
  
  m_UserGroupSettings->setMONenabledTD(m_ui_usergroup->kcfg_MONenabledTD->isChecked());
  m_UserGroupSettings->setTUEenabledTD(m_ui_usergroup->kcfg_TUEenabledTD->isChecked());
  m_UserGroupSettings->setWEDenabledTD(m_ui_usergroup->kcfg_WEDenabledTD->isChecked());
  m_UserGroupSettings->setTHUenabledTD(m_ui_usergroup->kcfg_THUenabledTD->isChecked());
  m_UserGroupSettings->setFRIenabledTD(m_ui_usergroup->kcfg_FRIenabledTD->isChecked());
  m_UserGroupSettings->setSATenabledTD(m_ui_usergroup->kcfg_SATenabledTD->isChecked());
  m_UserGroupSettings->setSUNenabledTD(m_ui_usergroup->kcfg_SUNenabledTD->isChecked());
  m_UserGroupSettings->setWEEKenabled(m_ui_usergroup->kcfg_WEEKenabled->isChecked());
  fillUi_TD();
  emit changed(true);
}




void UserGroupPageS::emitChanged_value_usergroup(int value)
{
   if (initial_fill) return;

   m_UserGroupSettings->setDAYfrom(m_ui_usergroup->kcfg_DAYfrom->value());
   m_UserGroupSettings->setDAYto(m_ui_usergroup->kcfg_DAYto->value());
   m_UserGroupSettings->setDAYmaxhr(m_ui_usergroup->kcfg_DAYmaxhr->value());
   m_UserGroupSettings->setDAYmaxmin(m_ui_usergroup->kcfg_DAYmaxmin->value());

   m_UserGroupSettings->setMONfrom(m_ui_usergroup->kcfg_MONfrom->value());
   m_UserGroupSettings->setMONto(m_ui_usergroup->kcfg_MONto->value());
   m_UserGroupSettings->setMONmaxhr(m_ui_usergroup->kcfg_MONmaxhr->value());
   m_UserGroupSettings->setMONmaxmin(m_ui_usergroup->kcfg_MONmaxmin->value());

   m_UserGroupSettings->setTUEfrom(m_ui_usergroup->kcfg_TUEfrom->value());
   m_UserGroupSettings->setTUEto(m_ui_usergroup->kcfg_TUEto->value());
   m_UserGroupSettings->setTUEmaxhr(m_ui_usergroup->kcfg_TUEmaxhr->value());
   m_UserGroupSettings->setTUEmaxmin(m_ui_usergroup->kcfg_TUEmaxmin->value());

   m_UserGroupSettings->setWEDfrom(m_ui_usergroup->kcfg_WEDfrom->value());
   m_UserGroupSettings->setWEDto(m_ui_usergroup->kcfg_WEDto->value());
   m_UserGroupSettings->setWEDmaxhr(m_ui_usergroup->kcfg_WEDmaxhr->value());
   m_UserGroupSettings->setWEDmaxmin(m_ui_usergroup->kcfg_WEDmaxmin->value());

   m_UserGroupSettings->setTHUfrom(m_ui_usergroup->kcfg_THUfrom->value());
   m_UserGroupSettings->setTHUto(m_ui_usergroup->kcfg_THUto->value());
   m_UserGroupSettings->setTHUmaxhr(m_ui_usergroup->kcfg_THUmaxhr->value());
   m_UserGroupSettings->setTHUmaxmin(m_ui_usergroup->kcfg_THUmaxmin->value());

   m_UserGroupSettings->setFRIfrom(m_ui_usergroup->kcfg_FRIfrom->value());
   m_UserGroupSettings->setFRIto(m_ui_usergroup->kcfg_FRIto->value());
   m_UserGroupSettings->setFRImaxhr(m_ui_usergroup->kcfg_FRImaxhr->value());
   m_UserGroupSettings->setFRImaxmin(m_ui_usergroup->kcfg_FRImaxmin->value());

   m_UserGroupSettings->setSATfrom(m_ui_usergroup->kcfg_SATfrom->value());
   m_UserGroupSettings->setSATto(m_ui_usergroup->kcfg_SATto->value());
   m_UserGroupSettings->setSATmaxhr(m_ui_usergroup->kcfg_SATmaxhr->value());
   m_UserGroupSettings->setSATmaxmin(m_ui_usergroup->kcfg_SATmaxmin->value());

   m_UserGroupSettings->setSUNfrom(m_ui_usergroup->kcfg_SUNfrom->value());
   m_UserGroupSettings->setSUNto(m_ui_usergroup->kcfg_SUNto->value());
   m_UserGroupSettings->setSUNmaxhr(m_ui_usergroup->kcfg_SUNmaxhr->value());
   m_UserGroupSettings->setSUNmaxmin(m_ui_usergroup->kcfg_SUNmaxmin->value());

//   m_UserGroupSettings->setWEEKenabled(m_ui_usergroup->kcfg_WEEKenabled->isChecked());
   m_UserGroupSettings->setWEEKmaxhr(m_ui_usergroup->kcfg_WEEKmaxhr->value());
   m_UserGroupSettings->setWEEKmaxmin(m_ui_usergroup->kcfg_WEEKmaxmin->value());

   emit changed(true);
}
