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

    Q_INVOKABLE bool isProjectFolderReady(const QString& projectFolder);

    void setLegoSetTableModel(QSharedPointer<LegoSetTableModel> legoSetTableModel);

Q_PROPERTY(QString ProjectFolder WRITE setProjectFolder READ projectFolder NOTIFY projectFolderChange)
public: void setProjectFolder(QString folder);
public: QString projectFolder();
private: QString mProjectFolder;
signals: void projectFolderChange();

private:
    QSharedPointer<LegoSetTableModel> mLegoSetTableModel;

};

#endif // LEGOSETIOMANAGER_H
