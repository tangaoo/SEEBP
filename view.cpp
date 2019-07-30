/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include "callout.h"
#include <QtGui/QMouseEvent>
#include <QDebug>

View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0),
      m_tooltip(0)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QList<QString> Lines;
    Lines = GetData("train.tra");

    QStringList LineStr = Lines[10].split(" ");

    // chart
    m_chart = new QChart;
    m_chart->setMinimumSize(640, 480);
    m_chart->setTitle("Hover the line to show callout. Click the line to make it stay");
    m_chart->legend()->hide();
    QLineSeries *series = new QLineSeries;
    for(int i = 10; i<108; i++)
    {
        series->append(i, LineStr[i].toFloat());
    }
//    series->append(1, 3);
//    series->append(2, 5);
//    series->append(3, 4.5);
//    series->append(4, 1);
//    series->append(5, 2);
    m_chart->addSeries(series);

    QSplineSeries *series2 = new QSplineSeries;
    series2->append(1, 0.01);
    series2->append(2, 0.02);
    series2->append(3, 0.01);
    series2->append(4, 0);
    series2->append(5, 0);
    m_chart->addSeries(series2);

    QSplineSeries *series3 = new QSplineSeries;
    series3->append(1, 0.01);
    series3->append(2, 0.01);
    series3->append(3, 0.02);
    series3->append(4, 0);
    series3->append(5, 0);
    m_chart->addSeries(series3);

    m_chart->createDefaultAxes();
    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("Y: ");

    connect(series, SIGNAL(clicked(QPointF)), this, SLOT(keepCallout()));
    connect(series, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF,bool)));

    connect(series2, SIGNAL(clicked(QPointF)), this, SLOT(keepCallout()));
    connect(series2, SIGNAL(hovered(QPointF, bool)), this, SLOT(tooltip(QPointF,bool)));

    this->setMouseTracking(true);
}

QList<QString> View::GetData(QString filePath)
{
    QList<QString> Lines;
    QFile inf(filePath);
    if (!inf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open file wrong!";
        //return NULL;
    }


    while (!inf.atEnd()) {
        QByteArray line = inf.readLine();
        QString str(line);
//       qDebug() << str;
        Lines.append(str);
    }

//    QStringList LineStr = Lines[10].split(" ");
    return Lines;
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
         m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
         foreach (Callout *callout, m_callouts)
             callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}

void View::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void View::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}
