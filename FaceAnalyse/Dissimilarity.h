#pragma once

#include "Sample.h"


class Dissimilarity
{
public:
	Dissimilarity();
	virtual ~Dissimilarity();

	//************************************
	// Method:    calculateDissimilarity 计算样本与质心的差异度
	// FullName:  MyKMeans::calculateDistance
	// Access:    protected 
	// Returns:   float
	// Qualifier:
	// Parameter: Sample sample
	// Parameter: Sample center
	//************************************
	virtual float calculateDissimilarity(Sample sample, Sample center) = 0;
};

