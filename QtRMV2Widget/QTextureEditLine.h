#pragma once

#include <QWidget>
#include <qcombobox.h>

#include <QMenu.h>
#include "ui_QTextureEditLine.h"
#include "..\ImporterLib\IRigidModelmporter.h"
#include "..\ImporterLib\RigidModelV2_Enums.h"

class QTextureEditLine : public QWidget, public Ui::QTextureEditLine
{
	Q_OBJECT

public:
	QTextureEditLine(QWidget* parent, std::string* _strTexturePath, ETextureType* _pETextureType);

	~QTextureEditLine();

public:
	//void bindToData(std::string* _strTexturePath, ETextureType* _pETextureType);

	std::pair<std::string*, ETextureType*> getTexture();

	void setTexture(std::pair<std::string*, ETextureType*>);

protected:
	void makeConnection();
	//protected:
public:
	std::string* m_pstrTexturePath = nullptr;
	uint32_t* pdwTexturetype = nullptr;
	ETextureType* m_pETextureType = nullptr;

private slots:
	void onTextEditingFinshed();
};

//typedef int vec3;
//
//using namespace std;
//void getTranslationLimit()
//{
//
//	vec3 max_tranlation_vector = 0;
//	vec3 min_tranlation_vector = 0;
//	TWAnimation Animation;
//	FileList FileListAnimations;
//
//	FileListAnimations.getAllFileWithSkeleton("humanoid01");
//
//	for (size_t file = 0; file < FileListAnimations.size(); file++)
//	{
//
//		FileListAnimations[file].loadAnimation(Animation);
//		for (size_t bone = 0; bo-ne < Animation.getBoneCount(); bone++)
//		{
//			vec3 bindpose_translation_vector = Animation.getBindPose().getBone(bone).translation;
//
//			vec3 max_translation_distance = 0;
//			vec3 min_translation_distance = 0;
//
//			for (size_t frame = 0; frame < Animation.getFrameCount(); frame++)
//			{
//				// set new max
//				vec3 current_translation_vector = Animation.getFrame(frame).getBone(bone).translation;
//
//				//operator >() is mesassuring (signed) scalar distance between vector
//				if ((current_translation_vector - bindpose_translation_vector) > max_tranlation_vector)
//					max_tranlation_vector = current_translation_vector;
//
//
//				// set new min
//				if ((current_translation_vector - bindpose_translation_vector) < min_tranlation_vector)
//					min_tranlation_vector = current_translation_vector;
//
//
//			}
//		}
//	}
//}
