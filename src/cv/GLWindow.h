#include <QtOpenGL>
class QWidget;

#include "VideoMan/VideoManControl.h"
#include "VideoMan/VideoManInputFormat.h"

class GLWindow: public QGLWidget
{
	Q_OBJECT

public:
	GLWindow(int timerInterval=30, QWidget *parent=0, const char *name=0);
	~GLWindow(){}

protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
  
public slots:
	virtual void timeOutSlot();
	virtual void openCamera();
  
private:
  QTimer *m_timer;
};