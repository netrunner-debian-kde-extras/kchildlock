/***************************************************************************
 *   Copyright (C) 2011 by Rene Landert <rene.landert@bluewin.ch>          *
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
 **************************************************************************/

#include "KchildlockDaemon.h"
#include <KAboutData>
#include <KPluginFactory>
#include <klocalizedstring.h>
#include <KComponentData>
#include "../common/kchildlockcommon.h"
#include "systray_status.h"
#include <KUserGroup>


K_PLUGIN_FACTORY(KchildlockFactory,
                 registerPlugin<KchildlockDaemon>();)
K_EXPORT_PLUGIN(KchildlockFactory("kchildlockdaemon"))


// the main program 
//=================

KchildlockDaemon::KchildlockDaemon(QObject *parent, const QList<QVariant>&)
        : KDEDModule(parent)
{
  
    QDate currdate;
    QTime currtime;
  
    KAboutData aboutData("kchildlockdaemon", "kchildlockdaemon", ki18n("Kchildlock Daemon"),
                         KCHILDLOCK_VERSION, ki18n("A Daemon for Kchildlock for KDE4"),
                         KAboutData::License_GPL, ki18n("(c), 2011 Rene Landert"),
                         KLocalizedString(), "This daemon "
                             "restricts the usage time of the computer per day and "
                             "per week for selected users.");

    aboutData.addAuthor(ki18n("Rene Landert"), ki18n("Maintainer"),  "rene.landert@bluewin.ch", "http://www.sourceforge.net/kchildlock");
    KGlobal::locale()->insertCatalog("kchildlock");
    
    KComponentData applicationData;
    applicationData = KComponentData(aboutData);
 

    notificationTimer = new QTimer(this);
    startupTimer = new QTimer(this);
    scanTimer = new QTimer(this);

    QString enable_debuglog_filename = debuglog_filename_part1;
    enable_debuglog_filename.append(debuglog_filename_part3);
    QFile enable_debuglog_file ( enable_debuglog_filename );
    if ( enable_debuglog_file.exists() ) {
       debugflag = true;
       QString debuglog_filename = debuglog_filename_part1;
       QDateTime currdt = QDateTime::currentDateTime ();
       debuglog_filename.append(currdt.toString ("yyyyMMdd_hhmmss"));
       debuglog_filename.append(debuglog_filename_part3);
       debugfile = fopen ( debuglog_filename.toAscii(), "a+");
       }
    else
       debugflag = false;
    
    if (debugflag) {
       fprintf (debugfile,"\nStarting kchildlock daemon (version %s) ...\n", KCHILDLOCK_VERSION);
       fprintf (debugfile,"date and time is %i.%i.%i. %i:%i:%i\n",
	QDate::currentDate().day(), QDate::currentDate().month(), QDate::currentDate().year(),
	QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
       fflush(debugfile);
       }


    connect(startupTimer, SIGNAL(timeout()), this, SLOT(startupTimePassed()));
    startupTimer->start(5 * 1000);

}
    

// Destroy the object
// ==================

KchildlockDaemon::~KchildlockDaemon()
{
    delete ckSessionInterface;
}




// after the initial delay begin work
// ==================================
void KchildlockDaemon::startupTimePassed()
{

   bool retvalue;
   int idx;
   
   if (debugflag) {
      fprintf (debugfile,"\nafter the initial timeout...\n");
      fprintf (debugfile,"date and time is %i.%i.%i. %i:%i:%i\n",
	QDate::currentDate().day(), QDate::currentDate().month(), QDate::currentDate().year(),
	QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
      fflush (debugfile);
      }

   startupTimer->stop();

//  First things first: let's set up Kchildlock to be aware of active session
    current_user = new CurrentUserState();
    QDBusConnection conn = QDBusConnection::systemBus();
    setUpConsoleKit();

   get_active_user (&(current_user->user), &(current_user->disp));
   my_consumption = new Consumption();
   retvalue = my_consumption->exists_record(current_user->user);

   if (debugflag) {
      fprintf (debugfile,"filename %s\n", my_consumption->get_consumption_filename().toAscii().constData());
      fflush(debugfile);
      }

// get values from consumed time file
    if ( retvalue )         // record found in file
    {
            my_consumption->read_record();

            if (debugflag) {
               fprintf (debugfile,"consumed times values from file;\n");
               fprintf (debugfile,"woy=%i. dow=%i. usd=%i, usw=%i\n", my_consumption->my_cons_record.pcu.weekofyear, my_consumption->my_cons_record.pcu.dayofweek, my_consumption->my_cons_record.pcu.pcusage_daily, my_consumption->my_cons_record.pcu.pcusage_weekly);
               for (idx=0; idx<KCH_NUMOFAPPS; idx++) {
                   fprintf (debugfile,"appPT=%s, appPD=%s, usd=%i, usw=%i\n", my_consumption->my_cons_record.app[idx].appnamePT, my_consumption->my_cons_record.app[idx].appnamePD, my_consumption->my_cons_record.app[idx].appusage_daily, my_consumption->my_cons_record.app[idx].appusage_weekly);
               }
               fflush(debugfile);
            }
            
      }
      else
// set default values, if no record was found in file
      {
         my_consumption->create_record(current_user->user);
         if (debugflag) {
            fprintf (debugfile,"User %s gets default consumed times values without file;\n", (current_user->user).toAscii().constData());
            fprintf (debugfile,"woy=%i. dow=%i. usd=%i, usw=%i\n", my_consumption->my_cons_record.pcu.weekofyear, my_consumption->my_cons_record.pcu.dayofweek, my_consumption->my_cons_record.pcu.pcusage_daily, my_consumption->my_cons_record.pcu.pcusage_weekly);
            fflush(debugfile);
          }
      }

   if ( my_consumption->my_cons_record.pcu.weekofyear != QDate::currentDate().weekNumber() )
      {
      // its a new week, reset all consumed times values to zero
      my_consumption->read_record();
      my_consumption->set_default_values(current_user->user);
      my_consumption->write_record();
      if (debugflag) {
         fprintf (debugfile,"User %s gets default consumed times values for a new week;\n", (current_user->user).toAscii().constData());
         fprintf (debugfile,"woy=%i. dow=%i. usd=%i, usw=%i\n", my_consumption->my_cons_record.pcu.weekofyear, my_consumption->my_cons_record.pcu.dayofweek, my_consumption->my_cons_record.pcu.pcusage_daily, my_consumption->my_cons_record.pcu.pcusage_weekly);
         fflush(debugfile);
         }
      }


    myGeneralSettingsKCG = KSharedConfig::openConfig("kchildlockrc")->group("General");
    UserSettings_filename  = get_UserSettings_filename(myGeneralSettingsKCG.readEntry("usernamestringlist"));
    GroupSettings_filename  = get_GroupSettings_filename(myGeneralSettingsKCG.readEntry("groupnamestringlist"));

    if (UserSettings_filename=="" && GroupSettings_filename=="") return;     // do nothing for this user
    if (UserSettings_filename != "") {
       myUserSettingsKCG = KSharedConfig::openConfig(UserSettings_filename)->group("UserGroup");
    }
    if (GroupSettings_filename != "") {
       myGroupSettingsKCG = KSharedConfig::openConfig(GroupSettings_filename)->group("UserGroup");
    }

// get global config settings
    scanint =  myGeneralSettingsKCG.readEntry("scaninterval", "60").toInt();
    if (myGeneralSettingsKCG.readEntry("enable_restrictions", "false") == "false") {
       return;		// do nothing if disabled
    }
    
// create limits object
   current_limits = new KchildlockLimits();
   monitor_user();

    if (myGeneralSettingsKCG.readEntry("enable_systray", "false") == "true") {
      if (!current_limits->get_nolimits()) {
         MainWindow *window = new MainWindow(0, current_limits, my_consumption);
      }
    }
   
  connect(scanTimer, SIGNAL(timeout()), this, SLOT(scanTimePassed()));
  scanTimer->start(1000 * scanint);

}




// after each scanintervall, moitor the system
// ===========================================
void KchildlockDaemon::scanTimePassed()
{
   scanTimer->stop();
   scanTimer->start(1000 * scanint);
   if (debugflag) {
      fprintf (debugfile,"\nafter the repeating timeout...\n");
      fprintf (debugfile,"date and time is %i.%i.%i. %i:%i:%i\n",
	QDate::currentDate().day(), QDate::currentDate().month(), QDate::currentDate().year(),
	QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
      fprintf (debugfile,"actual user is %s\n", current_user->user.toAscii().constData());
      fflush(debugfile);
      }

   get_active_user (&(current_user->user), &(current_user->disp));
   monitor_user();

}




// the active session has changed, get new active User
// ===================================================
void KchildlockDaemon::activeSessionChanged()
{
// get the active user
   get_active_user (&(current_user->user), &(current_user->disp));
   current_user->set_warn_usr_state(KCHILDLOCK_NOWARNSTATE);
   current_user->set_warn_app_state(KCHILDLOCK_NOWARNSTATE);

}




// setup the Communication to the console Kit to be informed about active session
// ==============================================================================
void KchildlockDaemon::setUpConsoleKit()
{
    // Let's cache the needed information to check if our session is actually active

    if (!QDBusConnection::systemBus().interface()->isServiceRegistered("org.freedesktop.ConsoleKit")) {
        // No way to determine if we are on the current session, simply suppose we are
        kDebug() << "Can't contact ck";
        ckAvailable = false;
        return;
    } else {
        ckAvailable = true;
    }

    // Otherwise, let's ask ConsoleKit
    QDBusInterface ckiface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager",
                           "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());

    QDBusReply<QDBusObjectPath> sessionPath = ckiface.call("GetCurrentSession");

    if (!sessionPath.isValid() || sessionPath.value().path().isEmpty()) {
        kDebug() << "The session is not registered with ck";
        ckAvailable = false;
        return;
    }

    ckSessionInterface = new QDBusInterface("org.freedesktop.ConsoleKit", sessionPath.value().path(),
            "org.freedesktop.ConsoleKit.Session", QDBusConnection::systemBus());

    if (!ckSessionInterface->isValid()) {
        // As above
        kDebug() << "Can't contact Session Interface";
        ckAvailable = false;
        return;
    }

    /* If everything's fine, let's attach ourself to the ActiveChanged signal.
     * You'll see we're attaching to activeSessionChanged without any further checks.
     * You know why? activeSessionChanged already checks if the console is active, 
     * so the check is already happening n the underhood
     */

    QDBusConnection::systemBus().connect("org.freedesktop.ConsoleKit", sessionPath.value().path(),                                     "org.freedesktop.ConsoleKit.Session", "ActiveChanged", this, SLOT(activeSessionChanged()));

// get the active user
   get_active_user (&(current_user->user), &(current_user->disp));

}



// get the filename for the User Settings of the active user
// =========================================================
QString KchildlockDaemon::get_UserSettings_filename(QString usernamestringlist)
{
  QStringList  usernames = usernamestringlist.split(",");
  QString retval;
  if (usernames.indexOf(current_user->get_user()) >= 0) {
    retval = "kchildlockrc_U_" + current_user->get_user();
  } else {
    retval = "";
  }
  return(retval);
}


// get the filename for the Group Settings of the active user (first group the user belongs to)
// ============================================================================================
QString KchildlockDaemon::get_GroupSettings_filename(QString groupnamestringlist)
{
   QStringList  groupnames = groupnamestringlist.split(",");
   QStringList myuserlist;
   QString retval = "";
   KUserGroup mygroup;
   bool ismember;

  for ( QStringList::Iterator it = groupnames.begin(); it != groupnames.end(); ++it ) {
     mygroup = KUserGroup(*it);
     myuserlist = mygroup.userNames();
     ismember = FALSE;
     for ( QStringList::Iterator it2 = myuserlist.begin(); it2 != myuserlist.end(); ++it2 ) {
       if ((*it2) == current_user->get_user()) {
         ismember = TRUE;
         break;
       }
     }
     if (ismember) 
        break;
  }
  if (ismember) {
     retval = "kchildlockrc_G_" + mygroup.name();
  }
  return(retval);
}





// functions for the handling of the current user
// ==============================================

CurrentUserState::CurrentUserState()
{
   int idx;
   user = "";
   disp = "";
   warn_usr_state = KCHILDLOCK_NOWARNSTATE;
   warn_app_state = KCHILDLOCK_NOWARNSTATE;
   for (idx=0; idx<KCH_NUMOFAPPS; idx++) {
       appisactivePT[idx] = FALSE;
       appisactivePD[idx] = FALSE;
   }
}

CurrentUserState::~CurrentUserState()
{
}

QString CurrentUserState::get_user()
{
   return user;
}

QString CurrentUserState::get_disp()
{
   return disp;
}

void CurrentUserState::set_user(QString newuser)
{
   user = newuser;
}

void CurrentUserState::set_disp(QString newdisp)
{
   disp = newdisp;
}

void CurrentUserState::set_appisactivePT(int idx, bool value)
{
   appisactivePT[idx] = value;
}

bool CurrentUserState::get_appisactivePT(int idx)
{
   return(appisactivePT[idx]);
}

void CurrentUserState::set_appisactivePD(int idx, bool value)
{
   appisactivePD[idx] = value;
}

bool CurrentUserState::get_appisactivePD(int idx)
{
   return(appisactivePD[idx]);
}


#include "moc_KchildlockDaemon.cpp"
