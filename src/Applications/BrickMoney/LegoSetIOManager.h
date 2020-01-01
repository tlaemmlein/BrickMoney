#ifndef LEGOSETIOMANAGER_H
#define LEGOSETIOMANAGER_H

#include <QObject>

class LegoSetIOManager : public QObject
{
    Q_OBJECT

public:
    explicit LegoSetIOManager(QObject *parent = nullptr);

    Q_INVOKABLE bool isProjectFolderReady(const QString& projectFolder);

Q_PROPERTY(QString ProjectFolder WRITE setProjectFolder READ projectFolder NOTIFY projectFolderChange)
public: void setProjectFolder(QString folder);
public: QString projectFolder();
private: QString mProjectFolder;
signals: void projectFolderChange();
};

#endif // LEGOSETIOMANAGER_H
