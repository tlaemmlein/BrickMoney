#include "LegoSetIOManager.h"

#include <QDir>
#include <QDebug>
#include <QUrl>


LegoSetIOManager::LegoSetIOManager(QObject *parent): QObject(parent)
    , mProjectFolder("Empty")
{

}


bool LegoSetIOManager::isProjectFolderReady(const QString& projectFolder)
{
    qDebug() << "+++ " << __FUNCTION__;

    qDebug() << "projectFolder: " << projectFolder;

    QUrl url(projectFolder);

    QString path = url.toLocalFile();

    qDebug() <<  path;

    QDir projDir(path);

    if ( !projDir.exists() )
    {
        qDebug() << "!exists";
        return false;
    }

    if ( !projDir.isEmpty())
    {
        qDebug() << "!isEmpty";
        return false;
    }

    return true;
}

void LegoSetIOManager::setLegoSetTableModel(QSharedPointer<LegoSetTableModel> legoSetTableModel)
{
    mLegoSetTableModel = legoSetTableModel;
}

void LegoSetIOManager::setProjectFolder(QString folder)
{
    if ( mLegoSetTableModel )
        mLegoSetTableModel->clearAll();

    if ( mProjectFolder != folder)
    {
        mProjectFolder = folder;
        emit projectFolderChange();
    }
}

QString LegoSetIOManager::projectFolder()
{
    return mProjectFolder;
}
