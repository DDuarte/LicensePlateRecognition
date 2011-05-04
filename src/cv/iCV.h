#ifndef ICV_H
#define ICV_H
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

namespace rdm {

    class iCV
    {
    private:
        cv::vector<cv::Point> foundPointsList; // Points found list
        cv::vector<double> confidencesList; // Percentage of matches list
        cv::Mat source;
        cv::Mat sourceClone;
        cv::Mat target;
        cv::Mat plate;
    
    public:
        iCV(cv::Mat& source, cv::Mat& target, cv::Mat& fullImg, cv::Mat& plateImg, QImage& qFull, QImage& qPlate);
        virtual ~iCV();
    
    protected:
        virtual void Match();
        virtual void Draw();
        virtual void Plate();
    
    	static QImage MatToQImage(cv::Mat mat);
    	static void ConvertToRGB(cv::Mat& src, cv::Mat& targ);
    };

}

#endif // ICV_H
