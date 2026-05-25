#include <cstdio>
#include <iostream>
#include <winscard.h>
#include "main.hpp"
#include "scardHandling.hpp"
#include "scardReading.hpp"
#include "scardWriting.hpp"
#include <string>
#include <vector>
#include <sstream>
#include "mainWindow.hpp"
#include "readerConnectionWindow.hpp"
#include <QApplication>
#include <QMainWindow>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ReaderConnectionWindow *readerWindow = new ReaderConnectionWindow();
    readerWindow->show();
    qApp->setQuitOnLastWindowClosed(false);
    return app.exec();
}