///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QtEthminer.                                       //
//    Copyright (C) 2015-2016 Jacob Dawid, jacob@omg-it.works                //
//                                                                           //
//    QtEthminer is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU Affero General Public License as         //
//    published by the Free Software Foundation, either version 3 of the     //
//    License, or (at your option) any later version.                        //
//                                                                           //
//    QtEthminer is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU Affero General Public License for more details.                    //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QtEthminer. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// Qt includes
#include <QSettings>
#include <QScrollBar>
#include <QDateTime>

// Own includes
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define CPU_MINER_STRING "CPU Miner"
#define OPENCL_MINER_STRING "OpenCL Miner"

MainWindow::MainWindow(EthereumMiner *ethereumMiner, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _settings = new QSettings("QtEthminer", "QtEthminer");
    _ethereumMiner = ethereumMiner;

    connect(_ethereumMiner, SIGNAL(dagCreationProgress(int)),
            this, SLOT(updateDAGProgress(int)));
    connect(_ethereumMiner, SIGNAL(receivedWorkPackage(QString,QString,QString)),
            this, SLOT(receivedWorkPackage(QString,QString,QString)));
    connect(_ethereumMiner, SIGNAL(solutionFound(QString,QString,QString)),
            this, SLOT(solutionFound(QString,QString,QString)));
    connect(_ethereumMiner, SIGNAL(platformInfo(QString)),
            this, SLOT(platformInfo(QString)));
    connect(_ethereumMiner, SIGNAL(currentStep(EthereumMiner::Step)),
            this, SLOT(currentStepChanged(EthereumMiner::Step)));

    _ethereumMiner->loadSettings(*_settings);
    _miningConfiguration = _ethereumMiner->miningConfiguration();
    ui->comboBoxMinerType->addItem(CPU_MINER_STRING);
    ui->comboBoxMinerType->addItem(OPENCL_MINER_STRING);
    updateUI();
    startTimer(2000);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateDAGProgress(int progress) {
    ui->progressBarDAGCreation->setValue(progress);
}

void MainWindow::receivedWorkPackage(
    QString headerHash,
    QString seedHash,
    QString boundary) {
    ui->lineEditLastWork->setText(QDateTime::currentDateTime().toString());
    ui->lineEditWorkPackageHeaderHash->setText(headerHash);
    ui->lineEditWorkPackageSeedHash->setText(seedHash);
    ui->lineEditWorkPackageBoundary->setText(boundary);
}

void MainWindow::solutionFound(
    QString nonce,
    QString headerHash,
    QString mixHash) {
    ui->lineEditLastSolutionFound->setText(QDateTime::currentDateTime().toString());
    ui->lineEditSubmittedNonce->setText(nonce);
    ui->lineEditSubmittedHeaderHash->setText(headerHash);
    ui->lineEditSubmittedMixHash->setText(mixHash);
}

void MainWindow::platformInfo(QString platformInfo) {
    ui->lineEditPlatformInfo->setText(platformInfo);
}

void MainWindow::currentStepChanged(EthereumMiner::Step step) {
    switch(step) {
    case EthereumMiner::Starting:
        ui->lineEditCurrentStep->setText("Starting");
        break;
    case EthereumMiner::Halted:
        ui->lineEditCurrentStep->setText("Halted");
        break;
    case EthereumMiner::Connecting:
        ui->lineEditCurrentStep->setText("Connecting");
        break;
    case EthereumMiner::LoggingIn:
        ui->lineEditCurrentStep->setText("Logging in");
        break;
    case EthereumMiner::Mining:
        ui->lineEditCurrentStep->setText("Mining");
        break;
    case EthereumMiner::BuildingDAG:
        ui->lineEditCurrentStep->setText("Building DAG");
        break;
    }
}

void MainWindow::updateUI() {
    ui->lineEditServer->setText(_miningConfiguration.server);
    ui->comboBoxCLGlobalWork->setCurrentText(QString("%1").arg(_miningConfiguration.globalWorkSizeMultiplier));

    QString minerTypeString;
    switch(_miningConfiguration.minerType) {
    case EthereumMiner::CPUMiner:
        minerTypeString = CPU_MINER_STRING;
        break;
    case EthereumMiner::OpenCLMiner:
        minerTypeString = OPENCL_MINER_STRING;
        break;
    }
    ui->comboBoxMinerType->setCurrentText(minerTypeString);

    ui->spinBoxVRAMReserve->setValue(_miningConfiguration.extraGPUMemory / 1000000);
    ui->spinBoxMiningThread->setValue(_miningConfiguration.maxMiningThreads);
    ui->lineEditPassword->setText(_miningConfiguration.password);
    ui->lineEditPort->setText(QString("%1").arg(_miningConfiguration.port));
    ui->comboBoxCLLocalWork->setCurrentText(QString("%1").arg(_miningConfiguration.localWorkSize));
    ui->lineEditMsPerBatch->setText(QString("%1").arg(_miningConfiguration.msPerBatch));
    ui->lineEditUsername->setText(_miningConfiguration.username);
    ui->checkBoxCPUCL->setChecked(_miningConfiguration.recognizeCPUAsOpenCLDevice);
    ui->checkBoxPrecomputeDAG->setChecked(_miningConfiguration.precomputeNextDAG);
}

void MainWindow::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e);
    ui->lineEditHashrate->setText(QString("%1").arg(_ethereumMiner->hashrate()));
}

void MainWindow::closeEvent(QCloseEvent *e) {
    Q_UNUSED(e);
    _ethereumMiner->setMiningConfiguration(_miningConfiguration);
    _ethereumMiner->saveSettings(*_settings);
}

void MainWindow::on_lineEditServer_textChanged(QString text) {
    _miningConfiguration.server = text;
}

void MainWindow::on_comboBoxCLGlobalWork_activated(QString text) {
    _miningConfiguration.globalWorkSizeMultiplier = text.toInt();
}

void MainWindow::on_comboBoxMinerType_activated(QString text) {
    EthereumMiner::MinerType type;
    if(text == CPU_MINER_STRING) {
        type = EthereumMiner::CPUMiner;
    }
    if(text == OPENCL_MINER_STRING) {
        type = EthereumMiner::OpenCLMiner;
    }
    _miningConfiguration.minerType = type;
}

void MainWindow::on_spinBoxVRAMReserve_valueChanged(int i) {
    _miningConfiguration.extraGPUMemory = i * 1000000;
}

void MainWindow::on_spinBoxMiningThread_valueChanged(int i) {
    _miningConfiguration.maxMiningThreads = i;
}

void MainWindow::on_lineEditPassword_textChanged(QString text) {
    _miningConfiguration.password = text;
}

void MainWindow::on_lineEditPort_textChanged(QString text) {
    _miningConfiguration.port = text.toInt();
}

void MainWindow::on_comboBoxCLLocalWork_activated(QString text) {
    _miningConfiguration.localWorkSize = text.toInt();
}

void MainWindow::on_lineEditMsPerBatch_textChanged(QString text) {
    _miningConfiguration.msPerBatch = text.toInt();
}

void MainWindow::on_lineEditUsername_textChanged(QString text) {
    _miningConfiguration.username = text;
}

void MainWindow::on_checkBoxCPUCL_stateChanged(int state) {
    _miningConfiguration.recognizeCPUAsOpenCLDevice = (state == Qt::Checked);
}

void MainWindow::on_checkBoxPrecomputeDAG_stateChanged(int state) {
    _miningConfiguration.precomputeNextDAG = (state == Qt::Checked);
}

void MainWindow::on_pushButtonStart_clicked() {
    ui->tabWidget->setCurrentWidget(ui->tabStatus);
    _ethereumMiner->setMiningConfiguration(_miningConfiguration);
    _ethereumMiner->startMining();
}

void MainWindow::on_pushButtonStop_clicked() {
    _ethereumMiner->stopMining();
}

void MainWindow::on_pushButtonRestart_clicked() {
    _ethereumMiner->restartMining();
}
