#include "ModelInfoWindow.hpp"
#include <QPushButton>
#include "scardHandling.hpp"
#include "scardReading.hpp"
#include "scardWriting.hpp"
#include "cardWaitThread.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QPolarChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

using namespace std;

ModelInfoWindow::ModelInfoWindow(
    const std::wstring& selectedReaderName,
    const Troop& troop,
    BYTE *cardData,
    SCARDHANDLE hCardHandle,
    DWORD uActiveProtocol,
    QWidget *parent
) : QMainWindow(parent),
      readerName(selectedReaderName),
      troop(troop),
      cardData(*cardData), //maybe wrong?
      hCardHandle(hCardHandle),
      uActiveProtocol(uActiveProtocol)
{  
    setWindowTitle("Model Info");
    resize(1200, 600);
        
    QWidget *central = new QWidget(this);

    // MAIN split layout (left | middleLeft | middleRight | right)
    QHBoxLayout *mainLayout = new QHBoxLayout(central);


    // ===============================================
    // ================== LEFT SIDE ==================
    // ===============================================

    QVBoxLayout *leftLayout = new QVBoxLayout;

    //card UID
    QLabel *uidLabel = new QLabel("Card UID:", this);
    QLineEdit *uidTextBox = new QLineEdit(this);
    leftLayout->addWidget(uidLabel);
    leftLayout->addWidget(uidTextBox);

    // name
    QLabel *nameLabel = new QLabel("Model Name:", this);
    nameTextBox = new QLineEdit(this);
    leftLayout->addWidget(nameLabel);
    leftLayout->addWidget(nameTextBox);

    // point cost
    QLabel *pointCostLabel = new QLabel("Point Cost:", this);
    pointCostTextBox = new QLineEdit(this);
    leftLayout->addWidget(pointCostLabel);
    leftLayout->addWidget(pointCostTextBox);

    // type
    QLabel *typeLabel = new QLabel("Model Type:", this);
    typeTextBox = new QLineEdit(this);
    leftLayout->addWidget(typeLabel);
    leftLayout->addWidget(typeTextBox);

    // number of models
    QLabel *numberOfModelsLabel = new QLabel("Number of Models in Unit:", this);
    numberOfModelsTextBox = new QLineEdit(this);
    leftLayout->addWidget(numberOfModelsLabel);
    leftLayout->addWidget(numberOfModelsTextBox);

    // health (put side-by-side)
    QLabel *healthLabel = new QLabel("Current/Max Health:", this);
    QHBoxLayout *healthRow = new QHBoxLayout;
    currentHealthTextBox = new QLineEdit(this);
    maxHealthTextBox = new QLineEdit(this);
    healthRow->addWidget(currentHealthTextBox);
    healthRow->addWidget(maxHealthTextBox);
    leftLayout->addWidget(healthLabel);
    leftLayout->addLayout(healthRow);

    // kills
    QLabel *killsLabel = new QLabel("Current/Total Kills:", this);
    QHBoxLayout *killsRow = new QHBoxLayout;
    QLineEdit *currentKillsTextBox = new QLineEdit(this);
    totalKillsTextBox = new QLineEdit(this);
    killsRow->addWidget(currentKillsTextBox);
    killsRow->addWidget(totalKillsTextBox);
    leftLayout->addWidget(killsLabel);
    leftLayout->addLayout(killsRow);

    // deaths
    QLabel *deathsLabel = new QLabel("Current/Total Deaths:", this);
    QHBoxLayout *deathsRow = new QHBoxLayout;
    QLineEdit *currentDeathsTextBox = new QLineEdit(this);
    totalDeathsTextBox = new QLineEdit(this);
    deathsRow->addWidget(currentDeathsTextBox);
    deathsRow->addWidget(totalDeathsTextBox);
    leftLayout->addWidget(deathsLabel);
    leftLayout->addLayout(deathsRow);

    // primary points
    QLabel *primaryLabel = new QLabel("Current/Total Primary Points:", this);
    QHBoxLayout *primaryRow = new QHBoxLayout;
    QLineEdit *currentPrimaryTextBox = new QLineEdit(this);
    totalPrimaryTextBox = new QLineEdit(this);
    primaryRow->addWidget(currentPrimaryTextBox);
    primaryRow->addWidget(totalPrimaryTextBox);
    leftLayout->addWidget(primaryLabel);
    leftLayout->addLayout(primaryRow);

    //secondary points
    QLabel *secondaryLabel = new QLabel("Current/Total Secondary Points:", this);
    QHBoxLayout *secondaryRow = new QHBoxLayout;
    QLineEdit *currentSecondaryTextBox = new QLineEdit(this);
    totalSecondaryTextBox = new QLineEdit(this);
    secondaryRow->addWidget(currentSecondaryTextBox);
    secondaryRow->addWidget(totalSecondaryTextBox);
    leftLayout->addWidget(secondaryLabel);  
    leftLayout->addLayout(secondaryRow);


    QPushButton *cancelButton = new QPushButton("Cancel", this);

    leftLayout->addStretch();  // keep everything at top

    leftLayout->addWidget(cancelButton, Qt::AlignBottom | Qt::AlignLeft);

    // ================================================
    // =================  MIDDLE LEFT =================
    // ================================================

    QVBoxLayout *middleLeftLayout = new QVBoxLayout;

    // K/D ratio
    QLabel *kdLabel = new QLabel("Current/Total K/D Ratio:", this);
    QHBoxLayout *kdRow = new QHBoxLayout;
    QLineEdit *currentKdTextBox = new QLineEdit(this);
    QLineEdit *totalKdTextBox = new QLineEdit(this);
    kdRow->addWidget(currentKdTextBox);
    kdRow->addWidget(totalKdTextBox);
    middleLeftLayout->addWidget(kdLabel);
    middleLeftLayout->addLayout(kdRow);

    // Points/health deficit/surplus
    QLabel *phdsLabel = new QLabel("PHDS:", this);
    phdsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(phdsLabel);
    middleLeftLayout->addWidget(phdsTextBox);

    // Epic Hero Kills
    QLabel *epicHeroKillsLabel = new QLabel("Epic Hero Kills:", this);
    epicHeroKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(epicHeroKillsLabel);
    middleLeftLayout->addWidget(epicHeroKillsTextBox);

    // Character Kills
    QLabel *characterKillsLabel = new QLabel("Character Kills:", this);
    characterKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(characterKillsLabel);
    middleLeftLayout->addWidget(characterKillsTextBox);

    // Vehicle Kills (fixed spelling)
    QLabel *vehicleKillsLabel = new QLabel("Vehicle Kills:", this);
    vehicleKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(vehicleKillsLabel);
    middleLeftLayout->addWidget(vehicleKillsTextBox);

    // Monster Kills
    QLabel *monsterKillsLabel = new QLabel("Monster Kills:", this);
    monsterKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(monsterKillsLabel);
    middleLeftLayout->addWidget(monsterKillsTextBox);

    // Battleline Kills
    QLabel *battleLineKillsLabel = new QLabel("Battleline Kills:", this);
    battleLineKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(battleLineKillsLabel);
    middleLeftLayout->addWidget(battleLineKillsTextBox);

    // Mounted Kills
    QLabel *mountedKillsLabel = new QLabel("Mounted Kills:", this);
    mountedKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(mountedKillsLabel);
    middleLeftLayout->addWidget(mountedKillsTextBox);

    // Transport Kills
    QLabel *transportKillsLabel = new QLabel("Transport Kills:", this);
    transportKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(transportKillsLabel);
    middleLeftLayout->addWidget(transportKillsTextBox);

    // Other Kills
    QLabel *otherKillsLabel = new QLabel("Other Kills:", this);
    otherKillsTextBox = new QLineEdit(this);
    middleLeftLayout->addWidget(otherKillsLabel);
    middleLeftLayout->addWidget(otherKillsTextBox);

    middleLeftLayout->addStretch();  // keep everything at top


    // ================================================
    // ================= MIDDLE RIGHT =================
    // ================================================

    QVBoxLayout *middleRightLayout = new QVBoxLayout;

    chartView = new QChartView(webGraphKills());
    chartView->setMinimumSize(200, 200);

    middleRightLayout->addWidget(chartView);

    

    // ==============================================
    // ================= RIGHT SIDE =================
    // ==============================================

     
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QHBoxLayout *topCenter = new QHBoxLayout;
    QVBoxLayout *rawDataLayout = new QVBoxLayout;

    QLabel *gaLabel = new QLabel("Greatest Achievement:", this);
    gaTextBox = new QLineEdit(this);
    rightLayout->addWidget(gaLabel);
    rightLayout->addWidget(gaTextBox);

    QLabel *waLabel = new QLabel("Worst Achievement:", this);
    waTextBox = new QLineEdit(this);
    rightLayout->addWidget(waLabel);
    rightLayout->addWidget(waTextBox);

    QLabel *linkLabel = new QLabel("DataSheet Link:", this);
    linkTextBox = new QLineEdit(this);
    rightLayout->addWidget(linkLabel);
    rightLayout->addWidget(linkTextBox);

    QPushButton *dataSheetButton = new QPushButton("DataSheet", this);
    connect(dataSheetButton, &QPushButton::clicked, this, &ModelInfoWindow::openLink); //change function
    rightLayout->addWidget(dataSheetButton, Qt::AlignBottom | Qt::AlignRight);

    QPushButton *updateHistoryButton = new QPushButton("Update History", this);
    connect(updateHistoryButton, &QPushButton::clicked, this, &ModelInfoWindow::updateInfo); //change function
    rightLayout->addWidget(updateHistoryButton, Qt::AlignBottom | Qt::AlignRight);

    QLabel *rawDataLabel = new QLabel("Raw Data:", this);
    QTextEdit *rawDataTextBox = new QTextEdit(this);
    rawDataTextBox->setMinimumWidth(75);
    rawDataTextBox->setMinimumHeight(150);
    rawDataTextBox->setReadOnly(true);

    rawDataTextBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    rawDataLayout->addWidget(rawDataLabel, 0, Qt::AlignHCenter);
    rawDataLayout->addWidget(rawDataTextBox);

    topCenter->addStretch();
    topCenter->addLayout(rawDataLayout);
    topCenter->addStretch();

    rightLayout->addLayout(topCenter);
    rightLayout->addStretch();  // keep it at top

    QPushButton *updateButton = new QPushButton("Update", this);
    connect(updateButton, &QPushButton::clicked, this, &ModelInfoWindow::updateInfo);
    rightLayout->addWidget(updateButton, Qt::AlignBottom | Qt::AlignRight);



    // ===============================================
    // =================== COMBINE ===================
    // ===============================================

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(middleLeftLayout);
    mainLayout->addLayout(middleRightLayout);
    mainLayout->addLayout(rightLayout);

    // make it 50/50 split
    mainLayout->setStretch(0, 1/4);
    mainLayout->setStretch(1/4, 2/4);
    mainLayout->setStretch(2/4, 3/4);
    mainLayout->setStretch(3/4, 1);


    setCentralWidget(central);

    
    
    // ================= FILL DATA =================

    //Double byte fields
    int totalKills = (static_cast<unsigned int>(troop.totalKills1 << 8)) | static_cast<unsigned int>(troop.totalKills2);
    int totalDeaths = (static_cast<unsigned int>(troop.totalDeaths1 << 8)) | static_cast<unsigned int>(troop.totalDeaths2);
    int totalPrimaryPoints = (static_cast<unsigned int>(troop.primaryPoints1 << 8)) | static_cast<unsigned int>(troop.primaryPoints2);
    int totalSecondaryPoints = (static_cast<unsigned int>(troop.secondaryPoints1 << 8)) | static_cast<unsigned int>(troop.secondaryPoints2);
    int pointCost = (static_cast<unsigned int>(troop.pointCost1 << 8)) | static_cast<unsigned int>(troop.pointCost2);
    int16_t PHDS = (int16_t)((uint16_t(troop.PHDS1) << 8) | uint16_t(troop.PHDS2));

    //Current fields
    int currentKills = 0;
    int currentDeaths = 0;
    int currentPrimary = 0;
    int currentSecondary = 0;
    float currentKd = 0.0f;
    float totalKd = (float)totalKills / (totalDeaths == 0 ? 1 : totalDeaths);
    QString cardDataFormatted = getRawDataFromCard(cardData, 0, 134);
    string uid = "125747885";

    //
    // SCARDCONTEXT smartCardContext;
    // SCARD_READERSTATEW readerState0;
    // initializeReader(readerName.c_str(), smartCardContext, readerState0);
    // getCardUID(smartCardContext, this->readerName, this->hCardHandle, this->uActiveProtocol);

    uidTextBox->setText(QString::fromStdString(uid));
    nameTextBox->setText(QString::fromStdString(troop.troopName));

    currentKillsTextBox->setText(QString::number(currentKills));
    totalKillsTextBox->setText(QString::number(totalKills));
    currentDeathsTextBox->setText(QString::number(currentDeaths));
    totalDeathsTextBox->setText(QString::number(totalDeaths));

    currentPrimaryTextBox->setText(QString::number(currentPrimary));
    totalPrimaryTextBox->setText(QString::number(totalPrimaryPoints));
    currentSecondaryTextBox->setText(QString::number(currentSecondary));
    totalSecondaryTextBox->setText(QString::number(totalSecondaryPoints));

    currentHealthTextBox->setText(QString::number(troop.curHealth));
    maxHealthTextBox->setText(QString::number(troop.maxHealth));
    typeTextBox->setText(QString::number(troop.modelType));
    numberOfModelsTextBox->setText(QString::number(troop.troopCount));

    epicHeroKillsTextBox->setText(QString::number(troop.epicHeroKills));
    characterKillsTextBox->setText(QString::number(troop.characterKills));
    vehicleKillsTextBox->setText(QString::number(troop.vehicleKills));
    monsterKillsTextBox->setText(QString::number(troop.monsterKills));

    battleLineKillsTextBox->setText(QString::number(troop.battleLineKills));
    mountedKillsTextBox->setText(QString::number(troop. mountedKills));
    transportKillsTextBox->setText(QString::number(troop.transportKills));
    otherKillsTextBox->setText(QString::number(troop.otherKills));

    pointCostTextBox->setText(QString::number(pointCost));
    phdsTextBox->setText(QString::number(PHDS));
    totalKdTextBox->setText(QString::number(totalKd, 'f', 2));
    currentKdTextBox->setText(QString::number(currentKd, 'f', 2));

    gaTextBox->setText(QString::fromStdString(troop.greatestAchievement));
    waTextBox->setText(QString::fromStdString(troop.worstAchievement));
    linkTextBox->setText(QString::fromStdString(troop.link));

    rawDataTextBox->setText(cardDataFormatted);

    chartView->setChart(webGraphKills());

    // nameTextBox->setPlaceholderText("Type here...");"

}

bool ModelInfoWindow::updateInfo(){

    size_t capacityName = 52;
    size_t capacityLink = 148;
    size_t capacityGA = 40;
    size_t capacityWA = 40;
    // size_t capacityHistory = 4;

    BYTE name[capacityName];
    BYTE link[capacityLink];
    BYTE ga[capacityGA];
    BYTE wa[capacityWA];
    
    QByteArray bytesName = nameTextBox->text().toUtf8();
    memset(name, 0, capacityName); // clear buffer (important)
    int lenName = qMin(bytesName.size(), static_cast<int>(capacityName-1)); // leave space for null terminator
    memcpy(name, bytesName.data(), lenName);

    QByteArray bytesLink = linkTextBox->text().toUtf8();
    memset(link, 0, capacityLink); // clear buffer (important)
    int lenLink = qMin(bytesLink.size(), static_cast<int>(capacityLink-1)); // leave space for null terminator
    memcpy(link, bytesLink.data(), lenLink);

    QByteArray bytesGA = gaTextBox->text().toUtf8();
    memset(ga, 0, capacityGA); // clear buffer (important)
    int lenGA = qMin(bytesGA.size(), static_cast<int>(capacityGA-1)); // leave space for null terminator
    memcpy(ga, bytesGA.data(), lenGA);

    QByteArray bytesWA = waTextBox->text().toUtf8();
    memset(wa, 0, capacityWA); // clear buffer (important)
    int lenWA = qMin(bytesWA.size(), static_cast<int>(capacityWA-1)); // leave space for null terminator
    memcpy(wa, bytesWA.data(), lenWA);


    size_t capacity = 24;
    BYTE dataPacket[capacity] = {
        (BYTE)troop.totalKills1,
        (BYTE)troop.totalKills2,
        (BYTE)troop.totalDeaths1,
        (BYTE)troop.totalDeaths1,
        (BYTE)troop.primaryPoints1, 
        (BYTE)troop.primaryPoints2,
        (BYTE)troop.secondaryPoints1,
        (BYTE)troop.secondaryPoints2,
        (BYTE)troop.epicHeroKills,
        (BYTE)troop.characterKills,
        (BYTE)troop.vehicleKills,
        (BYTE)troop.monsterKills,
        (BYTE)troop.battleLineKills,
        (BYTE)troop.mountedKills,
        (BYTE)troop.transportKills,
        (BYTE)troop.otherKills,
        (BYTE)troop.pointCost1,
        (BYTE)troop.pointCost2,
        (BYTE)troop.PHDS1,
        (BYTE)troop.PHDS2,
    };

    // unsigned 16-bit
    writeUInt16BEUnsigned(dataPacket, 0,  totalKillsTextBox->text().toUInt());
    writeUInt16BEUnsigned(dataPacket, 2,  totalDeathsTextBox->text().toUInt());
    writeUInt16BEUnsigned(dataPacket, 4,  totalPrimaryTextBox->text().toUInt());
    writeUInt16BEUnsigned(dataPacket, 6,  totalSecondaryTextBox->text().toUInt());

    // single-byte values
    dataPacket[8]  = maxHealthTextBox->text().toUInt();
    dataPacket[9]  = currentHealthTextBox->text().toUInt();
    dataPacket[10] = typeTextBox->text().toUInt();
    dataPacket[11] = numberOfModelsTextBox->text().toUInt();

    dataPacket[12] = epicHeroKillsTextBox->text().toUInt();
    dataPacket[13] = characterKillsTextBox->text().toUInt();
    dataPacket[14] = vehicleKillsTextBox->text().toUInt();
    dataPacket[15] = monsterKillsTextBox->text().toUInt();

    dataPacket[16] = battleLineKillsTextBox->text().toUInt();
    dataPacket[17] = mountedKillsTextBox->text().toUInt();
    dataPacket[18] = transportKillsTextBox->text().toUInt();
    dataPacket[19] = otherKillsTextBox->text().toUInt();

    // unsigned 16-bit
    writeUInt16BEUnsigned(dataPacket, 20, pointCostTextBox->text().toUInt());

    // signed 16-bit
    writeInt16BESigned(dataPacket, 22, phdsTextBox->text().toInt());

    updateStructInfo(&troop, dataPacket);
    updateTroopName(&troop, nameTextBox->text().toStdString());
    updateTroopGA(&troop, gaTextBox->text().toStdString());
    updateTroopWA(&troop, waTextBox->text().toStdString());
    updateTroopLink(&troop, linkTextBox->text().toStdString());

    SCARDCONTEXT smartCardContext;
    SCARD_READERSTATEW readerState0;
    initializeReader(readerName.c_str(), smartCardContext, readerState0);

    writeStatsToCard(dataPacket, sizeof(dataPacket), hCardHandle, uActiveProtocol);
    writeNameToCard(name, sizeof(name), hCardHandle, uActiveProtocol);
    writeLinkToCard(link, sizeof(link), hCardHandle, uActiveProtocol);
    writeGreatestAchievementToCard(ga, sizeof(ga), hCardHandle, uActiveProtocol);
    writeWorstAchievementToCard(wa, sizeof(wa), hCardHandle, uActiveProtocol);

    chartView->setChart(webGraphKills());

    return true;
}

void ModelInfoWindow::writeUInt16BEUnsigned(BYTE* data, int index, uint16_t value) {
    data[index]     = (value >> 8) & 0xFF; 
    data[index + 1] = value & 0xFF; 
}

void ModelInfoWindow::writeInt16BESigned(BYTE* data, int index, int16_t value) {
    data[index]     = (value >> 8) & 0xFF;
    data[index + 1] = value & 0xFF;
}

QPolarChart* ModelInfoWindow::webGraphKills(){

    auto *series = new QLineSeries();

    QList<int> values = {
        epicHeroKillsTextBox->text().toInt(),
        characterKillsTextBox->text().toInt(),
        vehicleKillsTextBox->text().toInt(),
        monsterKillsTextBox->text().toInt(),
        battleLineKillsTextBox->text().toInt(),
        mountedKillsTextBox->text().toInt(),
        transportKillsTextBox->text().toInt(),
        otherKillsTextBox->text().toInt()
    };

    int count = values.size();
    qreal step = 360.0 / count;

    // Add points
    qreal offset = step / 2.0;

    for (int i = 0; i < count; ++i) {
        series->append(step * i, values[i]);
    }

    // close shape
    series->append(360, values[0]);

    QPolarChart *chart = new QPolarChart();
    chart->addSeries(series);
    chart->setTitle("Kill Distribution");

    // Angular axis
    QCategoryAxis *angularAxis = new QCategoryAxis();
    angularAxis->setRange(0, 360);

    QStringList labels = {
        "Epic Hero", "Character", "Vehicle", "Monster",
        "Battleline", "Mounted", "Transport", "Other"
    };

    for (int i = 0; i < labels.size(); ++i) {
        angularAxis->append(labels[i], (360.0 / count) * (i) + offset);
    }

    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);
    series->attachAxis(angularAxis);

    // Radial axis
    QValueAxis *radialAxis = new QValueAxis();
    int maxVal = *std::max_element(values.begin(), values.end());
    radialAxis->setRange(0, maxVal + 5);
    radialAxis->setTickCount(9);

    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);
    series->attachAxis(radialAxis);

    // auto *areaSeries = new QAreaSeries(series);
    // areaSeries->setBrush(QColor(0, 120, 255, 100)); // fill
    // areaSeries->setPen(QPen(QColor(0, 120, 255), 2)); // outline
    // chart->addSeries(areaSeries);

    return chart;
}

void ModelInfoWindow::openLink(){
    QDesktopServices::openUrl(QUrl(QString::fromStdString(troop.link)));
}
