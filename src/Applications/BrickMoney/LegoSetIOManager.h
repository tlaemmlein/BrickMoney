#ifndef LEGOSETIOMANAGER_H
#define LEGOSETIOMANAGER_H

#include <QObject>

class LegoSetIOManager : public QObject
{
    Q_OBJECT

public:
    explicit LegoSetIOManager(QObject *parent = nullptr);

    Q_INVOKABLE bool isProjectFolderReady(const QString& projectFolder);
};

#endif // LEGOSETIOMANAGER_H
