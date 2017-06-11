#pragma once
#include "Dissimilarity.h"

/*!
* \enum DistanceRule
*
* \brief 样本间距离计算规则
*
* \author neross
* \date 六月 2017
*/
typedef enum DistanceRule
{
	EuclideanDistance = 0,		// 欧氏距离
	ManhattanDistance,			// 曼哈顿距离
	MinkowskiDistance,			// 闵可夫斯基距离
	ChebyshevDistance			// 切比雪夫距离
};

class GeometryDistance :
	public Dissimilarity
{
public:
	GeometryDistance();
	~GeometryDistance();

	virtual float calculateDissimilarity(Sample sample, Sample center);

	//************************************
	// Method:    setRule
	// FullName:  GeometryDistance::setRule
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: DistanceRule rule
	// Parameter: int p 当且仅当rule为MinkowskiDistance时，p有效
	//************************************
	void setRule(DistanceRule rule, int p = 2);

public:
	DistanceRule m_rule;
	int m_p;
};

