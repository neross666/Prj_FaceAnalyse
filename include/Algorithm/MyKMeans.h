/*!
 * \file MyKMeans.h
 *
 * \author neross
 * \date 五月 2017
 * \describe kmeans聚类算法实现
 * 
 */


#pragma once
#include <vector>

using namespace std;

#define RAND_GENERATOR  ((float)rand()/RAND_MAX)

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

	void zero() { for(float& var : data){ var = 0.0; } };
}Sample;


/*!
 * \enum InitRule
 *
 * \brief 初始质心选择规则
 *
 * \author neross
 * \date 六月 2017
 */
typedef enum InitRule
{
	RAND = 0,		// 分段随机
	KMEANS_PLUS		// kmeans++
};

/*!
 * \enum CalculateRule
 *
 * \brief 样本间距离计算规则
 *
 * \author neross
 * \date 六月 2017
 */
typedef enum CalculateRule
{
	Euclidean = 0		// 欧氏距离
};

/*!
 * \class MyKMeans
 *
 * \brief kmeans聚类算法类，可调整参数包括初始质心选择的规则、距离计算规则以及聚类的数目
 *
 * \author neross
 * \date 五月 2017
 */
class MyKMeans
{
public:
	MyKMeans();
	~MyKMeans();

	//************************************
	// Method:    kmeans
	// FullName:  MyKMeans::kmeans
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<Sample> & Samples 样本集，输入样本数据，输出样本所属的簇，以及与最近质心的距离
	// Parameter: vector<Sample> & centers 质心集，输出聚类结果后的质心
	// Parameter: int k 聚类的数目
	//************************************
	void kmeans(vector<Sample>& Samples, vector<Sample>& centers, int k);

	//************************************
	// Method:    setInitRule
	// FullName:  MyKMeans::setInitRule
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: InitRule rule 初始质心选择的规则
	//************************************
	void setInitRule(InitRule rule){ m_intiRule = rule; }

	//************************************
	// Method:    setCalculateRule
	// FullName:  MyKMeans::setCalculateRule
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: CalculateRule rule 距离计算规则
	//************************************
	void setCalculateRule(CalculateRule rule){ m_calculateRule = rule; }
	
protected:
	
	//************************************
	// Method:    initialCenter
	// FullName:  MyKMeans::initialCenter
	// Access:    protected 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<Sample> Samples
	// Parameter: vector<Sample> & centers
	// Parameter: int k
	//************************************
	void initialCenter(vector<Sample> Samples, vector<Sample>& centers, int k);

	
	//************************************
	// Method:    calculateDistance 计算样本与质心的距离
	// FullName:  MyKMeans::calculateDistance
	// Access:    protected 
	// Returns:   float
	// Qualifier:
	// Parameter: Sample sample
	// Parameter: Sample center
	//************************************
	float calculateDistance(Sample sample, Sample center);

	
	void classify(Sample& sample, vector<Sample> centers);
	
	
	//************************************
	// Method:    calculateCenter 使用样本数据计算下一轮迭代的质心
	// FullName:  MyKMeans::calculateCenter
	// Access:    protected 
	// Returns:   void
	// Qualifier:
	// Parameter: vector<Sample> Samples
	// Parameter: vector<Sample> & centers
	//************************************
	void calculateCenter(vector<Sample> Samples, vector<Sample>& centers);

	
	//************************************
	// Method:    compareCenter 比对新旧质心
	// FullName:  MyKMeans::compareCenter
	// Access:    protected 
	// Returns:   bool
	// Qualifier:
	// Parameter: vector<Sample> _new 新质心
	// Parameter: vector<Sample> _old 上一轮迭代的质心
	//************************************
	bool compareCenter(vector<Sample> _new, vector<Sample> _old);

private:
	InitRule m_intiRule;
	CalculateRule m_calculateRule;
};

