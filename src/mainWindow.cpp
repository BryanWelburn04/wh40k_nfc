#include "MainWindow.hpp"
#include <QPushButton>
#include "scardHandling.hpp"
#include "scardReading.hpp"
#include "scardWriting.hpp"
#include <QLabel>
#include <QThread>
#include "cardWaitThread.hpp"
#include "modelInfoWindow.hpp"
#include "troopInfoFunctions.hpp"

MainWindow::MainWindow(const wchar_t* selectedReaderName, QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("wh40k NFC");
    resize(800, 600);

    this->readerName = selectedReaderName;
    
    //QWidget *leftWidget = new QWidget(this);
    QLabel *nameLabel = new QLabel("Current Selected Reader: " + QString::fromWCharArray(selectedReaderName), this);
    nameLabel->setAlignment(Qt::AlignLeft);
    nameLabel->setGeometry(10, 10, 400, 30);

    QLabel *statusLabel = new QLabel("Awaiting Card... ", this);
    statusLabel->setAlignment(Qt::AlignLeft);
    statusLabel->setGeometry(10, 50, 400, 30);

    //create a new thread that waits for cards so ui can continue to update
    QThread *thread = new QThread;
    CardWaitThread *worker = new CardWaitThread(selectedReaderName);

    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &CardWaitThread::process);
    connect(worker, &CardWaitThread::cardDetected,
            this, [this](Troop troop, BYTE *cardData, SCARDHANDLE hCardHandle, DWORD uActiveProtocol) {

        qDebug("Card detected!");

        ModelInfoWindow *modelInfoWindow = new ModelInfoWindow(this->readerName, troop, cardData, hCardHandle, uActiveProtocol, this);
        modelInfoWindow->show();
    });
    //connect(worker, &CardWaitThread::cardDetected, thread, &QThread::quit);
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);

    thread->start();

    // QPushButton *button = new QPushButton("Connect to Reader", this);
    // setCentralWidget(button);
    // connect(button, &QPushButton::clicked, this, &MainWindow::initializeReader);
}

Troop MainWindow::getTroopInfoFromCard() {
    
    // THIS FUNCTION IS NOT CURRENTLY BEING USED
    // LOGIC IN CARDWAITTHERAD INSTEAD

    BYTE infoContainer[8];
    BYTE name[52];
    BYTE Link[148];
    BYTE ga[40];
    BYTE wa[40];

    SCARDCONTEXT smartCardContext;
    SCARD_READERSTATEW readerState0;
    SCARDHANDLE hCardHandle = 0;
    DWORD uActiveProtocol = 0;

    initializeReader(readerName.c_str(), smartCardContext, readerState0);

    readPages(4, 9, hCardHandle, uActiveProtocol, infoContainer);
    readPages(10, 22, hCardHandle, uActiveProtocol, name);
    readPages(23, 59, hCardHandle, uActiveProtocol, Link);
    readPages(60, 69, hCardHandle, uActiveProtocol, ga);
    readPages(70, 79, hCardHandle, uActiveProtocol, wa);
    Troop troop = initTroop(name, infoContainer, ga, wa, Link);


    printf("hCardHandle: %p\n", hCardHandle);
    printf("uActiveProtocol: %d\n", uActiveProtocol);

    this->hCardHandle = hCardHandle;
    this->uActiveProtocol = uActiveProtocol;

    return troop;
}
 