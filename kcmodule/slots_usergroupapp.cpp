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

#include "GeneralPageS.h"
#include "UserGroupPageS.h"

#include "KchildlockKCM.h"
#include "kchildlock_settings.h"


// User and Application Slots ===========================================================================




void UserGroupPageS::setupUi_app_PT(bool enabled, bool isChecked, QLineEdit *Appname, QGraphicsView *Appgraphics, QCheckBox *Appenabled, KIntNumInput *Appfrom, KIntNumInput *Appto)
{
   if (enabled) {
     if (isChecked) {
        Appname->setDisabled(FALSE);
        Appgraphics->setDisabled(FALSE);
        Appgraphics->setVisible(TRUE);
        Appenabled->setDisabled(FALSE);
        Appfrom->setDisabled(FALSE);
        Appto->setDisabled(FALSE);
     } else {
        Appname->setDisabled(TRUE);
        Appgraphics->setDisabled(TRUE);
        Appgraphics->setVisible(FALSE);
        Appenabled->setDisabled(FALSE);
        Appfrom->setDisabled(TRUE);
        Appto->setDisabled(TRUE);
     }
   } else {
        Appname->setDisabled(TRUE);
        Appgraphics->setDisabled(TRUE);
        Appgraphics->setVisible(FALSE);
        Appenabled->setDisabled(TRUE);
        Appfrom->setDisabled(TRUE);
        Appto->setDisabled(TRUE);
   }
}



void UserGroupPageS::setupUi_app_PD(bool enabled, bool isChecked, QCheckBox *Appenabled, QLineEdit *Appname, QSpinBox *AppmaxDhr, QSpinBox *AppmaxDmin, QSpinBox *AppmaxWhr, QSpinBox *AppmaxWmin)
{
   if (enabled) {
     if (isChecked) {
        Appenabled->setDisabled(FALSE);
        Appname->setDisabled(FALSE);
        AppmaxDhr->setDisabled(FALSE);
        AppmaxDmin->setDisabled(FALSE);
        AppmaxWhr->setDisabled(FALSE);
        AppmaxWmin->setDisabled(FALSE);
     } else {
        Appenabled->setDisabled(FALSE);
        Appname->setDisabled(TRUE);
        AppmaxDhr->setDisabled(TRUE);
        AppmaxDmin->setDisabled(TRUE);
        AppmaxWhr->setDisabled(TRUE);
        AppmaxWmin->setDisabled(TRUE);
     }
   } else {
        Appenabled->setDisabled(TRUE);
        Appname->setDisabled(TRUE);
        AppmaxDhr->setDisabled(TRUE);
        AppmaxDmin->setDisabled(TRUE);
        AppmaxWhr->setDisabled(TRUE);
        AppmaxWmin->setDisabled(TRUE);
   }
}





void UserGroupPageS::emitChanged_appenabledPT_usergroup(int)
{
  if (initial_fill) return;
  
  m_UserGroupSettings->setApp1enabledPT(m_ui_usergroup->kcfg_App1enabledPT->isChecked());
  m_UserGroupSettings->setApp2enabledPT(m_ui_usergroup->kcfg_App2enabledPT->isChecked());
  m_UserGroupSettings->setApp3enabledPT(m_ui_usergroup->kcfg_App3enabledPT->isChecked());
  m_UserGroupSettings->setApp4enabledPT(m_ui_usergroup->kcfg_App4enabledPT->isChecked());
  m_UserGroupSettings->setApp5enabledPT(m_ui_usergroup->kcfg_App5enabledPT->isChecked());
  fillUi_PT();
  emit changed(true);
}




void UserGroupPageS::emitChanged_appenabledPD_usergroup(int)
{
  if (initial_fill) return;
  
  m_UserGroupSettings->setApp1enabledPD(m_ui_usergroup->kcfg_App1enabledPD->isChecked());
  m_UserGroupSettings->setApp2enabledPD(m_ui_usergroup->kcfg_App2enabledPD->isChecked());
  m_UserGroupSettings->setApp3enabledPD(m_ui_usergroup->kcfg_App3enabledPD->isChecked());
  m_UserGroupSettings->setApp4enabledPD(m_ui_usergroup->kcfg_App4enabledPD->isChecked());
  m_UserGroupSettings->setApp5enabledPD(m_ui_usergroup->kcfg_App5enabledPD->isChecked());
  fillUi_PD();
  emit changed(true);
}



void UserGroupPageS::emitChanged_appnamePT_usergroup(QString)
{
   if (initial_fill) return;
   
   m_UserGroupSettings->setAppName1PT(m_ui_usergroup->kcfg_AppName1PT->text());
   m_UserGroupSettings->setAppName2PT(m_ui_usergroup->kcfg_AppName2PT->text());
   m_UserGroupSettings->setAppName3PT(m_ui_usergroup->kcfg_AppName3PT->text());
   m_UserGroupSettings->setAppName4PT(m_ui_usergroup->kcfg_AppName4PT->text());
   m_UserGroupSettings->setAppName5PT(m_ui_usergroup->kcfg_AppName5PT->text());
}



void UserGroupPageS::emitChanged_appnamePD_usergroup(QString)
{
   if (initial_fill) return;
   
   m_UserGroupSettings->setAppName1PD(m_ui_usergroup->kcfg_AppName1PD->text());
   m_UserGroupSettings->setAppName2PD(m_ui_usergroup->kcfg_AppName2PD->text());
   m_UserGroupSettings->setAppName3PD(m_ui_usergroup->kcfg_AppName3PD->text());
   m_UserGroupSettings->setAppName4PD(m_ui_usergroup->kcfg_AppName4PD->text());
   m_UserGroupSettings->setAppName5PD(m_ui_usergroup->kcfg_AppName5PD->text());
}



void UserGroupPageS::emitChanged_apptimePT_usergroup(int)
{
   if (initial_fill) return;
   
   m_UserGroupSettings->setApp1from(m_ui_usergroup->kcfg_App1from->value());
   m_UserGroupSettings->setApp1to(m_ui_usergroup->kcfg_App1to->value());
   m_UserGroupSettings->setApp2from(m_ui_usergroup->kcfg_App2from->value());
   m_UserGroupSettings->setApp2to(m_ui_usergroup->kcfg_App2to->value());
   m_UserGroupSettings->setApp3from(m_ui_usergroup->kcfg_App3from->value());
   m_UserGroupSettings->setApp3to(m_ui_usergroup->kcfg_App3to->value());
   m_UserGroupSettings->setApp4from(m_ui_usergroup->kcfg_App4from->value());
   m_UserGroupSettings->setApp4to(m_ui_usergroup->kcfg_App4to->value());
   m_UserGroupSettings->setApp5from(m_ui_usergroup->kcfg_App5from->value());
   m_UserGroupSettings->setApp5to(m_ui_usergroup->kcfg_App5to->value());
}



void UserGroupPageS::emitChanged_apptimePD_usergroup(int)
{
   if (initial_fill) return;
   
   m_UserGroupSettings->setApp1maxDhr(m_ui_usergroup->kcfg_App1maxDhr->value());
   m_UserGroupSettings->setApp1maxDmin(m_ui_usergroup->kcfg_App1maxDmin->value());
   m_UserGroupSettings->setApp1maxWhr(m_ui_usergroup->kcfg_App1maxWhr->value());
   m_UserGroupSettings->setApp1maxWmin(m_ui_usergroup->kcfg_App1maxWmin->value());
   m_UserGroupSettings->setApp2maxDhr(m_ui_usergroup->kcfg_App2maxDhr->value());
   m_UserGroupSettings->setApp2maxDmin(m_ui_usergroup->kcfg_App2maxDmin->value());
   m_UserGroupSettings->setApp2maxWhr(m_ui_usergroup->kcfg_App2maxWhr->value());
   m_UserGroupSettings->setApp2maxWmin(m_ui_usergroup->kcfg_App2maxWmin->value());
   m_UserGroupSettings->setApp3maxDhr(m_ui_usergroup->kcfg_App3maxDhr->value());
   m_UserGroupSettings->setApp3maxDmin(m_ui_usergroup->kcfg_App3maxDmin->value());
   m_UserGroupSettings->setApp3maxWhr(m_ui_usergroup->kcfg_App3maxWhr->value());
   m_UserGroupSettings->setApp3maxWmin(m_ui_usergroup->kcfg_App3maxWmin->value());
   m_UserGroupSettings->setApp4maxDhr(m_ui_usergroup->kcfg_App4maxDhr->value());
   m_UserGroupSettings->setApp4maxDmin(m_ui_usergroup->kcfg_App4maxDmin->value());
   m_UserGroupSettings->setApp4maxWhr(m_ui_usergroup->kcfg_App4maxWhr->value());
   m_UserGroupSettings->setApp4maxWmin(m_ui_usergroup->kcfg_App4maxWmin->value());
   m_UserGroupSettings->setApp5maxDhr(m_ui_usergroup->kcfg_App5maxDhr->value());
   m_UserGroupSettings->setApp5maxDmin(m_ui_usergroup->kcfg_App5maxDmin->value());
   m_UserGroupSettings->setApp5maxWhr(m_ui_usergroup->kcfg_App5maxWhr->value());
   m_UserGroupSettings->setApp5maxWmin(m_ui_usergroup->kcfg_App5maxWmin->value());
}


