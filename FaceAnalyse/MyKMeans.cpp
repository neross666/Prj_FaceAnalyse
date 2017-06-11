#include "MyKMeans.h"
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include<algorithm>
#include <numeric>
#include "Config/Config.h"

#define MIN_ERR 0.001
#define ITER_TIMES 100

MyKMeans::MyKMeans() : m_intiRule(InitRand)
{
	m_dissimilarityRule = new GeometryDistance();
}


MyKMeans::~MyKMeans()
{
}

void MyKMeans::kmeans(vector<Sample>& samples, vector<Sample>& centers, int k)
{
	m_k = k;

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
				float distance = m_dissimilarityRule->calculateDissimilarity(sample, center);
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
	case InitRand:
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
	case InitKmeansPlusPlus:
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
					float distance = m_dissimilarityRule->calculateDissimilarity(sample, center);
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
	case InitUniform:
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
		sum_err += m_dissimilarityRule->calculateDissimilarity(_old[i], _new[i]);
	}

	return (sum_err < MIN_ERR);
}

float MyKMeans::Silhouette(vector<Sample> Samples)
{
	float s = 0.0;									// 样本集的轮廓系数
	vector<float> S(Samples.size());				// 各个样本的轮廓系数
		
	for (size_t i = 0; i < Samples.size(); i++)
	{
		float a = 0.0;								// 样本i到簇内其他样本的距离的平均值，即样本i的内聚度
		float b = 0.0;								// 样本i到其他簇距离的平均值的最小值，即样本i的分离度
		vector<float> B(m_k, 0);					// 记录样本i到各簇样本的距离之和
		vector<float> C(m_k, 0);					// 记录各簇的样本数量
		for (Sample other : Samples)
		{
			B[other.cluster] += m_dissimilarityRule->calculateDissimilarity(Samples[i], other);
			C[other.cluster]++;
		}
		a = B[Samples[i].cluster] / C[Samples[i].cluster];
		
		float min_b = FLT_MAX;						// 样本i到其他簇距离之和的最小值
		int min_c;									// min_b对应簇的样本数量
		for (size_t j = 0; j < B.size(); j++)
		{
			if (j != Samples[i].cluster && min_b > B[j])
			{
				min_b = B[j];
				min_c = j;
			}
		}
		b = min_b / C[min_c];

		S[i] = (b - a)/max(a,b);
	}
		
	s = std::accumulate(std::begin(S), std::end(S), 0.0) / S.size();

	return s;
}

void MyKMeans::Normalize(vector<Sample>& Samples)
{
	assert(Samples.size() > 1);
	vector<float> min_elems(Samples[0].data.size(), FLT_MAX);	// 样本集每一维度的最小值
	vector<float> max_elems(Samples[0].data.size(), FLT_MIN);	// 样本集每一维度的最大值
	
	std::for_each(Samples.begin(), Samples.end(), [&](Sample& sample){
		int i = 0;
		std::for_each(sample.data.begin(), sample.data.end(), [&](float& var){
			if (min_elems[i] > var)
				min_elems[i] = var;
			if (max_elems[i] < var)
				max_elems[i] = var;
			i++;
		});
	});
		
	std::for_each(Samples.begin(), Samples.end(), [&](Sample& sample){
		int i = 0;
		std::for_each(sample.data.begin(), sample.data.end(), [&](float& var){
			var = (var - min_elems[i]) / (max_elems[i] - min_elems[i]);
			i++;
		});
	});
}
