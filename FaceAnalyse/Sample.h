#pragma once
#include <vector>

using namespace std;

/*!
* \class Sample
*
* \brief 样本数据结构定义
*
* \author neross
* \date 六月 2017
*/
typedef struct Sample{
	int cluster;			// 所属簇
	vector<float> data;		// 样本数据
	float min_dis;			// 样本与最近质心的距离

	bool operator!= (Sample other)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i] != other.data[i] )
				return true;
		}
		return false;
	}
	void zero() { for (float& var : data){ var = 0.0; } };
	
}Sample;


