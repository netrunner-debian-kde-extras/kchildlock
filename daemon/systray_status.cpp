/***************************************************************************
 *   Copyright (C) 2010 by Rene Landert <rene.landert@bluewin.ch>          *
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
#include "ui_userInfoPage.h"
#include <ksystemtrayicon.h>
#include <kstandardaction.h>
#include <KAction>
#include <KActionCollection>
#include <QMenu>
#include <klocalizedstring.h>
#include <kmessagebox.h>
#include <kdebug.h>
#include "systray_status.h"






MainWindow::MainWindow(QWidget *parent, KchildlockLimits *current_limits, Consumption *my_consumption)
  : KXmlGuiWindow(parent)
{
  
  my_limits = current_limits;
  my_consumedtime = my_consumption;
  QWidget* widget = new QWidget( this );
  my_mainWidget = new Ui::UserInfoPage();
  my_mainWidget->setupUi(widget);

  setCentralWidget(widget);

  setupActions();
//  setHelpMenuEnabled(FALSE);
//  setStandardToolBarMenuEnabled(FALSE);
  setupGUI(Save);
//  setupGUI(KXmlGuiWindow::Save);
  setupTrayIcon();
  setupConnections();

}

MainWindow::~MainWindow()
{
}


void MainWindow::setupActions()
{
  KStandardAction::quit(this, SLOT(slotCloseMainWindow()), this->actionCollection());
}


bool MainWindow::queryExit()
{
  hide();
  return (false);
}

void MainWindow::setupTrayIcon()
{
      my_systray = new KSystemTrayIcon("kchildlock", this);
      QMenu *my_systraymenu = new QMenu(i18n("KChildlock - Status Display for User Restrictions"));
      QAction *my_titleaction = my_systray->contextMenuTitle();
      my_titleaction->setIcon(QIcon("kchildlock"));
      my_titleaction->setText(i18n("KChildlock - Status Display for User Restrictions"));
      my_systray->setContextMenuTitle(my_titleaction);
      my_systray->setToolTip(i18n("KChildlock - Time Restrictions for Children"));
      QAction *my_show_action = my_systraymenu->addAction(i18n("Show Restrictions"));
      my_systraymenu->setDefaultAction(my_show_action);
      my_systray->setContextMenu(my_systraymenu);
      my_systraymenu->setTitle(i18n("KDE-Service KChildlock"));
      
      connect(my_show_action, SIGNAL(triggered(bool)), this, SLOT(slotShowRestrictions(bool)));

      my_systray->show();
  
}


void MainWindow::setupConnections()
{
  connect (my_mainWidget->ButtonOK, SIGNAL(clicked()),
           this, SLOT(slotCloseMainWindow()));
  connect (my_mainWidget->ButtonOK, SIGNAL(pressed()),
           this, SLOT(slotCloseMainWindow()));
}


void MainWindow::slotCloseMainWindow()
{
  hide();
}


void MainWindow::slotShowRestrictions(bool value)
{
  bool norestrictions;
  norestrictions = getcurrentconsumption();
  if (norestrictions) {
    my_mainWidget->lbl_MainText->setText(i18n("          You have currently no limitations!"));
    my_mainWidget->label_3->hide();
    my_mainWidget->label_5->hide();
    my_mainWidget->lbl_TODAYfrom->hide();
    my_mainWidget->lbl_TODAYto->hide();
    my_mainWidget->label_9->hide();
    my_mainWidget->label_7->hide();
    my_mainWidget->label_11->hide();
    my_mainWidget->lbl_TODAYmaxhr->hide();
    my_mainWidget->lbl_TODAYmaxmin->hide();
    my_mainWidget->label_12->hide();
    my_mainWidget->label_14->hide();
    my_mainWidget->label_16->hide();
    my_mainWidget->lbl_TODAYremainhr->hide();
    my_mainWidget->lbl_TODAYremainmin->hide();
    my_mainWidget->label_19->hide();
    my_mainWidget->label_21->hide();
    my_mainWidget->label_23->hide();
    my_mainWidget->lbl_THISWEEKmaxhr->hide();
    my_mainWidget->lbl_THISWEEKmaxmin->hide();
    my_mainWidget->lbl_THISWEEKremainhr->hide();
    my_mainWidget->lbl_THISWEEKremainmin->hide();
    my_mainWidget->bar_TODAYremain->hide();
    my_mainWidget->bar_THISWEEKremain->hide();
    my_mainWidget->label_25->hide();
    my_mainWidget->label_27->hide();
    my_mainWidget->label_29->hide();
  }
  show();
}




bool MainWindow::getcurrentconsumption()
{
  int min, hr, val;
  QString s;
  bool norestr;
  
  norestr = TRUE;
  
  hr = my_limits->get_lim_from() / 3600;
  min = ( my_limits->get_lim_from() % 3600 ) / 60;
  my_mainWidget->lbl_TODAYfrom->setText(s.sprintf("%02d:%02d", hr, min));
  if ((hr+min) >  0) norestr = FALSE;
  
  hr = my_limits->get_lim_to() / 3600;
  min = ( my_limits->get_lim_to() % 3600 ) / 60;
  if (hr == 24) min = 0;
  my_mainWidget->lbl_TODAYto->setText(s.sprintf("%02d:%02d", hr, min));
  if (hr < 24) norestr = FALSE;

  hr = my_limits->get_lim_du() / 3600;
  min = ( my_limits->get_lim_du() % 3600 ) / 60;
  kDebug() << "DEBUG LAR1 hr min" << hr << min;
  if (hr == 24) min = 0;
  my_mainWidget->lbl_TODAYmaxhr->setText(s.sprintf("%02d", hr));
  my_mainWidget->lbl_TODAYmaxmin->setText(s.sprintf("%02d", min));
  if (hr < 24) norestr = FALSE;

  hr = ( my_limits->get_lim_du() - my_consumedtime->my_cons_record.pcu.pcusage_daily ) / 3600;
  min = ( ( my_limits->get_lim_du() - my_consumedtime->my_cons_record.pcu.pcusage_daily )  % 3600 ) / 60;
  kDebug() << "DEBUG LAR2 consumed" << my_consumedtime->my_cons_record.pcu.pcusage_daily;
  kDebug() << "DEBUG LAR2 hr min" << hr << min;
  if (hr == 24) min = 0;
  my_mainWidget->lbl_TODAYremainhr->setText(s.sprintf("%02d", hr));
  my_mainWidget->lbl_TODAYremainmin->setText(s.sprintf("%02d", min));

  if (my_limits->get_lim_du() != 0) {
    val =  ( my_consumedtime->my_cons_record.pcu.pcusage_daily * 100 ) / my_limits->get_lim_du();
  }
  else {
    val = 0;
  }
  my_mainWidget->bar_TODAYremain->setValue(val);

  hr = my_limits->get_lim_wu() / 3600;
  min = ( my_limits->get_lim_wu() % 3600 ) / 60;
  my_mainWidget->lbl_THISWEEKmaxhr->setText(s.sprintf("%02d", hr));
  my_mainWidget->lbl_THISWEEKmaxmin->setText(s.sprintf("%02d", min));
  if (hr < 168) norestr = FALSE;
  
  hr = ( my_limits->get_lim_wu() - my_consumedtime->my_cons_record.pcu.pcusage_weekly ) / 3600;
  min = ( ( my_limits->get_lim_wu() - my_consumedtime->my_cons_record.pcu.pcusage_weekly )  % 3600 ) / 60;
  if (hr < 0)  hr=0;
  if (min < 0) min=0;
  my_mainWidget->lbl_THISWEEKremainhr->setText(s.sprintf("%02d", hr));
  my_mainWidget->lbl_THISWEEKremainmin->setText(s.sprintf("%02d", min));

  if (my_limits->get_lim_wu() != 0) {
    val =  ( my_consumedtime->my_cons_record.pcu.pcusage_weekly * 100 ) / my_limits->get_lim_wu();
  }
  else {
    val = 0;
  }
  my_mainWidget->bar_THISWEEKremain->setValue(val);
   
  return (norestr);
}


#include "moc_systray_status.cpp"

