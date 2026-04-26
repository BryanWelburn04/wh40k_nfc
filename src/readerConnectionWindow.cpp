#include "ReaderConnectionWindow.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "scardReading.hpp"
#include "scardWriting.hpp"
#include "scardHandling.hpp"
#include <QMessageBox>
#include "mainWindow.hpp"

ReaderConnectionWindow::ReaderConnectionWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Connect to Reader");
    resize(300, 200);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    readerListWidget = new QListWidget(this); 

    QList<QString> readerList = getAvailableReaders();

    for (const QString &reader : readerList) {
        readerListWidget->addItem(reader);
    }

    QPushButton *nextButton = new QPushButton("Next", this);

    layout->addWidget(readerListWidget);
    layout->addWidget(nextButton);

    setCentralWidget(central);

    connect(nextButton, &QPushButton::clicked,
            this, &ReaderConnectionWindow::initializeReader);
}

QList<QString> ReaderConnectionWindow::getAvailableReaders() {
    QList<QString> readers;

    wchar_t* pReaderString = getReaderList();

    if (pReaderString) {
        QString readerList = QString::fromWCharArray(pReaderString);
        readers = readerList.split(QChar(u'\0'), Qt::SkipEmptyParts);
    }
    return readers;
}

void ReaderConnectionWindow::initializeReader() {
    
    QListWidgetItem *item = readerListWidget->currentItem();

    if (!item){
        QMessageBox::warning(this, "No Reader Selected", "Please select a reader from the list before proceeding.");
        return;
    }

    QString selectedReader = item->text();

    MainWindow *mainWindow = new MainWindow(selectedReader.toStdWString().c_str());
    mainWindow->show();
    this->close();

    return;
}
 