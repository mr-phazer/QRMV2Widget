#pragma once

//#define CONSOLE_OUT

#include <QtWidgets\qplaintextedit.h>

class QConsole :
	public QPlainTextEdit
{
	Q_OBJECT
public:
	QConsole(QWidget* parent);

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

extern void _create_console(QWidget* parent);
extern void impl__log_action2(const std::string& _strMsg, bool _bSuccess);

#ifdef CONSOLE_OUT

#define _log_action(_MSG) impl__log_action(_MSG);

#else

#define _log_action_error(msg) ;

#endif
extern void impl__log_action(const std::string& _strMsg);

extern void _log_action_success(const std::string& _strMsg);

#ifdef CONSOLE_OUT

#define _log_action_error(msg) impl__log_action_error(msg);
#else

#define _log_action(msg) ;

#endif
extern bool impl__log_action_error(const std::string& _strMsg);

extern bool _log_action_warning(const std::string& _strMsg);
extern void _log_write(const std::string& _strMsg);

extern QConsole* g_poConsole;