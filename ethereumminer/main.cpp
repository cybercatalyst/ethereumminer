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
#include <QApplication>
#include <QThread>

// Own includes
#include "mainwindow.h"
#include "ethereumminer.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qRegisterMetaType<EthereumMiner::Step>("EthereumMiner::Step");

    EthereumMiner *ethereumMiner = new EthereumMiner();
    ethereumMiner->processInBackground();

    MainWindow w(ethereumMiner);
    w.show();

    return a.exec();
}
