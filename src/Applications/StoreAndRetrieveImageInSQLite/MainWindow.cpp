#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QBuffer>
#include <QDebug>
#include <QCryptographicHash>
#include <QPixmapCache>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	setWindowTitle(tr("StoreAndRetrieveImageInSQLite"));

	//Ref: https://wiki.qt.io/How_to_Store_and_Retrieve_Image_on_SQLite
	QString dbName("BMUserImgDB.db3");
    bool ret = QFile::remove(dbName);
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbName);
	db.open();
	QSqlQuery query = QSqlQuery(db);
	query.exec("CREATE TABLE IF NOT EXISTS imgTable ( name TEXT, md5sum TEXT, imagedata BLOB )");

	addImgToTable("71040", query);
	addImgToTable("71042", query);
	addImgToTable("71043", query);
	addImgToTable("71044", query);

	getImgFromTable(query);
	db.close();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::addImgToTable(const QString& name, QSqlQuery& query)
{
    // Ref: https://stackoverflow.com/questions/31522059/how-to-load-an-image-from-a-file-in-qt
	QPixmap pm(":/images/"+ name +".jpg");
	QByteArray inByteArray;
	QBuffer inBuffer(&inByteArray);
	inBuffer.open(QIODevice::WriteOnly);
	pm.save(&inBuffer, "JPG");
	QCryptographicHash hash(QCryptographicHash::Algorithm::Md5);
	hash.addData(inByteArray);
	QString md5sum = hash.result().toHex();

	// Insert image bytes into the database
	// Note: make sure to wrap the :placeholder in parenthesis
	query.prepare("INSERT INTO imgTable (name, md5sum, imagedata) VALUES ('" + name + "', '" + md5sum + "', :imageData)");
	query.bindValue(":imageData", inByteArray);
	if (!query.exec())
		qDebug() << "Error inserting image into table:\n" << query.lastError();
}

void MainWindow::getImgFromTable(QSqlQuery& query)
{
	// Get image data back from database
	if (!query.exec("SELECT * from imgTable"))
		qDebug() << "Error getting image info from imgTable:\n" << query.lastError();

	while (query.next())
	{
		QByteArray outByteArray = query.value("imagedata").toByteArray();
		QString name = query.value("name").toString();
		QString md5sum = query.value("md5sum").toString();
		QPixmap outPixmap = QPixmap();
		outPixmap.loadFromData(outByteArray);
		QPixmapCache::insert(name, outPixmap);

		//Image that the following code is somewhere else. 
		//Hence we use QPixmapCache to get the pixmap at the other (code) location.

		// ui->label->setPixmap()

		QHBoxLayout* horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName("horizontalLayout" + name);

		QLabel* nameLabel = new QLabel(ui->centralwidget);
		nameLabel->setObjectName("nameLabel" + name);
		nameLabel->setText(name);
		horizontalLayout->addWidget(nameLabel);

		QLabel* md5sumLabel = new QLabel(ui->centralwidget);
		md5sumLabel->setObjectName("md5sumLabel" +name);
		md5sumLabel->setText(md5sum);
		horizontalLayout->addWidget(md5sumLabel);

		QLabel* imageLabel = new QLabel(ui->centralwidget);
		imageLabel->setPixmap(outPixmap);
		imageLabel->setObjectName("imageLabel" +name);
		horizontalLayout->addWidget(imageLabel);

		ui->verticalLayoutScrollArea->addLayout(horizontalLayout);
	}
}


