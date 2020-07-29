#ifndef BRICKMONEYSETTINGS_H
#define BRICKMONEYSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QRect>

class BrickMoneySettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString brickMoneyFilePath READ brickMoneyFilePath WRITE setBrickMoneyFilePath NOTIFY brickMoneyFilePathChanged)
    Q_PROPERTY(bool brickMoneyIsDirty READ brickMoneyIsDirty WRITE setBrickMoneyIsDirty NOTIFY brickMoneyIsDirtyChanged)
    Q_PROPERTY(QRect mainWindow READ mainWindow WRITE setMainWindow NOTIFY mainWindowChanged)
    Q_PROPERTY(bool mainIsMaximized READ mainIsMaximized WRITE setMainIsMaximized NOTIFY mainIsMaximizedChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
public:
    static BrickMoneySettings* Inst();
    ~BrickMoneySettings();
    BrickMoneySettings(const BrickMoneySettings&) = delete;            // No copy ctor
    BrickMoneySettings& operator=(const BrickMoneySettings&) = delete; //No assign op

    QString brickMoneyFilePath() const;
    bool brickMoneyIsDirty() const;
    QRect mainWindow() const;
    bool mainIsMaximized() const;
    QString language() const;

public slots:
    void setBrickMoneyFilePath(QString brickMoneyFilePath);
    void setBrickMoneyIsDirty(bool brickMoneyIsDirty);
    void setMainWindow(QRect mainWindow);
    void setMainIsMaximized(bool mainIsMaximized);
    void setLanguage(QString language);

signals:
    void brickMoneyFilePathChanged(QString brickMoneyFilePath);
    void brickMoneyIsDirtyChanged(bool brickMoneyIsDirty);
    void mainWindowChanged(QRect mainWindow);
    void mainIsMaximizedChanged(bool mainIsMaximized);

    void languageChanged(QString language);

private:
    static std::unique_ptr<BrickMoneySettings> smInstance;
    BrickMoneySettings();

    QSettings* m_Settings;
    static const QString BrickMoneyName;
    QString m_brickMoneyFilePath;
    bool m_brickMoneyIsDirty;
    static const QString MainWindowName;
    QRect m_mainWindow;
    static const QString MainIsMaximizedName;
    bool m_mainIsMaximized;
    static const QString LanguageName;
    QString m_language;
};

#endif // BRICKMONEYSETTINGS_H
