#include "QRMV2WidgetModule.h"

QWidget* createRMV2Widget(QWidget* parent)
{
	return new QRMV2Widget(parent);
}

bool setRMV2Data(QWidget* dest, const QByteArray* sourceBuffer)
{
	QRMV2Widget* po = dynamic_cast<QRMV2Widget*>(dest);

	if (!po)
	{
		return false;
	}

	// copy the buffer to inside the widget object
	po->m_qtByteArray = *sourceBuffer; // TODO is the needed when the the data is stored in CommonmFile?

	//SystemLib::DataStream stream;
	//stream.open_from_memory((void*)sourceBuffer->data(), sourceBuffer->size());

	//ImporterLib::RigidModel::RigidModelFileCommon* poRMV2_File = nullptr;

	//ImporterLib::RigidModel::createRidigModFile(stream, &poRMV2_File);

	// parse rigidmodel from the inter ByteArray
	auto spoImporter = RigidModelV2::File_Importer_Common::create((void*)po->m_qtByteArray.data(), po->m_qtByteArray.size());

	if (!spoImporter)
	{
		return false;
	}

	if (spoImporter->getFile())
	{
		po->setFile_New(spoImporter->getFile());
		return true;
	}

	return false;
}

bool getRMV2Data(QWidget* source, QByteArray* destBuffer)
{
	QRMV2Widget* po = dynamic_cast<QRMV2Widget*>(source);

	if (!po)
	{
		*destBuffer = po->m_qtByteArray;
		return false;
	}

	//SystemLib::DataStream oDataStream;
	//po->m_poFile->write(oDataStream);

	auto Exporter = RigidModelV2::File_Exporter_Common::create();

	if (!Exporter->setFile(po->m_poRMV2FileCommon))
	{
		*destBuffer = po->m_qtByteArray;
		return false;
	}

	destBuffer->clear();
	destBuffer->resize(Exporter->m_spoStream->m_vecBuffer.size());

	memcpy(destBuffer->data(), Exporter->m_spoStream->m_vecBuffer.data(), destBuffer->size());

	return true;
}