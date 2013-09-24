/***************************************************************************
 *   Copyright (C) 2010 by Rene Landert <rene.landert@bluewin.ch>          *
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

#ifndef GENERALPAGES_H
#define GENERALPAGES_H

#include "kchildlock_settings.h"
#include "ui_generalPage.h"

class GeneralPageS : public KchildlockSettings, public Ui_generalPage
{
    Q_OBJECT

public:
    GeneralPageS(QWidget* parent, Ui_generalPage *ui_general, KchildlockSettings *myGeneralSettings);
    ~GeneralPageS();
    void fillUi();
    void load();
    void save();
//    virtual void defaults();
    

public:
  QWidget *m_parent;
  Ui_generalPage *m_ui_general;
  KchildlockSettings *m_GeneralSettings;
    
    
public slots:
    void emitChanged_enable_restrictions(int);
    void emitChanged_enable_systray(int);
    void emitChanged_scaninterval(int);

signals:
    void changed(bool);
};

#endif /* GENERALPAGES_H */
