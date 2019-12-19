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

#include "themewidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>


ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(100),
    m_valueMax(10),
    m_valueCount(96),
    m_dataTable(generateRandomData(m_listCount, m_valueMax, m_valueCount)),
    m_themeComboBox(createThemeBox()),
    m_antialiasCheckBox(new QCheckBox("Anti-aliasing")),
    m_animatedComboBox(createAnimationBox())
{
    getFileData("train.tra");
    m_legendComboBox = createLegendBox();
    connectSignals();
    // create layout
    QGridLayout *baseLayout = new QGridLayout();
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel("Theme:"));
    settingsLayout->addWidget(m_themeComboBox);
    settingsLayout->addWidget(new QLabel("Animation:"));
    settingsLayout->addWidget(m_animatedComboBox);
    settingsLayout->addWidget(new QLabel("Value:"));
    settingsLayout->addWidget(m_legendComboBox);
    settingsLayout->addWidget(m_antialiasCheckBox);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout, 0, 0, 1, 2);

    //create charts

    QChartView *chartView;

    chartView = new QChartView(createLineChart(m_values[0] + m_face[0], m_dataMap[m_values[0] + m_face[0]]));
    baseLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    chartView = new QChartView(createLineChart(m_values[0] + m_face[1], m_dataMap[m_values[0] + m_face[1]]));
    baseLayout->addWidget(chartView, 1, 1);
    m_charts << chartView;

    chartView = new QChartView(createLineChart(m_values[0] + m_face[2], m_dataMap[m_values[0] + m_face[2]]));
    baseLayout->addWidget(chartView, 2, 0);
    m_charts << chartView;

    chartView = new QChartView(createLineChart(m_values[0] + m_face[3], m_dataMap[m_values[0] + m_face[3]]));
    baseLayout->addWidget(chartView, 2, 1);
    m_charts << chartView;

    setLayout(baseLayout);

    // Set defaults
    m_antialiasCheckBox->setChecked(true);
    updateUI();
}

ThemeWidget::~ThemeWidget()
{
}

void ThemeWidget::connectSignals()
{
    connect(m_themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
    connect(m_antialiasCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateUI()));
    connect(m_animatedComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
    connect(m_legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
}

DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
//            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) m_valueMax / (qreal) valueCount), yValue);
            QPointF value(j, yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}


void ThemeWidget::getFileData(const QString &file)
{
    //DataMap dataMap;
    DataTable dataTable;

    QList<QString> Lines;
    QString l;
    QFile f(file);
    qint32 featurenum, i, linenum;
    QString country;
    QList<QString> y;
    QList<QString> values;
    QString faceTemp;
    QString valueTemp;
    QString valueFaceTemp;


    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open file wrong!";
    }

    if(!f.atEnd())
    {
        for(i=0; i<6; i++)   //前面6行非数据
        {
            l = f.readLine();
            Lines << l;
        }
        linenum = Lines[0].toInt();
        featurenum = Lines[2].toInt();
        m_valuenum = Lines[3].toInt();
        m_values = Lines[4].split(" ");
        country = Lines[5];
    }
    Lines.clear();

 //   while (!f.atEnd())
    for (int z(0); z < linenum; z++)
    {
        l = f.readLine();
        y = l.split(" ");

        //get face
        for(i=0; i<4; i++)
        {
            if(1.0 - y[i].toFloat() < 0.01 )
                break;
        }
        if(i >= 4)
            continue;
        faceTemp = m_face[i];

        //get values
        for(i=4; i<4+m_valuenum; i++)
        {
            if(1.0 - y[i].toFloat() < 0.01 )
                break;
        }
        if(i >= 4+m_valuenum)
            continue;
        valueTemp = m_values[i-4];
        valueFaceTemp = valueTemp + faceTemp;

        DataList dataList;
        for (int j(0); j < featurenum; j++)
        {
            QPointF value(j, y[j+m_valuenum+4].toFloat());
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }

        m_dataMap[valueFaceTemp].push_back(dataList);
    }

}

QComboBox *ThemeWidget::createThemeBox() const
{
    // settings layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Light", QChart::ChartThemeLight);
    themeComboBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    themeComboBox->addItem("Dark", QChart::ChartThemeDark);
    themeComboBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    themeComboBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    themeComboBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    themeComboBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    themeComboBox->addItem("Qt", QChart::ChartThemeQt);
    return themeComboBox;
}

QComboBox *ThemeWidget::createAnimationBox() const
{
    // settings layout
    QComboBox *animationComboBox = new QComboBox();
    animationComboBox->addItem("No Animations", QChart::NoAnimation);
    animationComboBox->addItem("GridAxis Animations", QChart::GridAxisAnimations);
    animationComboBox->addItem("Series Animations", QChart::SeriesAnimations);
    animationComboBox->addItem("All Animations", QChart::AllAnimations);
    return animationComboBox;
}

QComboBox *ThemeWidget::createLegendBox() const
{
    /*
    QComboBox *legendComboBox = new QComboBox();
    legendComboBox->addItem("No Legend ", 0);
    legendComboBox->addItem("Legend Top", Qt::AlignTop);
    legendComboBox->addItem("Legend Bottom", Qt::AlignBottom);
    legendComboBox->addItem("Legend Left", Qt::AlignLeft);
    legendComboBox->addItem("Legend Right", Qt::AlignRight);
    return legendComboBox;
    */
    QComboBox *legendComboBox = new QComboBox();
    for(int i(0); i<m_valuenum; i++ )
    {
        legendComboBox->addItem(m_values[i], 0);
    }

    return legendComboBox;
}


QChart *ThemeWidget::createLineChart(const QString &str, const DataTable &dataTable) const
{
    QChart *chart = new QChart();
    chart->setTitle(str);
    QString name("Series ");
    int nameIndex = 0;

    foreach (DataList list, dataTable ) {
        QLineSeries *series = new QLineSeries(chart);
        foreach (Data data, list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();

    return chart;
}

/*
void ThemeWidget::updateUI()
{
    int idx = m_legendComboBox->currentIndex();
}*/


void ThemeWidget::updateUI()
{
    QChart::ChartTheme theme = (QChart::ChartTheme) m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt();

    if (m_charts.at(0)->chart()->theme() != theme) {
        foreach (QChartView *chartView, m_charts)
            chartView->chart()->setTheme(theme);

        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight) {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeDark) {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBlueCerulean) {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBrownSand) {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeBlueNcs) {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeHighContrast) {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        } else if (theme == QChart::ChartThemeBlueIcy) {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }

    bool checked = m_antialiasCheckBox->isChecked();
    foreach (QChartView *chart, m_charts)
        chart->setRenderHint(QPainter::Antialiasing, checked);

    QChart::AnimationOptions options(m_animatedComboBox->itemData(m_animatedComboBox->currentIndex()).toInt());
    if (m_charts.at(0)->chart()->animationOptions() != options) {
        foreach (QChartView *chartView, m_charts)
            chartView->chart()->setAnimationOptions(options);
    }

    Qt::Alignment alignment(m_legendComboBox->itemData(m_legendComboBox->currentIndex()).toInt());

    if (!alignment) {
        foreach (QChartView *chartView, m_charts)
            chartView->chart()->legend()->hide();
    } else {
        foreach (QChartView *chartView, m_charts) {
            chartView->chart()->legend()->setAlignment(alignment);
            chartView->chart()->legend()->show();
//            chartView->chart()->show();
        }
    }
}


