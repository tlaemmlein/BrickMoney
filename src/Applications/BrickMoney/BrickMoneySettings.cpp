#include "Logging.h"
SET_LOGGER("BrickMoney.BrickMoneySettings")

#include "BrickMoneySettings.h"

#include <QStandardPaths>
#include <QSettings>

std::unique_ptr<BrickMoneySettings> BrickMoneySettings::smInstance = nullptr;


const QString BrickMoneySettings::ZoomFactorName("general/zoomFactor");
const QString BrickMoneySettings::BrickMoneyName("general/brickMoneyFilePath");

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

