#include "LegoSetIOManager.h"

#include <QDir>
#include <QDebug>
#include <QUrl>


LegoSetIOManager::LegoSetIOManager(QObject *parent): QObject(parent)
    , mProjectFolder("Empty")
    , mIsProjectReady(false)
{

}


bool LegoSetIOManager::isProjectReady()
{
    return mIsProjectReady;
}

void LegoSetIOManager::setLegoSetTableModel(QSharedPointer<LegoSetTableModel> legoSetTableModel)
{
    mLegoSetTableModel = legoSetTableModel;
}

void LegoSetIOManager::setProjectFolder(QString folder)
{
    if ( !isProjectFolderValid(folder))
    {
        mIsProjectReady = false;
        return;
    }

    if ( mLegoSetTableModel )
        mLegoSetTableModel->clearAll();

    mIsProjectReady = true;
    emit projectReadyChanged();

    if ( mProjectFolder != folder)
    {
        mProjectFolder = folder;
        emit projectFolderChanged();
    }
}

QString LegoSetIOManager::projectFolder()
{
    return mProjectFolder;
}

bool LegoSetIOManager::isProjectFolderValid(const QString& projectFolder)
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
