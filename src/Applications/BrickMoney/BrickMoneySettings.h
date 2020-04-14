#ifndef BRICKMONEYSETTINGS_H
#define BRICKMONEYSETTINGS_H

#include <QObject>
#include <QSettings>

class BrickMoneySettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)
    Q_PROPERTY(QString brickMoneyFilePath READ brickMoneyFilePath WRITE setBrickMoneyFilePath NOTIFY brickMoneyFilePathChanged)
    Q_PROPERTY(bool brickMoneyIsDirty READ brickMoneyIsDirty WRITE setBrickMoneyIsDirty NOTIFY brickMoneyIsDirtyChanged)
public:
    static BrickMoneySettings* Inst();
    ~BrickMoneySettings();
    BrickMoneySettings(const BrickMoneySettings&) = delete;            // No copy ctor
    BrickMoneySettings& operator=(const BrickMoneySettings&) = delete; //No assign op

    int zoomFactor() const;
    QString brickMoneyFilePath() const;
    bool brickMoneyIsDirty() const;

public slots:
    void setZoomFactor(int zoomFactor);
    void setBrickMoneyFilePath(QString brickMoneyFilePath);
    void setBrickMoneyIsDirty(bool brickMoneyIsDirty);

signals:
    void zoomFactorChanged(int zoomFactor);
    void brickMoneyFilePathChanged(QString brickMoneyFilePath);
    void brickMoneyIsDirtyChanged(bool brickMoneyIsDirty);

private:
    static std::unique_ptr<BrickMoneySettings> smInstance;
    BrickMoneySettings();

    QSettings* m_Settings;
    static const QString ZoomFactorName;
    int m_zoomFactor;
    static const QString BrickMoneyName;
    QString m_brickMoneyFilePath;
    bool m_brickMoneyIsDirty;
};

#endif // BRICKMONEYSETTINGS_H
