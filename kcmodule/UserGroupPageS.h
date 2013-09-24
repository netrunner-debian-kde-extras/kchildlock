/***************************************************************************
 *   Copyright (C) 2010 by Rene Landert <rene.landert@bluewin.ch>          *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef USERGROUPPAGES_H
#define USERGROUPPAGES_H

#include "kchildlock_settings.h"
#include "ui_usergroupPage.h"
#include "ruler_scene.h"

class UserGroupPageS : public KchildlockSettings, public Ui_usergroupPage
{
    Q_OBJECT

public:
    UserGroupPageS(QWidget *parent, Ui_usergroupPage *ui_usergroup, KchildlockSettings *myUserGroupSettings, KchildlockSettings *myGeneralSettings);
    ~UserGroupPageS();
    void fillUi();
    void cleanUi();
    void fillUi_LT();
    void fillUi_TD();
    void fillUi_PT();
    void fillUi_PD();
    void fillUi_rulerLT();
    void fillUi_rulerPT();
    void set_initial_fill(bool val);

    void setDifferentSettingsLT(bool diff_ena, bool enabled);
    void setSameSettingsLT(bool same_ena, bool enabled);
    void setDifferentSettingsTD(bool diff_ena, bool enabled);
    void setSameSettingsTD(bool same_ena, bool enabled);
    void setupUi_weekday_LT(bool enabled, bool isChecked, QGraphicsView *daygraphics, QCheckBox *dayenabled, KIntNumInput *dayfrom, KIntNumInput *dayto);
    void setupUi_weekday_TD(bool enabled, bool isChecked, QCheckBox *dayenabled, QSpinBox *daymaxhr, QSpinBox *daymaxmin);
    void setupUi_app_PT(bool enabled, bool isChecked, QLineEdit *AppName, QGraphicsView *Appgraphics, QCheckBox *Appenabled, KIntNumInput *Appfrom, KIntNumInput *Appto);
    void setupUi_app_PD(bool enabled, bool isChecked, QCheckBox *Appenabled, QLineEdit *Appname, QSpinBox *AppmaxDhr, QSpinBox *AppmaxDmin, QSpinBox *AppmaxWhr, QSpinBox *AppmaxWmin);
    void setupUi_rulerLT();
    void setupUi_rulerPT();
    void fillUi_weekday_rulerLT(KRuler *dayruler, QGraphicsView *daygraphics, KIntNumInput *dayfrom, KIntNumInput *dayto);
    void fillUi_app_rulerPT(KRuler *appruler, QGraphicsView *appgraphics, KIntNumInput *appfrom, KIntNumInput *appto);


    void load(QString U_or_G, QString curr_usergroup);
    void save();
//    void defaults();

public:
  QWidget *m_parent;
  Ui_usergroupPage *m_ui_usergroup;
  KchildlockSettings *m_UserGroupSettings;
  KchildlockSettings *m_GeneralSettings;
  MyScene *my_scene;
  bool initial_fill;


public slots:
    void emitChanged_enabled_usergroup(int);
    void emitChanged_DAYenabledLT_usergroup(int);
//    void emitChanged_usergroupname_usergroup(QString);
    void emitChanged_sameRestrictLT_usergroup(bool);
    void emitChanged_diffRestrictLT_usergroup(bool);
    void emitChanged_DAYenabledTD_usergroup(int);
    void emitChanged_sameRestrictTD_usergroup(bool);
    void emitChanged_diffRestrictTD_usergroup(bool);
    void emitChanged_value_usergroup(int);
    void emitChanged_appenabledPT_usergroup(int);
    void emitChanged_appnamePT_usergroup(QString);
    void emitChanged_apptimePT_usergroup(int);
    void emitChanged_appenabledPD_usergroup(int);
    void emitChanged_appnamePD_usergroup(QString);
    void emitChanged_apptimePD_usergroup(int);
  
signals:
    void changed(bool);
    void updateMe();
    
private slots:
};

#endif /* USERGROUPPAGES_H */
