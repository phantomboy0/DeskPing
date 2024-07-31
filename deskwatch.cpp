#include "deskwatch.h"
#include "ui_deskwatch.h"
#include <filesystem>
#include <QDateTime>
#include "INIReader.h"
#include <QTimer>



std::string configPathWatch;

using namespace std;

DeskWatch::DeskWatch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DeskWatch)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint |  Qt::SubWindow | Qt::WindowStaysOnBottomHint);



    // Load the configuration file
    configPathWatch = std::filesystem::current_path().string() ;
    configPathWatch +=R"(\config.ini)";
    INIReader reader(configPathWatch);

    if(!reader.GetBoolean("Watch","ShowWatch",true)) {
        return;
    }
 ui->setupUi(this);
    this->setGeometry(0,0,reader.GetInteger("Watch", "width", 400),reader.GetInteger("Watch", "width", 400));
    this->move(reader.GetInteger("Watch", "posX", 0),reader.GetInteger("Watch", "posY", 0));


    QCalendar calendar(QCalendar::System::Jalali);




    QFont font =  ui->txtTime->font();
    font.setPointSize(reader.GetInteger("Watch", "FontSize", 50));


    QTimer* timer = new QTimer();
    timer->setInterval(1000); //Time in milliseconds
    connect(timer, &QTimer::timeout, this, [=](){
        //Do your stuff in here, gets called every interval time
        QDateTime date = QDateTime::currentDateTime();
        QDate today = QDate::currentDate();
        std::string formattedMinTime = date.toString("mm").toStdString();

        int formattedHrTime = date.toString("HH").toInt();
        if (formattedHrTime > 12) {
            formattedHrTime = formattedHrTime - 12;
        }

        QString weekDay;

        switch (date.date().dayOfWeek()) {
        case 1:
            weekDay = "دوشنبه";
            break;
        case 2:
            weekDay = "سه شنبه";
            break;
        case 3:
            weekDay = "چهارشنبه";
            break;
        case 4:
            weekDay = "پنجشنبه";
            break;
        case 5:
            weekDay = "جمعه";
            break;
        case 6:
            weekDay = "شنبه";
            break;
        case 7:
            weekDay = "یکشنبه";
            break;
        default:
               weekDay = "نامشخص";
            break;
        }
        QCalendar::YearMonthDay ymd = calendar.partsFromDate(today);
        QString month;

        switch (ymd.month) {
        case 1:
            month = "فروردین";
            break;
        case 2:
            month = "اردیبهشت";
            break;
        case 3:
            month = "خرداد";
            break;
        case 4:
            month = "تیر";
            break;
        case 5:
            month = "مرداد";
            break;
        case 6:
            month = "شهریور";
            break;
        case 7:
            month = "مهر";
            break;
        case 8:
            month = "آبان";
            break;
        case 9:
            month = "آذر";
            break;
        case 10:
            month = "دی";
            break;
        case 11:
            month = "بهمن";
            break;
        case 12:
            month = "اسنفد";
            break;

        default:
             month = "نامشخص";
            break;
        }

        ui->txtTime->setText(QString::fromStdString(std::to_string(formattedHrTime) + ":" + formattedMinTime));
        ui->txtDate->setText((QString(weekDay + ", " + QString::number(ymd.day) +" " + month)));
    });
    timer->start(); //Call start() AFTER connect


}


DeskWatch::~DeskWatch()
{
    delete ui;
}
