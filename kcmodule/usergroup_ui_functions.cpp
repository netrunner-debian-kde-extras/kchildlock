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



void   UserGroupPageS::setupUi_weekday_TD(bool enabled, bool isChecked, QCheckBox *dayenabled, QSpinBox *daymaxhr, QSpinBox *daymaxmin)
{
   if (enabled) {
     if (isChecked) {
        dayenabled->setDisabled(FALSE);
        daymaxhr->setDisabled(FALSE);
        daymaxmin->setDisabled(FALSE);
     } else {
        dayenabled->setDisabled(FALSE);
        daymaxhr->setDisabled(TRUE);
        daymaxmin->setDisabled(TRUE);
     }
   } else {
        dayenabled->setDisabled(TRUE);
        daymaxhr->setDisabled(TRUE);
        daymaxmin->setDisabled(TRUE);
   }
}



void  UserGroupPageS::setupUi_weekday_LT(bool enabled, bool isChecked, QGraphicsView *daygraphics, QCheckBox *dayenabled, KIntNumInput *dayfrom, KIntNumInput *dayto)
{
   if (enabled) {
     if (isChecked) {
        daygraphics->setDisabled(FALSE);
        daygraphics->setVisible(TRUE);
        dayenabled->setDisabled(FALSE);
        dayfrom->setDisabled(FALSE);
        dayto->setDisabled(FALSE);
     } else {
        daygraphics->setDisabled(TRUE);
        daygraphics->setVisible(FALSE);
        dayenabled->setDisabled(FALSE);
        dayfrom->setDisabled(TRUE);
        dayto->setDisabled(TRUE);
     }
   } else {
        daygraphics->setDisabled(TRUE);
        daygraphics->setVisible(FALSE);
        dayenabled->setDisabled(TRUE);
        dayfrom->setDisabled(TRUE);
        dayto->setDisabled(TRUE);
   }
  
}





void UserGroupPageS::setDifferentSettingsLT(bool diff_ena, bool enable)
{
  m_ui_usergroup->kcfg_diffRestrictLT->setDisabled(!diff_ena);

  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_MONenabledLT->isChecked(), m_ui_usergroup->kcfg_MONgraphics, m_ui_usergroup->kcfg_MONenabledLT, m_ui_usergroup->kcfg_MONfrom, m_ui_usergroup->kcfg_MONto);
  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_TUEenabledLT->isChecked(), m_ui_usergroup->kcfg_TUEgraphics, m_ui_usergroup->kcfg_TUEenabledLT, m_ui_usergroup->kcfg_TUEfrom, m_ui_usergroup->kcfg_TUEto);
  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_WEDenabledLT->isChecked(), m_ui_usergroup->kcfg_WEDgraphics, m_ui_usergroup->kcfg_WEDenabledLT, m_ui_usergroup->kcfg_WEDfrom, m_ui_usergroup->kcfg_WEDto);
  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_THUenabledLT->isChecked(), m_ui_usergroup->kcfg_THUgraphics, m_ui_usergroup->kcfg_THUenabledLT, m_ui_usergroup->kcfg_THUfrom, m_ui_usergroup->kcfg_THUto);
  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_FRIenabledLT->isChecked(), m_ui_usergroup->kcfg_FRIgraphics, m_ui_usergroup->kcfg_FRIenabledLT, m_ui_usergroup->kcfg_FRIfrom, m_ui_usergroup->kcfg_FRIto);
  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_SATenabledLT->isChecked(), m_ui_usergroup->kcfg_SATgraphics, m_ui_usergroup->kcfg_SATenabledLT, m_ui_usergroup->kcfg_SATfrom, m_ui_usergroup->kcfg_SATto);
  setupUi_weekday_LT(enable, m_ui_usergroup->kcfg_SUNenabledLT->isChecked(), m_ui_usergroup->kcfg_SUNgraphics, m_ui_usergroup->kcfg_SUNenabledLT, m_ui_usergroup->kcfg_SUNfrom, m_ui_usergroup->kcfg_SUNto);
};


void UserGroupPageS::setSameSettingsLT(bool same_ena, bool enabled)
{
   m_ui_usergroup->kcfg_sameRestrictLT->setDisabled(!same_ena);

   if (enabled) {
        m_ui_usergroup->kcfg_DAYgraphics->setDisabled(FALSE);
        m_ui_usergroup->kcfg_DAYgraphics->setVisible(TRUE);
        m_ui_usergroup->kcfg_DAYfrom->setDisabled(FALSE);
        m_ui_usergroup->kcfg_DAYto->setDisabled(FALSE);
   } else {
        m_ui_usergroup->kcfg_DAYgraphics->setDisabled(TRUE);
        m_ui_usergroup->kcfg_DAYgraphics->setVisible(FALSE);
        m_ui_usergroup->kcfg_DAYfrom->setDisabled(TRUE);
        m_ui_usergroup->kcfg_DAYto->setDisabled(TRUE);
   }
};



void UserGroupPageS::setDifferentSettingsTD(bool diff_ena, bool enable)
{
  m_ui_usergroup->kcfg_diffRestrictTD->setDisabled(!diff_ena);

  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_MONenabledTD->isChecked(), m_ui_usergroup->kcfg_MONenabledTD, m_ui_usergroup->kcfg_MONmaxhr, m_ui_usergroup->kcfg_MONmaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_TUEenabledTD->isChecked(), m_ui_usergroup->kcfg_TUEenabledTD, m_ui_usergroup->kcfg_TUEmaxhr, m_ui_usergroup->kcfg_TUEmaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_WEDenabledTD->isChecked(), m_ui_usergroup->kcfg_WEDenabledTD, m_ui_usergroup->kcfg_WEDmaxhr, m_ui_usergroup->kcfg_WEDmaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_THUenabledTD->isChecked(), m_ui_usergroup->kcfg_THUenabledTD, m_ui_usergroup->kcfg_THUmaxhr, m_ui_usergroup->kcfg_THUmaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_FRIenabledTD->isChecked(), m_ui_usergroup->kcfg_FRIenabledTD, m_ui_usergroup->kcfg_FRImaxhr, m_ui_usergroup->kcfg_FRImaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_SATenabledTD->isChecked(), m_ui_usergroup->kcfg_SATenabledTD, m_ui_usergroup->kcfg_SATmaxhr, m_ui_usergroup->kcfg_SATmaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_SUNenabledTD->isChecked(), m_ui_usergroup->kcfg_SUNenabledTD, m_ui_usergroup->kcfg_SUNmaxhr, m_ui_usergroup->kcfg_SUNmaxmin);
  setupUi_weekday_TD(enable, m_ui_usergroup->kcfg_WEEKenabled->isChecked(), m_ui_usergroup->kcfg_WEEKenabled, m_ui_usergroup->kcfg_WEEKmaxhr, m_ui_usergroup->kcfg_WEEKmaxmin);
};



void UserGroupPageS::setSameSettingsTD(bool same_ena, bool enabled)
{
   m_ui_usergroup->kcfg_sameRestrictTD->setDisabled(!same_ena);

   if (enabled) {
        m_ui_usergroup->kcfg_DAYmaxhr->setDisabled(FALSE);
        m_ui_usergroup->kcfg_DAYmaxmin->setDisabled(FALSE);
   } else {
        m_ui_usergroup->kcfg_DAYmaxhr->setDisabled(TRUE);
        m_ui_usergroup->kcfg_DAYmaxmin->setDisabled(TRUE);
   }
};




