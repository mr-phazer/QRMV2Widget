



#include <QtWidgets/QApplication>
#include <qwidget.h>
#include "main.h"
#include <fstream>
#include <qbytearray.h>
#include <qshortcut.h>
#include <qfiledialog.h>



//sQWidget* g_poRmv2 = nullptr;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);


    auto  g_poRmv2 = createRMV2Widget(nullptr);

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

            setRMV2Data(g_poRmv2, &bytes);

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


    g_poRmv2->show();



    return a.exec();
}
