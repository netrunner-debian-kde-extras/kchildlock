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

#include "GeneralPageS.h"
#include "UserGroupPageS.h"
#include "ui_selectUserGroup.h"
#include "ui_deleteUserGroup.h"
 
#include <KPluginFactory>
#include <KAboutData>
#include <QVBoxLayout>
#include <QFile>
#include <KConfigDialog>
#include <KPageDialog>
#include <KConfigGroup>
#include <KUserGroup>
#include <KUser>
#include <kdebug.h>
#include <QStringListModel>
#include <QDir>
#include <KDirSelectDialog>
#include <KUrl>
#include <KFileDialog>

#include "KchildlockKCM.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


K_PLUGIN_FACTORY(KchildlockFactory,
                 registerPlugin<KchildlockKCM>();
                )
K_EXPORT_PLUGIN(KchildlockFactory("kcm_kchildlock"))

KchildlockKCM::KchildlockKCM(QWidget *parent, const QVariantList &args) :
        KCModule(KchildlockFactory::componentData(), parent, args)
        , initial_load(true)
        
{
    KGlobal::locale()->insertCatalog("kchildlock");

    KAboutData *about =
        new KAboutData("kcmkchildlock", "kchildlock", ki18n("Kchildlock Configuration"),
                       KCHILDLOCK_VERSION, ki18n("A configurator for Kchildlock"),
                       KAboutData::License_GPL, ki18n("(c), 2011 Rene Landert"),
                       ki18n("From this module, you can configure the Kchildlock Daemon, "
                             "which restricts the usage time of the computer and of applications per day and "
                             "per week for selected users. "));

    about->addAuthor(ki18n("Rene Landert"), ki18n("Main Developer") , "rene.landert@bluewin.ch", "http://www.sourceforge.net/kchildlock");

    setAboutData(about);
    setButtons(KCModule::Apply|KCModule::Help|KCModule::Default);

    setQuickHelp(i18n("<h1>Kchildlock configuration</h1> <p>This module lets you configure "
                      "Kchildlock. Kchildlock is a daemon (so it runs in background) that is started "
                      "upon KDE startup.</p> <p>Kchildlock has several levels of configuration: a general one, "
                      "with common settings, a list of users or groups for whom you can configure the restrictions. "
                      "Each tab for the user or group lets you edit the restriction details. "
                      "You do not have to restart Kchildlock, just click on the button "
                      "'Apply', and you are done.</p>"));

//    if (getuid() != 0){
//        KMessageBox::information(0,i18n("Only root can change the settings of this program!"));
//        setButtons(KCModule::Help);
//    }
      setUseRootOnlyMessage(TRUE);
      useRootOnlyMessage();

    current_user = "";
    current_group = "";
    KSharedConfigPtr GeneralConfigPtr = KSharedConfig::openConfig("kchildlockrc", KConfig::SimpleConfig);
    KSharedConfigPtr UserConfigPtr = KSharedConfig::openConfig("kchildlockrc_U", KConfig::SimpleConfig);
    KSharedConfigPtr GroupConfigPtr = KSharedConfig::openConfig("kchildlockrc_G", KConfig::SimpleConfig);

    myGeneralSettings = new KchildlockSettings(GeneralConfigPtr); 
    myUserSettings = new KchildlockSettings(UserConfigPtr);
    myGroupSettings = new KchildlockSettings(GroupConfigPtr);
    dialog = new KPageDialog (parent);

    dialog->setButtons(0);
    setButtons(KCModule::Apply|KCModule::Help|KCModule::Default);    
    dialog->setModal(false);
    
    QVBoxLayout *mylayout = new QVBoxLayout(this);
    mylayout->setSpacing(KDialog::spacingHint());
    mylayout->setMargin(0);
    mylayout->addWidget(dialog);

//  configuration dialog for general settings
    QWidget *generalSettingsDlg = new QWidget(this);
    ui_general = new Ui_generalPage();
    general = new GeneralPageS(generalSettingsDlg, ui_general, myGeneralSettings);
    ui_general->setupUi( generalSettingsDlg );
    generalPageItem = dialog->addPage ( generalSettingsDlg, i18n ( "General") );
    generalPageItem->setIcon(KIcon("configure"));

//  configuration dialog for user settings
    QWidget *userSettingsDlg = new QWidget(this);    
    ui_user = new Ui_usergroupPage();
    user = new UserGroupPageS(userSettingsDlg, ui_user, myUserSettings, myGeneralSettings);
    ui_user->setupUi( userSettingsDlg );
    userPageItem = dialog->addPage ( userSettingsDlg, i18n ( "User" ) );
    userPageItem->setIcon(KIcon("list-add-user"));

//  configuration dialog for group settings
    QWidget *groupSettingsDlg = new QWidget(this);    
    ui_group = new Ui_usergroupPage();
    group = new UserGroupPageS(groupSettingsDlg, ui_group, myGroupSettings, myGeneralSettings);
    ui_group->setupUi( groupSettingsDlg );
    groupPageItem = dialog->addPage ( groupSettingsDlg, i18n ( "Group" ) );
    groupPageItem->setIcon(KIcon("user-group-new"));
    group->m_ui_usergroup->label_username->setText(i18n("Groupname"));
    group->m_ui_usergroup->kcfg_enabled->setText(i18n("enable Restriction for this Group"));


  
//  connect signals and slots
    connect_general(general);
    connect_usergroup_part1(user);
    connect_usergroup_part1(group);
    initial_load=false;
    load();
    user->fillUi_rulerLT();
    group->fillUi_rulerLT();
    user->setupUi_rulerLT();
    group->setupUi_rulerLT();
    user->setupUi_rulerPT();
    group->setupUi_rulerPT();
    user->fillUi_rulerPT();
    group->fillUi_rulerPT();
    user->set_initial_fill(false);
    group->set_initial_fill(false);
    connect_usergroup_part2(this, user);
    connect_usergroup_part2(this, group);
   
//    emit updateMe();
    dialog->show();
    
}


KchildlockKCM::~KchildlockKCM()
{
}

void KchildlockKCM::load()
{
   kDebug() << "KCM is loading config";
   if (initial_load) return;
    general->load();
   
    if (myGeneralSettings->usernamestringlist().isEmpty())  {
      current_user = "";
    } else {
      current_user = myGeneralSettings->usernamestringlist().at(0);
    }
    if (myGeneralSettings->groupnamestringlist().isEmpty())  {
      current_group = "";
    } else {
      current_group = myGeneralSettings->groupnamestringlist().at(0);
    }
   
    user->load("U", current_user);
    group->load("G", current_group);

   general->fillUi();
   user->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(general->m_GeneralSettings->usernamestringlist()));
   user->fillUi();
   group->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(general->m_GeneralSettings->groupnamestringlist()));
   group->fillUi();

   if (getuid() !=0) {
     dialog->enableButtonApply(FALSE);
     dialog->enableButtonOk(FALSE);
// future extension: disable all widgets
    }

    emit updateMe();
}

void KchildlockKCM::save()
{
  char appcommand[100];
  QDir currentDir;
  QStringList currentFiles;
  if (getuid() != 0) {
        return;
   }
   general->save();
   user->save();
   group->save();

// write same file from root's home to the global config file
   QString newconfigpath = KStandardDirs::installPath("config");
   if (QFile::exists("/root/.kde4/share/config")) {
      currentDir = QDir(newconfigpath);
      currentFiles = currentDir.entryList(QStringList("kchildlockrc*"), QDir::Files);
      for ( QStringList::Iterator it = currentFiles.begin(); it != currentFiles.end(); ++it ) {
         sprintf(appcommand, "rm %s",  (newconfigpath+(*it)).toAscii().constData());
         system(appcommand);
         }
      currentDir = QDir("/root/.kde4/share/config");
      currentFiles = currentDir.entryList(QStringList("kchildlockrc*"), QDir::Files);
      for ( QStringList::Iterator it = currentFiles.begin(); it != currentFiles.end(); ++it ) {
         sprintf(appcommand, "cp -f %s %s", ("/root/.kde4/share/config/"+(*it)).toAscii().constData(), (newconfigpath+(*it)).toAscii().constData());
         system(appcommand);
         QFile::setPermissions(newconfigpath+(*it), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
         }

   } else {
      currentDir = QDir(newconfigpath);
      currentFiles = currentDir.entryList(QStringList("kchildlockrc*"), QDir::Files);
      for ( QStringList::Iterator it = currentFiles.begin(); it != currentFiles.end(); ++it ) {
         sprintf(appcommand, "rm %s",  (newconfigpath+(*it)).toAscii().constData());
         system(appcommand);
         }
      currentDir = QDir("/root/.kde/share/config");
      currentFiles = currentDir.entryList(QStringList("kchildlockrc*"), QDir::Files);
      for ( QStringList::Iterator it = currentFiles.begin(); it != currentFiles.end(); ++it ) {
         sprintf(appcommand, "cp -f %s %s", ("/root/.kde/share/config/"+(*it)).toAscii().constData(), (newconfigpath+(*it)).toAscii().constData());
         system(appcommand);
         QFile::setPermissions(newconfigpath+(*it), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
         }
   }
}



void KchildlockKCM::emitChanged_usergroupname(QModelIndex idx)
{
  KPageWidgetItem *current_item = dialog->currentPage();
  if (idx.isValid())  {
    if (current_item == userPageItem) {
        current_user = myGeneralSettings->usernamestringlist().at(idx.row());
        emit cleanUpRuler();
//        user->cleanUi();
        user->set_initial_fill(true);
        user->load("U",  current_user);
        user->fillUi();
        user->set_initial_fill(false);

    } else {
        current_group = myGeneralSettings->groupnamestringlist().at(idx.row());
        emit cleanUpRuler();
//        group->cleanUi();
        group->set_initial_fill(true);
        group->load("G",  current_group);
        group->fillUi();
        group->set_initial_fill(false);
    }
  }
  emit updateMe();
}







void KchildlockKCM::emitNewUserGroup()
{
    KDialog *usrdialog = new KDialog (this);
    QWidget *wg = new QWidget( usrdialog );
    Ui_selectUserGroupDialog *selectDlg = new Ui_selectUserGroupDialog();
    selectDlg->setupUi(usrdialog);
    usrdialog->setMainWidget(wg);

    if (dialog->currentPage() == userPageItem ) {
      selectDlg->comboBox->insertItems (0,this->getlistofusers());
    } else {
      usrdialog->setWindowTitle(i18n("Select Group"));
      selectDlg->comboBox->insertItems (0,this->getlistofgroups());
      selectDlg->label->setText(i18n("Select Group for Restrictions"));
    }
    
   connect( usrdialog, SIGNAL( okClicked() ), this, SLOT( save_usergroupnameOK() ) );
   connect( selectDlg->comboBox, SIGNAL( currentIndexChanged(QString)), this, SLOT(save_usergroupnameT(QString)));

   usrdialog->show();
  
}


void KchildlockKCM::save_usergroupnameT(QString val)
{
  new_usergroup = val;
}




void KchildlockKCM::save_usergroupnameOK()
{
  QStringList new_list;
  if (dialog->currentPage() == userPageItem ) {
    new_list << user->m_GeneralSettings->usernamestringlist();
    new_list.removeOne("");
    if (new_usergroup == "") {
      current_user  = new_usergroup;
      user->fillUi();
    } else {
      new_list.append(new_usergroup);
      new_list.removeOne("");
      user->m_GeneralSettings->setUsernamestringlist(new_list);
      user->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(new_list));
    
      int rows = user->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->rowCount();
      QModelIndex idx = user->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->index(rows-1,0);
      user->m_ui_usergroup->kcfg_usergroupnamestringlist->setCurrentIndex(idx);
        
      current_user  = new_usergroup;
      emit cleanUpRuler();
      user->load("U", current_user);
      user->fillUi();
    }
 
  } else {
    new_list << group->m_GeneralSettings->usernamestringlist();
    new_list.removeOne("");
    if (new_usergroup == "") {
      current_group = new_usergroup;
      group->fillUi();
    } else {
      new_list.append(new_usergroup);
      new_list.removeOne("");
      group->m_GeneralSettings->setGroupnamestringlist(new_list);
      group->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(new_list));
    
      int rows = group->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->rowCount();
      QModelIndex idx = group->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->index(rows-1,0);
      group->m_ui_usergroup->kcfg_usergroupnamestringlist->setCurrentIndex(idx);

      current_group = new_usergroup;
      emit cleanUpRuler();
      group->load("G", current_group);
      group->fillUi();
    }
  }
  new_usergroup = "";
  emit changed(true);
  emit updateMe();

}







void KchildlockKCM::emitDelUserGroup()
{
    KDialog *usrdialog = new KDialog (this);
    QWidget *wg = new QWidget( usrdialog );
    Ui_deleteUserGroupDialog *deleteDlg = new Ui_deleteUserGroupDialog();
    deleteDlg->setupUi(usrdialog);
    usrdialog->setMainWidget(wg);

    if (dialog->currentPage() == userPageItem ) {
      deleteDlg->label_name->setText (current_user);
    } else {
      usrdialog->setWindowTitle(i18n("Delete restrictions"));
      deleteDlg->label_name->setText(current_group);
      deleteDlg->label_text->setText(i18n("Delete Restrictions for Group"));
    }
    
   connect( usrdialog, SIGNAL( okClicked() ), this, SLOT( delete_usergroupnameOK() ) );

   usrdialog->show();
  
}




void KchildlockKCM::delete_usergroupnameOK()
{
   QString configfile1 = KStandardDirs::installPath("config").append("kchildlockrc");
   QString configfile2 = "/root/.kde4/share/config/kchildlockrc";
   QString configfile3 = "/root/.kde/share/config/kchildlockrc";
   QStringList new_list;
   
  if (dialog->currentPage() == userPageItem ) {
    if (current_user == "") return;
    new_list << user->m_GeneralSettings->usernamestringlist();
    new_list.removeOne("");
    new_list.removeOne(current_user);
    user->m_GeneralSettings->setUsernamestringlist(new_list);
    user->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(new_list));
    QFile::remove(configfile1.append("_U_").append(current_user));
    QFile::remove(configfile2.append("_U_").append(current_user));
    QFile::remove(configfile3.append("_U_").append(current_user));
    current_user  = "";
    emit cleanUpRuler();
    user->load("U", current_user);
    user->fillUi();
    

  } else {
    if (current_group == "") return;
    new_list << user->m_GeneralSettings->groupnamestringlist();
    new_list.removeOne("");
    new_list.removeOne(current_group);
    user->m_GeneralSettings->setGroupnamestringlist(new_list);
    user->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(new_list));
    QFile::remove(configfile1.append("_G_").append(current_group));
    QFile::remove(configfile2.append("_G_").append(current_group));
    QFile::remove(configfile2.append("_G_").append(current_group));
    current_group  = "";
    emit cleanUpRuler();
    user->load("G", current_group);
    user->fillUi();

  }
  emit changed(true);
  emit updateMe();

}







void KchildlockKCM::emitExpUserGroup()
{
    QDir currentDir;
    QStringList currentFiles;
    const QString startdir = "/home";
    const QString caption = i18n("Export KChildlock Profiles");
    KUrl dirName = KDirSelectDialog::selectDirectory(startdir, TRUE, this, caption);
    if (dirName.isEmpty()) {
        return;
    }

// write all files from root's home to the export dir
   if (QFile::exists("/root/.kde4/share/config/kchildlockrc")) {
      currentDir = QDir("/root/.kde4/share/config");
      currentFiles = currentDir.entryList(QStringList("kchildlockrc*"), QDir::Files | QDir::NoSymLinks);
      for ( QStringList::Iterator it = currentFiles.begin(); it != currentFiles.end(); ++it ) {
         QFile::copy(("/root/.kde4/share/config/")+(*it), dirName.toLocalFile()+("/")+(*it));
         QFile::setPermissions(dirName.toLocalFile().append("/").append(*it), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
         }

   } else {
      currentDir = QDir("/root/.kde/share/config");
      currentFiles = currentDir.entryList(QStringList("kchildlockrc*"), QDir::Files | QDir::NoSymLinks);
      for ( QStringList::Iterator it = currentFiles.begin(); it != currentFiles.end(); ++it ) {
         QFile::copy(("/root/.kde/share/config/")+(*it), dirName.toLocalFile()+("/")+(*it));
         QFile::setPermissions(dirName.toLocalFile().append("/").append(*it), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
         }
   }
}






void KchildlockKCM::emitImpUserGroup()
{
    QStringList new_list;
    const KUrl startdir = KUrl::fromPath("/home");
    const QString filter = "kchildlockrc_*";
    const QString caption = i18n("Import KChildlock Profiles");
    QString absFileName = KFileDialog::getOpenFileName(startdir, filter, this, caption);
    if (absFileName.isEmpty()) {
        return;
    }
    QFileInfo fi(absFileName);
    QString fileName = fi.fileName();
    if (fi.suffix() != "") {
       return;
    }
    QStringList fileparts = fileName.split("_");
    if (fileparts.at(2) == "") {
       return;
    }
    if (fileparts.at(1) == "U") {
       current_user = fileparts.at(2);
    }
    else if (fileparts.at(1) == "G") {
       current_group = fileparts.at(2);
    }
    else {
       return;
    }
    
// copy the file to the config dirs
   if (QFile::exists("/root/.kde4/share/config/kchildlockrc")) {
      QFile::remove((KStandardDirs::installPath("config"))+("/")+(fileName));
      QFile::remove(("/root/.kde4/share/config/")+(fileName));
      QFile::copy(absFileName, (KStandardDirs::installPath("config"))+("/")+(fileName));
      QFile::copy(absFileName, ("/root/.kde4/share/config/")+(fileName));
      QFile::setPermissions((KStandardDirs::installPath("config"))+("/")+(fileName), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
      QFile::setPermissions(("/root/.kde4/share/config/")+(fileName), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
   } else {
      QFile::remove((KStandardDirs::installPath("config"))+("/")+(fileName));
      QFile::remove(("/root/.kde/share/config/")+(fileName));
      QFile::copy(absFileName, (KStandardDirs::installPath("config"))+("/")+(fileName));
      QFile::copy(absFileName, ("/root/.kde/share/config/")+(fileName));
      QFile::setPermissions((KStandardDirs::installPath("config"))+("/")+(fileName), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
      QFile::setPermissions(("/root/.kde/share/config/")+(fileName), QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
   }

// add imported profile to the list of users / groups
  if (fileparts.at(1) == "U") {
      new_list << user->m_GeneralSettings->usernamestringlist();
      if (new_list.at(0) == "") {
        new_list << current_user;
      } else {
        new_list.append(current_user);
      }
      new_list.removeOne("");
      user->m_GeneralSettings->setUsernamestringlist(new_list);
      user->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(new_list));
    
      int rows = user->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->rowCount();
      QModelIndex idx = user->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->index(rows-1,0);
      user->m_ui_usergroup->kcfg_usergroupnamestringlist->setCurrentIndex(idx);
        
      emit cleanUpRuler();
      user->load("U", current_user);
      user->fillUi();
 
  } else {
      new_list << group->m_GeneralSettings->groupnamestringlist();
      if (new_list.at(0) == "") {
        new_list << current_group;
      } else {
        new_list.append(current_group);
      }
      new_list.removeOne("");
      group->m_GeneralSettings->setGroupnamestringlist(new_list);
      group->m_ui_usergroup->kcfg_usergroupnamestringlist->setModel(new QStringListModel(new_list));
    
      int rows = group->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->rowCount();
      QModelIndex idx = group->m_ui_usergroup->kcfg_usergroupnamestringlist->model()->index(rows-1,0);
      group->m_ui_usergroup->kcfg_usergroupnamestringlist->setCurrentIndex(idx);
        
      emit cleanUpRuler();
      group->load("G", current_group);
      group->fillUi();
 
  }

// write same general config file from root's home to the global config file
   QFile::remove((KStandardDirs::installPath("config"))+"/kchildlockrc");
   if (QFile::exists("/root/.kde4/share/config/kchildlockrc")) {
      QFile::copy(("/root/.kde4/share/config/kchildlockrc"), KStandardDirs::installPath("config")+"/kchildlockrc");
      QFile::setPermissions(KStandardDirs::installPath("config")+"/kchildlockrc", QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
   } else {
      QFile::copy(("/root/.kde/share/config/kchildlockrc"), KStandardDirs::installPath("config")+"/kchildlockrc");
      QFile::setPermissions(KStandardDirs::installPath("config")+"/kchildlockrc", QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);
   }


  emit changed(true);
  emit updateMe();
}




QStringList KchildlockKCM::getlistofusers()
{
    KUser* myKusers = new KUser();
    KUser myKuser;
    QStringList fulluserlist, effuserlist;
    fulluserlist = myKusers->allUserNames();
    effuserlist = QStringList();
    for ( QStringList::Iterator it = fulluserlist.begin(); it != fulluserlist.end(); ++it ) {
	myKuser = KUser(*it);
	if (myKuser.uid() >= USERIDLOWLIMIT) {
	   effuserlist.append(*it);
	}
    }		
    return effuserlist;
}


QStringList KchildlockKCM::getlistofgroups()
{
    KUserGroup* myKgroups = new KUserGroup();
    KUserGroup myKgroup;
    QStringList fullgrouplist, effgrouplist;
    fullgrouplist = myKgroups->allGroupNames();
    effgrouplist = QStringList();
    for ( QStringList::Iterator it = fullgrouplist.begin(); it != fullgrouplist.end(); ++it ) {
	myKgroup = KUserGroup(*it);
	if (myKgroup.gid() >= GROUPIDLOWLIMIT) {
	   effgrouplist.append(*it);
	}
    }		
    return effgrouplist;
}


QString KchildlockKCM::quickHelp() const
{
  return i18n("<h1>Kchildlock configuration</h1><p>This module allows you to configure your childrens restriction settings.</p>");
}



#include "KchildlockKCM.moc"
