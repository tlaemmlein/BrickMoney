#ifndef LEGOSETIOMANAGER_H
#define LEGOSETIOMANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "LegoSetTableModel.h"

class LegoSetIOManager : public QObject
{
    Q_OBJECT

public:
    explicit LegoSetIOManager(QObject *parent = nullptr);

    void setLegoSetTableModel(QSharedPointer<LegoSetTableModel> legoSetTableModel);

public: Q_PROPERTY(QString ProjectFolder WRITE setProjectFolder READ projectFolder NOTIFY projectFolderChanged)
public: void setProjectFolder(QString folder);
public: QString projectFolder();
private: QString mProjectFolder;
signals: void projectFolderChanged();

public: Q_PROPERTY(bool isProjectReady READ isProjectReady NOTIFY projectReadyChanged)
public: bool isProjectReady();
signals: void projectReadyChanged();

private:
    QSharedPointer<LegoSetTableModel> mLegoSetTableModel;

    bool mIsProjectReady;

    bool isProjectFolderValid(const QString& projectFolder);
};

#endif // LEGOSETIOMANAGER_H
