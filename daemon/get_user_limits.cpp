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
#include "kchildlock_settings.h"

#include <kuser.h>
#include <qdatetime.h>


// Constructor of class
// ====================
KchildlockLimits::KchildlockLimits()
{
  int idx;
  daily=24*60*60;
  weekly=7*24*60*60;
  from=0;
  to=24*60*60;
  for (idx=0; idx<KCH_NUMOFAPPS; idx++) {
     appnamePT[idx][0]=0;
     appnamePD[idx][0]=0;
  }
  no_limits=TRUE;
}

// Destructor of class
// ====================
KchildlockLimits::~KchildlockLimits()
{
}

// set the no limitations flag
// ===========================
void KchildlockLimits::set_nolimits(bool value)
{
  no_limits = value;
}

// get the no limitations flag
// ===========================
bool KchildlockLimits::get_nolimits()
{
  return(no_limits);
}

// get the limit values
// ====================
int KchildlockLimits::get_lim_du()
{
  return (daily);
}

int KchildlockLimits::get_lim_wu()
{
  return (weekly);
}
int KchildlockLimits::get_lim_from()
{
  return (from);
}
int KchildlockLimits::get_lim_to()
{
  return (to);
}

int KchildlockLimits::get_applim_du(int idx)
{
  return (app_daily[idx]);
}

int KchildlockLimits::get_applim_wu(int idx)
{
  return (app_weekly[idx]);
}
int KchildlockLimits::get_applim_from(int idx)
{
  return (app_from[idx]);
}
int KchildlockLimits::get_applim_to(int idx)
{
  return (app_to[idx]);
}

QString KchildlockLimits::get_appnamePT(int idx)
{
  // appname1 = empty string means "not enabled2 too
  return (appnamePT[idx]);
}
QString KchildlockLimits::get_appnamePD(int idx)
{
  // appname1 = empty string means "not enabled2 too
  return (appnamePD[idx]);
}




// get the TD limitations for the user if its the active user
// ==========================================================

void KchildlockLimits::get_current_userlimits_TDmax (KConfigGroup *act_user_settings)
      
{
   QDate actdate = QDate::currentDate();
   
      if (act_user_settings->readEntry("sameRestrictTD","true") == "true") {
         daily = (act_user_settings->readEntry("DAYmaxhr","2")).toInt() * 3600 + (act_user_settings->readEntry("DAYmaxmin","30")).toInt() * 60;
         no_limits = FALSE;
      } else {

         if (actdate.dayOfWeek() == 1 && act_user_settings->readEntry("MONenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("MONmaxhr","2")).toInt() * 3600 + (act_user_settings->readEntry("MONmaxmin","30")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 2 && act_user_settings->readEntry("TUEenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("TUEmaxhr","2")).toInt() * 3600 + (act_user_settings->readEntry("TUEmaxmin","30")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 3 && act_user_settings->readEntry("WEDenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("WEDmaxhr","2")).toInt() * 3600 + (act_user_settings->readEntry("WEDmaxmin","30")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 4 && act_user_settings->readEntry("THUenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("THUmaxhr","2")).toInt() * 3600 + (act_user_settings->readEntry("THUmaxmin","30")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 5 && act_user_settings->readEntry("FRIenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("FRImaxhr","3")).toInt() * 3600 + (act_user_settings->readEntry("FRImaxmin","0")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 6 && act_user_settings->readEntry("SATenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("SATmaxhr","3")).toInt() * 3600 + (act_user_settings->readEntry("SATmaxmin","0")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 7 && act_user_settings->readEntry("SUNenabledTD", "false") == "true" )
            {
            daily = (act_user_settings->readEntry("SUNmaxhr","2")).toInt() * 3600 + (act_user_settings->readEntry("SUNmaxmin","30")).toInt() * 60;
            no_limits = FALSE;
            }
         else
            {
//		daily = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		from = 0;
//		to = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		no_limits = FALSE;
            }
      }
}










// get the LT limitations for the user if its the active user
// ==========================================================

void KchildlockLimits::get_current_userlimits_LTfromto (KConfigGroup *act_user_settings)
{      
   QDate actdate = QDate::currentDate();
   
      if (act_user_settings->readEntry("sameRestrictLT","true") == "true") {
         from = (act_user_settings->readEntry("DAYfrom","480")).toInt() * 60;
         to = (act_user_settings->readEntry("DAYto","1200")).toInt() * 60;
         no_limits = FALSE;
      } else {

         if (actdate.dayOfWeek() == 1 && act_user_settings->readEntry("MONenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("MONfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("MONto","1200")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 2 && act_user_settings->readEntry("TUEenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("TUEfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("TUEto","1200")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 3 && act_user_settings->readEntry("WEDenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("WEDfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("WEDto","1200")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 4 && act_user_settings->readEntry("THUenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("THUfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("THUto","1200")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 5 && act_user_settings->readEntry("FRIenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("FRIfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("FRIto","1260")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 6 && act_user_settings->readEntry("SATenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("SATfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("SATto","1260")).toInt() * 60;
            no_limits = FALSE;
            }
         else if (actdate.dayOfWeek() == 7 && act_user_settings->readEntry("SUNenabledLT", "false") == "true" )
            {
            from = (act_user_settings->readEntry("SUNfrom","480")).toInt() * 60;
            to = (act_user_settings->readEntry("SUNto","1200")).toInt() * 60;
            no_limits = FALSE;
            }
         else
            {
//		daily = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		from = 0;
//		to = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		no_limits = FALSE;
            }
      }
}  




// get the app limitations for the user applications
// =================================================
void KchildlockLimits::get_current_userlimits_apps (KConfigGroup *act_user_settings, Consumption *my_consumption)
{      
   QString  appNr;
   QString tmpappname;
   int idx, idx2;

   QDate actdate = QDate::currentDate();
   
      for (idx=1; idx<=(KCH_NUMOFAPPS/2); idx++) {
         appNr = QString::number(idx);
         if ((act_user_settings->readEntry("App"+appNr+"enabledPT", "false")) == "true") {
            tmpappname = act_user_settings->readEntry("AppName"+appNr+"PT", "");
            for (idx2=0; idx2<tmpappname.length(); idx2++) {
               appnamePT[idx-1][idx2] = (tmpappname.toAscii())[idx2];
            }
            for (idx2=tmpappname.length(); idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePT[idx-1][idx2] = 0;
            }
               
         } else {
            for (idx2=0; idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePT[idx-1][idx2] = 0;
            }
         }
         if ((act_user_settings->readEntry("App"+appNr+"enabledPD", "false")) == "true") {
            tmpappname = act_user_settings->readEntry("AppName"+appNr+"PD", "");
            for (idx2=0; idx2<tmpappname.length(); idx2++) {
               appnamePD[idx-1][idx2] = (tmpappname.toAscii())[idx2];
            }
            for (idx2=tmpappname.length(); idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePD[idx-1][idx2] = 0;
            }
               
         } else {
            for (idx2=0; idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePD[idx-1][idx2] = 0;
            }
         }
         for (idx2=0; idx2<KCH_NAME_LENGTH; idx2++) {
            my_consumption->my_cons_record.app[idx-1].appnamePT[idx2] = appnamePT[idx-1][idx2];
            my_consumption->my_cons_record.app[idx-1].appnamePD[idx2] = appnamePD[idx-1][idx2];
         }
         app_daily[idx-1] = (act_user_settings->readEntry("App"+appNr+"maxDhr","2")).toInt() * 3600 + (act_user_settings->readEntry("App"+appNr+"maxDmin","30")).toInt() * 60;
         app_weekly[idx-1] = (act_user_settings->readEntry("App"+appNr+"maxWhr","8")).toInt() * 3600 + (act_user_settings->readEntry("App"+appNr+"maxWmin","0")).toInt() * 60;
         app_from[idx-1] = (act_user_settings->readEntry("App"+appNr+"from","480")).toInt() * 60;
         app_to[idx-1] = (act_user_settings->readEntry("App"+appNr+"to","1200")).toInt() * 60;
      }
}








// get the TD limitations for the group if its the active user
// ==========================================================
void KchildlockLimits::get_current_userlimits_groupTDmax (KConfigGroup *act_group_settings)
      
{
   QDate actdate = QDate::currentDate();
   int result;

   if (act_group_settings->readEntry("sameRestrictTD","true") == "true") {
      result = act_group_settings->readEntry("DAYmaxhr","2").toInt() * 3600 + act_group_settings->readEntry("DAYmaxmin","30").toInt() * 60;
      if (result < daily) daily=result;	// value from group is more restrictive
      no_limits = FALSE;
   } else {
	if (actdate.dayOfWeek() == 1 && act_group_settings->readEntry("MONenabledTD","false") == "true" )
	{
		result = act_group_settings->readEntry("MONmaxhr","2").toInt() * 3600 + act_group_settings->readEntry("MONmaxmin","30").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 2 && act_group_settings->readEntry("TUEenabledTD","false") == "true")
	{
		result = act_group_settings->readEntry("TUEmaxhr","2").toInt() * 3600 + act_group_settings->readEntry("TUEmaxmin","30").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 3 && act_group_settings->readEntry("WEDenabledTD","false") == "true")
	{
		result = act_group_settings->readEntry("WEDmaxhr","2").toInt() * 3600 + act_group_settings->readEntry("WEDmaxmin","30").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 4 && act_group_settings->readEntry("THUenabledTD","false") == "true")
	{
		result = act_group_settings->readEntry("THUmaxhr","2").toInt() * 3600 + act_group_settings->readEntry("THUmaxmin","30").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 5 && act_group_settings->readEntry("FRIenabledTD","false") == "true")
	{
		result = act_group_settings->readEntry("FRImaxhr","3").toInt() * 3600 + act_group_settings->readEntry("FRImaxmin","0").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 6 && act_group_settings->readEntry("SATenabledTD","false") == "true")
	{
		result = act_group_settings->readEntry("SATmaxhr","3").toInt() * 3600 + act_group_settings->readEntry("SATmaxmin","0").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 7 && act_group_settings->readEntry("SUNenabledTD","false") == "true")
	{
		result = act_group_settings->readEntry("SUNmaxhr","2").toInt() * 3600 + act_group_settings->readEntry("SUNmaxmin","30").toInt() * 60;
		if (result < daily) daily=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else
	{
//		daily = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		from = 0;
//		to = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		no_limits = FALSE;
	}
   }
}






// get the LT limitations for the group if its the active user
// ==========================================================

void KchildlockLimits::get_current_userlimits_groupLTfromto (KConfigGroup *act_group_settings)
{      
   int result;
   QDate actdate = QDate::currentDate();
   
   if (act_group_settings->readEntry("sameRestrictLT","true") == "true") {
      result = act_group_settings->readEntry("DAYfrom","480").toInt() * 60;
      if (result > from) from=result;	// value from group is more restrictive
      result = act_group_settings->readEntry("DAYto","1200").toInt() * 60;
      if (result < to) to=result;	// value from group is more restrictive
      no_limits = FALSE;
   } else {
	if (actdate.dayOfWeek() == 1 && act_group_settings->readEntry("MONenabledLT","false") == "true" )
	{
		result = act_group_settings->readEntry("MONfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("MONto","1200").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 2 && act_group_settings->readEntry("TUEenabledLT","false") == "true")
	{
		result = act_group_settings->readEntry("TUEfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("TUEto","1200").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 3 && act_group_settings->readEntry("WEDenabledLT","false") == "true")
	{
		result = act_group_settings->readEntry("WEDfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("WEDto","1200").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 4 && act_group_settings->readEntry("THUenabledLT","false") == "true")
	{
		result = act_group_settings->readEntry("THUfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("THUto","1200").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 5 && act_group_settings->readEntry("FRIenabledLT","false") == "true")
	{
		result = act_group_settings->readEntry("FRIfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("FRIto","1260").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 6 && act_group_settings->readEntry("SATenabledLT","false") == "true")
	{
		result = act_group_settings->readEntry("SATfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("SATto","1260").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else if (actdate.dayOfWeek() == 7 && act_group_settings->readEntry("SUNenabledLT","false") == "true")
	{
		result = act_group_settings->readEntry("SUNfrom","480").toInt() * 60;
		if (result > from) from=result;	// value from group is more restrictive
		result = act_group_settings->readEntry("SUNto","1200").toInt() * 60;
		if (result < to) to=result;	// value from group is more restrictive
		no_limits = FALSE;
	}
	else
	{
//		daily = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		from = 0;
//		to = 24 * 3600 + KCHILDLOCK_WARNTIME3;
//		no_limits = FALSE;
	}
   }
}









// get the app limitations for the group applications
// =================================================
void KchildlockLimits::get_current_userlimits_groupapps (KConfigGroup *act_group_settings, Consumption *my_consumption)
{      
   QString  appNr;
   QString tmpappname;
   int idx, idx2;

   QDate actdate = QDate::currentDate();
   
      // get the limitations for the group applications
      // ==============================================
      for (idx=1; idx<=(KCH_NUMOFAPPS/2); idx++) {
         appNr = QString::number(idx);
         if ((act_group_settings->readEntry("App"+appNr+"enabledPT","false")) == "true") {
            tmpappname = act_group_settings->readEntry("AppName"+appNr+"PT","");
            for (idx2=0; idx2<tmpappname.length(); idx2++) {
               appnamePT[idx+5-1][idx2] = (tmpappname.toAscii())[idx2];
            }
            for (idx2=tmpappname.length(); idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePT[idx+5-1][idx2] = 0;
            }
               
         } else {
            for (idx2=0; idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePT[idx+5-1][idx2] = 0;
            }
         }
         if ((act_group_settings->readEntry("App"+appNr+"enabledPD","false")) == "true") {
            tmpappname = act_group_settings->readEntry("AppName"+appNr+"PD","");
            for (idx2=0; idx2<tmpappname.length(); idx2++) {
               appnamePD[idx+5-1][idx2] = (tmpappname.toAscii())[idx2];
            }
            for (idx2=tmpappname.length(); idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePD[idx+5-1][idx2] = 0;
            }
               
         } else {
            for (idx2=0; idx2<KCH_NAME_LENGTH; idx2++) {
               appnamePD[idx+5-1][idx2] = 0;
            }
         }
         for (idx2=0; idx2<KCH_NAME_LENGTH; idx2++) {
            my_consumption->my_cons_record.app[idx+5-1].appnamePT[idx2] = appnamePT[idx+5-1][idx2];
            my_consumption->my_cons_record.app[idx+5-1].appnamePD[idx2] = appnamePD[idx+5-1][idx2];
         }
         app_daily[idx+5-1] = (act_group_settings->readEntry("App"+appNr+"maxDhr","2")).toInt() * 3600 + (act_group_settings->readEntry("App"+appNr+"maxDmin","30")).toInt() * 60;
         app_weekly[idx+5-1] = (act_group_settings->readEntry("App"+appNr+"maxWhr","8")).toInt() * 3600 + (act_group_settings->readEntry("App"+appNr+"maxWmin","0")).toInt() * 60;
         app_from[idx+5-1] = (act_group_settings->readEntry("App"+appNr+"from","480")).toInt() * 60;
         app_to[idx+5-1] = (act_group_settings->readEntry("App"+appNr+"to","1200")).toInt() * 60;
      }
}









// get the limitations for the user if its the active user
// =======================================================
void KchildlockLimits::get_current_userlimits(QString actuser, KConfigGroup *act_user_settings, KConfigGroup *act_group_settings, 
		              QString EmptyUsrName, QString EmptyGrpName, Consumption *my_consumption)

{
   QString  appNr;
   QString tmpappname;
   int idx;

   QDate actdate = QDate::currentDate();

   if ((EmptyUsrName == "") && (EmptyGrpName == "")) {
      no_limits = TRUE;
      return;
   }
   
   if (EmptyUsrName != "") {
      if (act_user_settings->readEntry("enabled", "false") == "false")
      {
         daily = 24 * 3600 + KCHILDLOCK_WARNTIME3;
         from = 0;
         to = 24 * 3600 + KCHILDLOCK_WARNTIME3;
         no_limits = FALSE;
         for (idx=0; idx<(KCH_NUMOFAPPS); idx++) {
               appnamePT[idx][0] = 0;
               appnamePD[idx][0] = 0;
         }
      } else {
      if (act_user_settings->readEntry("WEEKenabled","false") == "true") {
         weekly = (act_user_settings->readEntry("WEEKmaxhr","8")).toInt() * 3600 + (act_user_settings->readEntry("WEEKmaxmin","0")).toInt() * 60;
      }

      get_current_userlimits_TDmax (act_user_settings);
      get_current_userlimits_LTfromto (act_user_settings);
      get_current_userlimits_apps (act_user_settings, my_consumption);
      }
   }


// get the limitations for group if its the active user
// ===================================================
   QStringList myuserlist;
   KUserGroup mygroup;
   int result;

   if (EmptyGrpName == "") return;                    // check only if there are group restrictions found
   if (act_group_settings->readEntry("enabled","false") == "false")  return;	// check only if group restrictive are enabled

   if (act_group_settings->readEntry("WEEKenabled","false") == "true") {
      result = act_group_settings->readEntry("WEEKmaxhr","8").toInt() * 3600 + act_group_settings->readEntry("WEEKmaxmin","0").toInt() * 60;
      if (result < weekly) weekly=result;	// value from group is more restrictive
   }

   get_current_userlimits_groupTDmax (act_group_settings);
   get_current_userlimits_groupLTfromto (act_group_settings);
   get_current_userlimits_groupapps (act_group_settings, my_consumption);
  
 }
