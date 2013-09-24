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

#include <KConfigDialog>


#include "GeneralPageS.h"
#include "UserGroupPageS.h"

#include "KchildlockKCM.h"
#include "kchildlock_settings.h"


// general Slots =========================================================================

void GeneralPageS::emitChanged_enable_restrictions(int value)
{
   m_GeneralSettings->setEnable_restrictions(value);
//   kDebug() << "new value is:" << value;
   emit changed(true);
}

void GeneralPageS::emitChanged_enable_systray(int value)
{
   m_GeneralSettings->setEnable_systray(value);
//   kDebug() << "new value is:" << value;
   emit changed(true);
}

void GeneralPageS::emitChanged_scaninterval(int value)
{
   m_GeneralSettings->setScaninterval(value);
//   kDebug() << "new value is:" << value;
   emit changed(true);
}

