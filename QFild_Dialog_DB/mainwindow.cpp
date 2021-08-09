#include "mainwindow.h"

#include <QtWidgets>
#include <QtCore>

#include <QtXml>
#include <QFile>
#include <QDebug>
#include <QTemporaryDir>


FindDialog::FindDialog (QWidget *parent) : QDialog(parent){

    QBoxLayout *mainLayout;

    qDebug() << QCoreApplication::applicationDirPath();
    QDomDocument doc;

    QFile file(QCoreApplication::applicationDirPath()+ "/myFileDB.xml");

    if (!file.exists()) {
        qDebug() << "File didn't exist";
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open doc";
        return;
    }
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to load doc";
        file.close();
        return;
    }
    file.close();
    QDomElement elem = doc.documentElement();
    mainLayout = traverse(elem);
    setLayout(mainLayout);
    setWindowTitle("Find");
    setFixedHeight(sizeHint().height());
}

void FindDialog::findClicked(){
    QString text = lineEdit->text();
    if (caseCheckBox->isChecked())
        model->setFilter(QString("LogText LIKE '%%1%'").arg(text));
    else
        model->setFilter("LogID='"+text+"'");
}

void FindDialog::showAll(){
    model->setFilter("LogID > 0");
}

void FindDialog::addRow()
{
    model->insertRow(model->rowCount());
}


void FindDialog::deleteRow()
{
    row = qTableView->currentIndex().row();
    model->removeRow(row);
}


void FindDialog::enabledFindButton(const QString &text){
    findButton->setEnabled(!text.isEmpty());
}


QBoxLayout* FindDialog::traverse(const QDomNode &node){
    QBoxLayout *layout = new QHBoxLayout;
    if (node.toElement().tagName() == "QVBoxLayout"){
        layout = new QVBoxLayout;
    }
    QDomNode domNode = node.firstChild();
    while (!domNode.isNull()){
        if (domNode.isElement()){
            QDomElement domElement = domNode.toElement();
            if (!domElement.isNull()){
                QString tName = domElement.tagName();
                if (tName == "QHBoxLayout"){
                    layout->addLayout(traverse(domNode));
                } else if (tName == "QVBoxLayout"){
                    layout->addLayout(traverse(domNode));
                }
                else if (tName =="QLabel"){
                    label = new QLabel(domElement.attributeNode("Text").value());
                }
                else if (tName == "QLineEdit"){//MinimumWidth
                    lineEdit = new QLineEdit;
                    lineEdit->setMinimumWidth(domElement.attributeNode("MinimumWidth").value().toInt());
                    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(enabledFindButton(const QString &)));
                    layout->addWidget(lineEdit);
                    label->setBuddy(lineEdit);
                    layout->addWidget(label);
                }
                else if (tName == "QCheckBox"){
                    QString chBoxName = domElement.attributeNode("Name").value();
                    if (chBoxName == "caseCheckBox"){
                        caseCheckBox = new QCheckBox(domElement.attributeNode("Text").value());
                        layout->addWidget(caseCheckBox);
                    }
                }
                else if (tName == "QPushButton"){
                    QString buttonName = domElement.attributeNode("Name").value();
                    QPushButton *btn = new QPushButton(domElement.attributeNode("Text").value());
                    btn->setDefault(domElement.attributeNode("setDefault").value()=="true");
                    btn->setEnabled(domElement.attributeNode("setEnabled").value()=="true");
                    if (buttonName == "findButton"){
                        findButton = btn;
                        connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
                        layout->addWidget(findButton);
                    } else if (buttonName == "closeButton"){
                        closeButton = btn;
                        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
                        layout->addWidget(closeButton);
                    } else if (buttonName == "addRowButton"){
                        addRowButton = btn;
                        connect(addRowButton, SIGNAL(clicked()), this, SLOT(addRow()));
                        layout->addWidget(addRowButton);
                    } else if (buttonName == "showAllButton"){
                        showAllButton = btn;
                        connect(showAllButton, SIGNAL(clicked()), this, SLOT(showAll()));
                        layout->addWidget(showAllButton);
                    } else if (buttonName == "deteateRowButton"){
                        deteateRowButton = btn;
                        connect(deteateRowButton, SIGNAL(clicked()), this, SLOT(deleteRow()));
                        layout->addWidget(deteateRowButton);
                    } else {
                        layout->addWidget(btn);
                    }
                }
                else if (tName == "QTableView"){
                    qTableView = new QTableView();
                    db = QSqlDatabase::addDatabase("QSQLITE");
                    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/testDB.bd");
                    if (db.open())
                        qDebug() <<"open";
                    else
                        qDebug() << "no open";
                    query = new QSqlQuery(db);
                    model = new QSqlTableModel(this, db);
                    model->setTable("LogTable");
                    model->select();
                    qTableView->setModel(model);
                    layout->addWidget(qTableView);
                }
                else {
                    qDebug() << domElement.tagName();
                }
            }
        }
        domNode = domNode.nextSibling();
    }
    return layout;
}
