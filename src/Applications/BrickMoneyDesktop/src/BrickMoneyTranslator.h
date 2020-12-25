#ifndef BrickMoneyTranslator_H
#define BrickMoneyTranslator_H

#include <QTranslator>


/// https://wiki.qt.io/How_to_create_a_multi_language_application
class BrickMoneyTranslator 
{
public:
    static BrickMoneyTranslator* Inst();
    ~BrickMoneyTranslator();
    BrickMoneyTranslator(const BrickMoneyTranslator&) = delete;            // No copy ctor
    BrickMoneyTranslator& operator=(const BrickMoneyTranslator&) = delete; //No assign op

	void setGermanTranslation();
	void setEnglishTranslation();


private:
    static std::unique_ptr<BrickMoneyTranslator> smInstance;
    BrickMoneyTranslator();

	void switchTranslator(QTranslator& translator, const QString& resourcePath);

	QTranslator m_Translator;
	QTranslator m_translatorQt; // contains the translations for qt
};

#endif // BrickMoneyTranslator_H
