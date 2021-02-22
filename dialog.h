#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    inline int get_min(void) const { return min; }
    inline int get_max(void) const { return max; }
    inline int get_n(void) const { return n; }
    inline int get_dist_index(void) const { return dist_index; }

signals:
    void create_dist(void);

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(int index);
private:
    Ui::Dialog *ui;

    int min;
    int max;
    int n;
    int dist_index;
};

#endif // DIALOG_H
