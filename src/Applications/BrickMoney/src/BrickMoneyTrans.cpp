#include "Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyProject")

#include "BrickMoneyTrans.h"
#include "BrickMoneySettings.h"

#include <QDebug>
#include <QGuiApplication>
#include <QDir>


BrickMoneyTrans::BrickMoneyTrans(QQmlEngine *engine)
{
    _translator = new QTranslator(this);
    _engine = engine;
}

void BrickMoneyTrans::selectLanguage(QString language)
{
    QDir dir = QDir(qApp->applicationDirPath()).absolutePath();

    if ( !_translator->load(QString("bm_%1").arg(language),dir.path()) )
    {
        //qDebug() << "Failed to load translation file, falling back to English";
        language = "english";
    }
    // it's a global thing, we can use it anywhere (after #including <QGuiApplication>)
    BrickMoneySettings::Inst()->setLanguage(language);
    qApp->installTranslator(_translator);
    _engine->retranslate();

    emit languageChanged();
}
