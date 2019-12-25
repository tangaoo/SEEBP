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

#include "seebp.h"

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QDebug>

//QMap<QString, QList<QLineSeries *>> g_mapSeries;

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(100),
    m_valueMax(10),
    m_valueCount(96),
    m_lineEdit(new QLineEdit),
    m_button(new QPushButton("Open")),
    m_lineEdit2(new QLineEdit),
    m_button2(new QPushButton("Delete")),
    m_themeComboBox(createThemeBox()),
    m_antialiasCheckBox(new QCheckBox("Anti-aliasing")),
    m_animatedComboBox(createAnimationBox())
{
    m_legendComboBox = createLegendBox();
    connectSignals();
    // create layout
    baseLayout = new QGridLayout();
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(new QLabel(" Path:"));
    settingsLayout->addWidget(m_lineEdit);
    settingsLayout->addWidget(m_button);
    settingsLayout->addWidget(new QLabel("  ErrLines:"));
    m_lineEdit2->setReadOnly(true);
    settingsLayout->addWidget(m_lineEdit2);
    settingsLayout->addWidget(m_button2);
    settingsLayout->addWidget(new QLabel("  Value:"));
    settingsLayout->addWidget(m_legendComboBox);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout, 0, 0, 1, 2);

    //create charts

    QChartView *chartView;

    m_chart_A = createLineChart("null_A");
    chartView = new QChartView(m_chart_A);
    baseLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    m_chart_B = createLineChart("null_B");
    chartView = new QChartView(m_chart_B);
    baseLayout->addWidget(chartView, 1, 1);
    m_charts << chartView;

    m_chart_C = createLineChart("null_C");
    chartView = new QChartView(m_chart_C);
    baseLayout->addWidget(chartView, 2, 0);
    m_charts << chartView;

    m_chart_D = createLineChart("null_D");
    chartView = new QChartView(m_chart_D);
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
    connect(m_legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUIII()));
    connect(m_button, SIGNAL(released()), this, SLOT(buttonReleased()));
    connect(m_button2, SIGNAL(released()), this, SLOT(delBtnReleased()));

}

void ThemeWidget::getFileData(const QString &file)
{    
    QList<QString> Lines;
    QString l;
    QFile f(file);
    qint32 featurenum=0, linenum=0, i=0, j=0;
    QString country;
    QList<QString> y;
    QList<QString> values;
    QString faceTemp;
    QString valueTemp;
    QString valueFaceTemp;

    m_dataMap.clear();
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
        qDebug() << z;
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
        for (j = 0; j < featurenum; j++)
        {
            QPointF value(j, y[j+m_valuenum+4].toFloat());
            dataList << value;
        }
        QPointF value(j, z+7);
        qDebug() << value;
        dataList << value;  //标记行号

        m_dataMap[valueFaceTemp].push_back(dataList);
    }

    f.close();

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
    QComboBox *legendComboBox = new QComboBox();

    return legendComboBox;
}


QChart *ThemeWidget::createLineChart(const QString &str) const
{
    QChart *chart = new QChart();
    chart->setTitle(str);    

    chart->createDefaultAxes();

    return chart;
}

void ThemeWidget::delBtnReleased(void)
{
    if(m_fileName.isEmpty() || m_lineEdit2->text().isEmpty())
        return;

    //analysis data
    QList<QString> linenums = m_lineEdit2->text().split(" ");
    linenums.removeFirst();
    qDebug() << linenums.size();

    //write to new file
    QString w_fileName = m_fileName.split(".").first() + "_new." + m_fileName.split(".").last();
    QFile f_r(m_fileName);
    QFile f_w(w_fileName);

    qDebug() << w_fileName;

    if (!f_r.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "open f_r wrong!";
        return;
    }
    if (!f_w.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "open f_w wrong!";
        return;
    }

    QString l = "";
    QList<QString> l_list;
    while(!f_r.atEnd())
    {
        l = f_r.readLine();
        l_list << l;
    }
    QList<QString>::iterator it = l_list.begin();
    *it = QString::number((*it).toInt() - linenums.size()) + "\n";
    foreach (l, linenums)
    {
        l_list.removeAt(l.toInt() -1);
    }

    QTextStream stream(&f_w);
    stream.seek(0);
    foreach (l, l_list)
    {
        stream << l;
    }

    m_lineEdit2->clear();

    f_r.close();
    f_w.close();

    m_fileName = w_fileName;
    if(m_fileName.isEmpty())
        return;
    m_lineEdit->setText(m_fileName);

    disconnect(m_legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUIII()));
    m_legendComboBox->clear();
    connect(m_legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUIII()));

    getFileData(m_fileName);

    for(int i(0); i<m_valuenum; i++ )
    {
        m_legendComboBox->addItem(m_values[i], 0);
    }

}

void ThemeWidget::buttonReleased(void)
{
    QString curPath;
    if(m_lineEdit->text().isEmpty())
        curPath = QDir::currentPath();
    else
    {
        curPath = m_lineEdit->text();//.split(".").first();
    }
    QString digTitle = "Choose *.tra file";
    QString filter = "tra file(*.tra);;all files(*.*)";

    m_fileName = QFileDialog::getOpenFileName(this, digTitle, curPath, filter);
    if(m_fileName.isEmpty())
        return;
    m_lineEdit->setText(m_fileName);

    disconnect(m_legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUIII()));
    m_legendComboBox->clear();
    connect(m_legendComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUIII()));

    getFileData(m_fileName);

    for(int i(0); i<m_valuenum; i++ )
    {
        m_legendComboBox->addItem(m_values[i], 0);
    }

}

void ThemeWidget::selectedLine()
{
    QString errlineStr = "";

    QLineSeries *series = qobject_cast<QLineSeries*>(sender());  //get signal source

    errlineStr = m_lineEdit2->text() + " " + series->name();

    qDebug() << series->name();
    m_lineEdit2->setText(errlineStr);

}

void ThemeWidget::updateUIII()
{
    QString name("Series ");
    int idx = m_legendComboBox->currentIndex();
    QPointF value;

    //A面向
    m_chart_A->setTitle(m_values[idx] + m_face[0]);
    m_chart_A->removeAllSeries();
    foreach (DataList list, m_dataMap[m_values[idx] + m_face[0]] )
    {
        QLineSeries *series = new QLineSeries(m_chart_A);
        value = list.takeLast();
        series->append(list);
        series->setName(QString::number(value.ry()));
        connect(series, SIGNAL(clicked(QPointF)), this, SLOT(selectedLine()));
        m_chart_A->addSeries(series);
        series->setUseOpenGL(true);
    }
    m_chart_A->createDefaultAxes();

    //B面向
    m_chart_B->setTitle(m_values[idx] + m_face[1]);
    m_chart_B->removeAllSeries();
    foreach (DataList list, m_dataMap[m_values[idx] + m_face[1]] )
    {
        QLineSeries *series = new QLineSeries(m_chart_B);
        value = list.takeLast();
        series->append(list);
        series->setName(QString::number(value.ry()));
        connect(series, SIGNAL(clicked(QPointF)), this, SLOT(selectedLine()));
        m_chart_B->addSeries(series);
        series->setUseOpenGL(true);
    }
    m_chart_B->createDefaultAxes();

    //C面向
    m_chart_C->setTitle(m_values[idx] + m_face[2]);
    m_chart_C->removeAllSeries();
    foreach (DataList list, m_dataMap[m_values[idx] + m_face[2]] )
    {
        QLineSeries *series = new QLineSeries(m_chart_C);
        value = list.takeLast();
        series->append(list);
        series->setName(QString::number(value.ry()));
        connect(series, SIGNAL(clicked(QPointF)), this, SLOT(selectedLine()));
        m_chart_C->addSeries(series);
        series->setUseOpenGL(true);
    }
    m_chart_C->createDefaultAxes();

    //D面向
    m_chart_D->setTitle(m_values[idx] + m_face[3]);
    m_chart_D->removeAllSeries();
    foreach (DataList list, m_dataMap[m_values[idx] + m_face[3]] )
    {
        QLineSeries *series = new QLineSeries(m_chart_D);
        value = list.takeLast();
        series->append(list);
        series->setName(QString::number(value.ry()));
        connect(series, SIGNAL(clicked(QPointF)), this, SLOT(selectedLine()));
        m_chart_D->addSeries(series);
        series->setUseOpenGL(true);
    }
    m_chart_D->createDefaultAxes();

}


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
        }
    }
}


