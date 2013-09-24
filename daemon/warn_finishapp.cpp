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
#include <unistd.h>



// get and set warning states
// ==========================
int CurrentUserState::get_warn_app_state()
{
  return (warn_app_state);
};

void CurrentUserState::set_warn_app_state(int level)
{
  warn_app_state = level;
};




// force termination of the application
// ====================================
void KchildlockDaemon::force_app_termination(QString appname)
{

   char appcommand[100];

   if (debugflag) {
	fprintf (debugfile,"function=force_app_termination user=%s disp=%s app=%s at %i.%i. %i:%i:%i\n", 
         current_user->get_user().toAscii().constData(), current_user->get_disp().toAscii().constData(), 
         appname.toAscii().constData(),
         QDate::currentDate().day(), QDate::currentDate().month(), 
         QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
   }

   // the shell command is: killall -u 'username' -s SIGNAME 'appname'
   // signals are SIGHUP, SIGQUIT, SIGKILL, SIGTERM

   sprintf(appcommand, "killall -u %s -s SIGHUP %s", current_user->get_user().toAscii().constData(), appname.toAscii().constData());
   system(appcommand);
   sleep(3);
   sprintf(appcommand, "killall -u %s -s SIGQUIT %s", current_user->get_user().toAscii().constData(), appname.toAscii().constData());
   system(appcommand);
   sleep(3);
   sprintf(appcommand, "killall -u %s -s SIGKILL %s", current_user->get_user().toAscii().constData(), appname.toAscii().constData());
   system(appcommand);
   sleep(3);
   sprintf(appcommand, "killall -u %s -s SIGTERM %s", current_user->get_user().toAscii().constData(), appname.toAscii().constData());
   system(appcommand);

}






// display a warning message and eventually finish the application
// ===============================================================
void KchildlockDaemon::warning_or_finish_application (QString appname, int warn_level)
{
	
// set the environment and display the notification to the user

   if (debugflag) 
      fprintf (debugfile,"function=warning_or_finish_application begin user=%s disp=%s warnlevel=%i at %i.%i. %i:%i:%i\n", 
         current_user->get_user().toAscii().constData(), current_user->get_disp().toAscii().constData(), 
         warn_level,
         QDate::currentDate().day(), QDate::currentDate().month(), 
         QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());

   if (warn_level == KCHILDLOCK_WARNSTATE3)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, I18N_NOOP("The Usage time for your application \'")
         + appname + I18N_NOOP("\' is restricted. The application will be automatically terminated in 15 minutes."), 
         I18N_NOOP("Children Lock"));
      }
   else if (warn_level == KCHILDLOCK_WARNSTATE2)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, I18N_NOOP("The Usage time for your application \'") 
         + appname + I18N_NOOP("\' is restricted. The application will be automatically terminated in 10 minutes."), 
         I18N_NOOP("Children Lock"));
      }
   else if (warn_level == KCHILDLOCK_WARNSTATE1)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, I18N_NOOP("The Usage time for your application \'") 
         + appname + I18N_NOOP("\' is restricted. The application will be automatically terminated in 5 minutes."), 
         I18N_NOOP("Children Lock"));
      }
   else if (warn_level == KCHILDLOCK_WARNSTATE0)
      {
      KMessageBox::queuedMessageBox (0, KMessageBox::Error, I18N_NOOP("The Usage time for your application \'") 
         + appname + I18N_NOOP("\' is restricted. The application will be automatically terminated in 1 minute."), 
         I18N_NOOP("Children Lock"));
      } 
   else
      {
      // wrong calling parameter
      }


// logout the user after 20 seconds
   if (warn_level == KCHILDLOCK_WARNSTATE00)
      {
//    sleep (20);
      if (debugflag) 
         fprintf (debugfile,"function=warning_or_finish_application end at %i.%i. %i:%i:%i\n", 
         QDate::currentDate().day(), QDate::currentDate().month(), 
         QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());

      force_app_termination(appname);
      }

}
