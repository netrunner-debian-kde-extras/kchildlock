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

#ifndef RULERSCENE_H
#define RULERSCENE_H

#include <QGraphicsScene>
#include <QTabWidget>
#include <QTimeEdit>
#include <KIntNumInput>

#define KCH_SCENE_OFFSET_X 200
#define KCH_SCENE_OFFSET_Y 60
#define KCH_MIN_MOUSE_MOVE 4



class MyScene : public QGraphicsScene
{
  Q_OBJECT
  
public:
  explicit MyScene(QWidget *parent, const QRect &my_scenerect, KIntNumInput *my_timefrom, KIntNumInput *my_timeto);
  ~MyScene();

protected:
  bool event(QEvent *event);
  
private:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent*);

  QGraphicsRectItem *my_rect;
  QGraphicsRectItem *my_ltrim;
  QGraphicsRectItem *my_rtrim;
  KIntNumInput *m_timefrom;
  KIntNumInput *m_timeto;
  QGraphicsSimpleTextItem *my_ltext;
  QGraphicsSimpleTextItem *my_rtext;
  
  QString my_selection;
  int scene_x, scene_y, scene_w, scene_h, movepart;
  
public slots:
  void slotUpdateMe();
  void slotCleanUpRuler();

};


#endif /*RULERSCENE_H*/
