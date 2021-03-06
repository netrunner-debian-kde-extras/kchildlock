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



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <kxmlguiwindow.h>

namespace Ui {
  class UserInfoPage;
}

class KSystemTrayIcon;
class QCloseEvent;

class MainWindow : public KXmlGuiWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent=0, KchildlockLimits *current_limits=0, Consumption *my_consumption=0);
    virtual ~MainWindow();

  protected:
//    bool queryClose();

  private:
    void setupActions();
    void setupConnections();
    void setupTrayIcon();

    void showRestrictions();
    bool getcurrentconsumption();

  private slots:
    void slotShowRestrictions(bool value);
    void slotCloseMainWindow();

  private:
    Ui::UserInfoPage* my_mainWidget;
    KSystemTrayIcon* my_systray;
    KchildlockLimits *my_limits;
    Consumption *my_consumedtime;
    
  public slots:
    bool queryExit();
    
};

#endif
