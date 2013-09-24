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
 ***************************************************************************
 *                                                                         *
 *   Part of the code in this file was taken from KDE4Powersave and/or     *
 *   Lithium, where noticed.                                               *
 *                                                                         *
 **************************************************************************/

#include "KchildlockDaemon.h"

#include <kdebug.h>
#include <kuser.h>





// do the monitoring of usage for a given user
// ===========================================
void KchildlockDaemon::monitor_user()
{
  QDate actdate;
  QTime acttime;

  if (ckAvailable == false) return;                     // no console kit
  if (current_user->user == "") return;	        // no session active
  actdate = QDate::currentDate();
  acttime = QTime::currentTime();
  if (my_consumption->my_cons_record.pcu.weekofyear != actdate.weekNumber())
     {
     // its a new week, reset all daily/weekly working set values to zero
     my_consumption->set_default_values(current_user->user);
     }
  if (my_consumption->my_cons_record.pcu.dayofweek != actdate.dayOfWeek())
      {
      // its a new day, reset all daily working set values to zero
      my_consumption->reset_daily_values();
      current_user->set_warn_usr_state(KCHILDLOCK_NOWARNSTATE);
      current_user->set_warn_app_state(KCHILDLOCK_NOWARNSTATE);
      }

   if (debugflag) {
      fprintf (debugfile,"function=monitor_user user=%s at %i.%i.%i %i:%i:%i\n", current_user->user.toAscii().constData(),
               actdate.day(), actdate.month(), actdate.year(), acttime.hour(), acttime.minute(), acttime.second());
      fflush(debugfile);
      }

// check for pc usage limits and for application usage limits
     current_limits->set_nolimits(TRUE);
     current_limits->get_current_userlimits (current_user->user, &myUserSettingsKCG, &myGroupSettingsKCG, UserSettings_filename, GroupSettings_filename, my_consumption);

     if (!current_limits->get_nolimits()) {
        check_user_against_limits(current_limits, current_user->user
        );
     }
}





// get the current active KDE User
// ===============================
void KchildlockDaemon::get_active_user(QString *m_actusr, QString *m_dispid)
{

/* Just to remember:  the command line is:

# dbus-send --system --dest=org.freedesktop.ConsoleKit   --type=method_call --print-reply --reply-timeout=2000   /org/freedesktop/ConsoleKit/Seat1   org.freedesktop.ConsoleKit.Seat.GetActiveSession
method return sender=:1.1 -> dest=:1.71 reply_serial=2
Answer:   object path "/org/freedesktop/ConsoleKit/Session1"

dbus-send --system --dest=org.freedesktop.ConsoleKit   --type=method_call --print-reply --reply-timeout=2000   /org/freedesktop/ConsoleKit/Session1   org.freedesktop.ConsoleKit.Session.GetUnixUser
method return sender=:1.1 -> dest=:1.56 reply_serial=2
   int32 1000
*/

   int userid;
   bool okcode;

    QDBusInterface *ckseatiface = new QDBusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Seat1",
                           "org.freedesktop.ConsoleKit.Seat", QDBusConnection::systemBus());

    QDBusReply<QDBusObjectPath> actsessionPath = ckseatiface->call("GetActiveSession");
    if (debugflag) {
	fprintf (debugfile,"function=get_active_user 1: actsessionPath=%s\n", actsessionPath.value().path().toAscii().constData());
                fflush (debugfile);
	}

    if (!actsessionPath.isValid() || actsessionPath.value().path().isEmpty()) {
        kDebug() << "The active session is not registered with ConsoleKit";
        ckAvailable = false;
        if (debugflag) {
	fprintf (debugfile,"function=get_active_user 2: no active session\n");
                fflush (debugfile);
                }
        userid = 0;
        *m_actusr="";
        *m_dispid="";
        return;
        
    }
    ckseatiface->~QDBusInterface();

    QDBusInterface *cksessioniface = new QDBusInterface("org.freedesktop.ConsoleKit", actsessionPath.value().path(),
            "org.freedesktop.ConsoleKit.Session", QDBusConnection::systemBus());

//    QDBusReply<int> userid = cksessioniface->call("GetUnixUser");
    QDBusMessage reply = cksessioniface->call("GetUnixUser");
    if (reply.type() == QDBusMessage::ErrorMessage) {
       userid = 0;
       QDBusError err = reply;
       if (debugflag) {
	fprintf (debugfile,"function=get_active_user: Error %s\n%s\n", (err.name()).toAscii().constData(), (err.message()).toAscii().constData());
                fflush (debugfile);
	        }
    } else if (reply.type() != QDBusMessage::ReplyMessage) {
       if (debugflag) {
	fprintf (debugfile,"function=get_active_user: Error Invalid reply type %d\n", int(reply.type()));
                fflush(debugfile);
       }
       userid = 0;
    }       
    foreach (QVariant v, reply.arguments())
    {
       if (v.userType() == QVariant::StringList) {
// do nothing with an unexpected answer
       }
       else if (v.userType() == qMetaTypeId<QDBusArgument>()) {
// do nothing with an unexpected answer
      } else {
           userid = (v.toString()).toInt(&okcode, 10);
      } 
    }

    QDBusReply<QString> dispid = cksessioniface->call("GetX11Display");
    KUser my_kuser = KUser((int)userid);
    *m_actusr = my_kuser.loginName();
    *m_dispid = dispid;
    if (debugflag) {
        fprintf (debugfile,"function=get_active_user 4: userid=%i dispid=%s username=%s\n", (int)userid, ((QString)dispid).toAscii().constData(), current_user->get_user().toAscii().constData());
        fflush(debugfile);
        }
    cksessioniface->~QDBusInterface();

//    delete ckseatiface;
//    delete cksessioniface;
//    delete &my_kuser;
}

