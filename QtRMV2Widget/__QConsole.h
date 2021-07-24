#pragma once

#include <qplaintextedit.h>

class QConsole :
	public QPlainTextEdit
{
	Q_OBJECT
public:
	QConsole();

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
};

extern void _create_console();
extern void _log_action(const std::string& _strMsg, bool _bSuccess);

#define _log_action(msg) ;
#define _log_error(msg) ;

extern void __log_action(const std::string& _strMsg);

extern void // _log_action_success(const std::string& _strMsg);
extern bool _// _log_action_error(const std::string& _strMsg);
extern bool _log_action_warning(const std::string& _strMsg);
extern void _log_write(const std::string& _strMsg);

extern QConsole* g_poConsole;