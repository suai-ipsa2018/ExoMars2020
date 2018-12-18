#ifndef DBVIEWER_H
#define DBVIEWER_H

#include <QWidget>

namespace Ui {
class dbViewer;
}

class dbViewer : public QWidget
{
    Q_OBJECT

public:
    explicit dbViewer(QWidget *parent = nullptr);
    ~dbViewer();

private:
    Ui::dbViewer *ui;
};

#endif // DBVIEWER_H
