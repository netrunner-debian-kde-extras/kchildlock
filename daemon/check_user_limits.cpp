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

#include "KchildlockDaemon.h"
//#include "kchildlock.h"





// check active user against settings for limitations
// ==================================================
void KchildlockDaemon::check_user_against_limits(KchildlockLimits *current_limits, QString actuser)
{

   char appcommand[100];
   FILE *fpipe;
   int idx;
  
   if (debugflag) {
      fprintf (debugfile,"function=check_user_against_limits user=%s disp=%s at %i.%i. %i:%i:%i\n",
         current_user->get_user().toAscii().constData(), current_user->get_disp().toAscii().constData(), 
         QDate::currentDate().day(), QDate::currentDate().month(), 
         QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
      fflush(debugfile);
      }

   //check restrictions
   my_consumption->my_cons_record.pcu.pcusage_daily = my_consumption->my_cons_record.pcu.pcusage_daily + scanint;
   my_consumption->my_cons_record.pcu.pcusage_weekly = my_consumption->my_cons_record.pcu.pcusage_weekly + scanint;

   //check if application is active
   // the shell command is ps ux | grep 'appname'| grep -v grep | wc -l
   for (idx=0; idx<KCH_NUMOFAPPS; idx++) {
      if (((current_limits->get_appnamePD(idx)).toAscii().constData())[0] != 0) {
         appcommand[0]=0;
         sprintf(appcommand, "ps ux | grep \'%s\' | grep -v grep | wc -l", (current_limits->get_appnamePD(idx)).toAscii().constData());
         fpipe = (FILE*)popen(appcommand,"r");
         current_user->set_appisactivePD(idx, fgetc(fpipe) - '0');
         pclose(fpipe);

         if ((current_user->get_appisactivePD(idx) > 0) && !(current_limits->get_appnamePD(idx).isEmpty())) 
            {
            my_consumption->my_cons_record.app[idx].appusage_daily = my_consumption->my_cons_record.app[idx].appusage_daily + scanint;
            my_consumption->my_cons_record.app[idx].appusage_weekly = my_consumption->my_cons_record.app[idx].appusage_weekly + scanint;
            if (debugflag) 
               {
               fprintf (debugfile,"os-command for app1=%s result=%c\n", appcommand, current_user->get_appisactivePD(idx));
               fprintf (debugfile,"app1usage_d=%i app1usage_w=%i\n", my_consumption->my_cons_record.app[idx].appusage_daily, my_consumption->my_cons_record.app[idx].appusage_weekly);
               fflush(debugfile);
               }
            }
      }
   }

   my_consumption->write_record();

// check daily usage limits
   check_daily_usage_limits();
   check_daily_app_limits();
}




// check user against settings for daily and weekly  limitations
// =============================================================
void KchildlockDaemon::check_daily_usage_limits()
{
   int secondofday = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
   if (debugflag) {
      fprintf (debugfile,"function=check_daily_usage_limits user=%s disp=%s at %i.%i. %i:%i:%i\n",
         current_user->get_user().toAscii().constData(), current_user->get_disp().toAscii().constData(), 
         QDate::currentDate().day(), QDate::currentDate().month(), 
         QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
      fprintf (debugfile,"secondofday=%i, lim_du=%i, lim_wu=%i, lim_f=%i, lim_t=%i \n", 
         secondofday, current_limits->get_lim_du(), current_limits->get_lim_wu(), current_limits->get_lim_from(), 
         current_limits->get_lim_to() );
      fprintf (debugfile,"usage_daily=%i, usage_weekly=%i \n", my_consumption->my_cons_record.pcu.pcusage_daily, 
         my_consumption->my_cons_record.pcu.pcusage_weekly);
      fflush(debugfile);
      }

   // check the limit 'daily max usage'
   check_usage_limit_dwu(my_consumption->my_cons_record.pcu.pcusage_daily, current_limits->get_lim_du());
   // check the limit 'weekly max usage'
   check_usage_limit_dwu(my_consumption->my_cons_record.pcu.pcusage_weekly, current_limits->get_lim_wu());
		
   // check the  limit "from hour:minute" (is it to early?)
   if ((secondofday < current_limits->get_lim_from()) && (current_user->get_warn_usr_state() <= KCHILDLOCK_WARNSTATE0))
      {
      current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE00);
      warning_or_logout_session(KCHILDLOCK_WARNSTATE00);
      }
   if ((secondofday < current_limits->get_lim_from()) && (current_user->get_warn_usr_state() > KCHILDLOCK_WARNSTATE0))
      {
      current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE0);
      warning_or_logout_session(KCHILDLOCK_WARNSTATE0);
      }

   // check the  limit "to hour:minute" (is it to late?)
   check_usage_limit_dwu(secondofday, current_limits->get_lim_to());
}





// check user against limit 'daily or weekly max usage'
// ====================================================
void KchildlockDaemon::check_usage_limit_dwu(int actsecondofday, int limit)
{
                if ((actsecondofday >= limit) && (current_user->get_warn_usr_state() <= KCHILDLOCK_WARNSTATE0))
	{
		current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE00);
		warning_or_logout_session(KCHILDLOCK_WARNSTATE00);
	}
	else if ((actsecondofday >= limit) && (current_user->get_warn_usr_state() > KCHILDLOCK_WARNSTATE0))
	{
		current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE0);
		warning_or_logout_session(KCHILDLOCK_WARNSTATE0);
	}
	else if ((actsecondofday >= (limit - KCHILDLOCK_WARNTIME1)) && (current_user->get_warn_usr_state() > KCHILDLOCK_WARNSTATE1))
	{
		current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE1);
		warning_or_logout_session(KCHILDLOCK_WARNSTATE1);
	}
	else  if ((actsecondofday >= (limit - KCHILDLOCK_WARNTIME2)) && (current_user->get_warn_usr_state() > KCHILDLOCK_WARNSTATE2))
	{
		current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE2);
		warning_or_logout_session(KCHILDLOCK_WARNSTATE2);
	}
	else  if ((actsecondofday >= (limit - KCHILDLOCK_WARNTIME3)) && (current_user->get_warn_usr_state() > KCHILDLOCK_WARNSTATE3))
	{
		current_user->set_warn_usr_state(KCHILDLOCK_WARNSTATE3);
		warning_or_logout_session(KCHILDLOCK_WARNSTATE3);
	}
	else
	{
		// daily usage limit not yet reaches
	}
}




// check user against settings for daily and weekly application limitations
// ========================================================================
void KchildlockDaemon::check_daily_app_limits()
{
   int i;
   int secondofday = (QTime::currentTime().hour() * 3600) + (QTime::currentTime().minute() * 60) + QTime::currentTime().second();
   if (debugflag) {
      fprintf (debugfile,"function=check_daily_app_limits app1PT=%s app1PD=%s at %i.%i. %i:%i:%i\n",
         (current_limits->get_appnamePT(0)).toAscii().constData(), (current_limits->get_appnamePD(0)).toAscii().constData(), 
         QDate::currentDate().day(), QDate::currentDate().month(), 
         QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second());
      fprintf (debugfile,"secondofday=%i, lim_du=%i, lim_wu=%i, lim_f=%i, lim_t=%i \n", 
         secondofday, current_limits->get_applim_du(0), current_limits->get_applim_wu(0), current_limits->get_applim_from(0), 
         current_limits->get_applim_to(0) );
      fprintf (debugfile,"usage_daily=%i, usage_weekly=%i \n", my_consumption->my_cons_record.app[0].appusage_daily, 
         my_consumption->my_cons_record.app[0].appusage_weekly);
      fflush(debugfile);
      }


   for (i=0; i<KCH_NUMOFAPPS; i++) {
      if ((current_user->get_appisactivePD(i) > 0) && !(current_limits->get_appnamePD(i).isEmpty())) {
          // check the limit 'daily max usage'
          check_app_limit_dwu(my_consumption->my_cons_record.app[i].appusage_daily, current_limits->get_applim_du(i), my_consumption->my_cons_record.app[i].appnamePD);
          // check the limit 'weekly max usage'
          check_app_limit_dwu(my_consumption->my_cons_record.app[i].appusage_weekly, current_limits->get_applim_wu(i), my_consumption->my_cons_record.app[i].appnamePD);
      }
      if ((current_user->get_appisactivePT(i) > 0) && !(current_limits->get_appnamePT(i).isEmpty())) {
         // check the  limit "from hour:minute" (is it to early?)
         if ((secondofday < current_limits->get_applim_from(i)) && (current_user->get_warn_app_state() <= KCHILDLOCK_WARNSTATE0)) {
            current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE00);
            warning_or_finish_application(my_consumption->my_cons_record.app[i].appnamePT, KCHILDLOCK_WARNSTATE00);
         }
         if ((secondofday < current_limits->get_applim_from(i)) && (current_user->get_warn_app_state() > KCHILDLOCK_WARNSTATE0))  {
            current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE0);
            warning_or_finish_application(my_consumption->my_cons_record.app[i].appnamePT, KCHILDLOCK_WARNSTATE0);
         }
         // check the  limit "to hour:minute" (is it to late?)
         check_app_limit_dwu(secondofday, current_limits->get_applim_to(i), my_consumption->my_cons_record.app[i].appnamePT);
       }
   }
}





// check user app limits against limit 'daily or weekly max usage'
// ===============================================================
void KchildlockDaemon::check_app_limit_dwu(int appusage, int limit, QString appname)
{
                if ((appusage >= limit) && (current_user->get_warn_app_state() <= KCHILDLOCK_WARNSTATE0))
	{
		current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE00);
		warning_or_finish_application(appname, KCHILDLOCK_WARNSTATE00);
	}
	else if ((appusage >= limit) && (current_user->get_warn_app_state() > KCHILDLOCK_WARNSTATE0))
	{
		current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE0);
		warning_or_finish_application(appname, KCHILDLOCK_WARNSTATE0);
	}
	else if ((appusage >= (limit - KCHILDLOCK_WARNTIME1)) && (current_user->get_warn_app_state() > KCHILDLOCK_WARNSTATE1))
	{
		current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE1);
		warning_or_finish_application(appname, KCHILDLOCK_WARNSTATE1);
	}
	else  if ((appusage >= (limit - KCHILDLOCK_WARNTIME2)) && (current_user->get_warn_app_state() > KCHILDLOCK_WARNSTATE2))
	{
		current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE2);
		warning_or_finish_application(appname, KCHILDLOCK_WARNSTATE2);
	}
	else  if ((appusage >= (limit - KCHILDLOCK_WARNTIME3)) && (current_user->get_warn_app_state() > KCHILDLOCK_WARNSTATE3))
	{
		current_user->set_warn_app_state(KCHILDLOCK_WARNSTATE3);
		warning_or_finish_application(appname, KCHILDLOCK_WARNSTATE3);
	}
	else
	{
		// daily usage limit not yet reaches
	}
}
