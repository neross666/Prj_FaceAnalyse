#include <QLabel>
#include <QPainter>
#include <vector>
#include <iostream>
#include "FaceAnalyse.h"
#include "GeneratedFiles/ui_FaceAnalyse.h"
#include "Plugin/PluginInstance.h"
#include "Plugin/PluginManager.h"
#include "Config/Config.h"


using namespace std;

#pragma execution_character_set("utf-8")


#define PI 3.1415926


void get_random_pos(float center_x, float center_y, float radius, float&x, float& y)
{
	float u = sqrt(RAND_GENERATOR)*radius;
	float v = RAND_GENERATOR * 2 * PI;

	x = center_x + u*cos(v);
	y = center_y + u*sin(v);
}


FaceAnalyse::FaceAnalyse(QWidget *parent) :
QDialog(parent)
{
	ui = new Ui::FaceAnalyse;
	ui->setupUi(this);

	//ui->label->setText(tr("hello Qt!"));

	connect(ui->pushButton_sample, &QPushButton::clicked, this, &FaceAnalyse::GenerateSample);
	connect(ui->pushButton_kmeans, &QPushButton::clicked, this, &FaceAnalyse::KMeansTest);

	m_k = 8;
	makeColors(m_k);

	QStringList init_rule, calculate_rule;
	init_rule << tr("分段随机") << tr("Kmeans++");		ui->comboBox_initCenter->addItems(init_rule);
	calculate_rule << tr("欧氏距离");					ui->comboBox_disCalculate->addItems(calculate_rule);
		
	spdlog::get(LOGGER)->info("construct.");
}

void FaceAnalyse::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	
	PrintSamples(m_samples);
}

FaceAnalyse::~FaceAnalyse()
{
	PluginManager::GetInstance().UnLoad("PluginShape");// UnLoad之后必须停止所有插件的引用，否则造成空指针异常。
}


void FaceAnalyse::GenerateSample()
{
	srand((unsigned)time(nullptr));

	int sample_dim = 2;
	int sample_count = 1000;
	for (int i = 0; i < sample_count; i++)
	{
		Sample sample;
		sample.cluster = 0;
		///////////////////////////////////随机生成一个矩形///////////////////////////////////////
// 		for (int j = 0; j < sample_dim; j++)
// 		{
// 			float var = rand() % 100;
// 			sample.data.push_back(var);
// 		}
// 		m_samples.push_back(sample);

		///////////////////////////////////随机生成一个圆形///////////////////////////////////////
		float x, y;
		get_random_pos(120, 120, 80, x, y);
		sample.data.push_back(x);
		sample.data.push_back(y);
		m_samples.push_back(sample);
	}
	update();
}


void FaceAnalyse::KMeansTest()
{
	MyKMeans myKMeans;
	m_k = ui->lineEdit_k->text().toInt();
	myKMeans.setInitRule((InitRule)ui->comboBox_initCenter->currentIndex());
	myKMeans.setCalculateRule((CalculateRule)ui->comboBox_disCalculate->currentIndex());

	myKMeans.kmeans(m_samples, m_centers, m_k);

	update();
}


void FaceAnalyse::PrintSamples(vector<Sample> samples)
{
	QPainter painter(this);
	for (auto sample : m_samples)	// draw sample
	{
		QPen pen(m_colors[sample.cluster], 2);
		painter.setPen(pen);
		painter.drawPoint(20 + sample.data[0], 20 + sample.data[1]);							
	}	

	for (auto center : m_centers)	// draw center
	{
		QPen pen(QColor(0, 0, 0), 5);
		painter.setPen(pen);

		painter.drawPoint(20 + center.data[0], 20 + center.data[1]);
	}
}

void FaceAnalyse::makeColors(int k)
{
	srand((unsigned)time(nullptr));

	for (size_t i = 0; i < k; i++)
	{
		int r = RAND_GENERATOR * 255;
		int g = RAND_GENERATOR * 255;
		int b = RAND_GENERATOR * 255;

		m_colors.push_back(QColor(r, g, b));
	}	
}
