#pragma once

#define CONSOLE_OUT

#include <QtWidgets\QPlainTextEdit>
#include <QtCore\QString>

#include <fstream>

extern QString g_qstrLogFileName;

class QConsole :
	public QPlainTextEdit
{
	Q_OBJECT
public:
	QConsole(QWidget* parent);
	~QConsole()
	{
		oOutFile.close();
	}

	void write(
		const std::string& _str = "",
		const std::string& _color = "black",
		const std::string& _bg = "white",
		size_t size = 5);

	void writeLn(
		const std::string& _str = "",
		const std::string& _color = "black",
		const std::string& _bg = "white",
		size_t size = 5);

	std::ofstream oOutFile;
};

extern void _create_console(QWidget* parent);
extern void _log_action2(const std::string& _strMsg, bool _bSuccess);

#ifdef CONSOLE_OUT

#define _log_action_error(msg) _impl__log_action_error(msg);
#define _log_action(_MSG) _impl__log_action(_MSG);
#define _log_action_success(_MSG) _impl__log_action_success(_MSG);
#define _log_action_warning(_MSG) _impl__log_action_warning(_MSG);

#define _log_write(_MSG) _impl__log_write(_MSG);
//#define _log_action_success(_MSG) _impl_log_action_success(_MSG);

#else
#define _log_action_error(msg) ;
#define _log_action(_MSG) ;
#define _log_action_success(_MSG) ;
#define _log_action_warning(_MSG) ;

#endif

extern void _impl__log_action(const std::string& _strMsg);

extern void _impl__log_action_success(const std::string& _strMsg);

extern bool _impl__log_action_error(const std::string& _strMsg);

extern bool _impl__log_action_warning(const std::string& _strMsg);
extern void _impl__log_write(const std::string& _strMsg);

extern QConsole* g_poConsole;