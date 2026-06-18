#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include "troopInfoFunctions.hpp"
#include <QtCharts>
#include "NFCReader.hpp"


class ModelInfoWindow : public QMainWindow
{
    Q_OBJECT

public:
ModelInfoWindow(NFCReader* reader,
                const Troop& troop,
                BYTE *cardData,
                QWidget *parent = nullptr);
Troop troop;
BYTE cardData;
ModelInfoWindow* otherWindow = nullptr;
bool updateInfo();

QLineEdit *currentHealthTextBox;
 

private slots:
    void writeUInt16BEUnsigned(BYTE* data, int index, uint16_t value);
    void writeInt16BESigned(BYTE* data, int index, int16_t value);
    QPolarChart* webGraphKills();
    void openLink();
    void writeBackToHistory();
    void zeroHistory();
    QChart* lineGraphHistoryStats();

private:

    NFCReader* reader;

    unsigned char startPage = 0;
    unsigned char endPage = 134;

    QLineEdit *nameTextBox;
    QLineEdit *totalKillsTextBox;
    QLineEdit *totalDeathsTextBox;
    QLineEdit *totalPrimaryTextBox;
    QLineEdit *totalSecondaryTextBox;
    QLineEdit *maxHealthTextBox;
    QLineEdit *typeTextBox;
    QLineEdit *numberOfModelsTextBox;
    QLineEdit *pointCostTextBox;
    QLineEdit *epicHeroKillsTextBox;
    QLineEdit *characterKillsTextBox;
    QLineEdit *vehicleKillsTextBox;
    QLineEdit *monsterKillsTextBox;
    QLineEdit *battleLineKillsTextBox;
    QLineEdit *mountedKillsTextBox;
    QLineEdit *transportKillsTextBox;
    QLineEdit *otherKillsTextBox;
    QLineEdit *phdsTextBox;
    QLineEdit *linkTextBox;
    QLineEdit *gaTextBox;
    QLineEdit *waTextBox;

    QChartView *webChartView;
    QChartView *lineChartView;
};
