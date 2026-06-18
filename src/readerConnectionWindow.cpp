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
#include <QComboBox>
#include <QLabel>

ReaderConnectionWindow::ReaderConnectionWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Connect Readers");
    resize(400, 250);

    QWidget *central = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout(central);

    QLabel *readerALabel = new QLabel("Reader A:", this);
    QLabel *readerBLabel = new QLabel("Reader B:", this);

    readerABox = new QComboBox(this);
    readerBBox = new QComboBox(this);

    QList<QString> readerList = getAvailableReaders();

    for (const QString &reader : readerList) {
        readerABox->addItem(reader);
        readerBBox->addItem(reader);
    }

    QPushButton *nextButton = new QPushButton("Next", this);

    layout->addWidget(readerALabel);
    layout->addWidget(readerABox);

    layout->addWidget(readerBLabel);
    layout->addWidget(readerBBox);

    layout->addStretch();

    layout->addWidget(nextButton);

    setCentralWidget(central);

    connect(nextButton, &QPushButton::clicked, this, &ReaderConnectionWindow::initializeReaders);
}

QList<QString> ReaderConnectionWindow::getAvailableReaders() {

    QList<QString> readers;

    wchar_t* pReaderString = scardHandling::getReaderList();

    if (!pReaderString) {
        return readers;
    }

    const wchar_t* current = pReaderString;

    while (*current != L'\0') {

        QString reader = QString::fromWCharArray(current);

        readers.append(reader);

        current += wcslen(current) + 1;
    }

    return readers;
}

void ReaderConnectionWindow::initializeReaders()
{
    QString readerA =
        readerABox->currentText();

    QString readerB =
        readerBBox->currentText();

    if (readerA.isEmpty() || readerB.isEmpty()) {

        QMessageBox::warning(
            this,
            "Reader Selection",
            "Please select both readers."
        );

        return;
    }

    if (readerA == readerB) {

        QMessageBox::warning(
            this,
            "Reader Selection",
            "Please select two different readers."
        );

        return;
    }

    NFCReader *nfcReaderA = new NFCReader(readerABox->currentText().toStdWString());
    nfcReaderA->initializeReader();

    NFCReader *nfcReaderB = new NFCReader(readerBBox->currentText().toStdWString());
    nfcReaderB->initializeReader();

    MainWindow *mainWindow = new MainWindow(nfcReaderA, nfcReaderB, this);

    mainWindow->show();

    close();
}