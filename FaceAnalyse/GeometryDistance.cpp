#include "GeometryDistance.h"


GeometryDistance::GeometryDistance() : m_rule(EuclideanDistance), m_p(2)
{
}


GeometryDistance::~GeometryDistance()
{
}

float GeometryDistance::calculateDissimilarity(Sample sample, Sample center)
{
	float sum = 0.0;
	switch (m_rule)
	{
	case EuclideanDistance:
		for (size_t i = 0; i < sample.data.size(); i++)
		{
			sum += pow(sample.data[i] - center.data[i], 2);
		}
		sum = sqrt(sum);
		break;
	case ManhattanDistance:
		for (size_t i = 0; i < sample.data.size(); i++)
		{
			sum += fabs(sample.data[i] - center.data[i]);
		}
		break;
	case ChebyshevDistance:
	case MinkowskiDistance:
		for (size_t i = 0; i < sample.data.size(); i++)
		{
			sum += pow(fabs(sample.data[i] - center.data[i]), m_p);
		}
		sum = pow(sum, 1.0 / m_p);
		break;
	default:
		break;
	}

	return sum;
}

void GeometryDistance::setRule(DistanceRule rule, int p)
{
	m_rule = rule;
	switch (m_rule)
	{
	case EuclideanDistance:
		m_p = 2;
		break;
	case ManhattanDistance:
		m_p = 1;
		break;
	case MinkowskiDistance:
		m_p = p;
		break;
	case ChebyshevDistance:
		m_p = INT_MAX;
		break;
	default:
		break;
	}
}
