#include "perceptron.h"
#include "ui_perceptron.h"

Perceptron::Perceptron(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Perceptron)
{
    ui->setupUi(this);
    setWindowTitle("Point");

	init_w();
    lr = 0.8;
}

void Perceptron::init_w(void)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> X(-1, 1);

	w.push_back(X(gen));
	w.push_back(X(gen));
    w.push_back(X(gen));
}

double Perceptron::backpropagation(double input_x, double input_y, int target)
{
    double nn_error;
    double nn_answer;

    nn_answer = query(input_x, input_y);
    nn_error = 1./2. * pow(target - nn_answer, 2);
    w[0] += lr * input_x * (target - nn_answer) * d_af_sigmoid(nn_answer);
    w[1] += lr * input_y * (target - nn_answer) * d_af_sigmoid(nn_answer);
    w[2] += lr * (target - nn_answer) * d_af_sigmoid(nn_answer);
    return (nn_error);
	
}

void Perceptron::train_1(const QVector<QVector<double>> &class_1,
                       const QVector<QVector<double>> &class_2, double eps)
{
	int r;
	int r_class;
    double Q;
    double Qi;

    Q = 0.;
    Qi = __FLT_MAX_EXP__;
    while (abs(Q - Qi) > eps)
	{
        r_class = random();
        if (r_class & 1)
        {
            r = random() % class_1[0].size();
            Qi = backpropagation(class_1[0][r], class_1[1][r], 0);
        }
        else
        {
            r = random() % class_2[0].size();
            Qi = backpropagation(class_2[0][r], class_2[1][r], 1);
        }
        Q = (1. - 1e-3) * Q + 1e-3 * Qi;
	}
}

void Perceptron::train_2(const QVector<QVector<double>> &class_1,
                       const QVector<QVector<double>> &class_2, int epoches)
{
    int r;
    int r_class;

    while (epoches--)
    {
        r_class = random();
        if (r_class & 1)
        {
            r = random() % class_1[0].size();
            backpropagation(class_1[0][r], class_1[1][r], 0);
        }
        else
        {
            r = random() % class_2[0].size();
            backpropagation(class_2[0][r], class_2[1][r], 1);
        }
    }
}

Perceptron::~Perceptron()
{
    delete ui;
}

void Perceptron::on_pushButton_clicked()
{
    user_x = ui->lineEdit->text().toDouble();
    user_y = ui->lineEdit_2->text().toDouble();
    double class_num = query(user_x, user_y);
    QMessageBox::information(this, tr("Classification point"), QString("Class %0").arg(class_num > 0.5 ? 2 : 1));
    emit user_add_data();
}

void Perceptron::on_pushButton_2_clicked()
{
   close();
}
