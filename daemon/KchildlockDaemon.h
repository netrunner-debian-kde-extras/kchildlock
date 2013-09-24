/***************************************************************************
 *   Copyright (C) 2009 by Rene Landert <rene.landert@bluewin.ch>          *
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

#ifndef KCHILDLOCKDAEMON_H
#define KCHILDLOCKDAEMON_H

#define KCHILDLOCK_WARNTIME3 900
#define KCHILDLOCK_WARNTIME2 600
#define KCHILDLOCK_WARNTIME1 300

#define KCHILDLOCK_NOWARNSTATE 10
#define KCHILDLOCK_WARNSTATE3 3
#define KCHILDLOCK_WARNSTATE2 2
#define KCHILDLOCK_WARNSTATE1 1
#define KCHILDLOCK_WARNSTATE0 0
#define KCHILDLOCK_WARNSTATE00 -1

#define KCH_NUMOFAPPS 10            // 5 userapps + 5 groupapps

#define debuglog_filename_part1 "/var/opt/kchildlock/kchildlockd_"
#define debuglog_filename_part3 "_debuglog.tmp"

#include <kdedmodule.h>
#include <KComponentData>
#include <QStringList>
#include "ksmserver_interface.h"
#include <KNotification>
#include "consumption_file.h"
#include "kchildlock_settings.h"




class CurrentUserState
{
  
public:
   CurrentUserState();
   ~CurrentUserState();
   
private:
   int warn_usr_state;
   int warn_app_state;
   bool appisactivePT[KCH_NUMOFAPPS];
   bool appisactivePD[KCH_NUMOFAPPS];
   
public:
    QString user;
    QString disp;
    QString get_user();
    QString get_disp();
    void set_user(QString newuser);
    void set_disp(QString newdisp);
    void set_warn_usr_state(int);
    void set_warn_app_state(int);
    int get_warn_usr_state();
    int get_warn_app_state();
    void set_appisactivePT (int index, bool value);
    bool get_appisactivePT (int index);
    void set_appisactivePD (int index, bool value);
    bool get_appisactivePD (int index);
};   


class KchildlockLimits
{

public:
    KchildlockLimits();
    ~KchildlockLimits();
    
    
private:
   bool no_limits;
   int daily, weekly, from, to;
   char appnamePT[KCH_NUMOFAPPS][KCH_NAME_LENGTH];
   char appnamePD[KCH_NUMOFAPPS][KCH_NAME_LENGTH];
   int app_daily[KCH_NUMOFAPPS];
   int app_weekly[KCH_NUMOFAPPS];
   int app_from[KCH_NUMOFAPPS];
   int app_to[KCH_NUMOFAPPS];
   void get_current_userlimits_TDmax(KConfigGroup *my_UserSettings);
   void get_current_userlimits_LTfromto(KConfigGroup *my_UserSettings);
   void get_current_userlimits_apps(KConfigGroup *my_UserSettings, Consumption *my_consumption);
   void get_current_userlimits_groupTDmax(KConfigGroup *my_UserSettings);
   void get_current_userlimits_groupLTfromto(KConfigGroup *my_UserSettings);
   void get_current_userlimits_groupapps(KConfigGroup *my_UserSettings, Consumption *my_consumption);

public:
    void get_current_userlimits(QString user, KConfigGroup *my_UserSettings, KConfigGroup *my_GroupSettings, QString EmptyUsrName, QString EmptyGrpName, Consumption *my_consumption);
    void set_nolimits(bool value);
    bool get_nolimits();
    int get_lim_du();
    int get_lim_wu();
    int get_lim_from();
    int get_lim_to();
    QString get_appnamePT(int index);
    QString get_appnamePD(int index);
    int get_applim_du(int index);    
    int get_applim_wu(int index);    
    int get_applim_from(int index);    
    int get_applim_to(int index);    

};



class KDE_EXPORT KchildlockDaemon : public KDEDModule
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.Kchildlock")

public:
    KchildlockDaemon(QObject *parent, const QList<QVariant>&);
    virtual ~KchildlockDaemon();
    
    
private:
   CurrentUserState *current_user;
   Consumption *my_consumption;
   KchildlockLimits *current_limits;

    OrgKdeKSMServerInterfaceInterface *ksmServerIface;
    QDBusInterface *ckSessionInterface;
    bool ckAvailable;

    KConfigGroup myGeneralSettingsKCG;
    KConfigGroup myUserSettingsKCG;
    KConfigGroup myGroupSettingsKCG;
    QString UserSettings_filename;
    QString GroupSettings_filename;
    
    QTimer *notificationTimer;
    QTimer *startupTimer;
    QTimer *scanTimer;

    int scanint;	// if zero, then monitoring is disabled
    bool debugflag;
    FILE *debugfile;

public Q_SLOTS:
    void activeSessionChanged();

private Q_SLOTS:
    void startupTimePassed();
    void scanTimePassed();

Q_SIGNALS:
    
private:
    void get_active_user(QString *m_actusr, QString *m_dispid);
    void setUpConsoleKit();
    void monitor_user();
    void check_user_against_limits(KchildlockLimits *current_limits, QString actuser);
    void check_daily_usage_limits();
    void check_daily_app_limits();
    void check_usage_limit_dwu(int secondofday, int limit_to);
    void check_app_limit_dwu(int appusage, int limit, QString appname);
    void warning_or_logout_session(int level);
    void force_logout_session();
    void force_app_termination(QString appname);
    void warning_or_finish_application (QString appname, int warn_level);
    QString get_UserSettings_filename(QString usernamestringlist);
    QString get_GroupSettings_filename(QString groupnamestringlist);
    
private slots:
    
signals:
    void triggered(bool);
};


#endif /*KCHILDLOCKDAEMON_H*/
