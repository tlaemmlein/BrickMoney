#ifndef BRICKMONEYSETTINGS_H
#define BRICKMONEYSETTINGS_H

#include <QObject>
#include <QSettings>

class BrickMoneySettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)
public:
	BrickMoneySettings();
	~BrickMoneySettings();

public: int zoomFactor() const;
public slots: void setZoomFactor(int zoomFactor);
signals:void zoomFactorChanged(int zoomFactor);

private:

    QSettings* m_Settings;
    int m_zoomFactor;
    static const QString ZoomFactorName;
};

#endif // BRICKMONEYSETTINGS_H
