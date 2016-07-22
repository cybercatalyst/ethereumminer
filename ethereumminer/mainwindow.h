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

#pragma once

// Qt includes
#include <QMainWindow>
#include <QSocketNotifier>
#include <QTimerEvent>
#include <QSettings>

// Own includes
#include "ethereumminer.h"

namespace Ui {
class MainWindow;
}

class MainWindow :
    public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(EthereumMiner *ethereumMiner, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_lineEditServer_textChanged(QString text);
    void on_comboBoxCLGlobalWork_activated(QString text);
    void on_comboBoxMinerType_activated(QString text);
    void on_spinBoxVRAMReserve_valueChanged(int i);
    void on_spinBoxMiningThread_valueChanged(int i);
    void on_lineEditPassword_textChanged(QString text);
    void on_lineEditPort_textChanged(QString text);
    void on_comboBoxCLLocalWork_activated(QString text);
    void on_lineEditMsPerBatch_textChanged(QString text);
    void on_lineEditUsername_textChanged(QString text);
    void on_checkBoxCPUCL_stateChanged(int state);
    void on_checkBoxPrecomputeDAG_stateChanged(int state);
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonRestart_clicked();

protected:
    void updateUI();

    void timerEvent(QTimerEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void updateDAGProgress(int progress);
    void receivedWorkPackage(QString headerHash, QString seedHash, QString boundary);
    void solutionFound(QString nonce, QString headerHash, QString mixHash);
    void platformInfo(QString platformInfo);
    void currentStepChanged(EthereumMiner::Step step);

private:
    Ui::MainWindow *ui;

    EthereumMiner::MiningConfiguration _miningConfiguration;
    EthereumMiner *_ethereumMiner;

    QSettings *_settings;
};
