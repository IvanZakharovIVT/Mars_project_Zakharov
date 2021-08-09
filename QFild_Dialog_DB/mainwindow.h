#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

#include <QtWidgets>
#include <QtXml>

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = 0);
    QBoxLayout* traverse(const QDomNode &node );
signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
private slots:
    void findClicked();
    void showAll();
    void addRow();
    void deleteRow();
    void enabledFindButton(const QString &text);
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardChekBox;
    QPushButton *findButton;
    QPushButton *closeButton;
    QListWidget * qListWidget;
    QTableView * qTableView;

    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    int row;
    QPushButton *addRowButton;
    QPushButton *deteateRowButton;
    QPushButton *showAllButton;

};

#endif // FINDDIALOG_H
