/*
 * Camera.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_CAMERA_HPP_
#define INCLUDES_CAMERA_HPP_

#include "ICamera.hpp"
#include "Thread.hpp"

namespace infra
{

class Camera: public ICamera, public Thread::User
{
public:
	Camera(const CaptureModeCollection& captureModes, int overheatTime/*sec*/);
	virtual ~Camera();

    virtual void GetCaptureModeList( CaptureModeCollection& captureModes ) const;
    virtual void GetCaps(CameraCaps& caps);
    virtual Status Start( const CaptureParam& captureParam );
    virtual Status Stop();

	virtual void OnThread();

private:
	//not copyable
	Camera(const Camera& rop);
	const Camera& operator=(const Camera& rop);

	CaptureModeCollection mCaptureModes;
	int mStarted;
	int mTickCounter;
	CaptureParam mCaptureParam;
	Thread mThread;
	int mOverheatTime;

};

} //namespace infra


#endif /* INCLUDES_CAMERA_HPP_ */
