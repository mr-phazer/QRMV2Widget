#include "mesh_structs.h"
#include <algorithm>
//#include "..\D3D11_RENDER_LIB\vector_math.h"
//#include "..\QtRME_GUI\QConsole.h"
#include <DirectXPackedVector.h>

//const RMV2_Vertex_Raw_Cinematic& RMV2_Vertex_Common::getCinematicRaw()
//{
//	//auto& o = oCinematicRaw;
//
//
//	//o.pos_x = position.x;
//	//o.pos_y = position.y;
//	//o.pos_z = position.z;
//	//o.pos_w = 1.0f;
//
//	//o.u = textcords.x;
//	//o.v = textcords.y; // maybe this; maybe not
//	//
//	//normal = vector_math::vector3(normal).normalize();
//	//o.norm_x = ((float)normal.x + 1.0f) * 127.5f;
//	//o.norm_y = ((float)normal.y + 1.0f) * 127.5f;
//	//o.norm_z = ((float)normal.z + 1.0f) * 127.5f;
//	//o.norm_w = 0;
//
//	////tangent = lib3d::normalize3(tangent);
//	//o.tangent_x = ((float)tangent.x + 1.0f) * 127.5f;
//	//o.tangent_y = ((float)tangent.y + 1.0f) * 127.5f;
//	//o.tangent_z = ((float)tangent.z + 1.0f) * 127.5f;
//	//o.tangent_w = 0;
//
//	////bitangent = lib3d::normalize3(bitangent);
//	//o.bitangent_x = ((float)bitangent.x + 1.0f) * 127.5f;
//	//o.bitangent_y = ((float)bitangent.y + 1.0f) * 127.5f;
//	//o.bitangent_z = ((float)bitangent.z + 1.0f) * 127.5f;
//	//o.bitangent_w = 0;
//
//
//	//
//	//if (vecInfluences.size())
//	//{
//
//	//	// sort so the influence with higher weight is first
//	//	std::sort(vecInfluences.begin(), vecInfluences.end(),
//	//		[](const std::pair<int, float>& left, const std::pair<int, float>& right) {
//
//	//			return left.second > right.second;
//	//		});
//
//
//	//	if (vecInfluences.size() > 4)
//	//	{
//	//		// exclude the weights ver 4
//	//		for (size_t i = 0; i < vecInfluences.size() - 4; i++)
//	//			vecInfluences.pop_back();
//
//
//	//		// normalize the rest
//	//		float len = 0;
//	//		len =
//	//			vecInfluences[0].second +
//	//			vecInfluences[1].second +
//	//			vecInfluences[2].second +
//	//			vecInfluences[3].second;
//	//
//
//	//		vecInfluences[0].second = vecInfluences[0].second / len;
//	//		vecInfluences[1].second = vecInfluences[1].second / len;
//	//		vecInfluences[2].second = vecInfluences[2].second / len;
//	//		vecInfluences[3].second = vecInfluences[3].second / len;
//
//
//	//		int a = 0;
//
//	//	}
//
//	//	// fill bone ids
//	//
//
//
//	//}
//	//else
//	//	_log_action_warning("vertex has no weights!");
//	//
//	//
//	//o.weight1 = (float)vecInfluences.size() >= 1 ? (float)vecInfluences[0].second * 255.0f : 1.0;
//	//o.weight2 = (float)vecInfluences.size() >= 2 ? (float)vecInfluences[1].second * 255.0f : 0.0;
//	//o.weight3 = (float)vecInfluences.size() >= 3 ? (float)vecInfluences[2].second * 255.0f : 0.0;
//	//o.weight4 = (float)vecInfluences.size() == 4 ? (float)vecInfluences[3].second * 255.0f : 0.0;
//
//	//o.joint_id1 = (float)vecInfluences.size() >= 1 ? (float)vecInfluences[0].first : 0;
//	//o.joint_id2 = (float)vecInfluences.size() >= 2 ? (float)vecInfluences[1].first : vecInfluences[0].first;
//	//o.joint_id3 = (float)vecInfluences.size() >= 3 ? (float)vecInfluences[2].first : vecInfluences[0].first;
//	//o.joint_id4 = (float)vecInfluences.size() == 4 ? (float)vecInfluences[3].first : vecInfluences[0].first;
//
//
//
//	return oCinematicRaw;
//
//}

//const RMV2_Vertex_Raw_Cinematic& RMV2_Vertex_Common::getCinematicRaw2(vector<pair<int, float>>& _vecInfluences)
//{
//
//	auto& o = oCinematicRaw;
//
//
//	o.pos_x = position.x;
//	o.pos_y = position.y;
//	o.pos_z = position.z;
//	o.pos_w = 1.0f;
//
//	o.u = textcords.x;
//	o.v = textcords.y; // maybe this; maybe not
//
//	/*o.joint_id1 = joint_id1;
//	o.joint_id2 = joint_id2;
//	o.joint_id3 = joint_id3;
//	o.joint_id4 = joint_id4;*/
//
//
//	//delete thé {-1 , 0.0 } init influence
//	vector<pair<int, float>>::iterator it = _vecInfluences.begin();
//	while (it != _vecInfluences.end())
//	{
//		if (it->first == -1)
//		{
//			_vecInfluences.erase(it);
//			break;
//		}
//		else
//			++it;
//	}
//
//	float sum = 0;
//	for (auto it : _vecInfluences)
//		sum += it.second;
//
//	bool bStoreWeights = true;
//
//	if (sum < 0.1) {
//		_log_action_warning("Weighed Format: Vertex 0.0 total weighting!!");
//		bStoreWeights = false;
//	}
//
//	else if (_vecInfluences.size() == 0)
//	{
//		// _log_action_warning("Weighed Format: Vertex has no weighting!");
//		bStoreWeights = false;
//	}
//
//	if (bStoreWeights)
//	{
//
//		// sort so the influence with higher weight is first
//		std::sort(_vecInfluences.begin(), _vecInfluences.end(),
//			[](const std::pair<int, float>& left, const std::pair<int, float>& right) {
//
//			return left.second > right.second;
//		});
//
//		// only 4 influences allowed, remove the extra ones, and normalize the rest
//		if (_vecInfluences.size() > 4)
//		{
//			_vecInfluences.erase(_vecInfluences.begin() + 4, _vecInfluences.end());
//		}
//
//		// normalize
//		float sum = 0;
//		for (auto it : _vecInfluences)
//			sum += it.second;
//
//		float factor = 0;
//		if (sum < 1.0)
//		{
//			factor = 1.0f / sum; // factor to get sum to 1.0
//
//
//			for (auto& it : _vecInfluences)
//				it.second /= sum;
//
//
//			float sum2 = 0;
//			for (auto it : _vecInfluences)
//				sum2 += it.second;
//
//
//			int a = 0;
//		}
//
//		/*float total = 0;
//		if (_vecInfluences.size() > 2)
//		{
//			for (auto it : _vecInfluences)
//				total += it.second;
//
//
//				int a = 0;
//		}*/
//
//
//		if (_vecInfluences.size())
//		{
//			o.joint_id1 = _vecInfluences[0].first;
//			o.joint_id2 = _vecInfluences.size() > 1 ? _vecInfluences[1].first : o.joint_id1;
//			o.joint_id3 = _vecInfluences.size() > 2 ? _vecInfluences[2].first : o.joint_id1;
//			o.joint_id4 = _vecInfluences.size() > 3 ? _vecInfluences[3].first : o.joint_id1;
//
//
//			o.weight1 = _vecInfluences[0].second * 255.0f;;
//			o.weight2 = (_vecInfluences.size() > 1 ? _vecInfluences[1].second : 0.0) * 255.0f;;
//			o.weight3 = (_vecInfluences.size() > 2 ? _vecInfluences[2].second : 0.0) * 255.0f;;;
//			o.weight4 = (_vecInfluences.size() > 3 ? _vecInfluences[3].second : 0.0) * 255.0f;;;
//
//
//			//// if total is not 1.0, set weight4 = 1- total
//			//float total = (vertexWeight1 + vertexWeight2 + vertexWeight3 + vertexWeight4);
//			//if (total < 0.7)
//			//{
//			//	//	vertexWeight4 = 1 - total;
//			//	//	int a = 1;
//			//}
//		}
//		//else
//
//		// DDEBUG CODE
//		//////////////////////////////
//		/*if (false)
//		{
//			vertexWeight1 = 1.0f;
//			vertexWeight2 = 0;
//			vertexWeight3 = 0;
//			vertexWeight4 = 0;
//
//
//			o.joint_id1 = 0;
//			o.joint_id2 = 0;
//			o.joint_id3 = 0;
//			o.joint_id4 = 0;
//		}*/
//		////////////////////////////
//
//		/*o.weight1 = ((float)this->vertexWeight1) * 255.0f;
//		o.weight2 = ((float)this->vertexWeight2) * 255.0f;
//		o.weight3 = ((float)this->vertexWeight3) * 255.0f;
//		o.weight4 = ((float)this->vertexWeight4) * 255.0f;*/
//
//
//		//normal = lib3d::normalize3(normal);
//	//float test = vector_math::vector3(1,2,3).normalize().length();
//
//	}
//
//
//
//
//	//normal = vector_math::vector3(normal).normalize();
//	o.norm_x = ((float)normal.x + 1.0f) * 127.f;
//	o.norm_y = ((float)normal.y + 1.0f) * 127.f;
//	o.norm_z = ((float)normal.z + 1.0f) * 127.f;
//	o.norm_w = 0;
//
//	//tangent = lib3d::normalize3(tangent);
//	o.tangent_x = ((float)tangent.x + 1.0f) * 127.f;
//	o.tangent_y = ((float)tangent.y + 1.0f) * 127.f;
//	o.tangent_z = ((float)tangent.z + 1.0f) * 127.f;
//	o.tangent_w = 0;
//
//	//bitangent = lib3d::normalize3(bitangent);
//	o.bitangent_x = ((float)bitangent.x + 1.0f) * 127.f;
//	o.bitangent_y = ((float)bitangent.y + 1.0f) * 127.f;
//	o.bitangent_z = ((float)bitangent.z + 1.0f) * 127.f;
//	o.bitangent_w = 0;
//
//
//
//	return oCinematicRaw;
//
//
//
//
//
////
////
////	auto& o = oCinematicRaw;
////
////	o.pos_x = position.x;
////	o.pos_y = position.y;
////	o.pos_z = position.z;
////	o.pos_w = 1.0f;
////
////	o.u = textcords.x;
////	o.v = textcords.y; // maybe this; maybe not
////
////	normal = vector_math::vector3(normal).normalize();
////	o.norm_x = ((float)normal.x + 1.0f) * 127.5f;
////	o.norm_y = ((float)normal.y + 1.0f) * 127.5f;
////	o.norm_z = ((float)normal.z + 1.0f) * 127.5f;
////	o.norm_w = 0;
////
////	//tangent = lib3d::normalize3(tangent);
////	o.tangent_x = ((float)tangent.x + 1.0f) * 127.5f;
////	o.tangent_y = ((float)tangent.y + 1.0f) * 127.5f;
////	o.tangent_z = ((float)tangent.z + 1.0f) * 127.5f;
////	o.tangent_w = 0;
////
////	//bitangent = lib3d::normalize3(bitangent);
////	o.bitangent_x = ((float)bitangent.x + 1.0f) * 127.5f;
////	o.bitangent_y = ((float)bitangent.y + 1.0f) * 127.5f;
////	o.bitangent_z = ((float)bitangent.z + 1.0f) * 127.5f;
////	o.bitangent_w = 0;
////
////
////	//if (false)
////	if (_vecInfluences.size())
////	{
////
////		// sort so the influence with higher weight is first
////		std::sort(_vecInfluences.begin(), __vecInfluences.end(),
////			[](const std::pair<int, float>& left, const std::pair<int, float>& right) {
////
////				return left.second > right.second;
////			});
////
////
////		if (__vecInfluences.size() > 4)
////		{
////			// exclude the weights ver 4
////			for (size_t i = 0; i < __vecInfluences.size() - 4; i++)
////				__vecInfluences.pop_back();
////
////
////			// normalize the rest
////			float len = 0;
////			len =
////				__vecInfluences[0].second +
////				__vecInfluences[1].second +
////				__vecInfluences[2].second +
////				__vecInfluences[3].second;
////
////
////			__vecInfluences[0].second = __vecInfluences[0].second / len;
////			__vecInfluences[1].second = __vecInfluences[1].second / len;
////			__vecInfluences[2].second = __vecInfluences[2].second / len;
////			__vecInfluences[3].second = __vecInfluences[3].second / len;
////
////
////			int a = 0;
////
////		}
////
////		// fill bone ids
////
////
////
////	}
////	else
////		// _log_action_warning("vertex has no weights!");
////
////
////	o.joint_id1 = __vecInfluences[0].first;
////	o.joint_id2 = __vecInfluences.size() > 1 ? __vecInfluences[1].first : o.joint_id1;
////	o.joint_id3 = __vecInfluences.size() > 2 ? __vecInfluences[2].first : o.joint_id1;
////	o.joint_id4 = __vecInfluences.size() > 3 ? __vecInfluences[3].first : o.joint_id1;
////
////
////	o.weight1 = __vecInfluences[0].second * 255;
////	o.weight2 = (__vecInfluences.size() > 1 ? __vecInfluences[1].second : 0.0) * 255.0;
////	o.weight3 = (__vecInfluences.size() > 2 ? __vecInfluences[2].second : 0.0) * 255.0;;
////	o.weight4 = (__vecInfluences.size() > 3 ? __vecInfluences[3].second : 0.0) * 255.0;;
////
////
////
////	/*o.weight1 = (float)__vecInfluences.size() == 1 ? (float)__vecInfluences[0].second * 255.0f : 1.0;
////	o.weight2 = (float)__vecInfluences.size() == 2 ? (float)__vecInfluences[1].second * 255.0f : 0.0;
////	o.weight3 = (float)__vecInfluences.size() == 3 ? (float)__vecInfluences[2].second * 255.0f : 0.0;
////	o.weight4 = (float)__vecInfluences.size() == 4 ? (float)__vecInfluences[3].second * 255.0f : 0.0;
////
////	o.joint_id1 = (float)__vecInfluences.size() == 1 ? (float)__vecInfluences[0].first : 0;
////	o.joint_id2 = (float)__vecInfluences.size() == 2 ? (float)__vecInfluences[1].first : __vecInfluences[0].first;
////	o.joint_id3 = (float)__vecInfluences.size() == 3 ? (float)__vecInfluences[2].first : __vecInfluences[0].first;
////	o.joint_id4 = (float)__vecInfluences.size() == 4 ? (float)__vecInfluences[3].first : __vecInfluences[0].first;
////*/
////
////
////	return oCinematicRaw;;
//}

RMV2_Vertex_Raw_Weighted& RMV2_Vertex_Common_RAW::getWeighted()
{
	auto& o = oWeightedRawVertex;

	o.pos_x = pos_x;
	o.pos_y = pos_y;
	o.pos_z = pos_z;
	o.pos_w = 1.0f;

	o.u = this->u;
	o.v = this->v; // maybe this; maybe not

	o.joint_id1 = joint_id1;
	o.joint_id2 = joint_id2;

	float w1 = ((float)weight1) / 255.0f;
	float w2 = ((float)weight2) / 255.0f;

	if ((w1 + w2) < 1.0f)
	{
		float factor = 1.0 / (w1 + w2);

		w1 *= factor;
		w2 *= factor;
	}

	o.weight1 = w1 * 255.0f;
	o.weight2 = w2 * 255.0f;

	//normal = lib3d::normalize3(normal);
	o.normal_x = this->normal_x;
	o.normal_y = this->normal_y;
	o.normal_z = this->normal_z;
	o.normal_w = this->normal_w;;

	//tangent = lib3d::normalize3(tangent);
	o.tangent_x = tangent_x;
	o.tangent_y = tangent_y;
	o.tangent_z = tangent_z;
	o.tangent_w = tangent_w;

	//bitangent = lib3d::normalize3(bitangent);
	o.bitangent_x = bitangent_x;
	o.bitangent_y = bitangent_y;
	o.bitangent_z = bitangent_z;
	o.bitangent_w = bitangent_w;

	return o;
}

RMV2_Vertex_Raw_Default		RMV2_Vertex_Common_RAW::oDefaultRawVertex;
RMV2_Vertex_Raw_Cinematic	RMV2_Vertex_Common_RAW::oCinematicRawVertex;
RMV2_Vertex_Raw_Weighted	RMV2_Vertex_Common_RAW::oWeightedRawVertex;

RMV2_Vertex_Common_RAW		RMV2_Vertex_Common::oCommonRaw;

RMV2_Vertex_Raw_Default& RMV2_Vertex_Common_RAW::getDefaultRaw()
{
	auto& o = oDefaultRawVertex;

	float x = this->pos_x;
	float y = this->pos_y;
	float z = this->pos_z;

	o.pos_x = x;
	o.pos_y = y;
	o.pos_z = z;
	o.pos_w = 1;

	o.normal_x = this->normal_x;
	o.normal_y = this->normal_y;
	o.normal_z = this->normal_z;
	o.normal_w = this->normal_w;

	// ------------------------------------------------------------
	// TANGENT
	// ------------------------------------------------------------
	/*XMFLOAT3 T;
	T.x = lib3d::unorm8_to_float(this->tangent_x);
	T.y = lib3d::unorm8_to_float(this->tangent_y);
	T.z = lib3d::unorm8_to_float(this->tangent_z);*/

	o.tangent_x = this->bitangent_x;
	o.tangent_y = this->bitangent_y;
	o.tangent_z = this->bitangent_z;
	o.tangent_w = 0;

	/*o.tangent_x = lib3d::float_to_unorm8(T.x);
	o.tangent_y = lib3d::float_to_unorm8(T.y);
	o.tangent_z = lib3d::float_to_unorm8(T.z);
	o.tangent_w = this->tangent_w;*/

	// ------------------------------------------------------------
	// BITANGENT
	// ------------------------------------------------------------
	o.bitangent_x = this->tangent_x;
	o.bitangent_y = this->tangent_y;
	o.bitangent_z = this->tangent_z;
	o.bitangent_w = 0;

	/*XMFLOAT3 B;
	B.x = lib3d::unorm8_to_float(this->bitangent_x);
	B.y = lib3d::unorm8_to_float(this->bitangent_y);
	B.z = lib3d::unorm8_to_float(this->bitangent_z);

	o.bitangent_x = lib3d::float_to_unorm8(B.x);
	o.bitangent_y = lib3d::float_to_unorm8(B.y);
	o.bitangent_z = lib3d::float_to_unorm8(B.z);
	o.bitangent_w = this->tangent_w;
	*/

	o.u = this->u;
	o.v = this->v;

	o.textcord_u2 = 0;
	o.textcord_v2 = 0;

	o.color = this->color;

	return o;
}

RMV2_Vertex_Raw_Cinematic& RMV2_Vertex_Common_RAW::getCinematicRaw()
{
	auto& o = oCinematicRawVertex;

	o.pos_x = this->pos_x;
	o.pos_y = this->pos_y;
	o.pos_z = this->pos_z;
	o.pos_w = 1.0f;

	o.norm_x = this->normal_x;
	o.norm_y = this->normal_y;
	o.norm_z = this->normal_z;
	o.norm_w = 1.0f;

	o.bitangent_x = this->bitangent_x;
	o.bitangent_y = this->bitangent_y;
	o.bitangent_z = this->bitangent_z;
	o.bitangent_w = this->bitangent_w;

	o.tangent_x = this->tangent_x;
	o.tangent_y = this->tangent_y;
	o.tangent_z = this->tangent_z;
	o.tangent_w = this->tangent_w;

	o.u = this->u;
	o.v = this->v;

	o.joint_id1 = this->joint_id1;
	o.joint_id2 = this->joint_id2;
	o.joint_id3 = this->joint_id3;
	o.joint_id4 = this->joint_id4;

	o.weight1 = this->weight1;
	o.weight2 = this->weight2;
	o.weight3 = this->weight3;
	o.weight4 = this->weight4;

	o.color = this->color;

	return o;
}

RMV2_Vertex_Common_RAW& RMV2_Vertex_Common::getCommonRaw()
{
	auto& o = oCommonRaw;

	o.pos_x = position.x;
	o.pos_y = position.y;
	o.pos_z = position.z;
	o.pos_w = 1.0f;

	o.u = textcords.x;
	o.v = textcords.y; // maybe this; maybe not

	o.joint_id1 = PBone_Ids[0];
	o.joint_id2 = PBone_Ids[1];
	o.joint_id3 = PBone_Ids[2];
	o.joint_id4 = PBone_Ids[3];

	o.color = { 0, 0, 0, 0 };

	//delete thé {-1 , 0.0 } init influence
	/*vector<pair<int, float>>::iterator it = vecInfluences.begin();
	while (it != vecInfluences.end())
	{
		if (it->first == -1)
		{
			vecInfluences.erase(it);
			break;
		}
		else
			++it;
	}*/

	float sum = 0;
	for (auto it : vecInfluences)
		sum += it.second;

	bool bStoreWeights = true;

	if (sum < 0.1) {
		null_weight_error_count++;

		if (null_weight_error_count < 20)
			// _log_action_warning("Weighed Format: Vertex 0.0 total weighting!!");

			bStoreWeights = false;
	}

	else if (vecInfluences.size() == 0)
	{
		// _log_action_warning("Weighed Format: Vertex has no weighting!");
		bStoreWeights = false;
	}

	if (vecInfluences.size() > 0)
		if (bStoreWeights)
		{
			// sort so the influence with higher weight is first
			std::sort(vecInfluences.begin(), vecInfluences.end(),
				[](const std::pair<int, float>& left, const std::pair<int, float>& right) {
					return left.second > right.second;
				});

			// only 4 influences allowed, remove the extra ones, and normalize the rest
			const int size_allowed = 4;
			if (vecInfluences.size() > size_allowed)
			{
				/*for (size_t i = 4; i < vecInfluences.size(); i++)
					vecInfluences[3].second += vecInfluences[i].second;*/

				vecInfluences.erase(vecInfluences.begin() + size_allowed, vecInfluences.end());

				/*float test_sum = 0;
				for (auto it : vecInfluences)
					test_sum += it.second;

				int a = 0;*/
			}

			//if (false) // DEBUGGIN CODE
			{
				// normalize
				float sum = 0;
				for (auto it : vecInfluences)
					sum += it.second;

				float factor = 0;
				if (sum < 1.00)
				{
					float missin = (1 - sum);
					vecInfluences[vecInfluences.size() - 1].second += missin;

					//// _log_action_warning("Weight sum: " + to_string(sum));

					////factor = 1.0f / sum; // factor to get sum to 1.0

					//// (x + y) * a = 1
					////for (auto& it : vecInfluences)
					////	it.second *= factor;

					float sum2 = 0;
					for (auto it : vecInfluences)
						sum2 += it.second;

					//////// _log_action_warning("corrected to: " + to_string(sum2));

					int a = 0;
				}
			}
			/*float total = 0;
			if (vecInfluences.size() > 2)
			{
				for (auto it : vecInfluences)
					total += it.second;

					int a = 0;
			}*/

			if (vecInfluences.size())
			{
				o.joint_id1 = vecInfluences[0].first;
				o.joint_id2 = vecInfluences.size() > 1 ? vecInfluences[1].first : o.joint_id1;
				o.joint_id3 = vecInfluences.size() > 2 ? vecInfluences[2].first : o.joint_id1;
				o.joint_id4 = vecInfluences.size() > 3 ? vecInfluences[3].first : o.joint_id1;

				PBone_Ids[0] = vecInfluences[0].first;
				PBone_Ids[1] = vecInfluences.size() > 1 ? vecInfluences[1].first : o.joint_id1;
				PBone_Ids[2] = vecInfluences.size() > 2 ? vecInfluences[2].first : o.joint_id1;
				PBone_Ids[3] = vecInfluences.size() > 3 ? vecInfluences[3].first : o.joint_id1;

				PVertex_Weights[0] = vecInfluences[0].second;
				PVertex_Weights[1] = vecInfluences.size() > 1 ? vecInfluences[1].second : 0.0;
				PVertex_Weights[2] = vecInfluences.size() > 2 ? vecInfluences[2].second : 0.0;
				PVertex_Weights[3] = vecInfluences.size() > 3 ? vecInfluences[3].second : 0.0;

				//// if total is not 1.0, set weight4 = 1- total
				//float total = (vertexWeight1 + vertexWeight2 + vertexWeight3 + vertexWeight4);
				//if (total < 0.7)
				//{
				//	//	vertexWeight4 = 1 - total;
				//	//	int a = 1;
				//}
			}
			//else

			unsigned int w1 = min<unsigned int>(255, (unsigned int)(((float)this->PVertex_Weights[0]) * 255.0f));
			unsigned int w2 = min<unsigned int>(255, (unsigned int)(((float)this->PVertex_Weights[1]) * 255.0f));
			unsigned int w3 = min<unsigned int>(255, (unsigned int)(((float)this->PVertex_Weights[2]) * 255.0f));
			unsigned int w4 = min<unsigned int>(255, (unsigned int)(((float)this->PVertex_Weights[3]) * 255.0f));

			o.weight1 = w1;
			o.weight2 = w2;
			o.weight3 = w3;
			o.weight4 = w4;

			int DEBUG_a = 1;

			//normal = lib3d::normalize3(normal);
		//float test = vector_math::vector3(1,2,3).normalize().length();
		}

	// DDEBUG CODE	//////////////////////////////
	if (false)
	{
		PVertex_Weights[0] = 1.0f;
		PVertex_Weights[1] = 0;
		PVertex_Weights[2] = 0;
		PVertex_Weights[3] = 0;

		PBone_Ids[0] = 0;
		PBone_Ids[1] = 0;
		PBone_Ids[2] = 0;
		PBone_Ids[3] = 0;

		o.weight1 = ((float)this->PVertex_Weights[0]) * 255.0f;
		o.weight2 = ((float)this->PVertex_Weights[1]) * 255.0f;
		o.weight3 = ((float)this->PVertex_Weights[2]) * 255.0f;
		o.weight4 = ((float)this->PVertex_Weights[3]) * 255.0f;

		o.joint_id1 = 0;
		o.joint_id2 = 0;
		o.joint_id3 = 0;
		o.joint_id4 = 0;
	}
	// END: DEBUG CODE /////////////////////////

	//normal = vector_math::vector3(normal).normalize();
	/*o.norm_x = ((float)normal.x + 1.0f) * 127.f;
	o.norm_y = ((float)normal.y + 1.0f) * 127.f;
	o.norm_z = ((float)normal.z + 1.0f) * 127.f;
	*/
	normal = lib3d::normalize3(normal);
	o.normal_x = lib3d::float_to_unorm8(normal.x);
	o.normal_y = lib3d::float_to_unorm8(normal.y);
	o.normal_z = lib3d::float_to_unorm8(normal.z);
	o.normal_w = 0;

	tangent = lib3d::normalize3(tangent);
	o.tangent_x = lib3d::float_to_unorm8(tangent.x);
	o.tangent_y = lib3d::float_to_unorm8(tangent.y);
	o.tangent_z = lib3d::float_to_unorm8(tangent.z);
	o.tangent_w = 0;

	bitangent = lib3d::normalize3(bitangent);
	o.bitangent_x = lib3d::float_to_unorm8(bitangent.x);
	o.bitangent_y = lib3d::float_to_unorm8(bitangent.y);
	o.bitangent_z = lib3d::float_to_unorm8(bitangent.z);
	o.bitangent_w = 0;

	return o;
}

size_t RMV2_Vertex_Common::null_weight_error_count = 0;