#include "deskping.h"
#include "qtimer.h"
#include "ui_deskping.h"
#include <codecvt>
#include <cstdio>
#include <array>
#include <filesystem>
#include <locale>
#include <stdexcept>
#include <string>
#include <windows.h>
#include "INIReader.h"
#include <QTimerEvent>


using namespace std;

std::string serverName[10];
std::string serverIP[10];
std::string serverPing[10];
std::string configPath;

std::string numberOfPings;



DeskPing::DeskPing(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DeskPing)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint |  Qt::SubWindow | Qt::WindowStaysOnBottomHint);



    // Load the configuration file
  configPath = std::filesystem::current_path().string() ;
    configPath +=R"(\config.ini)";
    INIReader reader(configPath);

    if(!reader.GetBoolean("Theme"," ",true))
        return;

    ui->setupUi(this);
    this->setGeometry(0,0,reader.GetInteger("Theme", "width", 400),reader.GetInteger("Theme", "height", 400));
    this->move(reader.GetInteger("Theme", "posX", 0),reader.GetInteger("Theme", "posY", 0));
    numberOfPings = reader.Get("Network", "NumberOfPingsAverage", "1");

//get all values from config.ini
    for (int i = 0; i < 10; ++i) {
        serverIP[i] =reader.Get("Network", "ServerIp" + std::to_string(i +1) , "");
        serverName[i] = reader.Get("Network","ServerName" + std::to_string(i+1) , "");

    }

    serverIP[0] =reader.Get("Network", "ServerIp1" , "");

    int RefreshInterval = reader.GetInteger("Network", "RefreshInterval", 10);

    QTimer* timer = new QTimer();
    timer->setInterval(RefreshInterval*1000); //Time in milliseconds
    connect(timer, &QTimer::timeout, this, [=](){
     InitPing();
    });
    timer->start(); //Call start() AFTER connect

  InitPing();

}


DeskPing::~DeskPing()
{
    delete ui;
}


void DeskPing::InitPing()
{

    std::string allNames, allIps, allPings;

  for (int i = 0; i < 10; ++i) {
      if (!serverIP[i].empty()) {
          std::string command = "ping -n " + numberOfPings + " ";  // Command to ping the IP address 4 times (for Win)
          command += serverIP[i];

          try {
              std::string result = exec(command.c_str());
              serverPing[i] = getPartAfterText(result,"Average = ");
              allNames += serverName[i] + "\n\n";
              allIps += serverIP[i]+ "\n\n";
              allPings += serverPing[i] + "\n" ;
          } catch (const std::exception& e) {
              ui->txtIp->setText(QString::fromStdString("ERROR\n"));
          }
      }

  }

  ui->txtPing->setText(QString::fromStdString(allPings));
  ui->txtName->setText(QString::fromStdString(allNames));
  ui->txtIp->setText(QString::fromStdString(allIps));



}

// Function to retrieve the part of the string after the specified text
std::string DeskPing::getPartAfterText(const std::string& str, const std::string& text) {
    size_t pos = str.find(text);
    if (pos != std::string::npos) {
        // Move the position to the end of the specified text
        pos += text.length();
        // Return the substring from the position to the end of the string
        return str.substr(pos);
    }
    // If the specified text is not found, return an error string
    return "N/A\n";
}



std::string DeskPing::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    HANDLE hRead, hWrite;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    // Create the pipe
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        throw std::runtime_error("CreatePipe() failed!");
    }

    // Set the information for the process
    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi;
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    // Convert the command to a wide string
    std::wstring cmdWStr = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(cmd);
    // Create the process

    if (!CreateProcessW(NULL, const_cast<LPWSTR>(cmdWStr.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hRead);
        CloseHandle(hWrite);
        throw std::runtime_error("CreateProcessW() failed!");
    }

    // Close the write end of the pipe before reading from the read end of the pipe
    CloseHandle(hWrite);

    // Read the output a line at a time - output it
    DWORD bytesRead;
    while (ReadFile(hRead, buffer.data(), buffer.size(), &bytesRead, NULL) != FALSE) {
        result.append(buffer.data(), bytesRead);
    }

    // Close handles
    CloseHandle(hRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return result;
}







