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
 **************************************************************************/

#include "KchildlockDaemon.h"

#include <klocale.h>
#include <kmessagebox.h>



// get and set warning states
// ==========================
int CurrentUserState::get_warn_usr_state()
{
  return (warn_usr_state);
};

void CurrentUserState::set_warn_usr_state(int level)
{
  warn_usr_state = level;
};



// force logout of the active session on the PC
// ============================================
void KchildlockDaemon::force_logout_session()
{
/* just to remember: the command line is:

# dbus-send --system --dest=org.freedesktop.ConsoleKit   --type=method_call --print-reply --reply-timeout=2000   /org/freedesktop/ConsoleKit/Seat1   org.freedesktop.ConsoleKit.Seat.GetActiveSession
method return sender=:1.1 -> dest=:1.71 reply_serial=2
Answer:   object path "/org/freedesktop/ConsoleKit/Session1"

dbus-send --system --dest=org.freedesktop.ConsoleKit   --type=method_call --print-reply --reply-timeout=2000   /org/freedesktop/ConsoleKit/Manager   org.freedesktop.ConsoleKit.Manager.CloseSession
method return sender=:1.1 -> dest=:1.56 reply_serial=2
   int32 1000

dbus-send --session --dest=org.kde.ksmserver  --type=method_call --print-reply --reply-timeout=2000   /KSMServer   org.kde.KSMServerInterface.logout
method return sender=:1.1 -> dest=:1.56 reply_serial=2
   int32 1int32 1 int32 1
*/
    QDBusInterface *ckmgriface = new QDBusInterface("org.kde.ksmserver", "/KSMServer",
                           "org.kde.KSMServerInterface", QDBusConnection::sessionBus());

//    QString sessioncookie = getenv ("XDG_SESSION_COOKIE");

//    QDBusReply<bool> result = ckmgriface->call(QLatin1String("CloseSession"), sessioncookie);
    QDBusReply<QString> reply = ckmgriface->call(QLatin1String("logout"),0,0,2);
    reply = ckmgriface->call(QLatin1String("logout"),0,0,2);

/*
The three numbers are explained below (see source):
First parameter: confirm
 Obey the user’s confirmation setting: -1
 Don’t confirm, shutdown without asking: 0
 Always confirm, ask even if the user turned it off: 1
 Second parameter: type
 Select previous action or the default if it’s the first time: -1
 Only log out: 0
 Log out and reboot the machine: 1
 Log out and halt the machine: 2
 Third parameter: mode
 Select previous mode or the default if it’s the first time: -1
 Schedule a shutdown (halt or reboot) for the time all active sessions have
 exited: 0
 Shut down, if no sessions are active. Otherwise do nothing: 1
 Force shutdown. Kill any possibly active sessions: 2
 Pop up a dialog asking the user what to do if sessions are still active: 3
*/ 

     if (debugflag) {
        QDate actdate = QDate::currentDate();
        QTime acttime = QTime::currentTime();
        fprintf (debugfile,"function=force_logout_session user=%s disp=%s replyvalid=%i reply=%s at %i.%i. %i:%i:%i\n",
	  current_user->get_user().toAscii().constData(), current_user->get_disp().toAscii().constData(), 
	  reply.isValid(), reply.value().toAscii().constData(), 
                  actdate.day(), actdate.month(), acttime.hour(), acttime.minute(), acttime.second());
      fflush(debugfile);
    }

}





// display a warning message and eventually logout the user
// ========================================================
void KchildlockDaemon::warning_or_logout_session (int warn_level)
{
		
// set the environment and display the notification to the user
   QDate actdate = QDate::currentDate();
   QTime acttime = QTime::currentTime();

   if (debugflag) 
      fprintf (debugfile,"function=warning_or_logout_session begin user=%s disp=%s warnlevel=%i at %i.%i. %i:%i:%i\n",
              current_user->get_user().toAscii().constData(), current_user->get_disp().toAscii().constData(), warn_level, 
              actdate.day(), actdate.month(), acttime.hour(), acttime.minute(), acttime.second());

   if (warn_level == KCHILDLOCK_WARNSTATE3)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, 
	I18N_NOOP("The Usage time for you is restricted. You will be automatically logged out in 15 minutes."), 
                I18N_NOOP("Children Lock"));
      }
   else if (warn_level == KCHILDLOCK_WARNSTATE2)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, 
	I18N_NOOP("The Usage time for you is restricted. You will be automatically logged out in 10 minutes."), 
                I18N_NOOP("Children Lock"));
      }
   else if (warn_level == KCHILDLOCK_WARNSTATE1)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, 
                I18N_NOOP("The Usage time for you is restricted. You will be automatically logged out in 5 minutes."), 
                I18N_NOOP("Children Lock"));
      }
   else if (warn_level == KCHILDLOCK_WARNSTATE0)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, 
	I18N_NOOP("The Usage time for you is restricted. You will be automatically logged out in 1 minute."), 
	I18N_NOOP("Children Lock"));
      } 
   else
      {
      // wrong calling parameter
      }


// logout the user after 15 seconds
   if (warn_level == KCHILDLOCK_WARNSTATE00)
      {
//    sleep (15);
      if (debugflag) 
         fprintf (debugfile,"function=warning_or_logout_session end at %i.%i. %i:%i:%i\n", 
              actdate.day(), actdate.month(), acttime.hour(), acttime.minute(), acttime.second());

      force_logout_session();
	
      }
	
}

