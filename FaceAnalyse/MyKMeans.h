/*!
 * \file MyKMeans.h
 *
 * \author neross
 * \date 五月 2017
 * \describe kmeans聚类算法实现
 * 
 */


#pragma once
#include "GeometryDistance.h"


#define RAND_GENERATOR  ((float)rand()/RAND_MAX)


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
	InitRand = 0,			// 分段随机
	InitKmeansPlusPlus,		// kmeans++
	InitUniform,			// 长度等分点
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
	// Method:    setDissimilarityRule
	// FullName:  MyKMeans::setDissimilarityRule
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Dissimilarity rule 相异度计算规则
	//************************************
	void setDissimilarityRule(Dissimilarity* rule){ m_dissimilarityRule = rule; }
	Dissimilarity* getDissimilarityRule(){ return m_dissimilarityRule; };

	//************************************
	// Method:    Silhouette
	// FullName:  MyKMeans::Silhouette
	// Access:    public 
	// Returns:   float 样本集的轮廓系数
	// Qualifier:
	// Parameter: vector<Sample> Samples
	//************************************
	float Silhouette(vector<Sample> Samples);

	void Normalize(vector<Sample>& Samples);
	
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
	int m_k;
	InitRule m_intiRule;
	Dissimilarity* m_dissimilarityRule;
};

