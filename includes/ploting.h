#ifndef PLOTING_H
#define PLOTING_H

#include <QDialog>
#include <qcustomplot.h>
#include <perceptron.h>
#include <QVector>
#include <QMessageBox>

namespace Ui {
class Ploting;
}

class Ploting : public QDialog
{
    Q_OBJECT

public:
    explicit Ploting(QWidget *parent = 0);
    ~Ploting();

    void plot(void);
    void plot_plane(void);
    void init_plot(const QVector<double> &dataX, const QVector<double> &dataY);

public slots:
    void add_point(void);

private slots:

    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Ploting *ui;

	Perceptron *per;

    QVector<QVector<double>> class_user;

    double k;
    double b;

    double minX;
    double maxX;

    double minY;
    double maxY;

    QVector<QVector<double>> class_1;
    QVector<QVector<double>> class_2;
};

#endif // PLOTING_H
