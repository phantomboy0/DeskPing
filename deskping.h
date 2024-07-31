#ifndef DESKPING_H
#define DESKPING_H

#include "qcoreevent.h"
#include <QMainWindow>
#include <string>
QT_BEGIN_NAMESPACE
namespace Ui {
class DeskPing;
}
QT_END_NAMESPACE

class DeskPing : public QMainWindow
{
    Q_OBJECT

public:
    DeskPing(QWidget *parent = nullptr);
    ~DeskPing();
   void InitPing();
    std::string exec(const char* cmd);
   std::string getPartAfterText(const std::string& str, const std::string& text);

    void hide();
private slots:







private:


    Ui::DeskPing *ui;

};
#endif // DESKPING_H
