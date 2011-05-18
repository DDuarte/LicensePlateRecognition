/****************************************************************************
** RDM - Reconhecimento Digital de Matriculas
** Copyright (C) 2011  Duarte Duarte, Miguel Mendes
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** We are not to be held liable for any illegal use of this software.
**
****************************************************************************/

#include <QtGui/QImage>
#include <QDebug>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

#include "FastMatchTemplate.h"
#include "FindPlate.h"

namespace rdm
{

    FindPlate::FindPlate()
    {
    }

    FindPlate::FindPlate(cv::Mat source):
        source(source)
    {
    }

    FindPlate::~FindPlate()
    {
    }

    QImage FindPlate::GetQImageFromMat(cv::Mat img)
    {
        return QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    }

    cv::Mat FindPlate::ConvertToRGB(cv::Mat img)
    {
		if (img.empty())
			return img;
        cv::Mat imgClone = img.clone();
        cv::cvtColor(img, imgClone, CV_BGR2RGB);
        return imgClone;
    }

    void FindPlate::Process()
    {
        cv::Size targetSize = target.size();
        int targetWidth = targetSize.width;
        int targetHeight = targetSize.height;
		int newWidth = static_cast<int>(9.3 * targetWidth);
        this->SetPlateWidth(newWidth);
                      // 9.3 -> ratio between target width and plate size "average"

        if (!FastMatchTemplate(source, target, &foundPointsList, &confidencesList, confidenceMin, false))
        {
            qDebug() << "Plate not found";
            return;
        }
        fullImg = source;
        DrawFoundTargets(&fullImg, targetSize, foundPointsList, confidencesList, 255, 0, 0, 4, newWidth);

		if (foundPointsList.empty())
		{
			qDebug() << "No points found.";
			return;
		}
        int xPoint = foundPointsList[0].x + 5 + targetWidth * 0.5;
        int yPoint = foundPointsList[0].y - targetHeight * 0.5;
        //cv::Rect rect(xPoint, yPoint, plateWidth, targetHeight + 10); // manually adjusted
        cv::Rect rect(foundPointsList[0].x +targetWidth * 0.5, foundPointsList[0].y - targetHeight * 0.5, plateWidth, targetHeight);
        plateImg = source(rect);
        source(rect).convertTo(plateImg, plateImg.type());
    }

    cv::Mat FindPlate::GetFullImage() const
    {
        return fullImg;
    }

    cv::Mat FindPlate::GetPlateImage() const
    {
        return plateImg;
    }

    cv::vector<cv::Point> FindPlate::GetFoundPointsList() const
    {
        return foundPointsList;
    }

    cv::vector<double> FindPlate::GetConfidencesList() const
    {
        return confidencesList;
    }

    void FindPlate::SetTarget(cv::Mat target)
    {
        this->target = target;
    }

    void FindPlate::SetConfidenceMinium(int percentage /*= 20*/)
    {
        confidenceMin = percentage;
    }

    void FindPlate::SetPlateWidth(int width /*= 770*/)
    {
        plateWidth = width;
    }

    void FindPlate::Go()
    {
        if (source.empty())
        {
            qDebug() << "Source is empty, can't continue.";
            return;
        }
        if (target.empty())
        {
            qDebug() << "Target is empty, can't continue.";
            return;
        }
        if (!fullImg.empty())
            qDebug() << "fullImg is not empty";
        if (!plateImg.empty())
            qDebug() << "plateImg is not empty";

        Process();
        if (fullImg.empty())
            qDebug() << "ERROR: fullImg is empty after process";
        if (plateImg.empty())
            qDebug() << "ERROR: plateImg is empty after process";
    }

    void FindPlate::SaveImageToHardDrive(cv::Mat img, std::string fileName)
    {
        cv::imwrite(fileName, img);
    }
    
}
