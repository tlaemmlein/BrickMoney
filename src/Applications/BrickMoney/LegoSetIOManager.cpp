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

void LegoSetIOManager::saveProject()
{
    qDebug() << "+++ " << __FUNCTION__;

    if ( !mLegoSetTableModel )
        return;

    if ( !isProjectReady() )
        return;

    QString csvPath = projectFolder() + "/brick_money.csv";

    qDebug() << "Write to " << csvPath;

    QFile cvsData(csvPath);

    if ( !cvsData.open(QFile::WriteOnly))
    {
        qDebug() << "Could not write to " << csvPath;
        return;
    }

    QTextStream output(&cvsData);

    mLegoSetTableModel->saveDataTo(';', output);

    qDebug() << "--- " << __FUNCTION__;
}

void LegoSetIOManager::loadProject(const QString &projectFolderToLoad)
{
    qDebug() << "+++ " << __FUNCTION__;

    if ( !mLegoSetTableModel )
        return;

    setProjectFolder(projectFolderToLoad);

    if (!isProjectReady())
        return;

    QString csvPath = projectFolder() + "/brick_money.csv";

    qDebug() << "Read from " << csvPath;

    QFile cvsData(csvPath);

    if ( !cvsData.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not read from " << csvPath;
        return;
    }

    QTextStream input(&cvsData);

    mLegoSetTableModel->loadDataFrom(';', input);

    qDebug() << "--- " << __FUNCTION__;
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

    const QString new_project_folder = toLocalFile(folder);

    if ( mProjectFolder != new_project_folder)
    {
        mProjectFolder = new_project_folder;
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

    QDir projDir(toLocalFile(projectFolder));

    if ( !projDir.exists() )
    {
        qDebug() << "!exists";
        return false;
    }

    return true;
}

QString LegoSetIOManager::toLocalFile(const QString &projectFolder)
{
    QUrl url(projectFolder);

    return url.toLocalFile();
}
