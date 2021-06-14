#include <QtCore/QCoreApplication>
#include <qprocess.h>
#include <QDirIterator>
#include <iostream>
#include "..\ImporterLib\RigidModelV2_Reader.h"
#include "..\QtTestRMV2\QConsole.h"

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	//QProcess* pqProcess = new QProcess();

	//pqProcess->setArguments({ QString::fromStdWString(LR"(F:\docs\rigid_model_v2\WH2 Models\brt_grail_knights_body_01.rigid_model_v2)") });
	//pqProcess->, ,setProgram(R"(C:\Coding\QtTest_QtRMV2Widget\x64\Debug\@QtTestRMV2.exe)");

	QString qstrInPath = R"(e:\modding\wh2\vanilla\)";

	QStringList filter = { "*.rigid_model_v2" };
	QDirIterator it(qstrInPath, filter, QDir::Files, QDirIterator::Subdirectories);

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
		auto spoImporter = RigidModelV2::File_Importer_Common::create(vecFile[i]);

		_log_action("Validator String: " +
			spoImporter->oValidDator.strLastErrorString);

		/*int exit_code = QProcess::execute(R"(C:\Coding\QtTest_QtRMV2Widget\x64\Debug\@QtTestRMV2.exe)",

			{ QString::fromStdString(vecFile[i]) });*/

			/*int exit_code = QProcess::execute(R"(C:\Coding\QtTest_QtRMV2Widget\x64\Debug\@QtTestRMV2.exe)",
				{ QString::fromStdWString(LR"(C:\Coding\QtTest_QtRMV2Widget\x64\Debug\@QtTestRMV2.exe)") });*/

				//pqProcess->start();

				//while (pqProcess->exitStatus() != QProcess::NormalExit)
				//	;

				//int ret = pqProcess->exitCode();

		if (exit_code == 1000)
		{
			read_filse++;
		}
		else
		{
			file_errors++;
		}
	}
	return a.exec();
}