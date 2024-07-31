#ifndef DESKWATCH_H
#define DESKWATCH_H

#include "qcalendar.h"
#include <QMainWindow>

namespace Ui {
class DeskWatch;
}

class DeskWatch : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeskWatch(QWidget *parent = nullptr);
    ~DeskWatch();

private:
    Ui::DeskWatch *ui;
};

#endif // DESKWATCH_H
