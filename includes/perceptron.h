#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <QDialog>
#include <QVector>
#include <math.h>
#include <iostream>
#include <random>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Perceptron;
}

class Perceptron : public QDialog
{
    Q_OBJECT

public:
    explicit Perceptron(QWidget *parent = 0);
    ~Perceptron();

    void train_1(const QVector<QVector<double>> &class_1,
               const QVector<QVector<double>> &class_2,
               double eps);

    void train_2(const QVector<QVector<double>> &class_1,
               const QVector<QVector<double>> &class_2,
               int epoches);

    void init_w(void);

    double backpropagation(double input_x, double intput_y, int target);

    inline double query(double x, double y) const { return af_sigmoid(x * w[0] + y * w[1] + w[2]); }

    // activation function
    inline double af_sigmoid(double x) const { return 1. / (1. + exp(-x)); }
    inline double d_af_sigmoid(double x) const { return af_sigmoid(x) * (1. - af_sigmoid(x)); }

    inline double get_user_x(void) const { return user_x; }
    inline double get_user_y(void) const { return user_y; }

signals:

    void user_add_data(void);

private slots:

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Perceptron *ui;

    QVector<double> w;
    double lr;

    double user_x;
    double user_y;
};

#endif // PERCEPTRON_H
