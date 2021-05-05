#pragma once

#include <vector>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;


std::vector<DirectX::XMFLOAT3>  sm_vecCubeVertices =  {

	{-0.3, -0.3, -0.3}, 
	{0.3, -0.3, -0.3}, 
	{0.3, 0.3, -0.3}, 
	{-0.3, 0.3, -0.3},
	{-0.3, -0.3, 0.3}, 
	{0.3, -0.3, 0.3 }, 
	{0.3, 0.3, 0.3}, 
	{-0.3, 0.3, 0.3}
};

std::vector<uint16_t> sm_vecCubeIndices =
{ 0, 1,  1, 2,  2, 3,  3,0,   // side 1

4, 5,  5, 6,  6, 7,  7,4,   // side 1


0, 4,   1,5,   2,6,  3, 7 };  // connecting lines