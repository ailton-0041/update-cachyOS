#include "mainwindow.h"
#include <QApplication>
#include <QStyle>
#include <QFont>
#include <QRegularExpression>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , procPacman(new QProcess(this))
    , procFlatpak(new QProcess(this))
    , procUpdate(new QProcess(this))
    , checksPending(0)
{
    setupUI();
    QTimer::singleShot(500, this, &MainWindow::checkUpdates);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    setWindowTitle("Atualizador CachyOS");
    setMinimumSize(750, 500);
    resize(850, 600);

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    auto *title = new QLabel("Atualizador do Sistema");
    QFont titleFont = title->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    statusLabel = new QLabel("Verificando atualiza\u00E7\u00F5es...");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 13px; padding: 5px;");
    mainLayout->addWidget(statusLabel);

    progress = new QProgressBar();
    progress->setRange(0, 0);
    progress->setVisible(false);
    mainLayout->addWidget(progress);

    table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels({"Tipo", "Pacote", "Vers\u00E3o Atual", "Nova Vers\u00E3o"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);
    table->setStyleSheet(
        "QTableWidget { font-size: 12px; }"
        "QHeaderView::section { font-weight: bold; padding: 4px; }");
    mainLayout->addWidget(table, 1);

    auto *btnLayout = new QHBoxLayout();
    btnUpdate = new QPushButton("Atualizar Tudo");
    btnUpdate->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    btnUpdate->setMinimumHeight(45);
    btnUpdate->setStyleSheet(
        "QPushButton { background-color: #2e7d32; color: white; font-weight: bold; "
        "  font-size: 14px; border-radius: 6px; }"
        "QPushButton:hover { background-color: #388e3c; }"
        "QPushButton:disabled { background-color: #cccccc; color: #666; }");
    btnUpdate->setEnabled(false);
    connect(btnUpdate, &QPushButton::clicked, this, &MainWindow::doUpdate);

    btnLayout->addStretch();
    btnLayout->addWidget(btnUpdate);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    auto *footerLayout = new QHBoxLayout();
    btnAbout = new QPushButton("Sobre");
    btnAbout->setFixedSize(80, 30);
    btnAbout->setStyleSheet("QPushButton { font-size: 11px; }");
    connect(btnAbout, &QPushButton::clicked, this, &MainWindow::showAbout);
    footerLayout->addWidget(btnAbout);
    footerLayout->addStretch();
    mainLayout->addLayout(footerLayout);
}

void MainWindow::checkUpdates()
{
    updates.clear();
    table->setRowCount(0);
    checksPending = 2;

    btnUpdate->setEnabled(false);
    setBusy(true);
    statusLabel->setText("Verificando atualiza\u00E7\u00F5es...");
    statusLabel->setStyleSheet("font-size: 13px; padding: 5px; color: #2196F3;");

    connect(procPacman, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::onPacmanCheckFinished);
    procPacman->start("pacman", {"-Qu"});

    connect(procFlatpak, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::onFlatpakCheckFinished);
    procFlatpak->start("flatpak", {"update", "--dry-run", "--noninteractive"});
}

void MainWindow::onPacmanCheckFinished()
{
    QString output = QString::fromUtf8(procPacman->readAllStandardOutput());
    parsePacmanUpdates(output);
    checksPending--;
    if (checksPending == 0) populateTable();
}

void MainWindow::onFlatpakCheckFinished()
{
    QString output = QString::fromUtf8(procFlatpak->readAllStandardOutput());
    parseFlatpakUpdates(output);
    checksPending--;
    if (checksPending == 0) populateTable();
}

void MainWindow::parsePacmanUpdates(const QString &output)
{
    static QRegularExpression re(R"(^(\S+)\s+(\S+)\s*->\s*(\S+))",
                                 QRegularExpression::MultilineOption);
    auto it = re.globalMatch(output);
    while (it.hasNext()) {
        auto m = it.next();
        UpdateInfo info;
        info.tipo = "Sistema";
        info.pacote = m.captured(1);
        info.versaoAtual = m.captured(2);
        info.versaoNova = m.captured(3);
        updates.append(info);
    }
}

void MainWindow::parseFlatpakUpdates(const QString &output)
{
    static QRegularExpression re(R"(^\s*\d+\.\s+(\S+))",
                                 QRegularExpression::MultilineOption);
    auto it = re.globalMatch(output);
    while (it.hasNext()) {
        auto m = it.next();
        UpdateInfo info;
        info.tipo = "Flatpak";

        QString appId = m.captured(1);
        QStringList parts = appId.split('/');
        info.pacote = parts.isEmpty() ? appId : parts[0];

        if (parts.size() >= 2)
            info.versaoNova = parts[1];

        info.versaoAtual = "\u2014";
        updates.append(info);
    }
}

void MainWindow::populateTable()
{
    setBusy(false);

    table->setRowCount(updates.size());

    for (int i = 0; i < updates.size(); i++) {
        auto &u = updates[i];

        auto *tipoItem = new QTableWidgetItem(u.tipo);
        if (u.tipo == "Sistema")
            tipoItem->setIcon(qApp->style()->standardIcon(QStyle::SP_ComputerIcon));
        else
            tipoItem->setIcon(qApp->style()->standardIcon(QStyle::SP_DriveHDIcon));

        table->setItem(i, 0, tipoItem);
        table->setItem(i, 1, new QTableWidgetItem(u.pacote));
        table->setItem(i, 2, new QTableWidgetItem(u.versaoAtual));

        auto *novaItem = new QTableWidgetItem(u.versaoNova);
        novaItem->setForeground(QColor("#2e7d32"));
        QFont boldFont = novaItem->font();
        boldFont.setBold(true);
        novaItem->setFont(boldFont);
        table->setItem(i, 3, novaItem);
    }

    if (updates.isEmpty()) {
        statusLabel->setText("Sistema atualizado! Nenhuma atualiza\u00E7\u00E3o dispon\u00EDvel.");
        statusLabel->setStyleSheet("font-size: 13px; padding: 5px; color: #2e7d32; font-weight: bold;");
        btnUpdate->setEnabled(false);
    } else {
        int sys = 0, flat = 0;
        for (auto &u : updates) {
            if (u.tipo == "Sistema") sys++;
            else flat++;
        }
        statusLabel->setText(QString("%1 atualiza\u00E7\u00F5es dispon\u00EDveis (%2 Sistema, %3 Flatpak)")
                             .arg(updates.size()).arg(sys).arg(flat));
        statusLabel->setStyleSheet("font-size: 13px; padding: 5px; color: #e65100; font-weight: bold;");
        btnUpdate->setEnabled(true);
    }
}

void MainWindow::doUpdate()
{
    setBusy(true);
    btnUpdate->setEnabled(false);
    statusLabel->setText("Atualizando...");
    statusLabel->setStyleSheet("font-size: 13px; padding: 5px; color: #2196F3;");

    connect(procUpdate, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::onUpdateFinished);

    QString cmd = "pkexec";
    QStringList args = {"sh", "-c",
                        "pacman -Syu --noconfirm && flatpak update -y --noninteractive"};
    procUpdate->start(cmd, args);
}

void MainWindow::onUpdateFinished()
{
    setBusy(false);

    if (procUpdate->exitStatus() == QProcess::NormalExit && procUpdate->exitCode() == 0) {
        statusLabel->setText("Atualiza\u00E7\u00E3o conclu\u00EDda com sucesso!");
        statusLabel->setStyleSheet("font-size: 13px; padding: 5px; color: #2e7d32; font-weight: bold;");
    } else {
        statusLabel->setText("Erro durante a atualiza\u00E7\u00E3o.");
        statusLabel->setStyleSheet("font-size: 13px; padding: 5px; color: #c62828; font-weight: bold;");
    }

    QTimer::singleShot(2000, this, &MainWindow::checkUpdates);
}

void MainWindow::showAbout()
{
    QMessageBox about(this);
    about.setWindowTitle("Sobre o Atualizador CachyOS");
    about.setIconPixmap(qApp->style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(48, 48));
    about.setText(
        "<h2>Atualizador CachyOS</h2>"
        "<p>Vers\u00E3o 1.0</p>"
        "<p>Ferramenta simples para atualizar o sistema CachyOS<br>"
        "e aplicativos Flatpak com interface gr\u00E1fica.</p>"
        "<hr>"
        "<p><b>Desenvolvedor:</b> Ailton</p>"
        "<p><b>GitHub:</b> <a href='https://github.com/ailton-0041'>ailton-0041</a></p>"
        "<p><b>Projeto:</b> <a href='https://github.com/ailton-0041/update-cachyOS'>"
        "github.com/ailton-0041/update-cachyOS</a></p>"
        "<hr>"
        "<p><i>CachyOS + Qt6</i></p>"
    );
    about.setTextFormat(Qt::RichText);
    about.exec();
}

void MainWindow::setBusy(bool busy)
{
    progress->setVisible(busy);
    btnUpdate->setEnabled(!busy && !updates.isEmpty());
}
