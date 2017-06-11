#include <QLabel>
#include <QPainter>
#include <vector>
#include <iostream>
#include <QFileDialog>
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

	connect(ui->pushButton_generate_sample, &QPushButton::clicked, this, &FaceAnalyse::GenerateSample);
	connect(ui->pushButton_read_sample, &QPushButton::clicked, this, &FaceAnalyse::ReadSample);
	connect(ui->pushButton_kmeans, &QPushButton::clicked, this, &FaceAnalyse::KMeansTest);
	connect(ui->pushButton_output_sample, &QPushButton::clicked, this, &FaceAnalyse::OutputSamples);

	
	QStringList init_rule, calculate_rule;
	init_rule << tr("分段随机") << tr("Kmeans++") << tr("长度等分点");
	ui->comboBox_initCenter->addItems(init_rule);

	calculate_rule << tr("欧氏距离") << tr("曼哈顿距离") << tr("闵可夫斯基距离") << tr("切比雪夫距离");
	ui->comboBox_disCalculate->addItems(calculate_rule);
		
	spdlog::get(LOGGER)->info("construct.");
}

void FaceAnalyse::paintEvent(QPaintEvent * event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	
	//PrintSamples(m_samples);
}

FaceAnalyse::~FaceAnalyse()
{
	PluginManager::GetInstance().UnLoad("PluginShape");// UnLoad之后必须停止所有插件的引用，否则造成空指针异常。
}


void FaceAnalyse::ReadSample()
{
	m_samples.clear();

	QString title = QString(tr("选择样本文件"));
	QString filter = QString(tr("样本文件 (*.ds)"));

	QString selectedFilter;
	QString file_name = QFileDialog::getOpenFileName(
		this, title,
		nullptr,
		filter,
		&selectedFilter
		);

	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly))
	{
		spdlog::get(LOGGER)->error("can not open sample file.");
		return;
	}
	QByteArray bytes;
	while ((bytes = file.readLine()) != nullptr)
	{
		Sample sample;

		QString line(bytes);
		QStringList vars = line.split(",");		vars.removeAll("");
		if (vars[vars.size() - 1].contains("setosa"))
		{
			sample.cluster = 0;
		}
		else if (vars[vars.size() - 1].contains("versicolor"))
		{
			sample.cluster = 1;
		}
		else if (vars[vars.size() - 1].contains("virginica"))
		{
			sample.cluster = 2;
		}
		vars.removeAt(vars.size() - 1);
		for each (QString var in vars)
		{			
			sample.data.push_back(var.toFloat());
		}

		m_samples.push_back(sample);
	}
}

void FaceAnalyse::GenerateSample()
{
	m_samples.clear();

	makeColors(1);

	srand((unsigned)time(nullptr));

	int sample_dim = 2;
	int sample_count = 500;
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
	makeColors(m_k);
	myKMeans.setInitRule((InitRule)ui->comboBox_initCenter->currentIndex());	

	Dissimilarity* prule;
	switch (ui->comboBox_disCalculate->currentIndex())
	{
	case 0:
		prule = new GeometryDistance;
		static_cast<GeometryDistance*>(prule)->setRule(EuclideanDistance);
		break;
	case 1:
		prule = new GeometryDistance;
		static_cast<GeometryDistance*>(prule)->setRule(ManhattanDistance);
		break;
	case 2:
		prule = new GeometryDistance;
		static_cast<GeometryDistance*>(prule)->setRule(MinkowskiDistance, ui->lineEdit_p->text().toInt());
		break;
	case 3:
		prule = new GeometryDistance;
		static_cast<GeometryDistance*>(prule)->setRule(ChebyshevDistance);
		break;
	default:
		break;
	}
	myKMeans.setDissimilarityRule(prule);


// 	vector<Sample> samples(5);
// 	samples[0].data.push_back(12);
// 	samples[0].data.push_back(22);
// 	samples[1].data.push_back(32);
// 	samples[1].data.push_back(13);
// 	samples[2].data.push_back(14);
// 	samples[2].data.push_back(28);
// 	samples[3].data.push_back(19);
// 	samples[3].data.push_back(9);
// 	samples[4].data.push_back(21);
// 	samples[4].data.push_back(11);
	

	myKMeans.kmeans(m_samples, m_centers, m_k);

	spdlog::get(LOGGER)->info(myKMeans.Silhouette(m_samples));

	//update();
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
	m_colors.resize(0);

	srand((unsigned)time(nullptr));
	for (size_t i = 0; i < k; i++)
	{
		int r = RAND_GENERATOR * 255;
		int g = RAND_GENERATOR * 255;
		int b = RAND_GENERATOR * 255;

		m_colors.push_back(QColor(r, g, b));
	}	
}

void FaceAnalyse::OutputSamples()
{
	QString title = QString(tr("选择样本文件"));
	QString filter = QString(tr("样本文件 (*.ds)"));

	QString selectedFilter;
	QString file_name = QFileDialog::getSaveFileName(
		this, title,
		nullptr,
		filter,
		&selectedFilter
		);

	QFile file(file_name);
	if (!file.open(QIODevice::WriteOnly))
	{
		spdlog::get(LOGGER)->error("can not open sample file.");
		return;
	}


	for (Sample sample : m_samples)
	{
		for (float var : sample.data)
		{
			QString s_var = QString().sprintf("%0.1f", var);	
			
			file.write(s_var.toLatin1());
			file.write(",");
		}
		switch (sample.cluster)
		{
		case 0:
			file.write("Iris-setosa\n");
			break;
		case 1:
			file.write("Iris-versicolor\n");
			break;
		case 2:
			file.write("Iris-virginica\n");
			break;
		default:
			break;
		}
	}
}
