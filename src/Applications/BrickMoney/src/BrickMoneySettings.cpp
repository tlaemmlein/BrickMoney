#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.BrickMoneySettings")

#include "BrickMoneySettings.h"

#include <QStandardPaths>
#include <QSettings>

std::unique_ptr<BrickMoneySettings> BrickMoneySettings::smInstance = nullptr;


const QString BrickMoneySettings::BrickMoneyName("general/brickMoneyFilePath");
const QString BrickMoneySettings::MainWindowName("general/mainWindow");
const QString BrickMoneySettings::MainIsMaximizedName("general/mainIsMaximized");
const QString BrickMoneySettings::LanguageName("general/language");


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

    m_brickMoneyFilePath = m_Settings->value(BrickMoneyName, "").toString();
    m_brickMoneyIsDirty = false;
    m_mainWindow = m_Settings->value(MainWindowName, QRect(10,10,1600,800)).toRect();
    m_mainIsMaximized = m_Settings->value(MainIsMaximizedName, true).toBool();
    m_language = m_Settings->value(LanguageName, "german").toString();
}


BrickMoneySettings::~BrickMoneySettings()
{
}


QString BrickMoneySettings::brickMoneyFilePath() const
{
    return m_brickMoneyFilePath;
}

bool BrickMoneySettings::brickMoneyIsDirty() const
{
    return m_brickMoneyIsDirty;
}

QRect BrickMoneySettings::mainWindow() const
{
    return m_mainWindow;
}

bool BrickMoneySettings::mainIsMaximized() const
{
    return m_mainIsMaximized;
}

QString BrickMoneySettings::language() const
{
    return m_language;
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

void BrickMoneySettings::setMainWindow(QRect mainWindow)
{
    if (m_mainWindow == mainWindow)
        return;

    m_mainWindow = mainWindow;
    m_Settings->setValue(MainWindowName, m_mainWindow);
    emit mainWindowChanged(m_mainWindow);
}

void BrickMoneySettings::setMainIsMaximized(bool mainIsMaximized)
{
    if (m_mainIsMaximized == mainIsMaximized)
        return;

    m_mainIsMaximized = mainIsMaximized;
    m_Settings->setValue(MainIsMaximizedName, m_mainIsMaximized);
    emit mainIsMaximizedChanged(m_mainIsMaximized);
}

void BrickMoneySettings::setLanguage(QString language)
{
    if (m_language == language)
        return;

    m_language = language;
    m_Settings->setValue(LanguageName, m_language);
    emit languageChanged(m_language);
}
