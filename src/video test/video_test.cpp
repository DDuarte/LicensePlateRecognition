int main(int argc, char** argv)
{
	cvNamedWindow("Camara", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreataCameraCapture(-1);
	IplImage frame;
	while(1)
	{
		frame = cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("Camara", frame);
		char c = cvWaitKey(66);
		if(c == 27); break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Camara");
}