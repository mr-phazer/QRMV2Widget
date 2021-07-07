#include "QConsole.h"
//#include "constants.h"
#include <QtWidgets/qdesktopwidget.h>
#include <QtWidgets/qapplication.h>
#include <fstream>

QConsole* g_poConsole = nullptr;

QString g_qstrLogFileName = "log.txt";

QConsole::QConsole(QWidget* parent) :
	QPlainTextEdit::QPlainTextEdit(nullptr)
{
	setWindowTitle("Log Window");

	std::ofstream oOutFile;

	QString qstrLogFileName = "log.txt";

	oOutFile.open(g_qstrLogFileName.toStdWString(), std::ios::app);

	std::string strDate = __DATE__ + std::string(" : ");
	strDate += __TIME__;

	oOutFile << "-----------------------------------------------" << std::endl;
	oOutFile << "'Qt RME' Log" << std::endl;
	oOutFile << strDate << std::endl;
	oOutFile << "-----------------------------------------------" << std::endl;

	oOutFile.close();
	/*setWindowFlag(Qt::Popup);
	setWindowFlag(Qt::Tool);
	setWindowFlag(Qt::PopupFocusReason);*/

	setStyleSheet("QPlainTextEdit {background-color: black; color: white;}");
	/*setStyleSheet("QPlainTextEdit {"
		"font: 14px console;"
		"background-color: black;"
		"color: white;"
		";}"
	);*/

	QSize size;

	QRect rec = QApplication::desktop()->screenGeometry();
	int height = rec.height() / 2;
	int width = rec.width() / 2;

	setMinimumSize(width, height);

	//appendHtml("****** RMEditor 0.9.1a Log Log ******");
	//toHtml();

	show();
}

void QConsole::write(const std::string& _str, const std::string& _color, const std::string& _bg, size_t size)
{
	//    std::string strTemp =
	//     /*   "<font size = \""+
	//        std::to_string(size) +  "\"" +
	//     */   "<p style = \"color: " +
	//        _color +
	//        "; background-color: " +
	//        _bg +
	//        "\" >"+
	//
	//        _str +
	//
	//        "</p>";

		//std::string strTemp =
		//
		//           "<p style = \"color: " +
		//           _color +
		//           "; background-color: " +
		//           _bg +
		//
		//            "\" >" +
		//
		//           _str +
		//
		//           "</p>";    //std::string strTemp =

		////   "<font size = \"" +
		//   std::to_string(size) + "\"" +
		//   "style = \"color: " +
		//   _color +
		//   "; background-color: " +
		//   _bg +
		//   "\" >" +
		//   _str +
		//   "</font>";

		//appendHtml("<p style=\"color:white;white-space:pre\">" + QString("testing") + "</p>");

	std::string strTemp =
		"<font size = \"" +
		std::to_string(size) + "\"" +
		"style = \"color: " +
		_color +
		"; background-color: " +
		_bg +
		"\" >" +
		_str +
		"</font>";

	moveCursor(QTextCursor::End);
	this->textCursor().insertHtml(strTemp.c_str());
	//this->textCursor().insertHtml("<p style=\"color:white;white-space:pre\">" + QString("testing") + "</p>");
	moveCursor(QTextCursor::End);

	//oOutFile.open(g_qstrLogFileName.toStdWString(), std::ios::app);
	//oOutFile << _str;
	//oOutFile.close();
}

void QConsole::writeLn(const std::string& _str, const std::string& _color, const std::string& _bg, size_t size)
{
	std::string strTemp =
		"<font size = \"" +
		std::to_string(size) + "\"" +
		"style = \"color: " +
		_color +
		"; background-color: " +
		_bg +
		"\" >" +
		_str +
		"</font>";

	//std::string strTemp =
	//    /*   "<font size = \""+
	//       std::to_string(size) +  "\"" +
	//    */   "<p style = \"color: " +
	//    _color +
	//    "; background-color: " +
	//    _bg +
	//    "\" >" +
	//    _str +
	//    "</p>";

	appendHtml(strTemp.c_str());
}

void _create_console(QWidget* parent)
{
	g_poConsole = new QConsole(parent);
}

void _log_action2(const std::string& _strMsg, bool _bSuccess)
{
	g_poConsole->writeLn();
	g_poConsole->write("ACTION:", "white", "blue", 4);

	g_poConsole->write("&#160;", "white", "black", 4);
	g_poConsole->write("&#160;", "white", "black", 4);

	g_poConsole->write((_strMsg).c_str(), "white", "black", 4);

	if (_bSuccess)
	{
		g_poConsole->write("&#160;", "white", "black", 4);
		g_poConsole->write("&#160;", "white", "black", 4);
		g_poConsole->write("Success.", "white", "green", 4);
	}
	else
	{
		g_poConsole->write("&#160;", "white", "black", 4);
		g_poConsole->write("&#160;", "white", "black", 4);
		g_poConsole->write("Error!", "white", "red", 4);
	}
}

void _impl__log_action(const std::string& _strMsg)
{
	/*std::ofstream logFile("log.txt", std::ios::app);;
	logFile << "ACTION: ";
	logFile << _strMsg.c_str();
	logFile << std::endl;*/

#if 0
	g_poConsole->writeLn();
	g_poConsole->write("ACTION:", "white", "blue", 4);

	g_poConsole->write("&#160;", "white", "black", 4);
	g_poConsole->write("&#160;", "white", "black", 4);

	g_poConsole->write((_strMsg).c_str(), "white", "black", 4);
#endif
	
	std::ofstream oOutFile(g_qstrLogFileName.toStdWString(), std::ios::app);
	oOutFile << std::endl << "ACTION: " << _strMsg;
	oOutFile.close();
}

void _impl__log_action_success(const std::string& _strMsg)
{
	/*std::ofstream logFile("log.txt", std::ios::app);;
	logFile << "SUCCESS: ";
	logFile << _strMsg.c_str();
	logFile << std::endl;*/

	/*g_poConsole->writeLn();
	g_poConsole->write("ACTION:", "white", "blue", 4);

	g_poConsole->write("&#160;", "white", "black", 4);
	g_poConsole->write("&#160;", "white", "black", 4);
	g_poConsole->write((_strMsg).c_str(), "white", "black", 4);

	g_poConsole->write("&#160;", "white", "black", 4);
	g_poConsole->write("&#160;", "white", "black", 4);
	g_poConsole->write("Success.", "white", "green", 4);*/

	std::ofstream oOutFile(g_qstrLogFileName.toStdWString(), std::ios::app);
	oOutFile << std::endl << "ACTION: " << _strMsg << ". Success.";
	oOutFile.close();
}

bool _impl__log_action_error(const std::string& _strMsg)
{
	/*std::ofstream logFile("log.txt", std::ios::app);;
	logFile << "Error: ";
	logFile << _strMsg.c_str();
	logFile << std::endl;*/

	//g_poConsole->writeLn();
	//g_poConsole->write("ERROR:", "white", "red", 4);

	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write("&#160;", "white", "black", 4);

	//g_poConsole->write((_strMsg).c_str(), "white", "black", 4);

	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write("Error!", "white", "red", 4);

	std::ofstream oOutFile(g_qstrLogFileName.toStdWString(), std::ios::app);
	oOutFile << std::endl << "ERROR!: " << _strMsg;
	oOutFile.close();

	return false;
}

bool _impl__log_action_warning(const std::string& _strMsg)
{
	//g_poConsole->writeLn();
	//g_poConsole->write("Warning:", "white", "purple", 4);

	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write((_strMsg).c_str(), "white", "black", 4);

	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write("&#160;", "white", "black", 4);
	//g_poConsole->write("Success.", "white", "green", 4);

	std::ofstream oOutFile(g_qstrLogFileName.toStdWString(), std::ios::app);
	oOutFile << std::endl << "Warning: " << _strMsg;
	oOutFile.close();

	//g_poConsole->writeLn();
	//g_poConsole->write("ACTION:", "white", "blue", 4);
	//g_poConsole->write("&#160;");
	//g_poConsole->write((_strMsg + " - ").c_str(), "red", "white", 4);

	//g_poConsole->write("Warning!", "purple", "white", 4);

	return false;
}

void _impl__log_write(const std::string& _strMsg)
{
	//	g_poConsole->write(_strMsg, "black", "gray", 4);
	//	g_poConsole->writeLn();
}