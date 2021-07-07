#include <QtWidgets/QApplication>
#include <qwidget.h>
#include "main.h"
#include <fstream>
#include <qbytearray.h>
#include <qshortcut.h>
#include <qfiledialog.h>
#include <qdiriterator.h>
#include "QConsole.h"

#define TESTING

//sQWidget* g_poRmv2 = nullptr;

int BatchTest()
{
	// --- make a file name based on date and time
	time_t rawtime;

	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);

	strftime(buffer, sizeof(buffer), "%d_%m_%Y_%H_%M_%S", &timeinfo);
	std::string strTime(buffer);

	string strFíleName = "RMV2_Log_" + strTime + ".txt";

	g_qstrLogFileName = strFíleName.c_str();

	//QCoreApplication a();

	//QProcess* pqProcess = new QProcess();

	//pqProcess->setArguments({ QString::fromStdWString(LR"(F:\docs\rigid_model_v2\WH2 Models\brt_grail_knights_body_01.rigid_model_v2)") });
	//pqProcess->, ,setProgram(R"(C:\Coding\QtTest_QtRMV2Widget\x64\Debug\@QtTestRMV2.exe)");

	//QString qstrInPath = R"(e:\modding\wh2\vanilla\)";
	QString qstrInPath = R"(E:\modding\Atilla_vanilla\)";

	QStringList filter = { "*.rigid_model_v2" };
	QDirIterator it(qstrInPath, filter, QDir::Files, QDirIterator::Subdirectories);

	_log_action("Starting");

	std::vector<std::wstring> vecFile;
	while (it.hasNext())
	{
		QString qstrPath = it.next();

		//wstring strPathIn = qstrInPath.toStdWString();

		//// make relative path
		//wstring wstrRelativePath = qstrPath.toStdWString();
		//wstrRelativePath.erase(wstrRelativePath.begin(), wstrRelativePath.begin() + strPathIn.size());

		vecFile.push_back(qstrPath.toStdWString());
	}

	size_t start_file_count = vecFile.size();
	size_t read_filse = 0;
	size_t file_errors = 0;

	std::cout << "Qt Version: " << QT_VERSION_STR << std::endl;

	int exit_code = -1;
	for (size_t i = 0; i < vecFile.size(); i++)
	{
		_log_action("-------------------------------------------------------");
		_log_action("Opening File:" + QString::fromStdWString(vecFile[i]).toStdString());
		_log_action("Creating importer...");
		auto spoImporter = RigidModelV2::File_Importer_Common::create(vecFile[i]);

		if (!spoImporter->isValid())
		{
			_log_action("Importer returned 'invalid'");
			//	_log_action("Last Error String: " + spoImporter->m_strLastErrorString);

			file_errors++;
		}

		//_log_action("Exit Code: " + std::to_string(exit_code));
		_log_action("-------------------------------------------------------");
		_log_action("File Read Count: " + std::to_string(i));
		_log_action("File Error Count: " + std::to_string(file_errors));
	}

	float error_rate = ((float)file_errors) / ((float)vecFile.size());

	_log_action("-------------------------------------------------------");
	_log_action("Processing done!");
	_log_action("-------------------------------------------------------");

	_log_action("Files attempted read: " + std::to_string(vecFile.size()));
	_log_action("File read errors: " + std::to_string(file_errors));
	_log_action("Error %: " + to_string(error_rate * 100.f) + " %");

	return exit_code;
	//	return a.exec();
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	//_create_console(NULL);
#if 0
	BatchTest();
#endif

	auto  g_poRmv2 = createRMV2Widget(nullptr);
	g_poRmv2->show();

	QShortcut* qpShortcut_OpenFile = new QShortcut(QKeySequence(QRMV2Widget::tr("Ctrl+O", "File|Open")),
		g_poRmv2);

	QShortcut* qpShortcut_SaveFile = new QShortcut(QKeySequence(QRMV2Widget::tr("Ctrl+S", "File|Save")),
		g_poRmv2);

	QObject::connect(qpShortcut_OpenFile, &QShortcut::activated, [g_poRmv2]()
		{
			QString qstrPath = QFileDialog::getOpenFileName(g_poRmv2, "Open RMV2file", QString(),
				"TW RigidModelV2 Files (*.rigid_model_v2);;All Files (*.*)"
			);
			if (qstrPath.isEmpty())
			{
				return;
			}

			QByteArray bytes;
			std::ifstream inFile(qstrPath.toStdWString(), std::ios::binary);
			char byte = 0;
			while (!inFile.eof())
			{
				inFile.read(&byte, 1);
				bytes.append(byte);
			}

			bool result = setRMV2Data(g_poRmv2, &bytes);

			if (!result)
			{
			}

			g_poRmv2->update();
		}
	);

	QObject::connect(qpShortcut_SaveFile, &QShortcut::activated, [g_poRmv2]()
		{
			QString qstrPath = QFileDialog::getSaveFileName(g_poRmv2, "Save RMV2file", QString(),
				"TW RigidModelV2 Files (*.rigid_model_v2);;All Files (*.*)"
			);
			if (qstrPath.isEmpty())
			{
				return;
			}

			QByteArray qBuffer;
			bool result = getRMV2Data(g_poRmv2, &qBuffer);

			ofstream fileOut(qstrPath.toStdWString(), ios::binary);
			fileOut.write(qBuffer.data(), qBuffer.size());
			fileOut.close();
		}
	);

	return a.exec();

	//#endif
}