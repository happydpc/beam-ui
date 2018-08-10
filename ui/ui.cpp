// Copyright 2018 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <QApplication>
#include <QtQuick>

#include <QInputDialog>
#include <QMessageBox>

#include <qqmlcontext.h>
#include "viewmodel/start.h"
#include "viewmodel/main.h"
#include "viewmodel/dashboard.h"
#include "viewmodel/address_book.h"
#include "viewmodel/wallet.h"
#include "viewmodel/notifications.h"
#include "viewmodel/help.h"
#include "viewmodel/settings.h"

#include "wallet/wallet_db.h"
#include "utility/logger.h"
#include "core/ecc_native.h"

#include "translator.h"

#include "utility/options.h"

#include <QtCore/QtPlugin>

#include "version.h"

#include "utility/string_helpers.h"

#if defined(BEAM_USE_STATIC)

#if defined Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined Q_OS_MAC
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif defined Q_OS_LINUX
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif

Q_IMPORT_PLUGIN(QtQuick2Plugin)
Q_IMPORT_PLUGIN(QtQuick2WindowPlugin)
Q_IMPORT_PLUGIN(QtQuickControls1Plugin)
Q_IMPORT_PLUGIN(QtQuickControls2Plugin)
Q_IMPORT_PLUGIN(QtGraphicalEffectsPlugin)
Q_IMPORT_PLUGIN(QtGraphicalEffectsPrivatePlugin)
Q_IMPORT_PLUGIN(QSvgPlugin)
Q_IMPORT_PLUGIN(QtQuickLayoutsPlugin)
Q_IMPORT_PLUGIN(QtQuickTemplates2Plugin)

#endif

using namespace beam;
using namespace std;
using namespace ECC;

int main (int argc, char* argv[])
{
	QApplication app(argc, argv);

	QApplication::setApplicationName("Beam");
	QApplication::setOrganizationName("beam-mw.com");

	QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

	try
	{
		po::options_description options = createOptionsDescription();
		po::variables_map vm;

		try
		{
			vm = getOptions(argc, argv, "beam-ui.cfg", options);
		}
		catch (const po::error& e)
		{
			cout << e.what() << std::endl;
			cout << options << std::endl;

			return -1;
		}

		if (vm.count(cli::HELP))
		{
			cout << options << std::endl;

			return 0;
		}

		if (vm.count(cli::VERSION))
		{
			cout << PROJECT_VERSION << endl;
			return 0;
		}

		if (vm.count(cli::GIT_COMMIT_HASH))
		{
			cout << GIT_COMMIT_HASH << endl;
			return 0;
		}

		if (vm.count(cli::APPDATA_PATH))
		{
			appDataDir = QString::fromStdString(vm[cli::APPDATA_PATH].as<string>());
		}

		int logLevel = getLogLevel(cli::LOG_LEVEL, vm, LOG_LEVEL_DEBUG);
		int fileLogLevel = getLogLevel(cli::FILE_LOG_LEVEL, vm, LOG_LEVEL_INFO);
#if LOG_VERBOSE_ENABLED
		logLevel = LOG_LEVEL_VERBOSE;
#endif
		
		auto logger = beam::Logger::create(logLevel, logLevel, fileLogLevel, "beam_ui_", appDataDir.filePath("./logs").toStdString());

		try
		{
			Rules::get().UpdateChecksum();
			LOG_INFO() << "Rules signature: " << Rules::get().Checksum;

			auto walletStorage = appDataDir.filePath("wallet.db").toStdString();
            auto bbsStorage = appDataDir.filePath("keys.bbs").toStdString();
			std::string walletPass;

			QQuickView view;
			view.setResizeMode(QQuickView::SizeRootObjectToView);

			StartViewModel startViewModel(walletStorage);

			QQmlContext *ctxt = view.rootContext();
			ctxt->setContextProperty("viewModel", &startViewModel);

			view.setSource(QUrl("qrc:///start.qml"));
			view.show();

			return app.exec();

		//	if (!Keychain::isInitialized(walletStorage))
		//	{
		//		bool ok = true;
		//		QString seed;

		//		while (seed.isEmpty() && ok)
		//		{
		//			seed = QInputDialog::getText(0, "Beam", "wallet.db not found\nPlease, enter a seed to initialize your wallet:", QLineEdit::Normal, nullptr, &ok);
		//		}

		//		if (ok && !seed.isEmpty())
		//		{
		//			QString pass;

		//			while (pass.isEmpty() && ok)
		//			{
		//				pass = QInputDialog::getText(0, "Beam", "Please, enter a password:", QLineEdit::Password, nullptr, &ok);
		//			}

		//			if (ok && !pass.isEmpty()) 
		//			{
		//				walletPass = pass.toStdString();

		//				NoLeak<uintBig> walletSeed;
		//				walletSeed.V = Zero;
		//				{
		//					Hash::Value hv;
		//					Hash::Processor() << seed.toStdString().c_str() >> hv;
		//					walletSeed.V = hv;
		//				}

		//				auto keychain = Keychain::init(walletStorage, walletPass, walletSeed);

		//				if (keychain)
		//				{
		//					QMessageBox::information(0, "Beam", "wallet.db successfully created.", QMessageBox::Ok);
		//				}
		//				else
		//				{
		//					QMessageBox::critical(0, "Error", "Your wallet isn't created. Something went wrong.", QMessageBox::Ok);
		//					return -1;
		//				}

  //                      try
  //                      {
  //                          IKeyStore::Options options;
  //                          options.flags = IKeyStore::Options::local_file | IKeyStore::Options::enable_all_keys;
  //                          options.fileName = bbsStorage;

  //                          IKeyStore::Ptr keystore = IKeyStore::create(options, walletPass.c_str(), walletPass.size());

  //                          // generate default address
  //                          WalletAddress defaultAddress = {};
  //                          defaultAddress.m_own = true;
  //                          defaultAddress.m_label = "default";
  //                          defaultAddress.m_createTime = getTimestamp();
  //                          defaultAddress.m_duration = numeric_limits<uint64_t>::max();
  //                          keystore->gen_keypair(defaultAddress.m_walletID, walletPass.c_str(), walletPass.size(), true);

  //                          keychain->saveAddress(defaultAddress);
  //                      }
  //                      catch (const std::runtime_error& ex)
  //                      {
  //                          QMessageBox::critical(0, "Error", "Failed to generate default address", QMessageBox::Ok);
  //                      }
  //                  }
  //                  else
  //                  {
  //                      return 0;
		//			}
		//		}
		//	}
		//	else
		//	{
		//		bool ok = true;
		//		QString pass;

		//		while (pass.isEmpty() && ok)
		//		{
		//			pass = QInputDialog::getText(0, "Beam", "Please, enter a password:", QLineEdit::Password, nullptr, &ok);
		//		}

		//		if (ok && !pass.isEmpty())
		//		{
		//			walletPass = pass.toStdString();
		//		}
		//		else return 0;
		//	}

		//	{
		//		auto keychain = Keychain::open(walletStorage, walletPass);

		//		if (keychain)
		//		{
		//			std::string nodeAddr = "0.0.0.0";

		//			if (!vm.count(cli::NODE_ADDR))
		//			{
		//				LOG_ERROR() << "Please, provide node address!";
		//			}
		//			else
		//			{
		//				nodeAddr = vm[cli::NODE_ADDR].as<string>();
		//			}

		//			qmlRegisterType<PeerAddressItem>("AddressBook", 1, 0, "PeerAddressItem");
		//			qmlRegisterType<OwnAddressItem>("AddressBook", 1, 0, "OwnAddressItem");

  //                  IKeyStore::Options options;
  //                  options.flags = IKeyStore::Options::local_file | IKeyStore::Options::enable_all_keys;
  //                  options.fileName = bbsStorage;

  //                  IKeyStore::Ptr keystore = IKeyStore::create(options, walletPass.c_str(), walletPass.size());

		//			WalletModel model(keychain, keystore, nodeAddr);

		//			model.start();

		//			struct ViewModel
		//			{
		//				MainViewModel			main;
		//				DashboardViewModel		dashboard;
		//				WalletViewModel			wallet;
  //                      AddressBookViewModel    addressBook;
		//				NotificationsViewModel	notifications;
		//				HelpViewModel			help;
		//				SettingsViewModel		settings;

		//				ViewModel(WalletModel& model)
		//					: wallet(model)
  //                          , addressBook(model)
  //                      {
  //                      }

		//			} viewModel(model);

		//			Translator translator;

		//			QQuickView view;
		//			view.setResizeMode(QQuickView::SizeRootObjectToView);

		//			QQmlContext *ctxt = view.rootContext();

  //                  // TODO: try move instantiation of view models to views
		//			ctxt->setContextProperty("mainViewModel", &viewModel.main);

		//			ctxt->setContextProperty("walletViewModel", &viewModel.wallet);
  //                  ctxt->setContextProperty("addressBookViewModel", &viewModel.addressBook);

		//			ctxt->setContextProperty("translator", &translator);

		//			view.setSource(QUrl("qrc:///main.qml"));
		//			view.show();

		//			return app.exec();
		//		}
		//		else
		//		{
		//			QMessageBox::critical(0, "Error", "Invalid password or wallet data unreadable.\nRestore wallet.db from latest backup or delete it and reinitialize the wallet.", QMessageBox::Ok);

		//			LOG_ERROR() << "Wallet data unreadable, restore wallet.db from latest backup or delete it and reinitialize the wallet.";
		//			return -1;
		//		}
		//	}

		}
		catch (const po::error& e)
		{
			LOG_ERROR() << e.what();
			return -1;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return -1;
	}
}
