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


ModelInfoWindow::ModelInfoWindow(
    const std::wstring& selectedReaderName,
    const Troop& troop,
    BYTE *cardData,
    QWidget *parent
) : QMainWindow(parent),
      readerName(selectedReaderName),
      troop(troop),
      cardData(*cardData)
{  
    setWindowTitle("Model Info");
    resize(800, 600);
        
    QWidget *central = new QWidget(this);

    // MAIN split layout (left | right)
    QHBoxLayout *mainLayout = new QHBoxLayout(central);


    // ================= LEFT SIDE =================
    QVBoxLayout *leftLayout = new QVBoxLayout;

    //card UID
    QLabel *uidLabel = new QLabel("Card UID:", this);
    QLineEdit *uidTextBox = new QLineEdit(this);
    leftLayout->addWidget(uidLabel);
    leftLayout->addWidget(uidTextBox);

    // name
    QLabel *nameLabel = new QLabel("Model Name:", this);
    QLineEdit *nameTextBox = new QLineEdit(this);
    leftLayout->addWidget(nameLabel);
    leftLayout->addWidget(nameTextBox);

    // type
    QLabel *typeLabel = new QLabel("Model Type:", this);
    QLineEdit *typeTextBox = new QLineEdit(this);
    leftLayout->addWidget(typeLabel);
    leftLayout->addWidget(typeTextBox);

    // number of models
    QLabel *numberOfModelsLabel = new QLabel("Number of Models in Unit:", this);
    QLineEdit *numberOfModelsTextBox = new QLineEdit(this);
    leftLayout->addWidget(numberOfModelsLabel);
    leftLayout->addWidget(numberOfModelsTextBox);

    // health (put side-by-side)
    QLabel *healthLabel = new QLabel("Current/Max Health:", this);
    QHBoxLayout *healthRow = new QHBoxLayout;
    QLineEdit *currentHealthTextBox = new QLineEdit(this);
    QLineEdit *maxHealthTextBox = new QLineEdit(this);
    healthRow->addWidget(currentHealthTextBox);
    healthRow->addWidget(maxHealthTextBox);
    leftLayout->addWidget(healthLabel);
    leftLayout->addLayout(healthRow);

    // kills
    QLabel *killsLabel = new QLabel("Current/Total Kills:", this);
    QHBoxLayout *killsRow = new QHBoxLayout;
    QLineEdit *currentKillsTextBox = new QLineEdit(this);
    QLineEdit *totalKillsTextBox = new QLineEdit(this);
    killsRow->addWidget(currentKillsTextBox);
    killsRow->addWidget(totalKillsTextBox);
    leftLayout->addWidget(killsLabel);
    leftLayout->addLayout(killsRow);

    // deaths
    QLabel *deathsLabel = new QLabel("Current/Total Deaths:", this);
    QHBoxLayout *deathsRow = new QHBoxLayout;
    QLineEdit *currentDeathsTextBox = new QLineEdit(this);
    QLineEdit *totalDeathsTextBox = new QLineEdit(this);
    deathsRow->addWidget(currentDeathsTextBox);
    deathsRow->addWidget(totalDeathsTextBox);
    leftLayout->addWidget(deathsLabel);
    leftLayout->addLayout(deathsRow);

    // K/D ratio
    QLabel *kdLabel = new QLabel("Current/Total K/D Ratio:", this);
    QHBoxLayout *kdRow = new QHBoxLayout;
    QLineEdit *currentKdTextBox = new QLineEdit(this);
    QLineEdit *totalKdTextBox = new QLineEdit(this);
    kdRow->addWidget(currentKdTextBox);
    kdRow->addWidget(totalKdTextBox);
    leftLayout->addWidget(kdLabel);
    leftLayout->addLayout(kdRow);

    // primary points
    QLabel *primaryLabel = new QLabel("Current/Total Primary Points:", this);
    QHBoxLayout *primaryRow = new QHBoxLayout;
    QLineEdit *currentPrimaryTextBox = new QLineEdit(this);
    QLineEdit *totalPrimaryTextBox = new QLineEdit(this);
    primaryRow->addWidget(currentPrimaryTextBox);
    primaryRow->addWidget(totalPrimaryTextBox);
    leftLayout->addWidget(primaryLabel);
    leftLayout->addLayout(primaryRow);

    //secondary points
    QLabel *secondaryLabel = new QLabel("Current/Total Secondary Points:", this);
    QHBoxLayout *secondaryRow = new QHBoxLayout;
    QLineEdit *currentSecondaryTextBox = new QLineEdit(this);
    QLineEdit *totalSecondaryTextBox = new QLineEdit(this);
    secondaryRow->addWidget(currentSecondaryTextBox);
    secondaryRow->addWidget(totalSecondaryTextBox);
    leftLayout->addWidget(secondaryLabel);  
    leftLayout->addLayout(secondaryRow);


    QPushButton *cancelButton = new QPushButton("Cancel", this);

    leftLayout->addStretch();  // keep everything at top

    leftLayout->addWidget(cancelButton, Qt::AlignBottom | Qt::AlignLeft);



    // ================= RIGHT SIDE =================
    QVBoxLayout *rightLayout = new QVBoxLayout;

    // top-centered raw data
    QHBoxLayout *topCenter = new QHBoxLayout;

    QVBoxLayout *rawDataLayout = new QVBoxLayout;
    QLabel *rawDataLabel = new QLabel("Raw Data:", this);
    QTextEdit *rawDataTextBox = new QTextEdit(this);

    QPushButton *updateButton = new QPushButton("Update", this);


    // make it wider
    rawDataTextBox->setMinimumWidth(250);
    rawDataTextBox->setMinimumHeight(500);
    rawDataTextBox->setReadOnly(true);

    rawDataTextBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    rawDataLayout->addWidget(rawDataLabel, 0, Qt::AlignHCenter);
    rawDataLayout->addWidget(rawDataTextBox, 0, Qt::AlignHCenter);


    topCenter->addStretch();
    topCenter->addLayout(rawDataLayout);
    topCenter->addStretch();

    rightLayout->addLayout(topCenter);


    rightLayout->addStretch();  // keep it at top

    rightLayout->addWidget(updateButton, Qt::AlignBottom | Qt::AlignRight);


    // ================= COMBINE =================
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    // make it 50/50 split
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(1, 1);

    setCentralWidget(central);

    
    
    // ================= FILL DATA =================

    int currentKills = 0;
    int currentDeaths = 0;
    int currentPrimary = 0;
    int currentSecondary = 0;
    float currentKd = 0.0f;
    float totalKd = (float)troop.totalKills / (troop.totalDeaths == 0 ? 1 : troop.totalDeaths);
    QString cardDataFormatted = getRawDataFromCard(cardData, 0, 134);

    nameTextBox->setText(QString::fromStdString(troop.troopName));

    currentKillsTextBox->setText(QString::number(currentKills));
    totalKillsTextBox->setText(QString::number(troop.totalKills));
    currentDeathsTextBox->setText(QString::number(currentDeaths));
    totalDeathsTextBox->setText(QString::number(troop.totalDeaths));
    currentPrimaryTextBox->setText(QString::number(currentPrimary));
    totalPrimaryTextBox->setText(QString::number(troop.primaryPoints));
    currentSecondaryTextBox->setText(QString::number(currentSecondary));
    totalSecondaryTextBox->setText(QString::number(troop.secondaryPoints));

    currentHealthTextBox->setText(QString::number(troop.curHealth));
    maxHealthTextBox->setText(QString::number(troop.maxHealth));
    typeTextBox->setText(QString::number(troop.modelType));
    numberOfModelsTextBox->setText(QString::number(troop.troopCount));

    totalKdTextBox->setText(QString::number(totalKd, 'f', 2));
    currentKdTextBox->setText(QString::number(currentKd, 'f', 2));

    rawDataTextBox->setText(cardDataFormatted);

    // nameTextBox->setPlaceholderText("Type here...");"

}