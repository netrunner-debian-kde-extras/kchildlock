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

#include "../common/kchildlockcommon.h"
#include "consumption_file.h"
#include <QDate>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>




Consumption::Consumption()
{
  Consumption::set_default_values ("");

// create file with the consumed times if its not existing
// =======================================================
   consumption_file = fopen( consumption_filename, "r+" );
   if (consumption_file == NULL ) {
      consumption_file = fopen ( consumption_filename, "w" );
      if ( consumption_file == NULL ) {
//          fclose(consumption_file);              bugfix 3381398
          return;
      } else {
        chmod(consumption_filename,  S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
        fclose(consumption_file);
      }
   }
};


Consumption::~Consumption()
{
};

// check if user record of th consumed times exists
// ================================================
bool Consumption::exists_record(QString user)
{
   int i, i2;
   char inbfr[KCH_NAME_LENGTH];

   userindex=0;
   consumption_file = fopen( consumption_filename, "r+" );
   if (consumption_file == 0) return(FALSE);
   
   for (i=0; i<KCH_NAME_LENGTH; i++) {
       my_cons_record.pcu.username[i] = 0;
   }
   for (i=0; i<user.size(); i++) {
       my_cons_record.pcu.username[i] = (user.toAscii())[i];
   }
  
   fseek(consumption_file, 10, 0);
   bool notfound=TRUE;
   do {
     for (i=0; i<KCH_NAME_LENGTH; i++) {
       inbfr[i] = fgetc(consumption_file);
     }
     if (feof(consumption_file)) {
       notfound=FALSE;
     } else {
       if (strcmp(inbfr, my_cons_record.pcu.username) == 0) {
          notfound=FALSE;
       } else {
         fscanf(consumption_file, "%8d%8d%4d%4d",&my_cons_record.pcu.pcusage_daily, &my_cons_record.pcu.pcusage_weekly, &my_cons_record.pcu.dayofweek, &my_cons_record.pcu.weekofyear);
         for (i2=0; i2<KCH_NUMOFAPPS; i2++) {
            for (i=0; i<KCH_NAME_LENGTH; i++) {
               my_cons_record.app[i2].appnamePT[i]=fgetc(consumption_file);
            }
            for (i=0; i<KCH_NAME_LENGTH; i++) {
               my_cons_record.app[i2].appnamePD[i]=fgetc(consumption_file);
            }
            fscanf(consumption_file, "%8d%8d", &my_cons_record.app[i2].appusage_daily, &my_cons_record.app[i2].appusage_weekly);
         }     
       userindex++;
       }
     }
   } while (notfound);

   if (feof(consumption_file)) {
       fclose(consumption_file);
       return(FALSE);
   } else {
       fclose(consumption_file);
       return(TRUE);
   }
};




// create new user record of file with the consumed times
// ======================================================
void Consumption::create_record(QString user)
{
   int i, i2;
   char inbfr[KCH_NAME_LENGTH];

   consumption_file = fopen( consumption_filename, "r+" );
   if (consumption_file == 0) return;
   
   for (i=0; i<KCH_NAME_LENGTH; i++) {
       my_cons_record.pcu.username[i] = 0;
   }
   for (i=0; i<user.size(); i++) {
       my_cons_record.pcu.username[i] = (user.toAscii())[i];
   }
  
   fseek(consumption_file, 10, 0);
   bool notfound=TRUE;
   userindex=0;
   do {
     for (i=0; i<KCH_NAME_LENGTH; i++) {
       inbfr[i] = fgetc(consumption_file);
     }
     if (feof(consumption_file)) {
       notfound=FALSE;
       my_cons_record.pcu.pcusage_daily=0; my_cons_record.pcu.pcusage_weekly=0;
       my_cons_record.pcu.dayofweek=QDate::currentDate().dayOfWeek();;
       my_cons_record.pcu.weekofyear=QDate::currentDate().weekNumber();
       for (i2=0; i2<KCH_NUMOFAPPS; i2++) {
          for (i=0; i<KCH_NAME_LENGTH; i++) {
              my_cons_record.app[i2].appnamePT[i]=0; 
          }
          for (i=0; i<KCH_NAME_LENGTH; i++) {
              my_cons_record.app[i2].appnamePD[i]=0; 
          }
          my_cons_record.app[i2].appusage_daily=0; 
          my_cons_record.app[i2].appusage_weekly=0;
       }
     } else {
       fscanf(consumption_file, "%8d%8d%4d%4d", &my_cons_record.pcu.pcusage_daily, &my_cons_record.pcu.pcusage_weekly, &my_cons_record.pcu.dayofweek, &my_cons_record.pcu.weekofyear);
       for (i2=0; i2<KCH_NUMOFAPPS; i2++) {
          for (i=0; i<KCH_NAME_LENGTH; i++) {
             my_cons_record.app[i2].appnamePT[i]=fgetc(consumption_file);
          }
          for (i=0; i<KCH_NAME_LENGTH; i++) {
             my_cons_record.app[i2].appnamePD[i]=fgetc(consumption_file);
          }
          fscanf(consumption_file, "%8d%8d", &my_cons_record.app[i2].appusage_daily, &my_cons_record.app[i2].appusage_weekly);
       }            
     userindex++;
     } 
   } while (notfound);
   fclose(consumption_file);
};





// set the default valuse for the consumed times
// =============================================
void Consumption::set_default_values (QString user)
{
   uint i, i2;

   for (i=0; i<KCH_NAME_LENGTH; i++) {
       my_cons_record.pcu.username[i] = 0;
   }
   for (i=0; i<user.size(); i++) {
       my_cons_record.pcu.username[i] = (user.toAscii())[i];
   }

   for (i=0; i<KCH_VERS_LENGTH; i++) {
      my_cons_record.hdr.version[i] = 0;
   }
   for (i=0; i<sizeof(KCHILDLOCK_VERSION); i++) {
      my_cons_record.hdr.version[i] = KCHILDLOCK_VERSION[i];
   }
   my_cons_record.pcu.weekofyear = QDate::currentDate().weekNumber();
   my_cons_record.pcu.dayofweek = QDate::currentDate().dayOfWeek();
   my_cons_record.pcu.pcusage_daily = 0;
   my_cons_record.pcu.pcusage_weekly = 0;

   for (i=0; i<KCH_NUMOFAPPS; i++) {
       my_cons_record.app[i].appusage_daily = 0;
       my_cons_record.app[i].appusage_weekly = 0;
       for (i2=0; i2<KCH_NAME_LENGTH; i2++) {
         my_cons_record.app[i].appnamePT[i2] = 0;
         my_cons_record.app[i].appnamePD[i2] = 0;
       }
   }

};


// reset the daily valuse for a new day
// ====================================
void Consumption::reset_daily_values ()
{
   int i;
   
   my_cons_record.pcu.weekofyear = QDate::currentDate().weekNumber();
   my_cons_record.pcu.dayofweek = QDate::currentDate().dayOfWeek();
   my_cons_record.pcu.pcusage_daily = 0;
   for (i=0; i<KCH_NUMOFAPPS; i++) {
       my_cons_record.app[i].appusage_daily = 0;
   }
};


// read the values from the consumed time file
// ===========================================
void Consumption::read_record()
{

   int i, i2;
   char inbfr[KCH_NAME_LENGTH];
   
   consumption_file = fopen( consumption_filename, "r" );
   if (consumption_file == 0) return;
   fseek(consumption_file, 0, 0);
   for (i=0; i<KCH_VERS_LENGTH; i++) {
        my_cons_record.hdr.version[i] = fgetc(consumption_file);
   }
   
   fseek(consumption_file, 10+userindex*1024, 0);
   for (i=0; i<KCH_NAME_LENGTH; i++) {
       inbfr[i] = fgetc(consumption_file);
   }

   if (!feof(consumption_file)) {
      fscanf(consumption_file, "%8d%8d%4d%4d", &my_cons_record.pcu.pcusage_daily, &my_cons_record.pcu.pcusage_weekly,  &my_cons_record.pcu.dayofweek, &my_cons_record.pcu.weekofyear);
      for (i2=0; i2<KCH_NUMOFAPPS; i2++) {
          for (i=0; i<KCH_NAME_LENGTH; i++) {
              my_cons_record.app[i2].appnamePT[i] = fgetc(consumption_file);
          }
          for (i=0; i<KCH_NAME_LENGTH; i++) {
              my_cons_record.app[i2].appnamePD[i] = fgetc(consumption_file);
          }
          fscanf(consumption_file, "%8d%8d", &my_cons_record.app[i2].appusage_daily, &my_cons_record.app[i2].appusage_weekly);
       }
    }
   fclose(consumption_file);

};


// write the values to consumed time file
// ======================================
void Consumption::write_record()
{

   int i, i2;

   consumption_file = fopen( consumption_filename, "r+" );
   if (consumption_file == 0) return;
   fseek(consumption_file, 0 ,0);
   fprintf(consumption_file, "%-9s%c", my_cons_record.hdr.version, 0);
   
   fseek(consumption_file, 10+userindex*1024 , SEEK_SET);
   for (i=0; i<KCH_NAME_LENGTH; i++) {
     fputc(my_cons_record.pcu.username[i], consumption_file);
   }
   fprintf(consumption_file, "%8d%8d%4d%4d", my_cons_record.pcu.pcusage_daily, my_cons_record.pcu.pcusage_weekly, my_cons_record.pcu.dayofweek, my_cons_record.pcu.weekofyear);
   for (i=0; i<KCH_NUMOFAPPS; i++) {
      for (i2=0; i2<KCH_NAME_LENGTH; i2++) {
         fputc(my_cons_record.app[i].appnamePT[i2], consumption_file);
      }
      for (i2=0; i2<KCH_NAME_LENGTH; i2++) {
         fputc(my_cons_record.app[i].appnamePD[i2], consumption_file);
      }
      fprintf(consumption_file, "%8d%8d", my_cons_record.app[i].appusage_daily, my_cons_record.app[i].appusage_weekly);
   }
   fclose(consumption_file);

};





// return the appropriate values
//==============================
QString Consumption::get_consumption_filename()
{
   return consumption_filename;
};



int Consumption::get_weekofyear()
{
   return my_cons_record.pcu.weekofyear;
};

int Consumption::get_dayofweek()
{
   return my_cons_record.pcu.dayofweek;
};

