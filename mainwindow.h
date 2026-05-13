#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QTableWidget>
#include <QHeaderView>

struct UpdateInfo {
    QString tipo;
    QString pacote;
    QString versaoAtual;
    QString versaoNova;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPacmanCheckFinished();
    void onFlatpakCheckFinished();
    void onUpdateFinished();
    void doUpdate();
    void showAbout();

private:
    void setupUI();
    void checkUpdates();
    void parsePacmanUpdates(const QString &output);
    void parseFlatpakUpdates(const QString &output);
    void populateTable();
    void setBusy(bool busy);

    QTableWidget *table;
    QPushButton *btnUpdate;
    QPushButton *btnAbout;
    QLabel *statusLabel;
    QProgressBar *progress;
    QProcess *procPacman;
    QProcess *procFlatpak;
    QProcess *procUpdate;

    QList<UpdateInfo> updates;
    int checksPending;
};

#endif
