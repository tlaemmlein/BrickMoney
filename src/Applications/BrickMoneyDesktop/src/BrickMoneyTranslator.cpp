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
	switchTranslator(m_Translator, ":/translations/bm_de.qm");
	switchTranslator(m_translatorQt, ":/translations/qtbase_de.qm");
}

void BrickMoneyTranslator::setEnglishTranslation()
{
	qApp->removeTranslator(&m_Translator);
	qApp->removeTranslator(&m_translatorQt);
}


void BrickMoneyTranslator::switchTranslator(QTranslator & translator, const QString & resourcePath)
{
	qApp->removeTranslator(&translator);
	if (!translator.load(resourcePath)) {
		LOG_ERROR("Could not load translation from " << resourcePath.toStdWString());
	}
	qApp->installTranslator(&translator);
}
