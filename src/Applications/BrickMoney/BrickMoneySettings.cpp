#include "Logging.h"
SET_LOGGER("BrickMoney.BrickMoneySettings")

#include "BrickMoneySettings.h"

#include <QStandardPaths>
#include <QSettings>

const QString BrickMoneySettings::ZoomFactorName("general/zoomFactor");


BrickMoneySettings::BrickMoneySettings()
{
    const QString settingsFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "\\BrickMoneySettings.ini";

    LOG_INFO("Manage Settings from " << settingsFilePath.toStdWString());

    m_Settings = new QSettings(settingsFilePath, QSettings::IniFormat, this);

	m_zoomFactor = m_Settings->value(ZoomFactorName, 150).toInt();
}


BrickMoneySettings::~BrickMoneySettings()
{
    LOG_INFO("~BrickMoneySettings");
    //m_Settings->sync();
    LOG_INFO("after sync");
}


int BrickMoneySettings::zoomFactor() const
{
    return m_zoomFactor;
}

void BrickMoneySettings::setZoomFactor(int zoomFactor)
{
    if (m_zoomFactor == zoomFactor)
        return;

    m_zoomFactor = zoomFactor;
    m_Settings->setValue(ZoomFactorName, m_zoomFactor);
    emit zoomFactorChanged(m_zoomFactor);
}


