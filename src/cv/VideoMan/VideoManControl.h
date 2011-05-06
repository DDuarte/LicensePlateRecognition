#ifndef VIDEOMANCONTROL_H
#define VIDEOMANCONTROL_H

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

#include "VideoManInputFormat.h"

class VideoManInputController;
class VideoManControlPrivate;

/** \brief VideoMan Control class
\par Demo Code:

\code
	videoMan.getAvailableDevices( list, numDevices );
	if ( numDevices > 0 )
	{
		device = list[0];
		format.SetFormat( 640, 320,30, RGB24, RGB24 );
		videoInputID = videoMan.addVideoInput( device, &format );
	}
	videoMan.freeAvailableDevicesList( list, numDevices );
	...
	videoMan.getFrame( videoInputID );
	videoMan.updateTexture( videoInputID );
	videoMan.renderFrame( videoInputID );
	videoMan.releaseFrame( videoInputID );
\endcode

\author Javier Barandiaran Martirena (2006)
*/

class VIDEOMAN_API VideoManControl
{
private:
	
	VideoManControl(const VideoManControl &);
	VideoManControl & operator=(const VideoManControl &);

public:

	typedef enum{ NO_RENDERER, OPENGL_RENDERER } Renderer;
	
	/** \brief Constructor
		\param useRenderer [in] If VideoMan will use renderer to display the images or not
	*/
	VideoManControl( Renderer useRenderer = OPENGL_RENDERER );

	/** \brief Destructor
	*/
	~VideoManControl(void);

	/** \brief Check if the given identifier is supported by any of the loaded input modules
			The user should must call to freeSupportedIdentifiersList
		\param identifiers [out] list of identifiers
		\param numIdentifiers [out] Number of supported identifers
	*/
	void getSupportedIdentifiers( char **&identifiers, int &numIdentifiers );

	/** \brief Free the memory used for the identifiers list
		\param identifiers [in/out] list of identifiers
		\param numIdentifiers [in/out] Number of supported identifers
	*/
	void freeSupportedIdentifiersList( char **&identifiers, int &numIdentifiers );

	/** \brief Check if the given identifier is supported by any of the loaded input modules
		\return true if the identifier is supported
	*/
	bool supportedIdentifier( const char *identifier );

//! \name General Input Control
//@{
	/** \brief  Add a video input
		\param identifier [in] The input Identification (see inputIdentification)		
		\param format [in/out] The preferred format of the video input, the resolution and fps will be taken from the video file
		\return false if something fails
	*/
	int addVideoInput( const inputIdentification &identifier, VideoManInputFormat *format );

	/** \brief  Delete the rquested video input
		\param input [in] the video input index
	*/
	void deleteInput( size_t input );

	/** \brief  Get a new frame of this video input
		\param input [in] the video input index 
		\param wait [in] if it must wait for next sample or not
		\return the frame of the video
	*/
	char *getFrame( size_t input, bool wait=false );
	
	/** \brief Release the last captured frame
		\param input [in] the video input index 
	*/
	void releaseFrame( size_t input );

	/** \brief Get the video format of the input
		\param input [in] the video input index 
		\param format [out] the format
	*/
	void getFormat( size_t input, VideoManInputFormat &format );

	/** \brief Get the identification of the input
		\param input [in] the video input index 
		\param identification [out] the identification
	*/
	void getInputIdentification( size_t input, inputIdentification &identification );

	/** \brief  Get the number of inputs		
		\return the number of initialized video inputs
	*/
	int getNumberOfInputs();

	typedef void (*frameCallback)(char *pixelBuffer);

	/** \brief Set the callback that will be called when a new frame is available
		\param input [in] the video input index 
		\param callback [in] a callback of the type frameCallback
	*/
	void setFrameCallback( const size_t &input, frameCallback callback );

	/** \brief Ask if the input support new frame callback
		\param input [in] the video input index 
	*/
	bool supportFrameCallback( const size_t &input );
//@}

//! \name Renderer Control
//@{
	/** \brief Activate all the video inputs. In the next render all the inputs will be shown	
	*/
	void activateAllVideoInputs();

	/** \brief Activate this video input. In the next render this input will be shown
		\param input [in] the video input index
	*/
	void activateVideoInput( const size_t &input );

	/** \brief Deactivate all the video inputs. In the next render anyone of the inputs will be shown
	*/
	void deactivateAllVideoInputs();

	/** \brief Deactivate this video input. In the next render this input will not be shown
		\param input [in] the video input index
	*/
	void deactivateVideoInput( const size_t &input );

	/** \brief Check if this video input is activated or not
		\param input [in] the video input index
		\return true if the inputs is activated
	*/
	bool isActivated( const size_t &input );

	/** \brief notify to the renderer the change of the screen
		\param left [in] the x coordinate of the top left corner of the region where the renderer must draw
		\param up [in] the y coordinate of the top left corner of the region where the renderer must draw
		\param width [in] the width of the region where the renderer must draw
		\param height [in] the height of the region where the renderer must draw
	*/
	void changeScreenSize( const int &left, const int &up, const int &width, const int &height );

	/** \brief Change the visualization layout, i.e., the way the inputs are displayed
		\param layout [in] The layout number
	*/
	void changeVisualizationMode( const int &layout);

	/** \brief Change the input that is shown in the principal location of the layout, the effect depends on the visualization layout
		\param input [in] the video input index
	*/
	void changeMainVisualizationInput( const size_t &input );

	/**	\brief Get the index of the main visualization input
		\return the index of the main visualization input
	*/
	int getMainVisualizationInput();

	/** \brief  Turn the vertical flip (only affects to the renderer not to the image) on or off
		\param input [in] the video input index
		\param value [in] true turn on\false turn off
	*/
	void setVerticalFlip( const size_t &input, bool value );
	
	/** \brief  Check if the vertical flip is on or off
		\param input [in] the video input index		
		\return true if the vertical flip is on
	*/
	bool getVerticalFlip( const size_t &input );
	
	/** \brief  Turn the horizontal flip (only affects to the renderer not to the image) on or off
		\param input [in] the video input index
		\param value [in] true turn on\false turn off
	*/
	void setHorizontalFlip( const size_t &input, bool value );
	
	/** \brief  Check if the horizontal flip is on or off
		\param input [in] the video input index		
		\return true if the horizontal flip is on
	*/
	bool getHorizontalFlip( const size_t &input );

	/** \brief Set the zoom values (only affects to the renderer not to the image). Following renders will show the zoom
		\param input [in] the video input index	
		\param x,y [in] image coordinates of the zoom center
		\param zoomFactor [in] the zoom scale ( 1.0 no zoom )
		\param center [in] If the input must be centered at the zoom center
	*/
	void setRendererZoom( const size_t &input, const float &x, const float &y, const float &zoomFactor, bool center = false);
	
	/** \brief Get the zoom values
		\param input [in] the video input index	
		\param x,y,zoomFactor [out] the zoom values
	*/
	void getRendererZoom( const size_t &input, float &x, float &y, float &zoomFactor );

	/** \brief Reset the zoom value
		\param input [in] the video input index	
	*/
	void resetRendererZoom( const size_t &input );

	/** \brief Set the rotation angle(only affects to the renderer not to the image)
		\param angle [in] the angle in degrees
	*/
	void setRendererRotation( const size_t &input, float angle );

	/** \brief Get the rotation angle
		\return the angle in degrees
	*/
	float getRendererRotation( const size_t &input );

	/** \brief Render all the activated video inputs
	*/
	void renderFrames() const;	

	/** \brief Render this video input
		\param input [in] the video input index
	*/
	void renderFrame( size_t input ) const;

	/** \brief Update the texture to be rendered with the image
		\param input [in] the video input index
		\param image [in] the new image
	*/
	void updateTexture( size_t input, const char *image );

	/** \brief Update the texture to be rendered with the last captured frame
		\param input [in] the video input index		
	*/
	void updateTexture( size_t input );

	/** \brief Transform screen coordinates to image coordinates
		\param x [in\out] the x or horizontal coordinate
		\param y [in\out] the y or vertical coordinate
		\return if the coordinates are inside the image of any of the inputs, the index of that input is returned, -1 otherwise		
	*/
	int screenToImageCoords( float &x, float &y );

	/** \brief Transform image (corresponding to a specific input) coordinates to screen coordinates
		\param input [in] the video input index
		\param x,y [in\out] the x,y (horizontal,vertical) coordinates	
	*/
	void imageToScreenCoords( const size_t &input, float &x, float &y );

	/** \brief Get the screen coordinates of the input image coordinates
		\param input [in] the video input index
		\param left, bottom, width, height [out] the returned screen coordinates
		\return true if the input is activated, false otherwise
	*/
	bool getScreenCoords( const size_t &input, int &left, int &bottom, int &width, int &height );

	bool getScreenCoordsArea( const size_t &input, int &left, int &bottom, int &width, int &height );

	/** \brief Get the coordinates of the texture corresponding to the input index
		\param input [in] the video input index
		\param left,up,right,bottom [out] the texture coordinates
	*/
	void getTextureCoords( const size_t &input, float &left, float &bottom, float &right, float &up );

	/** \brief Activate the renderer viewport of this video input
		\param input [in] the video input index
	*/
	void activateViewport( const size_t &input );

	/** \brief Activate the renderer texture of this video input
		\param input [in] the video input index
	*/
	void activateTexture( const size_t &input );

	/** \brief Activate the 2D drawing setup for the input in order to draw 2D elemnts in image coordinates
		\param input [in] the video input index
	*/
	void activate2DDrawingSetup( const size_t &input );

	/** \brief Draw a line border around the input
		\param input [in] the video input index
		\param thickness [in] thickness of the line
		\param r,g,b [in] color of the line
	*/
	void drawInputBorder( const size_t &input, const float &thickness, const float &r, const float &g, const float &b );

	/** \brief Set the filtering mode of the renderer textures
		\param textureFiltering [in] Filtering mode ( NEAREST = 0, LINEAR = 1 )
	*/
	void setTextureFiltering( int textureFiltering );

	/** \brief Get the filtering mode of the renderer textures
		\return the filtering mode ( NEAREST = 0, LINEAR = 1 )
	*/
	int getTextureFiltering();

	/** \brief Set if the renderer must keep the aspect ratio of the input (only affects to the renderer not to the image)
		\param input [in] the video input index
		\param keep [in] if the renderer must keep the aspect ratio or not
	*/
	void setKeepAspectRatio( const size_t &input, bool keep );

	/** \brief Check if the renderer keeps the aspect ratio of the input
		\param input [in] the video input index
		\return True if the renderer keeps the aspect ratio
	*/
	bool getKeepAspectRatio( const size_t &input );

	/** \brief Set a region of interest. UpdateTexture will update only that region (only affects to the renderer not to the image)
		\param input [in] the video input index
		\param x,y [in] coordinates of the upper-left corner of the region must be >= 0
		\param width,height [in] width an height of the region ROI coordinates
	*/
	void setImageROI( const size_t &input, const int &x, const int &y, const int &width, const int &height );

	/** \brief Get the region of interest parameters
		\param input [in] the video input index
		\param x,y [out] coordinates of the upper-left corner of the region must be >= 0
		\param width,height [out] width an height of the region ROI coordinates
	*/
	void getImageROI( const size_t &input, int &x, int &y, int &width, int &height );

	/** \brief Reset the region of interest
		\param input [in] the video input index
	*/
	void resetImageROI( const size_t &input );
//@}

//! \name User input Control
//@{
	/** \brief  Associate an image with an input, when the texture of the input will be updated, that image will be used
		\param input [in] the video input index
	*/
	void setUserInputImage( const size_t &input, char* image );

	char *getUserInputImage(  const size_t &input );
//@}

//! \name Camera Control
//@{
	/** \brief Create and link a controller to the video input
			The user must call to deleteController
		\param input [in] the video input index
		\param controllerIdentifier [in] Controller identifer
		\return the pointer to the controller
	*/
	VideoManInputController *createController( const size_t &input, const char *controllerIdentifier );

	/** \brief Delete the controller
		\param controller [in/out] the controller
	*/
	void deleteController( VideoManInputController **controller );

	/** \brief Link a controller to the video input
		\param input [in] the video input index
		\param controller [in] the controller to be linked
		\return true if the link is possible
	*/
	bool linkController( const size_t &input, VideoManInputController *controller  );

	/** \brief Get the controller linked to the video input
		\param input [in] the video input index
		\return the pointer to the controller
	*/
	VideoManInputController *getController( const size_t &input ) const;
		
	/** \brief Show the property page of this video input (only works with cameras)
		\param input [in] the video input index		
	*/
	void showPropertyPage( const size_t &input ) const;

	/** \brief Get the list of available devices that can be initialized with all the available input modules
			The user should must call to freeAvailableDevicesList
		\param deviceList [out] the list of available devices
		\param numDevices [out] the number of devices in the list
	*/
	void getAvailableDevices( inputIdentification **deviceList, int &numDevices );

	/** \brief Get the list of available devices that can be initialized with a specific input module
			The user should must call to freeAvailableDevicesList
		\param identifier [in] Identifier of the inut module
		\param deviceList [out] the list of available devices
		\param numDevices [out] the number of devices in the list
	*/
	void getAvailableDevices( const char *identifier, inputIdentification **deviceList, int &numDevices );

	/** \brief Free the memory used for the device list			
		\param deviceList [in] the list of available devices
		\param numDevices [in] the number of devices in the list
	*/
	void freeAvailableDevicesList( inputIdentification **deviceList, int &numDevices );

//@}

//! \name Video file Control
//@{
	/** \brief Go to this frame of this input (only for video files)
		\param input [in] the video input index 
		\param frame [in] the frame position
	*/
	void goToFrame( const size_t &input, int frame );

	/** \brief Go to this milisecond of this input (only for video files)
		\param input [in] the video input index 
		\param milisecond [in] the position in miliseconds
	*/
	void goToMilisecond( size_t input, double milisecond );

	/** \brief Get the length of the video file in frames
		\param input [in] the video input index 
		\return the number of frames
	*/
	int getLengthFrames( const size_t &input );

	/** \brief Get the length of the video file in seconds
		\param input [in] the video input index 
		\return the number of seconds
	*/
	double getLengthSeconds( const size_t &input );

	/** \brief Get the actual position of the video file in frames
		\param input [in] the video input index 
		\return the frame number
	*/
	int getPositionFrames( const size_t &input );

	/** \brief Get the actual position of the video file in seconds
		\param input [in] the video input index 
		\return the time in seconds
	*/
	double getPositionSeconds( const size_t &input );

	/** \brief Stop the playback of a video file
		\param input [in] the video input index 		
	*/
	void stopVideo( const size_t &input );
	
	/** \brief Pause the playback of a video file
		\param input [in] the video input index 		
	*/
	void pauseVideo( const size_t &input );

	/** \brief Continue the playback of a video file
		\param input [in] the video input index 		
	*/
	void playVideo( const size_t &input );
//@}

private:

	VideoManControlPrivate *videoManPrivate;	
};

#endif