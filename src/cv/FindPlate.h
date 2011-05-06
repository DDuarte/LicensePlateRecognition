#ifndef FINDPLATE_H
#define FINDPLATE_H
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

namespace rdm
{

    class FindPlate
    {
    private:
        cv::Mat source;
        cv::Mat target;
        cv::Mat fullImg;
        cv::Mat plateImg;
        cv::vector<cv::Point> foundPointsList;
        cv::vector<double> confidencesList;
        int confidenceMin;
        int plateWidth;
        
    public:
        FindPlate();
        explicit FindPlate(cv::Mat source);
        virtual ~FindPlate();

        virtual void Go();

        virtual cv::Mat GetFullImage() const;
        virtual cv::Mat GetPlateImage() const;
        virtual cv::vector<cv::Point> GetFoundPointsList() const; // Points found list
        virtual cv::vector<double> GetConfidencesList() const; // Percentage of matches list
        
        virtual void SetTarget(cv::Mat target);
        virtual void SetConfidenceMinium(int percentage = 20);
        virtual void SetPlateWidth(int width = 770);
    public: // static
        static QImage GetQImageFromMat(cv::Mat img);
        static cv::Mat ConvertToRGB(cv::Mat img);
        static void SaveImageToHD(cv::Mat img, std::string fileName);
    private:
        virtual void Process();
    };

}

#endif // FINDPLATE_H

