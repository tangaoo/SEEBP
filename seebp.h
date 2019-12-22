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

#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QGridLayout>
#include <QMap>


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPointF                  Data;
typedef QList<Data>              DataList;
typedef QList<DataList>          DataTable;
typedef QMap<QString, DataTable> DataMap;

QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();
    void updateUIII();
    void buttonReleased();
    void selectedLine();

private:
    DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    void getFileData(const QString &file);
    QComboBox *createThemeBox() const;
    QComboBox *createAnimationBox() const;
    QComboBox *createLegendBox() const;
    void connectSignals();
    QChart *createLineChart(const QString &str) const;
    void reStart(void);

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    int m_valuenum;
    QList<QChartView *> m_charts;
    DataTable m_dataTable;
    DataMap m_dataMap;
    const QVector<QString> m_face = {"_A", "_B", "_C", "_D", ""}; //向量越界
    QList<QString> m_values;

    QChart *m_chart_A;
    QChart *m_chart_B;
    QChart *m_chart_C;
    QChart *m_chart_D;

    QLineEdit *m_lineEdit;
    QPushButton *m_button;
    QLineEdit *m_lineEdit2;
    QPushButton *m_button2;
    QGridLayout *baseLayout;
    QComboBox *m_themeComboBox;
    QCheckBox *m_antialiasCheckBox;
    QComboBox *m_animatedComboBox;
    QComboBox *m_legendComboBox;
};

#endif /* THEMEWIDGET_H */
