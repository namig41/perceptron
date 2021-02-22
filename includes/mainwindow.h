#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <random>
#include <QDebug>
#include <QColorDialog>
#include <QPen>
#include <QBrush>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <vector>
#include <QTableWidget>
#include <QResizeEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <dialog.h>
#include <ploting.h>
#include <perceptron.h>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void change_color_table(int index);
    void update_table(void);
    void draw_hist(bool flag_resize);
    void clear_memory(void);
    void calc_bins(void);
    void init_plot(void);
    void init_sample(void);
    void read_sample(void);
    void write_sample(void);
    void update_all(void);

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private slots:

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void pressed_table(int raw, int column);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionClose_triggered();

    void on_actionGeneration_triggered();

    void create_dist(void);

    void on_actionMake_points_triggered();

private:
    Ui::MainWindow *ui;


    enum {
        UNIFROM,
        NORM
    };

    QString path;

    int n;
    float max;
    float min;
    int bins;
    int dist_index;

    QVector<double> sample;
    std::vector<int> bins_array;

    QGraphicsScene *scene;
    std::vector<QGraphicsRectItem *> hist;

    QPen *pen;
    QBrush *brush;

    Dialog *dlg;
    Ploting *plt;
    Perceptron *pp;
};

#endif // MAINWINDOW_H
