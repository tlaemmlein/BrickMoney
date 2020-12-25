#include "Packages/Logging/Logging.h"
SET_LOGGER("BrickMoney.BrickMoneyTranslator")

#include "BrickMoneyTranslator.h"

#include <QCoreApplication>


std::unique_ptr<BrickMoneyTranslator> BrickMoneyTranslator::smInstance = nullptr;


BrickMoneyTranslator *BrickMoneyTranslator::Inst()
{
    if(!smInstance)
    {
        smInstance = std::unique_ptr<BrickMoneyTranslator>(new BrickMoneyTranslator());
    }
    return smInstance.get();
}

BrickMoneyTranslator::BrickMoneyTranslator()
{
}


BrickMoneyTranslator::~BrickMoneyTranslator()
{
}

void BrickMoneyTranslator::setGermanTranslation()
{
	qApp->removeTranslator(&m_Translator);
	QString language = "de";
	QString resourecePath = QString(":/translations/bm_%1.qm").arg(language);
	if (!m_Translator.load(resourecePath)) {
		LOG_ERROR("Could not load translation from " << resourecePath.toStdWString());
	}
	qApp->installTranslator(&m_Translator);
}

void BrickMoneyTranslator::setEnglishTranslation()
{
	qApp->removeTranslator(&m_Translator);
}


