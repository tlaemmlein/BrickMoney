#ifndef TRANS_H
#define TRANS_H

#include <QObject>
#include <QTranslator>
#include <QQmlEngine>

class BrickMoneyTrans : public QObject
{
    Q_OBJECT
    // that's the "magic" string for the trick (not needed anymore since Qt 5.10)
    //Q_PROPERTY(QString emptyString READ getEmptyString NOTIFY languageChanged)

public:
    BrickMoneyTrans(QQmlEngine *engine);

    //QString getEmptyString();

    Q_INVOKABLE void selectLanguage(QString language);

signals:
    void languageChanged();

private:
    QTranslator *_translator;
    QQmlEngine *_engine;
};

#endif // TRANS_H
