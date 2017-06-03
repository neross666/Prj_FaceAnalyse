#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QTextCodec>
#include <QDebug>
#include "Config/Config.h"
#include "FaceAnalyse.h"
#include "Algorithm/GMM/KMeans.h"
#include "Algorithm/GMM/GMM.h"


using namespace std;

int KMeansTest();

bool initialApp();

#define GRAY_LEVEL 255


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	initialApp();
	FaceAnalyse win;
	//win.setWindowFlags(Qt::FramelessWindowHint);
	//win.setAttribute(Qt::WA_TranslucentBackground);
	win.show();
		
	
	return a.exec();
}

bool initialApp()
{
	{// 配置app
		Config::generateLogger();
		Config::generateFolder();
		Config::appAutoRun(true);
	}

	{// 加载语言包
		QTranslator* translator = new QTranslator;
		if (translator->load("./Resources/Language/faceanalyse_zh.qm"))
			qApp->installTranslator(translator);
	}
	
	{// 加载皮肤
		QFile file("./Resources/skin/app.css");
		if (file.open(QFile::ReadOnly))
		{
			QString styleSheet = QLatin1String(file.readAll());
			qApp->setStyleSheet(styleSheet);
			file.close();
		}
	}

	return true;
}