#ifndef BRICKMONEYSETTINGS_H
#define BRICKMONEYSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QRect>

class BrickMoneySettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)
    Q_PROPERTY(QString brickMoneyFilePath READ brickMoneyFilePath WRITE setBrickMoneyFilePath NOTIFY brickMoneyFilePathChanged)
    Q_PROPERTY(bool brickMoneyIsDirty READ brickMoneyIsDirty WRITE setBrickMoneyIsDirty NOTIFY brickMoneyIsDirtyChanged)
    Q_PROPERTY(int viewSettings READ viewSettings WRITE setViewSettings NOTIFY viewSettingsChanged)
    Q_PROPERTY(QRect mainWindow READ mainWindow WRITE setMainWindow NOTIFY mainWindowChanged)
public:
    static BrickMoneySettings* Inst();
    ~BrickMoneySettings();
    BrickMoneySettings(const BrickMoneySettings&) = delete;            // No copy ctor
    BrickMoneySettings& operator=(const BrickMoneySettings&) = delete; //No assign op

    int zoomFactor() const;
    QString brickMoneyFilePath() const;
    bool brickMoneyIsDirty() const;
    int viewSettings() const;
    QRect mainWindow() const;

public slots:
    void setZoomFactor(int zoomFactor);
    void setBrickMoneyFilePath(QString brickMoneyFilePath);
    void setBrickMoneyIsDirty(bool brickMoneyIsDirty);
    void setViewSettings(int viewSettings);
    void setMainWindow(QRect mainWindow);

signals:
    void zoomFactorChanged(int zoomFactor);
    void brickMoneyFilePathChanged(QString brickMoneyFilePath);
    void brickMoneyIsDirtyChanged(bool brickMoneyIsDirty);
    void viewSettingsChanged(int viewSettings);
    void mainWindowChanged(QRect mainWindow);

private:
    static std::unique_ptr<BrickMoneySettings> smInstance;
    BrickMoneySettings();

    QSettings* m_Settings;
    static const QString ZoomFactorName;
    int m_zoomFactor;
    static const QString BrickMoneyName;
    QString m_brickMoneyFilePath;
    bool m_brickMoneyIsDirty;
    static const QString ViewSettingsName;
    int m_viewSettings;
    static const QString MainWindowName;
    QRect m_mainWindow;
};

#endif // BRICKMONEYSETTINGS_H
