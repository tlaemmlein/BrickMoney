#ifndef LEGOSETMODEL_H
#define LEGOSETMODEL_H

#include <QStandardItemModel>

class LegoSetModel : public QStandardItemModel
{
    Q_OBJECT

public:
 explicit LegoSetModel(QObject *parent = nullptr);

    enum Column {SetID, SetNumber};

    enum {
        DescriptionRole = Qt::UserRole,
        IdRole
    };

    /// Reset the Model.
    /// It clears the content, but add one row again.
    /// There must be always one row available.
    void reset();

    void addSet();


private:

    void initialize();
    void clear();

    unsigned int m_MaxSetID;


};

#endif // LEGOSETMODEL_H
