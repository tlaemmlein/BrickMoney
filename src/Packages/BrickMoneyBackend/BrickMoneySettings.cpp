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
const QString BrickMoneySettings::InStockFlagsName("general/inStockFlags");
const QString BrickMoneySettings::ForSaleFlagsName("general/forSaleFlags");
const QString BrickMoneySettings::SoldFlagsName("general/soldFlags");


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
    m_mainWindow = m_Settings->value(MainWindowName, QRect(10,10,1600,800)).toRect();
    m_mainIsMaximized = m_Settings->value(MainIsMaximizedName, true).toBool();
    m_language = m_Settings->value(LanguageName, "german").toString();
    const auto defaultFlags = ~static_cast<uint>(0);
    m_inStockFlags = m_Settings->value(InStockFlagsName, defaultFlags).toUInt();
    m_forSaleFlags = m_Settings->value(ForSaleFlagsName, defaultFlags).toUInt();
    m_soldFlags = m_Settings->value(SoldFlagsName, defaultFlags).toUInt();
}


BrickMoneySettings::~BrickMoneySettings()
{
}


QString BrickMoneySettings::brickMoneyFilePath() const
{
    return m_brickMoneyFilePath;
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

uint BrickMoneySettings::inStockFlags() const
{
    return m_inStockFlags;
}

uint BrickMoneySettings::forSaleFlags() const
{
    return m_forSaleFlags;
}

uint BrickMoneySettings::soldFlags() const
{
    return m_soldFlags;
}

void BrickMoneySettings::setBrickMoneyFilePath(QString brickMoneyFilePath)
{
    if (m_brickMoneyFilePath == brickMoneyFilePath)
        return;

    m_brickMoneyFilePath = brickMoneyFilePath;
    m_Settings->setValue(BrickMoneyName, m_brickMoneyFilePath);
    emit brickMoneyFilePathChanged(m_brickMoneyFilePath);
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

void BrickMoneySettings::setInStockFlags(uint inStockFlags)
{
    if (m_inStockFlags == inStockFlags)
        return;

    m_inStockFlags = inStockFlags;
    m_Settings->setValue(InStockFlagsName, m_inStockFlags);
    emit inStockFlagsChanged(m_inStockFlags);
}

void BrickMoneySettings::setForSaleFlags(uint forSaleFlags)
{
    if (m_forSaleFlags == forSaleFlags)
        return;

    m_forSaleFlags = forSaleFlags;
    m_Settings->setValue(ForSaleFlagsName, m_forSaleFlags);
    emit forSaleFlagsChanged(m_forSaleFlags);
}

void BrickMoneySettings::setSoldFlags(uint soldFlags)
{
    if (m_soldFlags == soldFlags)
        return;

    m_soldFlags = soldFlags;
    m_Settings->setValue(SoldFlagsName, m_soldFlags);
    emit soldFlagsChanged(m_soldFlags);
}
