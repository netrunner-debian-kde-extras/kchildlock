/***************************************************************************
 *   Copyright (C) 2010 by Rene Landert <rene.landert@bluewin.ch>          *
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

#include "GeneralPageS.h"
#include "UserGroupPageS.h"

#include "KchildlockKCM.h"
#include "kchildlock_settings.h"
#include <QStringListModel>


void GeneralPageS::fillUi()
{
   m_ui_general->kcfg_enable_restrictions->setChecked(m_GeneralSettings->enable_restrictions());
   m_ui_general->kcfg_enable_systray->setChecked(m_GeneralSettings->enable_systray());
   m_ui_general->kcfg_scaninterval->setValue(m_GeneralSettings->scaninterval());
}

void UserGroupPageS::fillUi()
{
//   kDebug() << "User / Group gui/sett =" << m_ui_usergroup->kcfg_usergroupnamestring << m_UserGroupSettings->usergroupnamestring();
   m_ui_usergroup->kcfg_enabled->setChecked(m_UserGroupSettings->enabled());
   fillUi_LT();
   fillUi_TD();
   fillUi_PT();
   fillUi_PD();
//   emit changed(true);
}


void UserGroupPageS::cleanUi()
{
   m_ui_usergroup->kcfg_enabled->setChecked(FALSE);
   m_UserGroupSettings->setEnabled(FALSE);
   m_ui_usergroup->kcfg_MONenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setMONenabledLT(FALSE);
   m_ui_usergroup->kcfg_TUEenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setTUEenabledLT(FALSE);
   m_ui_usergroup->kcfg_WEDenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setWEDenabledLT(FALSE);
   m_ui_usergroup->kcfg_THUenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setTHUenabledLT(FALSE);
   m_ui_usergroup->kcfg_FRIenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setFRIenabledLT(FALSE);
   m_ui_usergroup->kcfg_SATenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setSATenabledLT(FALSE);
   m_ui_usergroup->kcfg_SUNenabledLT->setChecked(FALSE);
   m_UserGroupSettings->setSUNenabledLT(FALSE);
   // AND SAME FOR PD APP ...
}



void UserGroupPageS::fillUi_LT()
{
   m_ui_usergroup->kcfg_sameRestrictLT->setChecked(m_UserGroupSettings->sameRestrictLT());
   m_ui_usergroup->kcfg_diffRestrictLT->setChecked(m_UserGroupSettings->diffRestrictLT());

   m_ui_usergroup->kcfg_DAYfrom->setValue(m_UserGroupSettings->dAYfrom());
   m_ui_usergroup->kcfg_DAYto->setValue(m_UserGroupSettings->dAYto());

   m_ui_usergroup->kcfg_MONenabledLT->setChecked(m_UserGroupSettings->mONenabledLT());
   m_ui_usergroup->kcfg_MONfrom->setValue(m_UserGroupSettings->mONfrom());
   m_ui_usergroup->kcfg_MONto->setValue(m_UserGroupSettings->mONto());

   m_ui_usergroup->kcfg_TUEenabledLT->setChecked(m_UserGroupSettings->tUEenabledLT());
   m_ui_usergroup->kcfg_TUEfrom->setValue(m_UserGroupSettings->tUEfrom());
   m_ui_usergroup->kcfg_TUEto->setValue(m_UserGroupSettings->tUEto());
   
   m_ui_usergroup->kcfg_WEDenabledLT->setChecked(m_UserGroupSettings->wEDenabledLT());
   m_ui_usergroup->kcfg_WEDfrom->setValue(m_UserGroupSettings->wEDfrom());
   m_ui_usergroup->kcfg_WEDto->setValue(m_UserGroupSettings->wEDto());

   m_ui_usergroup->kcfg_THUenabledLT->setChecked(m_UserGroupSettings->tHUenabledLT());
   m_ui_usergroup->kcfg_THUfrom->setValue(m_UserGroupSettings->tHUfrom());
   m_ui_usergroup->kcfg_THUto->setValue(m_UserGroupSettings->tHUto());

   m_ui_usergroup->kcfg_FRIenabledLT->setChecked(m_UserGroupSettings->fRIenabledLT());
   m_ui_usergroup->kcfg_FRIfrom->setValue(m_UserGroupSettings->fRIfrom());
   m_ui_usergroup->kcfg_FRIto->setValue(m_UserGroupSettings->fRIto());

   m_ui_usergroup->kcfg_SATenabledLT->setChecked(m_UserGroupSettings->sATenabledLT());
   m_ui_usergroup->kcfg_SATfrom->setValue(m_UserGroupSettings->sATfrom());
   m_ui_usergroup->kcfg_SATto->setValue(m_UserGroupSettings->sATto());

   m_ui_usergroup->kcfg_SUNenabledLT->setChecked(m_UserGroupSettings->sUNenabledLT());
   m_ui_usergroup->kcfg_SUNfrom->setValue(m_UserGroupSettings->sUNfrom());
   m_ui_usergroup->kcfg_SUNto->setValue(m_UserGroupSettings->sUNto());
  
// check for same restrictions every day / enabled
   if (m_ui_usergroup->kcfg_enabled->isChecked()) {
     if (m_ui_usergroup->kcfg_sameRestrictLT->isChecked())  {
        setDifferentSettingsLT(TRUE, FALSE);
        setSameSettingsLT(TRUE, TRUE);
     } else {
        setDifferentSettingsLT(TRUE, TRUE);
        setSameSettingsLT(TRUE, FALSE);
     }
   } else {
     setDifferentSettingsLT(FALSE, FALSE);
     setSameSettingsLT(FALSE, FALSE);
   }
   
}



void UserGroupPageS::fillUi_TD()
{
   m_ui_usergroup->kcfg_sameRestrictTD->setChecked(m_UserGroupSettings->sameRestrictTD());
   m_ui_usergroup->kcfg_diffRestrictTD->setChecked(m_UserGroupSettings->diffRestrictTD());

   m_ui_usergroup->kcfg_DAYmaxhr->setValue(m_UserGroupSettings->dAYmaxhr());
   m_ui_usergroup->kcfg_DAYmaxmin->setValue(m_UserGroupSettings->dAYmaxmin());

   m_ui_usergroup->kcfg_MONenabledTD->setChecked(m_UserGroupSettings->mONenabledTD());
   m_ui_usergroup->kcfg_MONmaxhr->setValue(m_UserGroupSettings->mONmaxhr());
   m_ui_usergroup->kcfg_MONmaxmin->setValue(m_UserGroupSettings->mONmaxmin());

   m_ui_usergroup->kcfg_TUEenabledTD->setChecked(m_UserGroupSettings->tUEenabledTD());
   m_ui_usergroup->kcfg_TUEmaxhr->setValue(m_UserGroupSettings->tUEmaxhr());
   m_ui_usergroup->kcfg_TUEmaxmin->setValue(m_UserGroupSettings->tUEmaxmin());
   
   m_ui_usergroup->kcfg_WEDenabledTD->setChecked(m_UserGroupSettings->wEDenabledTD());
   m_ui_usergroup->kcfg_WEDmaxhr->setValue(m_UserGroupSettings->wEDmaxhr());
   m_ui_usergroup->kcfg_WEDmaxmin->setValue(m_UserGroupSettings->wEDmaxmin());

   m_ui_usergroup->kcfg_THUenabledTD->setChecked(m_UserGroupSettings->tHUenabledTD());
   m_ui_usergroup->kcfg_THUmaxhr->setValue(m_UserGroupSettings->tHUmaxhr());
   m_ui_usergroup->kcfg_THUmaxmin->setValue(m_UserGroupSettings->tHUmaxmin());

   m_ui_usergroup->kcfg_FRIenabledTD->setChecked(m_UserGroupSettings->fRIenabledTD());
   m_ui_usergroup->kcfg_FRImaxhr->setValue(m_UserGroupSettings->fRImaxhr());
   m_ui_usergroup->kcfg_FRImaxmin->setValue(m_UserGroupSettings->fRImaxmin());

   m_ui_usergroup->kcfg_SATenabledTD->setChecked(m_UserGroupSettings->sATenabledTD());
   m_ui_usergroup->kcfg_SATmaxhr->setValue(m_UserGroupSettings->sATmaxhr());
   m_ui_usergroup->kcfg_SATmaxmin->setValue(m_UserGroupSettings->sATmaxmin());

   m_ui_usergroup->kcfg_SUNenabledTD->setChecked(m_UserGroupSettings->sUNenabledTD());
   m_ui_usergroup->kcfg_SUNmaxhr->setValue(m_UserGroupSettings->sUNmaxhr());
   m_ui_usergroup->kcfg_SUNmaxmin->setValue(m_UserGroupSettings->sUNmaxmin());

   m_ui_usergroup->kcfg_WEEKenabled->setChecked(m_UserGroupSettings->wEEKenabled());
   m_ui_usergroup->kcfg_WEEKmaxhr->setValue(m_UserGroupSettings->wEEKmaxhr());
   m_ui_usergroup->kcfg_WEEKmaxmin->setValue(m_UserGroupSettings->wEEKmaxmin());
  
// check for same restrictions every day / enabled
   if (m_ui_usergroup->kcfg_enabled->isChecked()) {
     m_ui_usergroup->kcfg_WEEKenabled->setDisabled(FALSE);
     if (m_ui_usergroup->kcfg_sameRestrictTD->isChecked())  {
        setDifferentSettingsTD(TRUE, FALSE);
        setSameSettingsTD(TRUE, TRUE);
     } else {
        setDifferentSettingsTD(TRUE, TRUE);
        setSameSettingsTD(TRUE, FALSE);
     };
   } else {
     m_ui_usergroup->kcfg_WEEKenabled->setDisabled(TRUE);
     setDifferentSettingsTD(FALSE, FALSE);
     setSameSettingsTD(FALSE, FALSE);
   }   
}



void UserGroupPageS::fillUi_PT()
{
   m_ui_usergroup->kcfg_App1enabledPT->setChecked(m_UserGroupSettings->app1enabledPT());
   m_ui_usergroup->kcfg_AppName1PT->setText(m_UserGroupSettings->appName1PT());
   m_ui_usergroup->kcfg_App1from->setValue(m_UserGroupSettings->app1from());
   m_ui_usergroup->kcfg_App1to->setValue(m_UserGroupSettings->app1to());

   m_ui_usergroup->kcfg_App2enabledPT->setChecked(m_UserGroupSettings->app2enabledPT());
   m_ui_usergroup->kcfg_AppName2PT->setText(m_UserGroupSettings->appName2PT());
   m_ui_usergroup->kcfg_App2from->setValue(m_UserGroupSettings->app2from());
   m_ui_usergroup->kcfg_App2to->setValue(m_UserGroupSettings->app2to());

   m_ui_usergroup->kcfg_App3enabledPT->setChecked(m_UserGroupSettings->app3enabledPT());
   m_ui_usergroup->kcfg_AppName3PT->setText(m_UserGroupSettings->appName3PT());
   m_ui_usergroup->kcfg_App3from->setValue(m_UserGroupSettings->app3from());
   m_ui_usergroup->kcfg_App3to->setValue(m_UserGroupSettings->app3to());

   m_ui_usergroup->kcfg_App4enabledPT->setChecked(m_UserGroupSettings->app4enabledPT());
   m_ui_usergroup->kcfg_AppName4PT->setText(m_UserGroupSettings->appName4PT());
   m_ui_usergroup->kcfg_App4from->setValue(m_UserGroupSettings->app4from());
   m_ui_usergroup->kcfg_App4to->setValue(m_UserGroupSettings->app4to());

   m_ui_usergroup->kcfg_App5enabledPT->setChecked(m_UserGroupSettings->app5enabledPT());
   m_ui_usergroup->kcfg_AppName5PT->setText(m_UserGroupSettings->appName5PT());
   m_ui_usergroup->kcfg_App5from->setValue(m_UserGroupSettings->app5from());
   m_ui_usergroup->kcfg_App5to->setValue(m_UserGroupSettings->app5to());

   setupUi_app_PT(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App1enabledPT->isChecked(), m_ui_usergroup->kcfg_AppName1PT, m_ui_usergroup->kcfg_App1graphics, m_ui_usergroup->kcfg_App1enabledPT, m_ui_usergroup->kcfg_App1from, m_ui_usergroup->kcfg_App1to);
   setupUi_app_PT(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App2enabledPT->isChecked(), m_ui_usergroup->kcfg_AppName2PT, m_ui_usergroup->kcfg_App2graphics, m_ui_usergroup->kcfg_App2enabledPT, m_ui_usergroup->kcfg_App2from, m_ui_usergroup->kcfg_App2to);
   setupUi_app_PT(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App3enabledPT->isChecked(), m_ui_usergroup->kcfg_AppName3PT, m_ui_usergroup->kcfg_App3graphics, m_ui_usergroup->kcfg_App3enabledPT, m_ui_usergroup->kcfg_App3from, m_ui_usergroup->kcfg_App3to);
   setupUi_app_PT(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App4enabledPT->isChecked(), m_ui_usergroup->kcfg_AppName4PT, m_ui_usergroup->kcfg_App4graphics, m_ui_usergroup->kcfg_App4enabledPT, m_ui_usergroup->kcfg_App4from, m_ui_usergroup->kcfg_App4to);
   setupUi_app_PT(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App5enabledPT->isChecked(), m_ui_usergroup->kcfg_AppName5PT, m_ui_usergroup->kcfg_App5graphics, m_ui_usergroup->kcfg_App5enabledPT, m_ui_usergroup->kcfg_App5from, m_ui_usergroup->kcfg_App5to);
   
}



void UserGroupPageS::fillUi_PD()
{
   m_ui_usergroup->kcfg_App1enabledPD->setChecked(m_UserGroupSettings->app1enabledPD());
   m_ui_usergroup->kcfg_AppName1PD->setText(m_UserGroupSettings->appName1PD());
   m_ui_usergroup->kcfg_App1maxDhr->setValue(m_UserGroupSettings->app1maxDhr());
   m_ui_usergroup->kcfg_App1maxDmin->setValue(m_UserGroupSettings->app1maxDmin());
   m_ui_usergroup->kcfg_App1maxWhr->setValue(m_UserGroupSettings->app1maxWhr());
   m_ui_usergroup->kcfg_App1maxWmin->setValue(m_UserGroupSettings->app1maxWmin());

   m_ui_usergroup->kcfg_App2enabledPD->setChecked(m_UserGroupSettings->app2enabledPD());
   m_ui_usergroup->kcfg_AppName2PD->setText(m_UserGroupSettings->appName2PD());
   m_ui_usergroup->kcfg_App2maxDhr->setValue(m_UserGroupSettings->app2maxDhr());
   m_ui_usergroup->kcfg_App2maxDmin->setValue(m_UserGroupSettings->app2maxDmin());
   m_ui_usergroup->kcfg_App2maxWhr->setValue(m_UserGroupSettings->app2maxWhr());
   m_ui_usergroup->kcfg_App2maxWmin->setValue(m_UserGroupSettings->app2maxWmin());

   m_ui_usergroup->kcfg_App3enabledPD->setChecked(m_UserGroupSettings->app3enabledPD());
   m_ui_usergroup->kcfg_AppName3PD->setText(m_UserGroupSettings->appName3PD());
   m_ui_usergroup->kcfg_App3maxDhr->setValue(m_UserGroupSettings->app3maxDhr());
   m_ui_usergroup->kcfg_App3maxDmin->setValue(m_UserGroupSettings->app3maxDmin());
   m_ui_usergroup->kcfg_App3maxWhr->setValue(m_UserGroupSettings->app3maxWhr());
   m_ui_usergroup->kcfg_App3maxWmin->setValue(m_UserGroupSettings->app3maxWmin());

   m_ui_usergroup->kcfg_App4enabledPD->setChecked(m_UserGroupSettings->app4enabledPD());
   m_ui_usergroup->kcfg_AppName4PD->setText(m_UserGroupSettings->appName4PD());
   m_ui_usergroup->kcfg_App4maxDhr->setValue(m_UserGroupSettings->app4maxDhr());
   m_ui_usergroup->kcfg_App4maxDmin->setValue(m_UserGroupSettings->app4maxDmin());
   m_ui_usergroup->kcfg_App4maxWhr->setValue(m_UserGroupSettings->app4maxWhr());
   m_ui_usergroup->kcfg_App4maxWmin->setValue(m_UserGroupSettings->app4maxWmin());

   m_ui_usergroup->kcfg_App5enabledPD->setChecked(m_UserGroupSettings->app5enabledPD());
   m_ui_usergroup->kcfg_AppName5PD->setText(m_UserGroupSettings->appName5PD());
   m_ui_usergroup->kcfg_App5maxDhr->setValue(m_UserGroupSettings->app5maxDhr());
   m_ui_usergroup->kcfg_App5maxDmin->setValue(m_UserGroupSettings->app5maxDmin());
   m_ui_usergroup->kcfg_App5maxWhr->setValue(m_UserGroupSettings->app5maxWhr());
   m_ui_usergroup->kcfg_App5maxWmin->setValue(m_UserGroupSettings->app5maxWmin());

   setupUi_app_PD(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App1enabledPD->isChecked(), m_ui_usergroup->kcfg_App1enabledPD, m_ui_usergroup->kcfg_AppName1PD, m_ui_usergroup->kcfg_App1maxDhr, m_ui_usergroup->kcfg_App1maxDmin, m_ui_usergroup->kcfg_App1maxWhr,  m_ui_usergroup->kcfg_App1maxWmin);
   setupUi_app_PD(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App2enabledPD->isChecked(), m_ui_usergroup->kcfg_App2enabledPD, m_ui_usergroup->kcfg_AppName2PD, m_ui_usergroup->kcfg_App2maxDhr, m_ui_usergroup->kcfg_App2maxDmin, m_ui_usergroup->kcfg_App2maxWhr,  m_ui_usergroup->kcfg_App2maxWmin);
   setupUi_app_PD(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App3enabledPD->isChecked(), m_ui_usergroup->kcfg_App3enabledPD, m_ui_usergroup->kcfg_AppName3PD, m_ui_usergroup->kcfg_App3maxDhr, m_ui_usergroup->kcfg_App3maxDmin, m_ui_usergroup->kcfg_App3maxWhr,  m_ui_usergroup->kcfg_App3maxWmin);
   setupUi_app_PD(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App4enabledPD->isChecked(), m_ui_usergroup->kcfg_App4enabledPD, m_ui_usergroup->kcfg_AppName4PD, m_ui_usergroup->kcfg_App4maxDhr, m_ui_usergroup->kcfg_App4maxDmin, m_ui_usergroup->kcfg_App4maxWhr,  m_ui_usergroup->kcfg_App4maxWmin);
   setupUi_app_PD(m_ui_usergroup->kcfg_enabled->isChecked(), m_ui_usergroup->kcfg_App5enabledPD->isChecked(), m_ui_usergroup->kcfg_App5enabledPD, m_ui_usergroup->kcfg_AppName5PD, m_ui_usergroup->kcfg_App5maxDhr, m_ui_usergroup->kcfg_App5maxDmin, m_ui_usergroup->kcfg_App5maxWhr,  m_ui_usergroup->kcfg_App5maxWmin);
   
}



void  UserGroupPageS::fillUi_weekday_rulerLT(KRuler *dayruler, QGraphicsView *daygraphics, KIntNumInput *dayfrom, KIntNumInput *dayto)
{
//    m_ui_usergroup->kcfg_username->insertItem(-1, mySettings->usernamestring());
//    m_ui_usergroup->kcfg_username->insertItems (1, this->getlistofusers());
//    m_ui_usergroup->labelA->setText(m_ui_usergroup->labelA->text() + " " + numberstring);
    dayruler->setRulerMetricStyle(KRuler::Custom);
    dayruler->setMediumMarkDistance(8);
    dayruler->setOffset(0);
    daygraphics->setMouseTracking(TRUE);
    daygraphics->viewport()->setMouseTracking(TRUE);
    daygraphics->setInteractive(TRUE);
    daygraphics->setFocusPolicy(Qt::StrongFocus);
    daygraphics->show();
    dayfrom->hide();
    dayto->hide();
}





void  UserGroupPageS::fillUi_rulerLT()
{
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_DAYruler, m_ui_usergroup->kcfg_DAYgraphics, m_ui_usergroup->kcfg_DAYfrom, m_ui_usergroup->kcfg_DAYto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_MONruler, m_ui_usergroup->kcfg_MONgraphics, m_ui_usergroup->kcfg_MONfrom, m_ui_usergroup->kcfg_MONto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_TUEruler, m_ui_usergroup->kcfg_TUEgraphics, m_ui_usergroup->kcfg_TUEfrom, m_ui_usergroup->kcfg_TUEto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_WEDruler, m_ui_usergroup->kcfg_WEDgraphics, m_ui_usergroup->kcfg_WEDfrom, m_ui_usergroup->kcfg_WEDto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_THUruler, m_ui_usergroup->kcfg_THUgraphics, m_ui_usergroup->kcfg_THUfrom, m_ui_usergroup->kcfg_THUto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_FRIruler, m_ui_usergroup->kcfg_FRIgraphics, m_ui_usergroup->kcfg_FRIfrom, m_ui_usergroup->kcfg_FRIto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_SATruler, m_ui_usergroup->kcfg_SATgraphics, m_ui_usergroup->kcfg_SATfrom, m_ui_usergroup->kcfg_SATto);
    fillUi_weekday_rulerLT(m_ui_usergroup->kcfg_SUNruler, m_ui_usergroup->kcfg_SUNgraphics, m_ui_usergroup->kcfg_SUNfrom, m_ui_usergroup->kcfg_SUNto);

// check for same restrictions every day / enabled
/*   if (m_UserGroupSettings->enabled()) {
     if (m_UserGroupSettings->sameRestrictLT())  {
        setDifferentSettingsLT(TRUE, FALSE);
        setSameSettingsLT(TRUE, TRUE);
     } else {
        setDifferentSettingsLT(TRUE, TRUE);
        setSameSettingsLT(TRUE, FALSE);
     };
   } else {
     setDifferentSettingsLT(FALSE, FALSE);
     setSameSettingsLT(FALSE, FALSE);
   }
*/
}






void  UserGroupPageS::fillUi_rulerPT()
{
    fillUi_app_rulerPT(m_ui_usergroup->kcfg_App1ruler, m_ui_usergroup->kcfg_App1graphics, m_ui_usergroup->kcfg_App1from, m_ui_usergroup->kcfg_App1to);
    fillUi_app_rulerPT(m_ui_usergroup->kcfg_App2ruler, m_ui_usergroup->kcfg_App2graphics, m_ui_usergroup->kcfg_App2from, m_ui_usergroup->kcfg_App2to);
    fillUi_app_rulerPT(m_ui_usergroup->kcfg_App3ruler, m_ui_usergroup->kcfg_App3graphics, m_ui_usergroup->kcfg_App3from, m_ui_usergroup->kcfg_App3to);
    fillUi_app_rulerPT(m_ui_usergroup->kcfg_App4ruler, m_ui_usergroup->kcfg_App4graphics, m_ui_usergroup->kcfg_App4from, m_ui_usergroup->kcfg_App4to);
    fillUi_app_rulerPT(m_ui_usergroup->kcfg_App5ruler, m_ui_usergroup->kcfg_App5graphics, m_ui_usergroup->kcfg_App5from, m_ui_usergroup->kcfg_App5to);
}








void  UserGroupPageS::fillUi_app_rulerPT(KRuler *appruler, QGraphicsView *appgraphics, KIntNumInput *appfrom, KIntNumInput *appto)
{
    appruler->setRulerMetricStyle(KRuler::Custom);
    appruler->setMediumMarkDistance(8);
    appruler->setOffset(0);
    appgraphics->setMouseTracking(TRUE);
    appgraphics->viewport()->setMouseTracking(TRUE);
    appgraphics->setInteractive(TRUE);
    appgraphics->setFocusPolicy(Qt::StrongFocus);
    appgraphics->show();
    appfrom->hide();
    appto->hide();
}

