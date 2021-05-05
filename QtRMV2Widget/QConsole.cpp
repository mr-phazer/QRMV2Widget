#include "QConsole.h"
#include "constants.h"
#include <qdesktopwidget.h>
#include <qapplication.h>

QConsole* g_poConsole = nullptr;

QConsole::QConsole() :
    QPlainTextEdit::QPlainTextEdit()
{	
    setWindowTitle("Log Window");
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
        
    appendHtml("****** RMEditor 0.9.1a Log Log ******");
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

void _create_console()
{
    g_poConsole = new QConsole;
}

void _log_action(const std::string& _strMsg, bool _bSuccess)
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

void _log_action(const std::string& _strMsg)
{
    g_poConsole->writeLn();
    g_poConsole->write("ACTION:", "white", "blue", 4);

    g_poConsole->write("&#160;", "white", "black", 4);    
    g_poConsole->write("&#160;", "white", "black", 4);    

    g_poConsole->write( (_strMsg).c_str(), "white", "black", 4);       

   


}

void _log_action_success(const std::string& _strMsg)
{
    g_poConsole->writeLn();
    g_poConsole->write("ACTION:", "white", "blue", 4);

    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write((_strMsg).c_str(), "white", "black", 4);

    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write("Success.", "white", "green", 4);
}

bool _log_action_error(const std::string& _strMsg)
{
    g_poConsole->writeLn();
    g_poConsole->write("ACTION:", "white", "blue", 4);

    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write("&#160;", "white", "black", 4);

    g_poConsole->write((_strMsg).c_str(), "white", "black", 4);

    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write("&#160;", "white", "black", 4);
    g_poConsole->write("Error!", "white", "red", 4);

    return false;
}

bool _log_action_warning(const std::string& _strMsg)
{
    g_poConsole->writeLn();
    g_poConsole->write("ACTION:", "white", "blue", 4);
    g_poConsole->write("&#160;");
    g_poConsole->write((_strMsg + " - ").c_str(), "red", "white", 4);

    g_poConsole->write("Warning!", "purple", "white", 4);

    return false;
}

void _log_write(const std::string& _strMsg)
{
 
    
    g_poConsole->write(_strMsg, "black", "gray", 4);
    g_poConsole->writeLn();

}
