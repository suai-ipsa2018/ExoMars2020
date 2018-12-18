#include "dbviewer.h"
#include "ui_dbviewer.h"

dbViewer::dbViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dbViewer)
{
    ui->setupUi(this);
    int rc = sqlite3_open_v2("../ExoMars2020/Network/logs/Network_0.db", &db, SQLITE_OPEN_READONLY, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to open database: " << sqlite3_errmsg(db) << std::endl;
        exit(-1);
    }

    char* zErrMsg;

    rc = sqlite3_exec(db, "SELECT name FROM sqlite_master WHERE type = 'table'",
                      [](void* w, int argc, char** argv, char**) -> int {
        QComboBox* cb = static_cast<QComboBox*>(w);
        for (int i=0; i < argc; i++)
            cb->addItem(argv[i]);
        return 0;
    }, static_cast<void*>(ui->tablesComboBox), &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to gather table names: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}

dbViewer::~dbViewer()
{
    sqlite3_close(db);
    delete ui;
}

void dbViewer::on_tablesComboBox_currentIndexChanged(const QString &table)
{
    char* zErrMsg;
    ui->dbTable->setRowCount(0);
    int rc = sqlite3_exec(db, ("SELECT * FROM " + table + " WHERE TIME = 0").toStdString().c_str(),
                      [](void* w, int argc, char**, char**) -> int {
        static_cast<QTableWidget*>(w)->setColumnCount(argc);
        return 0;
    }, static_cast<void*>(ui->dbTable), &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to count columns: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }


    rc = sqlite3_exec(db, ("SELECT * FROM " + table + " WHERE TIME = 0").toStdString().c_str(),
                      [](void* w, int argc, char**, char** azColName) -> int {
        QStringList qsl;
        for (int i = 0; i < argc; i++)
        {
            qsl << azColName[i];
        }
        static_cast<QTableWidget*>(w)->setHorizontalHeaderLabels(qsl);
        return 0;
    }, static_cast<void*>(ui->dbTable), &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to count columns: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

    rc = sqlite3_exec(db, ("SELECT * FROM " + table).toStdString().c_str(),
                          [](void* w, int argc, char** argv, char**) -> int {
            QTableWidget* dbTable = static_cast<QTableWidget*>(w);
            dbTable->setRowCount(dbTable->rowCount() + 1);
            for (int i = 0; i < argc; i++)
            {
                dbTable->setItem(dbTable->rowCount() - 1, i, new QTableWidgetItem(argv[i]));
            }
            return 0;
        }, static_cast<void*>(ui->dbTable), &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to count columns: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }

}
