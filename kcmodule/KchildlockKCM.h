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

#ifndef KCHILDLOCKKCM_H
#define KCHILDLOCKKCM_H

#include <kcmodule.h>
#include <QDBusConnection>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <kconfig.h>
#include <kpagedialog.h>
#include <kconfiggroup.h>

#include "../common/kchildlockcommon.h"
#include "kchildlock_settings.h"
#include "ui_generalPage.h"
#include "ui_usergroupPage.h"

#define USERIDLOWLIMIT 200
#define GROUPIDLOWLIMIT 100



class QVBoxLayout;
class QWidget;


class KchildlockKCM : public KCModule, public KConfig
{
    Q_OBJECT

public:
    KchildlockKCM(QWidget *parent, const QVariantList& = QVariantList());
    ~KchildlockKCM();
    void load();
    void save();
    QString quickHelp() const;
    QString current_user;
    QString current_group;

    
signals:
     void settingsChanged(const QString& mystr);
     void updateMe();
     void cleanUpRuler();
    
private:
    KPageDialog *dialog;
    KchildlockSettings *myGeneralSettings;    
    KchildlockSettings *myUserSettings;    
    KchildlockSettings *myGroupSettings;    
    Ui_generalPage *ui_general;
    Ui_usergroupPage *ui_user;
    Ui_usergroupPage *ui_group;
    GeneralPageS *general;
    UserGroupPageS *user;
    UserGroupPageS *group;
    KPageWidgetItem *generalPageItem;
    KPageWidgetItem *userPageItem;
    bool initial_load;
    KPageWidgetItem *groupPageItem;
    QString new_usergroup;

    QStringList getlistofusers();
    QStringList getlistofgroups();
    
    void connect_general(GeneralPageS *my_general);
    void connect_usergroup_part1(UserGroupPageS *usergroup);
    void connect_usergroup_part2(KchildlockKCM *thisobj, UserGroupPageS *usergroup);

private slots:

    void emitNewUserGroup();
    void emitDelUserGroup();
    void emitExpUserGroup();
    void emitImpUserGroup();
    void save_usergroupnameOK();
    void save_usergroupnameT(QString);
    void emitChanged_usergroupname(QModelIndex);
    void delete_usergroupnameOK();

  public slots:
    void val_changed(bool);
};


#endif /*KCHILDLOCKKCM_H*/
