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

#include <QGraphicsView>
#include <QBrush>
#include <Qt>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <kdebug.h>
#include <QGraphicsSceneMouseEvent>
#include <QTime>

#include "ruler_scene.h"


// Create green rectangle for allowed time range
// For the first call, the time values are not yet loaded

MyScene::MyScene(QWidget *parent, const QRect &my_scenerect, KIntNumInput *my_timefrom, KIntNumInput *my_timeto)
{
    int posx, length;
    
    m_timefrom = my_timefrom;
    m_timeto = my_timeto;
    scene_x = my_scenerect.x() + KCH_SCENE_OFFSET_X;
    scene_y = my_scenerect.y() + KCH_SCENE_OFFSET_Y ;
    scene_w = my_scenerect.width() - 1;
    scene_h = my_scenerect.height();
    movepart = 0;
   
    posx = scene_x + ( ( m_timefrom->value() * scene_w ) / 1440 );
    length = ( ( m_timeto->value() - m_timefrom->value() ) * scene_w) / 1440;
    my_rect = addRect(posx+2, scene_y, length-4, scene_h, QPen(Qt::green), QBrush(Qt::green));
    my_rect->setFocus();
    my_rect->setCursor(Qt::OpenHandCursor);
    my_rect->setVisible(TRUE);
    my_rect->setEnabled(TRUE);
    
//    kDebug() << "SCENE" << scene_x << scene_y << scene_w << scene_h;
//    kDebug() << "TIMES" << m_timefrom->value() << m_timeto->value();
}





MyScene::~MyScene()
{
}


// after the settings are loaded, draw the green rectangle with left and right trim
// the middle rectangle does moving, the left and right move the upper and lower limit

void MyScene::slotUpdateMe()
{
    int posx, length;

    posx = scene_x + ( m_timefrom->value() * scene_w) / 1440;
    length = ( ( m_timeto->value() - m_timefrom->value() ) * scene_w) / 1440;
    my_rect->setRect(posx+2, scene_y, length-4, scene_h);
    
    my_ltrim = addRect(posx, scene_y, 2, scene_h, QPen(Qt::yellow), QBrush(Qt::yellow));
    my_ltrim->setFocus();
    my_ltrim->setCursor(Qt::SplitHCursor);
    my_ltrim->setVisible(TRUE);
    my_ltrim->setEnabled(TRUE);

    my_rtrim = addRect(posx+length-1, scene_y, 2, scene_h, QPen(Qt::yellow), QBrush(Qt::yellow));
    my_rtrim->setFocus();
    my_rtrim->setCursor(Qt::SplitHCursor);
    my_rtrim->setVisible(TRUE);
    my_rtrim->setEnabled(TRUE);

    QString s;
    my_ltext = addSimpleText(s.sprintf("%02d:%02d",(m_timefrom->value() / 60), (m_timefrom->value() % 60)));
    my_ltext->setPos(posx-35, scene_y);
    my_ltext->setVisible(TRUE);
    my_ltext->setEnabled(TRUE);
    my_ltext->setZValue(900);

    my_rtext = addSimpleText(s.sprintf("%02d:%02d",(m_timeto->value() / 60), (m_timeto->value() % 60)));
    my_rtext->setPos(posx+length-35, scene_y);
    my_rtext->setVisible(TRUE);
    my_rtext->setEnabled(TRUE);
    my_rtext->setZValue(900);
//   kDebug() << "call with" << posx << scene_y << length;
    
    update();
}




void MyScene::slotCleanUpRuler()
{
    int posx, length;

    posx = scene_x + 1440 / 2;
    length = 0;
    my_rect->setRect(posx, scene_y, length, scene_h);
    
    my_ltrim->~QGraphicsRectItem();
    my_rtrim->~QGraphicsRectItem();
    my_ltext->~QGraphicsSimpleTextItem();
    my_rtext->~QGraphicsSimpleTextItem();
    kDebug() << "clean up Ruler";
    
//    update();
}



void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
  QGraphicsScene::mousePressEvent(ev);
  if (ev->buttons() == Qt::LeftButton) {
     my_selection = " ";
     movepart = 0;
//     kDebug() << "Mouse Down";
     if (my_rect->isUnderMouse()) my_selection = "M";
     if (my_rtrim->isUnderMouse()) my_selection = "R";
     if (my_ltrim->isUnderMouse()) my_selection = "L";
  }
}


void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev)
{
  int val;
  qreal deltax;
  QString s;
  QGraphicsScene::mouseMoveEvent(ev);

  movepart = movepart + ev->scenePos().x() - ev->lastScenePos().x();
  deltax = (movepart / KCH_MIN_MOUSE_MOVE) * KCH_MIN_MOUSE_MOVE;
  movepart = movepart % KCH_MIN_MOUSE_MOVE;

  //  moving the range - adjust from time and to time    
  if ((ev->buttons() == Qt::LeftButton) && (my_selection == "M") && (deltax != 0)) {
       if ( my_rtrim->rect().x() + my_rtrim->x() + my_rtrim->rect().width() + 1 + deltax > scene_x + scene_w ) {
           deltax =  scene_x + scene_w - my_rtrim->x() - my_rtrim->rect().x() - 1 ;
       }
       if ( my_ltrim->rect().x() + my_ltrim->x() + deltax - scene_x < 0 ) {
           deltax = scene_x - my_ltrim->rect().x() - my_ltrim->x();	
       }
       my_rect->setPos(my_rect->x() + deltax, my_rect->y());
       my_ltrim->setPos(my_ltrim->x() + deltax, my_ltrim->y());
       my_rtrim->setPos(my_rtrim->x() + deltax, my_rtrim->y());
  
       val = ( ( my_ltrim->rect().x() + my_ltrim->x() - scene_x) * 1440 ) / scene_w;
       m_timefrom->setValue(val);
       val = ( ( my_rtrim->rect().x() + my_rtrim->x() + 1 - scene_x) * 1440 ) / scene_w;
       m_timeto->setValue(val);

       my_ltext->setText(s.sprintf("%02d:%02d",(m_timefrom->value() / 60), (m_timefrom->value() % 60)));
       my_ltext->setPos(my_ltext->x() + deltax, my_ltext->y());

       my_rtext->setText(s.sprintf("%02d:%02d",(m_timeto->value() / 60), (m_timeto->value() % 60)));
       my_rtext->setPos(my_rtext->x() + deltax, my_rtext->y());
  }

  // moving left trim - adjust from time
  if ((ev->buttons() == Qt::LeftButton) && (my_selection == "L") && (deltax != 0)) {
       if ( my_ltrim->rect().x() + my_ltrim->x() + deltax - scene_x < 0 ) {
           deltax = scene_x - my_ltrim->rect().x() - my_ltrim->x();
       }
       if ( my_ltrim->rect().x() + my_ltrim->x() + deltax - scene_x + 4 >=
            my_rtrim->rect().x() + my_rtrim->x() + deltax - scene_x ) {
           deltax = my_rtrim->rect().x() + my_rtrim->x() - my_ltrim->rect().x() - my_ltrim->x() -4;
       }
       my_rect->setRect(my_rect->rect().x() + deltax, my_rect->rect().y(), my_rect->rect().width() - deltax, my_rect->rect().height());
       my_ltrim->setPos(my_ltrim->x() + deltax, my_ltrim->y());
       
       val = ( ( my_ltrim->rect().x() + my_ltrim->x() - scene_x) * 1440 ) / scene_w;
       m_timefrom->setValue(val);

       my_ltext->setText(s.sprintf("%02d:%02d",(m_timefrom->value() / 60), (m_timefrom->value() % 60)));
       my_ltext->setPos(my_ltext->x() + deltax, my_ltext->y());
    }    
      
  // moving right trim - adjust to time
  if ((ev->buttons() == Qt::LeftButton) && (my_selection == "R") && (deltax != 0)) {
       if ( my_rtrim->rect().x() + my_rtrim->x() + my_rtrim->rect().width() + 1 + deltax > scene_x + scene_w ) {
           deltax =  scene_x + scene_w - my_rtrim->x() - my_rtrim->rect().x() - 1 ;
       }
       if ( my_ltrim->rect().x() + my_ltrim->x() + 4 > my_rtrim->rect().x() + my_rtrim->x() + deltax + 1) {
            deltax = my_ltrim->rect().x() + my_ltrim->x() - my_rtrim->rect().x() - my_rtrim->x() + 4 - 1;
       }
       my_rect->setRect(my_rect->rect().x(), my_rect->rect().y(), my_rect->rect().width() + deltax, my_rect->rect().height());
       my_rtrim->setPos(my_rtrim->x() + deltax, my_rtrim->y());
       
       val = ( ( my_rtrim->rect().x() + my_rtrim->x() + 1 - scene_x ) * 1440 ) / scene_w;
       m_timeto->setValue(val);

       my_rtext->setText(s.sprintf("%02d:%02d",(m_timeto->value() / 60), (m_timeto->value() % 60)));
       my_rtext->setPos(my_rtext->x() + deltax, my_rtext->y());
  }    
  update();
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
  QGraphicsScene::mouseReleaseEvent(ev);
  update();
}


bool MyScene::event(QEvent *ev)
{
  bool ret = QGraphicsScene::event(ev);
  return ret;
}
