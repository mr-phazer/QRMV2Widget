#pragma once

//class QRMV2Widget;
//class QWidget;
//class QByteArray;
//class QString;

#include "QtRMV2Widget.h"

// Creates widget, return NULL on error, parent has to be supplied for Qt to clean up resources
// only EVER used when a NEW widget it created
extern "C" QWidget * createRMV2Widget(QWidget * parent);

/// <summary>
/// Fills the RMV2 data in the widget
/// </summary>
/// <param name="dest">An existing QRMV2Widget</param>
/// <param name="sourceBuffer">Bufer container RMV2 Data</param>
/// <returns>false if parsen went wrong</returns>
extern "C" bool setRMV2Data(QWidget * dest, const QByteArray * sourceBuffer); // used to set data in the widget

/// <summary>
/// Get raw RMV2 file data from the widget
/// </summary>
/// <param name="source">The QMV2Widget pointer</param>
/// <param name="destBuffer">byte array with raw RMV2 Data</param>
/// <returns>false if getting of data had errors </returns>
extern "C"  bool getRMV2Data(QWidget * source, QByteArray * destBuffer); // used to get data from the widget

//class ErrorMessagges
//{
//
//	static extern map<QWidget*, QString> g_mapPerWidgetErrorStrings;
//	static QString sm_qstrLastLibraryErrorString;
//};

extern "C"  bool getLastErrorString(QWidget * source, QString * str); // used to get data from the widget
