#include "Logging.h"
SET_LOGGER("BrickMoney.BrickMoneySettings")

#include "BrickMoneySettings.h"

#include <QStandardPaths>
#include <QSettings>

std::unique_ptr<BrickMoneySettings> BrickMoneySettings::smInstance = nullptr;


const QString BrickMoneySettings::ZoomFactorName("general/zoomFactor");
const QString BrickMoneySettings::BrickMoneyName("general/brickMoneyFilePath");
const QString BrickMoneySettings::ViewSettingsName("general/viewSettings");
const QString BrickMoneySettings::MainWindowName("general/mainWindow");


BrickMoneySettings *BrickMoneySettings::Inst()
{
    if(!smInstance)
    {
        smInstance = std::unique_ptr<BrickMoneySettings>(new BrickMoneySettings());
    }
    return smInstance.get();
}

BrickMoneySettings::BrickMoneySettings()
{
    const QString settingsFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "\\BrickMoneySettings.ini";

    LOG_INFO("Manage Settings from " << settingsFilePath.toStdWString());

    m_Settings = new QSettings(settingsFilePath, QSettings::IniFormat, this);

    m_zoomFactor = m_Settings->value(ZoomFactorName, 150).toInt();
    m_brickMoneyFilePath = m_Settings->value(BrickMoneyName, "").toString();
    m_brickMoneyIsDirty = false;
    m_viewSettings = m_Settings->value(ViewSettingsName, 0).toInt();
    m_mainWindow = m_Settings->value(MainWindowName, QRect(10,10,1600,800)).toRect();
}


BrickMoneySettings::~BrickMoneySettings()
{
}


int BrickMoneySettings::zoomFactor() const
{
    return m_zoomFactor;
}

QString BrickMoneySettings::brickMoneyFilePath() const
{
    return m_brickMoneyFilePath;
}

bool BrickMoneySettings::brickMoneyIsDirty() const
{
    return m_brickMoneyIsDirty;
}

int BrickMoneySettings::viewSettings() const
{
    return m_viewSettings;
}

QRect BrickMoneySettings::mainWindow() const
{
    return m_mainWindow;
}

void BrickMoneySettings::setZoomFactor(int zoomFactor)
{
    if (m_zoomFactor == zoomFactor)
        return;

    m_zoomFactor = zoomFactor;
    m_Settings->setValue(ZoomFactorName, m_zoomFactor);
    emit zoomFactorChanged(m_zoomFactor);
}

void BrickMoneySettings::setBrickMoneyFilePath(QString brickMoneyFilePath)
{
    if (m_brickMoneyFilePath == brickMoneyFilePath)
        return;

    m_brickMoneyFilePath = brickMoneyFilePath;
    m_Settings->setValue(BrickMoneyName, m_brickMoneyFilePath);
    emit brickMoneyFilePathChanged(m_brickMoneyFilePath);
}

void BrickMoneySettings::setBrickMoneyIsDirty(bool brickMoneyIsDirty)
{
    if (m_brickMoneyIsDirty == brickMoneyIsDirty)
        return;

    m_brickMoneyIsDirty = brickMoneyIsDirty;
    emit brickMoneyIsDirtyChanged(m_brickMoneyIsDirty);
}

void BrickMoneySettings::setViewSettings(int viewSettings)
{
    if (m_viewSettings == viewSettings)
        return;

    m_viewSettings = viewSettings;
    m_Settings->setValue(ViewSettingsName, m_viewSettings);
    emit viewSettingsChanged(m_viewSettings);
}

void BrickMoneySettings::setMainWindow(QRect mainWindow)
{
    if (m_mainWindow == mainWindow)
        return;

    m_mainWindow = mainWindow;
    m_Settings->setValue(MainWindowName, m_mainWindow);
    emit mainWindowChanged(m_mainWindow);
}
