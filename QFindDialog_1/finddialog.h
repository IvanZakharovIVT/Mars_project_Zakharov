#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

#include <QtWidgets>
#include <QtXml>


class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;


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
    void enabledFindButton(const QString &text);
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardChekBox;
    QPushButton *findButton;
    QPushButton *closeButton;
    QListWidget * qListWidget;
};

#endif // FINDDIALOG_H
