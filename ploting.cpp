#include "ploting.h"
#include "ui_ploting.h"

Ploting::Ploting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ploting)
{
    ui->setupUi(this);
    setWindowTitle("Plot");
    ui->widget->legend->setVisible(true);

	per = new Perceptron(this);

    class_1.resize(2);
    class_2.resize(2);
    class_user.resize(2);

    QObject::connect(per, &Perceptron::user_add_data, this, &Ploting::add_point);
}

Ploting::~Ploting()
{
    delete per;
    delete ui;
}

void Ploting::init_plot(const QVector<double> &dataX, const QVector<double> &dataY)
{
    k = (double)rand() / ((double)RAND_MAX + 1);
    b = (double)rand() / ((double)RAND_MAX + 1);

    class_1[0].clear();
    class_1[1].clear();

    class_2[0].clear();
    class_2[1].clear();

    class_user[0].clear();
    class_user[1].clear();

    for (int i = 0; i < dataX.size(); i++)
    {
        if (k * dataX[i] + b > dataY[i])
        {
            class_1[0].push_back(dataX[i]);
            class_1[1].push_back(dataY[i]);
        }
        else
        {
            class_2[0].push_back(dataX[i]);
            class_2[1].push_back(dataY[i]);
        }
    }

    if (class_1[0].empty() || class_2[0].empty())
    {
        QMessageBox::warning(this, tr("Error"), tr("All points belong to the same class"));
        return ;
    }

    minX = *std::min_element(dataX.begin(), dataX.end()) - 1;
    maxX = *std::max_element(dataX.begin(), dataX.end()) + 1;

    minY = *std::min_element(dataY.begin(), dataY.end()) - 1;
    maxY = *std::max_element(dataY.begin(), dataY.end()) + 1;

    per->train_2(class_1, class_2, 1e6);
    plot();
    show();
}

void Ploting::plot(void)
{
    ui->widget->clearGraphs();

    double h = 0.01;

    QCPScatterStyle myScatter;
    myScatter.setSize(6);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::red));
    myScatter.setBrush(Qt::red);
    myScatter.setShape(QCPScatterStyle::ssTriangle);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(myScatter);
    ui->widget->graph(0)->setName(QString("Class 1"));
    ui->widget->graph(0)->setData(class_1[0], class_1[1]);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::green));
    myScatter.setBrush(Qt::green);
    myScatter.setShape(QCPScatterStyle::ssDiamond);
    ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(1)->setScatterStyle(myScatter);
    ui->widget->graph(1)->setName(QString("Class 2"));
    ui->widget->graph(1)->setData(class_2[0], class_2[1]);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::blue);
    myScatter.setShape(QCPScatterStyle::ssStar);
    ui->widget->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(2)->setScatterStyle(myScatter);
    ui->widget->graph(2)->setName(QString("User point"));
    ui->widget->graph(2)->setData(class_user[0], class_user[1]);

    h = 1.;
    int i = 0;
    int min = std::min(minX, minY);
    int max = std::max(maxX, maxY);

    QVector<double> x((max - min) / h + 1);
    QVector<double> y((max - min) / h + 1);

    for (double tmp_x = min; tmp_x <= max; tmp_x += h)
    {
        x[i] = tmp_x;
        y[i] = k * tmp_x + b;
        i++;
    }

    ui->widget->addGraph();
    ui->widget->graph(3)->setName(QString("Line"));
    ui->widget->graph(3)->setData(x, y);

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    ui->widget->xAxis->setRange(minX, maxX);
    ui->widget->yAxis->setRange(minY, maxY);

    ui->widget->replot();
}

void Ploting::plot_plane(void)
{
    ui->widget->clearGraphs();

    QVector<QVector<double>> c_1(2);
    QVector<QVector<double>> c_2(2);

    QCPScatterStyle myScatter;
    myScatter.setSize(6);

    double h = QInputDialog::getDouble(this, tr("Steps"), tr("Input step:"), 1.0);

    for (double tx = minX; tx < maxX; tx += h)
    {
        for (double ty = minY; ty < maxY; ty += h)
        {
           if (per->query(tx, ty) < 0.5)
           {
               c_1[0].append(tx);
               c_1[1].append(ty);
           }
           else
           {
               c_2[0].append(tx);
               c_2[1].append(ty);
           }
        }
    }

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::red));
    myScatter.setBrush(Qt::red);
    myScatter.setShape(QCPScatterStyle::ssTriangle);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(myScatter);
    ui->widget->graph(0)->setName(QString("Class 1"));
    ui->widget->graph(0)->setData(c_1[0], c_1[1]);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::green));
    myScatter.setBrush(Qt::green);
    myScatter.setShape(QCPScatterStyle::ssDiamond);
    ui->widget->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(1)->setScatterStyle(myScatter);
    ui->widget->graph(1)->setName(QString("Class 2"));
    ui->widget->graph(1)->setData(c_2[0], c_2[1]);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::blue);
    myScatter.setShape(QCPScatterStyle::ssStar);
    ui->widget->graph(2)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(2)->setScatterStyle(myScatter);
    ui->widget->graph(2)->setName(QString("User point"));
    ui->widget->graph(2)->setData(class_user[0], class_user[1]);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::darkCyan));
    myScatter.setBrush(Qt::darkCyan);
    myScatter.setShape(QCPScatterStyle::ssDiamond);
    ui->widget->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(3)->setScatterStyle(myScatter);
    ui->widget->graph(3)->setName(QString("Class 1"));
    ui->widget->graph(3)->setData(class_1[0], class_1[1]);

    ui->widget->addGraph();
    myScatter.setPen(QPen(Qt::darkGray));
    myScatter.setBrush(Qt::darkGray);
    myScatter.setShape(QCPScatterStyle::ssCircle);
    ui->widget->graph(4)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(4)->setScatterStyle(myScatter);
    ui->widget->graph(4)->setName(QString("Class 2"));
    ui->widget->graph(4)->setData(class_2[0], class_2[1]);

    int i = 0;
    int min = std::min(minX, minY);
    int max = std::max(maxX, maxY);

    QVector<double> x((max - min) / h + 1);
    QVector<double> y((max - min) / h + 1);

    for (double tmp_x = min; tmp_x <= max; tmp_x += h)
    {
        x[i] = tmp_x;
        y[i] = k * tmp_x + b;
        i++;
    }

    ui->widget->addGraph();
    ui->widget->graph(5)->setName(QString("Line"));
    ui->widget->graph(5)->setData(x, y);

    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");

    ui->widget->xAxis->setRange(minX, maxX);
    ui->widget->yAxis->setRange(minY, maxY);

    ui->widget->replot();
}

void Ploting::on_buttonBox_accepted()
{
    per->close();
}

void Ploting::on_pushButton_clicked()
{
   per->show();
}

void Ploting::add_point(void)
{
    class_user[0].push_back(per->get_user_x());
    class_user[1].push_back(per->get_user_y());
    plot();
}

void Ploting::on_pushButton_2_clicked()
{
    plot_plane();
}

void Ploting::on_pushButton_3_clicked()
{
   plot();
}
