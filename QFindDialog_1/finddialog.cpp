#include <QtWidgets>
#include <QtCore>

#include <QtXml>
#include <QFile>
#include <QDebug>
#include <QTemporaryDir>

#include "finddialog.h"


FindDialog::FindDialog (QWidget *parent) : QDialog(parent){

    QBoxLayout *mainLayout;

    qDebug() << QCoreApplication::applicationDirPath();
    QDomDocument doc;
    QFile file("/home/ivan/qt/QFindDialog_1/myFile.xml");

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

    qListWidget->clear();
    QFile file("/home/ivan/qt/QFindDialog_1/logFile.txt");
    if (!file.exists()) {
        qDebug() << "File didn't exist";
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open doc";
    }
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if (caseCheckBox->isChecked()) {
            if (line.toUpper().indexOf(text.toUpper()) > 0){
                qListWidget->addItem(line);
            }
        }
        else if (line.indexOf(text) > 0){
            qListWidget->addItem(line);
        }
    }
    file.close();

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
                else if (tName == "QLineEdit"){
                    lineEdit = new QLineEdit;
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
                    if (buttonName == "findButton"){
                        findButton = new QPushButton(domElement.attributeNode("Text").value());
                        findButton->setDefault(domElement.attributeNode("setDefault").value()=="true");
                        findButton->setEnabled(domElement.attributeNode("setEnabled").value()=="true");
                        connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
                        layout->addWidget(findButton);
                    } else if (buttonName == "closeButton"){
                        closeButton = new QPushButton(domElement.attributeNode("Text").value());
                        closeButton->setDefault(domElement.attributeNode("setDefault").value()=="true");
                        closeButton->setEnabled(domElement.attributeNode("setEnabled").value()=="true");
                        connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
                        layout->addWidget(closeButton);
                    }
                }
                else if (tName == "QListWidget"){
                    qListWidget = new QListWidget;
                    layout->addWidget(qListWidget);
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
