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


#define consumption_filename "/var/opt/kchildlock/.kch_time.dat"
#define KCH_NAME_LENGTH 40
#define KCH_VERS_LENGTH 10
#define KCH_NUMOFAPPS 10            // 5 userapps + 5 groupapps

#include <qstring.h>
#include <qfile.h>
#include <stdio.h>


class Consumption
{
public:
   Consumption();
   ~Consumption();

private:
   FILE *consumption_file;

   int userindex;

public:
   struct cons_header {
      char version[KCH_VERS_LENGTH];     // in the file 10 bytes
   };
   struct cons_app {
   char appnamePT[KCH_NAME_LENGTH];
   char appnamePD[KCH_NAME_LENGTH];
   int appusage_daily, appusage_weekly;    // in the file 40+40+8+8=96 bytes
   };
  struct cons_pcusage {
   char username[KCH_NAME_LENGTH];
   int pcusage_daily;
   int pcusage_weekly;
   int dayofweek;
   int weekofyear;         // in the file 40+8+8+4+4=64 bytes
  };
  struct cons_record {
   cons_header hdr;    // in the file 10 bytes
   cons_pcusage pcu;    // in the file 64 bytes
   cons_app app[KCH_NUMOFAPPS];    // in the file 10*96=960 bytes
  };    // in the file 10+userindex*(64+960)bytes  or 10 + userindex * 1024 bytes
  

  cons_record my_cons_record;
    
    
public:
   bool exists_record(QString);
   void create_record(QString);
   void read_record();
   void write_record();
   void set_default_values(QString);
   void reset_daily_values();
   QString get_consumption_filename();
   int get_weekofyear();
   int get_dayofweek();

};
