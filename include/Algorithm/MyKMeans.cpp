#include "MyKMeans.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include<algorithm>
#include "Config/Config.h"

#define MIN_ERR 0.001
#define ITER_TIMES 100

MyKMeans::MyKMeans() : m_intiRule(RAND), m_calculateRule(Euclidean)
{
}


MyKMeans::~MyKMeans()
{
}

void MyKMeans::kmeans(vector<Sample>& samples, vector<Sample>& centers, int k)
{
	initialCenter(samples, centers, k);

	int iter = 0;
	while (iter < ITER_TIMES)
	{
		iter++;
		for (Sample &sample : samples)
		{
			sample.min_dis = FLT_MAX;
			for (Sample center : centers)
			{
				float distance = calculateDistance(sample, center);
				if (distance < sample.min_dis)
				{
					sample.min_dis = distance;
					sample.cluster = center.cluster;
				}
			}
		}

		vector<Sample> old_centers = centers;
		calculateCenter(samples, centers);
		if (compareCenter(old_centers, centers))
		{
			break;
		}
	}
	spdlog::get(LOGGER)->info(iter);
}

void MyKMeans::initialCenter(vector<Sample> samples, vector<Sample>& centers, int k)
{
	assert(samples.size() > k);

	centers.resize(0);

	switch (m_intiRule)
	{
	case RAND:
	{
		int inteval = samples.size() / k;
		srand((unsigned)time(nullptr));
		for (int i = 0; i < k; i++)
		{
			int _k = i*inteval + rand() % inteval;
			samples[_k].cluster = i;
			centers.push_back(samples[_k]);
		}
	}
		break;
	case KMEANS_PLUS:
	{
		srand((unsigned)time(nullptr));
		int k0 = rand() % samples.size();
		samples[k0].cluster = 0;
		centers.push_back(samples[k0]);

		for (size_t i = 1; i < k; i++)
		{
			for (auto &sample : samples)
			{
				sample.min_dis = FLT_MAX;
				for (auto center : centers)
				{
					float distance = calculateDistance(sample, center);
					if (distance < sample.min_dis)
					{
						sample.min_dis = distance;
					}
				}
			}

			float sum = 0.0;
			for (auto sample : samples)
			{
				sum += sample.min_dis;
			}
			float c = sum*RAND_GENERATOR;

			for (auto sample : samples)
			{
				c -= sample.min_dis;
				if (c <= 0)
				{
					sample.cluster = i;
					centers.push_back(sample);
					break;
				}
			}
		}
	}
		break;
	default:
	{
		int inteval = samples.size() / k;
		for (int i = 0; i < k; i++)
		{
			int _k = i*inteval;
			samples[_k].cluster = i;
			centers.push_back(samples[_k]);
		}
	}
		break;
	}
}

float MyKMeans::calculateDistance(Sample sample, Sample center)
{
	float sum = 0.0;
	switch (m_calculateRule)
	{
	case Euclidean:
	{
					  for (int i = 0; i < sample.data.size(); i++)
					  {
						  sum += pow(sample.data[i] - center.data[i], 2);
					  }
	}
		break;
	default:
		break;
	}

	return sqrt(sum);
}

void MyKMeans::classify(Sample& sample, vector<Sample> centers)
{

}

void MyKMeans::calculateCenter(vector<Sample> Samples, vector<Sample>& centers)
{
	vector<int> counts;
	for (Sample &center : centers)
	{
		center.zero();
		counts.push_back(0);
	}

	for (Sample sample : Samples)
	{
		for (int j = 0; j < centers.size(); j++)
		{
			if (sample.cluster == centers[j].cluster)
			{
				for (int i = 0; i < sample.data.size(); i++)
				{
					centers[j].data[i] += sample.data[i];
				}
				counts[j]++;
				break;
			}
		}
	}

	for (int j = 0; j < centers.size(); j++)
	{
		for (float& var : centers[j].data)
		{
			var = var / counts[j];
		}
	}
}

bool MyKMeans::compareCenter(vector<Sample> _new, vector<Sample> _old)
{
	assert(_new.size() == _old.size());

	float sum_err = 0.0;
	for (size_t i = 0; i < _new.size(); i++)
	{
		sum_err += calculateDistance(_old[i], _new[i]);
	}

	return (sum_err < MIN_ERR);
}
