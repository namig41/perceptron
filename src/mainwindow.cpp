#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Histogram");

    pen = new QPen();
    brush = new QBrush(Qt::red);
    scene = new QGraphicsScene(this);
    dlg = new Dialog(this);
    plt = new Ploting(this);
    pp = new Perceptron(this);

    ui->graphicsView->setScene(scene);
    ui->tableWidget->verticalHeader()->setVisible(false);

    connect(ui->tableWidget, &QTableWidget::cellPressed, this, &MainWindow::pressed_table);
    connect(dlg, &Dialog::create_dist, this, &MainWindow::create_dist);
}

MainWindow::~MainWindow()
{
    qDeleteAll(scene->items());
    delete pp;
    delete plt;
    delete scene;
    delete dlg;
    delete pen;
    delete brush;
    delete ui;
}

void MainWindow::draw_hist(bool flag_resize=false)
{
    double h_hist;
    int k;
    int h;
    int w;

    w = ui->graphicsView->width();
    h = ui->graphicsView->height();
    max = *max_element(bins_array.begin(), bins_array.end());
    k = w / bins;
    for (int i = 0; i < bins; i++)
    {
        h_hist = (double)bins_array[i] / max;
        h_hist *= h;
        if (flag_resize)
            hist[i]->setRect(QRect(k * i, 0, k, -h_hist));
        else
            hist.push_back(scene->addRect(k * i, 0, k, -h_hist, *pen, *brush));
    }
}

void MainWindow::init_sample(void)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    sample.clear();
    switch (dist_index) {
    case UNIFROM:
    {
        std::uniform_real_distribution<> X(min, max);
        for (int i = 0; i < n; i++) sample.push_back(X(gen));
        break;
    }
    case NORM:
    {
        float mean = (max - min) / 2.f;
        float sstd = mean / 3.f;

        std::normal_distribution<> X(mean, sstd);
        for (int i = 0; i < n; i++) sample.push_back(X(gen));
        break;
    }
    default:
        break;
    }
}

void MainWindow::calc_bins(void)
{
    double l;
    int count;

    bins = 1 + static_cast<int>(log2(sample.size()));
    min = *std::min_element(sample.begin(), sample.end());
    max = *std::max_element(sample.begin(), sample.end());
    l = (max - min) / bins;
    for (int i = 0; i < bins; i++)
    {
        count = 0;
        for (int j = 0; j < sample.size(); j++)
        {
           if (min + l * i <= sample[j] && sample[j] <= min + l * (i + 1))
               count++;
        }
        bins_array.push_back(count);
    }
}


void MainWindow::write_sample(void)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        for (int i = 0; i < sample.size(); i++)
            stream << sample[i] << "\n";
        QMessageBox::information(this, tr("File"), tr("Write successfully"));
    }
}

void MainWindow::init_plot(void)
{
    QVector<double> x;
    QVector<double> y;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList	data = line.split(' ');
            if (data.size() == 2)
            {
                x.append(data.at(0).toFloat());
                y.append(data.at(1).toFloat());
            }
        }
        plt->init_plot(x, y);
    }
}

void MainWindow::read_sample(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
    QFile file(fileName);

    sample.clear();
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        while (!stream.atEnd())
        {
            QString line = stream.readLine();
            sample.append(line.toFloat());
        }
        update_all();
        draw_hist();
    }
}

void MainWindow::on_pushButton_clicked()
{
    init_plot();
}

void MainWindow::on_pushButton_4_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (color.isValid())
       pen->setColor(color);

    if (!hist.empty())
    {
        for (auto &it : hist)
            it->setPen(*pen);
        scene->update();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose color");
    if (color.isValid())
       brush->setColor(color);

    if (!hist.empty())
    {
        for (auto &it : hist)
            it->setBrush(*brush);
        scene->update();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
   close();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key;

    key = event->key();
    if (key == Qt::Key_Escape)
        close();
    else if (key == Qt::Key_Q)
        dlg->show();
}

void MainWindow::update_table(void)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(bins);
    ui->tableWidget->setColumnCount(2);

    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("bins"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("N"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    for (int i = 0; i < bins; i++)
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(i + 1));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(bins_array[i]));

        item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        item1->setTextAlignment(Qt::AlignHCenter);
        item2->setTextAlignment(Qt::AlignHCenter);

        ui->tableWidget->setItem(i, 0, item1);
        ui->tableWidget->setItem(i, 1, item2);
    }
}

void MainWindow::clear_memory(void)
{
    bins_array.clear();
    qDeleteAll(scene->items());
}

void MainWindow::pressed_table(int row, int column)
{
    if (!hist.empty())
    {
        if (hist[row]->brush().color() == Qt::green)
            hist[row]->setBrush(*brush);
        else
            hist[row]->setBrush(QBrush(Qt::green));
        scene->update();
    }
    Q_UNUSED(column);
}

void MainWindow::on_pushButton_3_clicked()
{
    if (!bins_array.empty())
    {
        int min_index = std::min_element(bins_array.begin(), bins_array.end()) - bins_array.begin();
        change_color_table(min_index);
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    if (!bins_array.empty())
    {
        int max_index = std::max_element(bins_array.begin(), bins_array.end()) - bins_array.begin();
        change_color_table(max_index);
    }
}

void MainWindow::change_color_table(int index)
{
    QMessageBox::information(this, tr("Information"), QString("bins = %0\nelement = %1").arg(index).arg(bins_array[index]),
                             QMessageBox::Ok | QMessageBox::Cancel);
    ui->tableWidget->item(index, 0)->setBackground(Qt::green);
    ui->tableWidget->item(index, 1)->setBackground(Qt::green);
    hist[index]->setBrush(QBrush(Qt::green));
    scene->update();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if (!bins_array.empty())
    {
        draw_hist(true);
        scene->update();
    }
    Q_UNUSED(event);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    pen->setWidth(value);
    if (!hist.empty())
    {
        for (auto &it : hist)
            it->setPen(*pen);
        scene->update();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    read_sample();
}

void MainWindow::on_actionSave_triggered()
{
   write_sample();
}

void MainWindow::on_actionSave_as_triggered()
{
   path = QFileDialog::getOpenFileName(this, tr("Choose File"), ".");
   write_sample();
}

void MainWindow::on_actionClose_triggered()
{
   close();
}

void MainWindow::on_actionGeneration_triggered()
{
    dlg->show();
}

void MainWindow::on_actionMake_points_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose File"), ".");
    QFile file(filename);

    if (file.open(QIODevice::WriteOnly))
    {
        QVector<double> tmp(sample);
        std::random_shuffle(sample.begin(), sample.end());

        QTextStream stream(&file);
        for (int i = 0; i < sample.size(); i++)
            stream << sample[i] << " " << tmp[i] << "\n";
        QMessageBox::information(this, tr("File"), tr("Write successfully"));
    }
}

void MainWindow::create_dist(void)
{
    min = dlg->get_min();
    max = dlg->get_max();
    n = dlg->get_n();
    dist_index = dlg->get_dist_index();
    init_sample();

    update_all();
    draw_hist();
}

void MainWindow::update_all(void)
{
    clear_memory();
    calc_bins();
    scene->update();
    update_table();
}

