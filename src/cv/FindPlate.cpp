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

#include <iostream>

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
        cv::Mat imgClone = img.clone();
        cv::cvtColor(img, imgClone, CV_BGR2RGB);
        return imgClone;
    }

    void FindPlate::Process()
    {
        cv::Size targetSize = target.size();
        int targetWidth = targetSize.width;
        int targetHeight = targetSize.height;

        FastMatchTemplate(source, target, &foundPointsList, &confidencesList, confidenceMin, false);
        fullImg = source;
        DrawFoundTargets(&fullImg, targetSize, foundPointsList, confidencesList, 255, 0, 0, 4);

        int xPoint = foundPointsList[0].x + 5 + targetWidth * 0.5;
        int yPoint = foundPointsList[0].y - targetHeight * 0.5;
        cv::Rect rect(xPoint, yPoint, plateWidth, targetHeight + 10); // manually adjusted

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

    void FindPlate::SetConfidenceMinium( int percentage )
    {
        confidenceMin = percentage;
    }

    void FindPlate::Go()
    {
        if (source.empty())
        {
            std::cout << "Source is empty, can't continue." << std::endl;
            return;
        }
        if (target.empty())
        {
            std::cout << "Target is empty, can't continue." << std::endl;
            return;
        }
        if (!fullImg.empty())
            std::cout << "fullImg is not empty" << std::endl;
        if (!plateImg.empty())
            std::cout << "plateImg is not empty" << std::endl;

        Process();
        if (fullImg.empty())
            std::cout << "ERROR: fullImg is empty after process" << std::endl;
        if (plateImg.empty())
            std::cout << "ERROR: plateImg is empty after process" << std::endl;
    }

    void FindPlate::SetPlateWidth( int width /*= 770*/ )
    {
        plateWidth = width;
    }



}
