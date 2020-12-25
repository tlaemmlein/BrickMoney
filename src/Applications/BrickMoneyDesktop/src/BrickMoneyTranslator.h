#ifndef BrickMoneyTranslator_H
#define BrickMoneyTranslator_H

#include <QTranslator>


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

	QTranslator m_Translator;
};

#endif // BrickMoneyTranslator_H
