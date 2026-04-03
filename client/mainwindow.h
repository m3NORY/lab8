#pragma once

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QNetworkAccessManager>

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow();

private:
    QPushButton *btnUpload;
    QPushButton *btnList;
    QPushButton *btnDownload;
    QTextEdit *text;
    QLineEdit *input;
    QNetworkAccessManager *manager;

private slots:
    void uploadFile();
    void getFiles();
    void downloadFile();
};
