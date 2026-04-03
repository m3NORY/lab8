#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>

MainWindow::MainWindow() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    btnUpload = new QPushButton("Upload");
    btnList = new QPushButton("List");
    btnDownload = new QPushButton("Download");
    text = new QTextEdit();
    input = new QLineEdit();

    layout->addWidget(btnUpload);
    layout->addWidget(btnList);
    layout->addWidget(input);
    layout->addWidget(btnDownload);
    layout->addWidget(text);

    manager = new QNetworkAccessManager(this);

    connect(btnUpload, &QPushButton::clicked, this, &MainWindow::uploadFile);
    connect(btnList, &QPushButton::clicked, this, &MainWindow::getFiles);
    connect(btnDownload, &QPushButton::clicked, this, &MainWindow::downloadFile);
}

void MainWindow::uploadFile() {
    QString path = QFileDialog::getOpenFileName(this, "Select file");
    if(path.isEmpty()) return;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) return;
    QByteArray data = file.readAll();

    QNetworkRequest req(QUrl("http://localhost:8080/files"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain");
    manager->post(req, data);
}

void MainWindow::getFiles() {
    QNetworkRequest req(QUrl("http://localhost:8080/files"));
    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, [=]() {
        text->setPlainText(reply->readAll());
        reply->deleteLater();
    });
}

void MainWindow::downloadFile() {
    QString hash = input->text();
    if(hash.isEmpty()) return;
    QNetworkRequest req(QUrl("http://localhost:8080/files/" + hash));
    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, [=]() {
        text->setPlainText(reply->readAll());
        reply->deleteLater();
    });
}
