/***************************************************************************
 *   Copyright (C) 2011 by Rene Landert <rene.landert@bluewin.ch>          *
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


UserGroupPageS::UserGroupPageS(QWidget *parent, Ui_usergroupPage *ui_usergroup, KchildlockSettings *myUserGroupSettings, KchildlockSettings *myGeneralSettings)
{
    initial_fill = true;
    m_parent = parent;
    m_ui_usergroup = ui_usergroup;
    m_UserGroupSettings = myUserGroupSettings;
    m_GeneralSettings = myGeneralSettings;
}


UserGroupPageS::~UserGroupPageS()
{
}

void UserGroupPageS::load(QString U_or_G, QString curr_usergroup)
{
  QString newconfig = "kchildlockrc_";
//  if (curr_usergroup != "") {
    newconfig = ((newconfig.append(U_or_G)).append("_")).append(curr_usergroup);
//  }
  if (!(m_UserGroupSettings->config() == 0)) {
    m_UserGroupSettings->config()->~KConfig();
  }
  KSharedConfigPtr newConfigPtr = KSharedConfig::openConfig(newconfig, KConfig::SimpleConfig);
  m_UserGroupSettings->setSharedConfig(newConfigPtr);
  m_UserGroupSettings->readConfig();
  kDebug() << "loading config from file " << newconfig;
}

void UserGroupPageS::save()
{
  m_UserGroupSettings->writeConfig();
  kDebug() << "saving config to file ";
}


void  UserGroupPageS::setupUi_rulerLT()
{
//    m_ui_usergroup->kcfg_username->insertItem(-1, mySettings->usernamestring());
//    m_ui_usergroup->kcfg_username->insertItems (1, this->getlistofusers());
//    m_ui_usergroup->labelA->setText(m_ui_usergroup->labelA->text() + " " + numberstring);
    my_scene = new MyScene(m_ui_usergroup->widget_DAY, m_ui_usergroup->kcfg_DAYgraphics->geometry(), m_ui_usergroup->kcfg_DAYfrom, m_ui_usergroup->kcfg_DAYto );
    m_ui_usergroup->kcfg_DAYgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_MON, m_ui_usergroup->kcfg_MONgraphics->geometry(), m_ui_usergroup->kcfg_MONfrom, m_ui_usergroup->kcfg_MONto );
    m_ui_usergroup->kcfg_MONgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_TUE, m_ui_usergroup->kcfg_TUEgraphics->geometry(), m_ui_usergroup->kcfg_TUEfrom, m_ui_usergroup->kcfg_TUEto );
    m_ui_usergroup->kcfg_TUEgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_WED, m_ui_usergroup->kcfg_WEDgraphics->geometry(), m_ui_usergroup->kcfg_WEDfrom, m_ui_usergroup->kcfg_WEDto );
    m_ui_usergroup->kcfg_WEDgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_THU, m_ui_usergroup->kcfg_THUgraphics->geometry(), m_ui_usergroup->kcfg_THUfrom, m_ui_usergroup->kcfg_THUto );
    m_ui_usergroup->kcfg_THUgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_FRI, m_ui_usergroup->kcfg_FRIgraphics->geometry(), m_ui_usergroup->kcfg_FRIfrom, m_ui_usergroup->kcfg_FRIto );
    m_ui_usergroup->kcfg_FRIgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_SAT, m_ui_usergroup->kcfg_SATgraphics->geometry(), m_ui_usergroup->kcfg_SATfrom, m_ui_usergroup->kcfg_SATto );
    m_ui_usergroup->kcfg_SATgraphics->setScene(my_scene);

    my_scene = new MyScene(m_ui_usergroup->widget_SUN, m_ui_usergroup->kcfg_SUNgraphics->geometry(), m_ui_usergroup->kcfg_SUNfrom, m_ui_usergroup->kcfg_SUNto );
    m_ui_usergroup->kcfg_SUNgraphics->setScene(my_scene);
}




void  UserGroupPageS::setupUi_rulerPT()
{
    my_scene = new MyScene(m_ui_usergroup->widget_App1, m_ui_usergroup->kcfg_App1graphics->geometry(), m_ui_usergroup->kcfg_App1from, m_ui_usergroup->kcfg_App1to );
    m_ui_usergroup->kcfg_App1graphics->setScene(my_scene);
    my_scene = new MyScene(m_ui_usergroup->widget_App2, m_ui_usergroup->kcfg_App2graphics->geometry(), m_ui_usergroup->kcfg_App2from, m_ui_usergroup->kcfg_App2to );
    m_ui_usergroup->kcfg_App2graphics->setScene(my_scene);
    my_scene = new MyScene(m_ui_usergroup->widget_App3, m_ui_usergroup->kcfg_App3graphics->geometry(), m_ui_usergroup->kcfg_App3from, m_ui_usergroup->kcfg_App3to );
    m_ui_usergroup->kcfg_App3graphics->setScene(my_scene);
    my_scene = new MyScene(m_ui_usergroup->widget_App4, m_ui_usergroup->kcfg_App4graphics->geometry(), m_ui_usergroup->kcfg_App4from, m_ui_usergroup->kcfg_App4to );
    m_ui_usergroup->kcfg_App4graphics->setScene(my_scene);
    my_scene = new MyScene(m_ui_usergroup->widget_App5, m_ui_usergroup->kcfg_App5graphics->geometry(), m_ui_usergroup->kcfg_App5from, m_ui_usergroup->kcfg_App5to );
    m_ui_usergroup->kcfg_App5graphics->setScene(my_scene);
}






void UserGroupPageS::set_initial_fill(bool val)
{
  initial_fill = val;
}



#include "moc_UserGroupPageS.cpp"