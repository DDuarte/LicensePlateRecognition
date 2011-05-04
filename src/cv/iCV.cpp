#include <QtGui/QImage>
#include <QDebug>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include "FastMatchTemplate.h"
#include "iCV.h"

namespace rdm {

iCV::iCV(cv::Mat& source, cv::Mat& target, cv::Mat& fullImg, cv::Mat& plateImg,
    QImage& qFull, QImage& qPlate):
    source(source),
    target(target)
{
    cv::setUseOptimized(true); // !? FASTAH

    sourceClone = source.clone();
	plate = 0;

    Match();
    Draw();
    Plate();

    ConvertToRGB(sourceClone, fullImg);
    ConvertToRGB(plate, plateImg);
	plate = 0;
	sourceClone = 0;

    qFull = MatToQImage(fullImg);
    qPlate = MatToQImage(plateImg);
}

iCV::~iCV()
{
}

void iCV::Match()
{
    if(!FastMatchTemplate(source, target, &foundPointsList, &confidencesList, 10, false))
        qDebug() << "ERROR: Fast match template failed.";
}

void iCV::Draw()
{
    DrawFoundTargets(&sourceClone, target.size(), foundPointsList, confidencesList, 255, 0, 0, 4);
}

void iCV::Plate()
{
    cv::Size size = target.size();
    int width = size.width;
    int height = size.height;

	int xPoint = foundPointsList[0].x + 5 + width * 0.5;
	int yPoint = foundPointsList[0].y - height * 0.5;
    cv::Rect rect(xPoint, yPoint, 770, height + 10); // manually adjusted
    plate = source(rect);
}

void iCV::ConvertToRGB(cv::Mat& src, cv::Mat& targ)
{
    cv::cvtColor(src, targ, CV_BGR2RGB);
}

QImage iCV::MatToQImage(cv::Mat mat)
{
    return QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
}

}
