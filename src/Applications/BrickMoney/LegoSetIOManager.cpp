#include "Logging.h"
SET_LOGGER("BrickMoney.LegoSetIOManager")

#include "LegoSetIOManager.h"

#include <QDir>
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
    LOG_SCOPE_METHOD(L"");

    if ( !mLegoSetTableModel )
        return;

    if ( !isProjectReady() )
        return;

    QString csvPath = projectFolder() + "/brick_money.csv";

    LOG_INFO("Write to " << csvPath.toStdWString());

    QFile cvsData(csvPath);

    if ( !cvsData.open(QFile::WriteOnly))
    {
        LOG_ERROR("Could not write to " << csvPath.toStdWString());
        return;
    }

    QTextStream output(&cvsData);

    mLegoSetTableModel->saveDataTo(';', output, projectFolder());
}

void LegoSetIOManager::loadProject(const QString &projectFolderToLoad)
{
    LOG_SCOPE_METHOD(L"");

    if ( !mLegoSetTableModel )
        return;

    setProjectFolder(projectFolderToLoad);

    if (!isProjectReady())
        return;

    QString csvPath = projectFolder() + "/brick_money.csv";

    LOG_INFO("Read from " << csvPath.toStdWString());

    QFile cvsData(csvPath);

    if ( !cvsData.open(QFile::ReadOnly | QIODevice::Text))
    {
        LOG_ERROR("Could not read from " << csvPath.toStdWString());
        return;
    }

    QTextStream input(&cvsData);
    input.setAutoDetectUnicode (true);

    mLegoSetTableModel->loadDataFrom(';', input, projectFolder());
}

void LegoSetIOManager::setProjectFolder(QString folder)
{
    LOG_SCOPE_METHOD(L"");
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
    LOG_SCOPE_METHOD(L"");

    LOG_DEBUG("projectFolder: " << projectFolder.toStdWString());

    QDir projDir(toLocalFile(projectFolder));

    if ( !projDir.exists() )
    {
        LOG_ERROR("projDir doesn't exists: " << projDir.path().toStdWString());
        return false;
    }

    return true;
}

QString LegoSetIOManager::toLocalFile(const QString &projectFolder)
{
    QUrl url(projectFolder);

    return url.toLocalFile();
}
