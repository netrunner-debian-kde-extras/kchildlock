/***************************************************************************
 *   Copyright (C) 2011 by Rene Landert <rene.landert@bluewin.ch>          *
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


GeneralPageS::GeneralPageS(QWidget *parent, Ui_generalPage *ui_general, KchildlockSettings *myGeneralSettings)
{
    m_parent = parent;
    m_ui_general = ui_general;
    m_GeneralSettings = myGeneralSettings;
}


GeneralPageS::~GeneralPageS()
{
}

void GeneralPageS::load()
{
  m_GeneralSettings->readConfig();
}

void GeneralPageS::save()
{
  m_GeneralSettings->writeConfig();
}


#include "moc_GeneralPageS.cpp"
