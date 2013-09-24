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


void KchildlockKCM::val_changed(bool val)
{
  emit changed(val);
}

void KchildlockKCM::connect_general(GeneralPageS *my_general)
{
// connections for general page
    connect(my_general->m_ui_general->kcfg_scaninterval, SIGNAL(valueChanged(int)), my_general, SLOT(emitChanged_scaninterval(int)));
    connect(my_general->m_ui_general->kcfg_enable_restrictions, SIGNAL(stateChanged(int)), my_general, SLOT(emitChanged_enable_restrictions(int)));
    connect(my_general->m_ui_general->kcfg_enable_systray, SIGNAL(stateChanged(int)), my_general, SLOT(emitChanged_enable_systray(int)));
    connect(my_general, SIGNAL(changed(bool)), this, SLOT(val_changed(bool)));
}


void KchildlockKCM::connect_usergroup_part1(UserGroupPageS *my_usergroup)
{
//connections for user page
    connect(my_usergroup->m_ui_usergroup->pushButton_new, SIGNAL(released()), this, SLOT(emitNewUserGroup()));
    connect(my_usergroup->m_ui_usergroup->pushButton_del, SIGNAL(released()), this, SLOT(emitDelUserGroup()));
    connect(my_usergroup->m_ui_usergroup->pushButton_exp, SIGNAL(released()), this, SLOT(emitExpUserGroup()));
    connect(my_usergroup->m_ui_usergroup->pushButton_imp, SIGNAL(released()), this, SLOT(emitImpUserGroup()));

    connect(my_usergroup->m_ui_usergroup->kcfg_enabled, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_enabled_usergroup(int)));
//    connect(my_usergroup->m_ui_usergroup->kcfg_usergroupname, SIGNAL(activated(QString)), my_usergroup, SLOT(emitChanged_username_user(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_sameRestrictLT, SIGNAL(toggled(bool)), my_usergroup, SLOT(emitChanged_sameRestrictLT_usergroup(bool)));
    connect(my_usergroup->m_ui_usergroup->kcfg_diffRestrictLT, SIGNAL(toggled(bool)), my_usergroup, SLOT(emitChanged_diffRestrictLT_usergroup(bool)));
    connect(my_usergroup->m_ui_usergroup->kcfg_sameRestrictTD, SIGNAL(toggled(bool)), my_usergroup, SLOT(emitChanged_sameRestrictTD_usergroup(bool)));
    connect(my_usergroup->m_ui_usergroup->kcfg_diffRestrictTD, SIGNAL(toggled(bool)), my_usergroup, SLOT(emitChanged_diffRestrictTD_usergroup(bool)));

    connect(my_usergroup->m_ui_usergroup->kcfg_DAYfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_DAYto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_DAYmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_DAYmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_MONenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_MONenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_MONfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_MONto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_MONmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_MONmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_TUEenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_TUEenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_TUEfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_TUEto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_TUEmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_TUEmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_WEDenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEDenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEDfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEDto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEDmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEDmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_THUenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_THUenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_THUfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_THUto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_THUmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_THUmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_FRIenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_FRIenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_FRIfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_FRIto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_FRImaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_FRImaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_SATenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SATenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SATfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SATto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SATmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SATmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_SUNenabledLT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledLT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SUNenabledTD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SUNfrom, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SUNto, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SUNmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_SUNmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_WEEKenabled, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_DAYenabledTD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEEKmaxhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_WEEKmaxmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_value_usergroup(int)));


    connect(my_usergroup->m_ui_usergroup->kcfg_App1enabledPT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName1PT, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePT_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1from, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1to, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1enabledPD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName1PD, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePD_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1maxDhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1maxDmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1maxWhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App1maxWmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_App2enabledPT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName2PT, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePT_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2from, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2to, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2enabledPD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName2PD, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePD_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2maxDhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2maxDmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2maxWhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App2maxWmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_App3enabledPT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName3PT, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePT_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3from, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3to, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3enabledPD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName3PD, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePD_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3maxDhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3maxDmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3maxWhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App3maxWmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_App4enabledPT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName4PT, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePT_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4from, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4to, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4enabledPD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName4PD, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePD_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4maxDhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4maxDmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4maxWhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App4maxWmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));

    connect(my_usergroup->m_ui_usergroup->kcfg_App5enabledPT, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName5PT, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePT_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5from, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5to, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePT_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5enabledPD, SIGNAL(stateChanged(int)), my_usergroup, SLOT(emitChanged_appenabledPD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_AppName5PD, SIGNAL(textChanged(QString)), my_usergroup, SLOT(emitChanged_appnamePD_usergroup(QString)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5maxDhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5maxDmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5maxWhr, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
    connect(my_usergroup->m_ui_usergroup->kcfg_App5maxWmin, SIGNAL(valueChanged(int)), my_usergroup, SLOT(emitChanged_apptimePD_usergroup(int)));
   
    connect(my_usergroup, SIGNAL(changed(bool)), this, SLOT(val_changed(bool)));
    
    connect(my_usergroup->m_ui_usergroup->kcfg_usergroupnamestringlist, SIGNAL(activated(QModelIndex)), this, SLOT(emitChanged_usergroupname(QModelIndex)));
}



void KchildlockKCM::connect_usergroup_part2(KchildlockKCM *thisobj, UserGroupPageS *my_usergroup)
{
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_DAYgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_MONgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_TUEgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_WEDgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_THUgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_FRIgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_SATgraphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_SUNgraphics->scene(), SLOT(slotCleanUpRuler()));

    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_App1graphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_App2graphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_App3graphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_App4graphics->scene(), SLOT(slotCleanUpRuler()));
    connect(thisobj, SIGNAL(cleanUpRuler()), my_usergroup->m_ui_usergroup->kcfg_App5graphics->scene(), SLOT(slotCleanUpRuler()));

    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_DAYgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_MONgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_TUEgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_WEDgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_THUgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_FRIgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_SATgraphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_SUNgraphics->scene(), SLOT(slotUpdateMe()));

    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_App1graphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_App2graphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_App3graphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_App4graphics->scene(), SLOT(slotUpdateMe()));
    connect(thisobj, SIGNAL(updateMe()), my_usergroup->m_ui_usergroup->kcfg_App5graphics->scene(), SLOT(slotUpdateMe()));
    
}
