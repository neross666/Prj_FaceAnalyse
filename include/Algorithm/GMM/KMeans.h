/***************************************************************************
Module Name:
	KMeans

History:
	2003/10/16	Fei Wang
	2013 luxiaoxun
***************************************************************************/

#pragma once
#include <fstream>

class KMeans
{
public:
	enum InitMode
	{
		InitRandom,
		InitManual,
		InitUniform,
	};

	enum DissimilarityMode
	{
		EuclideanDistance,	// 欧式距离
		ManhattanDistance,	// 曼哈顿距离
		MinkowskiDistance	// 闵可夫斯基距离
	};

	KMeans(int dimNum = 1, int clusterNum = 1);
	~KMeans();

	void SetMean(int i, const double* u)			{ memcpy(m_means[i], u, sizeof(double) * m_dimNum); }
	void SetInitMode(InitMode i)					{ m_initMode = i; }
	void SetDissimilarityMode(DissimilarityMode i)	{ m_dissimilarityMode = i; }
	void SetMaxIterNum(int i)						{ m_maxIterNum = i; }
	void SetEndError(double f)						{ m_endError = f; }

	double* GetMean(int i)							{ return m_means[i]; }
	InitMode GetInitMode()							{ return m_initMode; }
	DissimilarityMode GetDissimilarityMode()		{ return m_dissimilarityMode; }
	int GetMaxIterNum()								{ return m_maxIterNum; }
	double GetEndError()							{ return m_endError; }
	
	/*
	选取初始质心
	data:样本数据
	sample_size:样本数量
	*/
	void Init(double *data, int sample_size);

	/*
	迭代聚类
	data:样本数据
	sample_size:样本数量
	Label:每个样本所属的簇
	*/
	void Cluster(double *data, int sample_size, int *Label);

	/*	SampleFile: <size><dim><data>...
	LabelFile:	<size><label>...
	*/
	void Init(std::ifstream& sampleFile);
	void Cluster(const char* sampleFileName, const char* labelFileName);
	
	friend std::ostream& operator<<(std::ostream& out, KMeans& kmeans);

private:
	int m_dimNum;			// 数据维数
	int m_clusterNum;		// 簇数
	double** m_means;		// 每簇中所有样本的均值

	InitMode m_initMode;						// 质心选择方式
	DissimilarityMode m_dissimilarityMode;		// 相异度选择方式
	int m_maxIterNum;							// 最大迭代次数
	double m_endError;							// 误差阈值

	double GetLabel(const double* x, int* label);						// 获取样本x所属的簇
	double CalcDistance(const double* x, const double* u, int dimNum);	// 计算样本x与样本u的欧氏距离
};
