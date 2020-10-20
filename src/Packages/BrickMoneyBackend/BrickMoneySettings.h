#ifndef BRICKMONEYSETTINGS_H
#define BRICKMONEYSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QRect>

class BrickMoneySettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString brickMoneyFilePath READ brickMoneyFilePath WRITE setBrickMoneyFilePath NOTIFY brickMoneyFilePathChanged)
    Q_PROPERTY(QRect mainWindow READ mainWindow WRITE setMainWindow NOTIFY mainWindowChanged)
    Q_PROPERTY(bool mainIsMaximized READ mainIsMaximized WRITE setMainIsMaximized NOTIFY mainIsMaximizedChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(uint inStockFlags READ inStockFlags WRITE setInStockFlags NOTIFY inStockFlagsChanged)
    Q_PROPERTY(uint forSaleFlags READ forSaleFlags WRITE setForSaleFlags NOTIFY forSaleFlagsChanged)
    Q_PROPERTY(uint soldFlags READ soldFlags WRITE setSoldFlags NOTIFY soldFlagsChanged)
public:
    static BrickMoneySettings* Inst();
    ~BrickMoneySettings();
    BrickMoneySettings(const BrickMoneySettings&) = delete;            // No copy ctor
    BrickMoneySettings& operator=(const BrickMoneySettings&) = delete; //No assign op

    QString brickMoneyFilePath() const;
    QRect mainWindow() const;
    bool mainIsMaximized() const;
    QString language() const;
    uint inStockFlags() const;
    uint forSaleFlags() const;
    uint soldFlags() const;

public slots:
    void setBrickMoneyFilePath(QString brickMoneyFilePath);
    void setMainWindow(QRect mainWindow);
    void setMainIsMaximized(bool mainIsMaximized);
    void setLanguage(QString language);
    void setInStockFlags(uint inStockFlags);
    void setForSaleFlags(uint forSaleFlags);
    void setSoldFlags(uint soldFlags);

signals:
    void brickMoneyFilePathChanged(QString brickMoneyFilePath);
    void mainWindowChanged(QRect mainWindow);
    void mainIsMaximizedChanged(bool mainIsMaximized);
    void languageChanged(QString language);
    void inStockFlagsChanged(uint inStockFlags);
    void forSaleFlagsChanged(uint forSaleFlags);

    void soldFlagsChanged(uint soldFlags);

private:
    static std::unique_ptr<BrickMoneySettings> smInstance;
    BrickMoneySettings();

    QSettings* m_Settings;

    static const QString BrickMoneyName;
    QString m_brickMoneyFilePath;

    static const QString MainWindowName;
    QRect m_mainWindow;

    static const QString MainIsMaximizedName;
    bool m_mainIsMaximized;

    static const QString LanguageName;
    QString m_language;

    static const QString InStockFlagsName;
    uint m_inStockFlags;

    static const QString ForSaleFlagsName;
    uint m_forSaleFlags;

    static const QString SoldFlagsName;
    uint m_soldFlags;
};

#endif // BRICKMONEYSETTINGS_H
