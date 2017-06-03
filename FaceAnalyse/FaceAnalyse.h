#pragma once
#include <QDialog>
#include "MyKMeans.h"



namespace Ui{
	class FaceAnalyse;
}

class FaceAnalyse : public QDialog
{
	Q_OBJECT
public:
	explicit FaceAnalyse(QWidget *parent = NULL);
	virtual ~FaceAnalyse();

	void makeColors(int k);
	void PrintSamples(vector<Sample> samples);

public slots:
	void KMeansTest();
	void GenerateSample();

protected:
	void paintEvent(QPaintEvent * event);

signals:

public slots :

private:
	Ui::FaceAnalyse *ui;

	vector<Sample> m_samples;
	vector<Sample> m_centers;
	vector<QColor> m_colors;
	int m_k;
};