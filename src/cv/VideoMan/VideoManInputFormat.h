#ifndef VIDEOMANINPUTFORMAT_H
#define VIDEOMANINPUTFORMAT_H

#ifdef WIN32
#ifdef VideoMan_EXPORTS
#define VIDEOMAN_API __declspec(dllexport)
#else
#define VIDEOMAN_API __declspec(dllimport)
#endif
#endif
#ifdef linux
#define VIDEOMAN_API 
#endif

struct inputIdentification
{
	char* fileName;
	char* friendlyName;
	char* devicePath;
	unsigned long serialNumber;
	char* identifier;

	inputIdentification()
	{
		fileName = 0;
		friendlyName = 0;
		devicePath = 0;
		serialNumber = 0;
		identifier = 0;
	}
};

enum PIXEL_FORMAT { RGB24, RGB32, BGR24, BGR32, YUV422, YUV411, IYUV, GREY8, GREY16, RAW8, UNKNOWN};

struct PIXEL_FORMATS
{
	PIXEL_FORMAT formatOut;
	PIXEL_FORMAT formatIn;
};

class VIDEOMAN_API VideoManInputFormat
{
public:
	VideoManInputFormat(void);
	VideoManInputFormat(VideoManInputFormat const &format);
	virtual ~VideoManInputFormat(void);
	VideoManInputFormat &operator=( VideoManInputFormat const &format );

	bool SetFormat( int awidth, int aheight, double afps, PIXEL_FORMAT apixelFormatIn, PIXEL_FORMAT apixelFormatOut );
	bool setPixelFormat( PIXEL_FORMAT apixelFormatIn, PIXEL_FORMAT apixelFormatOut );
	PIXEL_FORMAT getPixelFormatIn();
	PIXEL_FORMAT getPixelFormatOut();
	bool validFormat();	

	int nChannels;  //Number of channels
	int depth;		//bits per channel
	int width;		//resolution width
	int height;		//resolution height
	double fps;		//frames per second

	bool showDlg;	//(capture device) Show init dialog to select format
	
	bool dropFrames; //drop frames while playing or not	
	bool clock; //(video file) Use clock to reproduce the video with its corresponding framerate or not
	bool renderAudio; //(video file) Render the audio channel	

private:
	PIXEL_FORMATS pixelFormats;                          
};

#endif