/*
 * Camera.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#include <string.h>
#include <unistd.h>

#include "Camera.hpp"
#include "Logger.hpp"


const char FRAME_DATA[] = "Frame";

using namespace infra;

Camera::Camera(const CaptureModeCollection& captureModes, int overheatTime):
		mCaptureModes(captureModes),
		mStarted(0),
		mTickCounter(0),
		mThread(this),
		mOverheatTime(overheatTime)
{
}

Camera::~Camera()
{

}

void Camera::GetCaptureModeList(CaptureModeCollection& captureModes) const
{
	captureModes = mCaptureModes;
}

Status Camera::Start( const CaptureParam& captureParam )
{
	Status st = ST_OK;

	if (mStarted)
		return st;

	mThread.Start();
	mCaptureParam = captureParam;
	mStarted = 1;

	ps_log_debug("Camera started");

	return st;
}

Status Camera::Stop()
{
	ps_log_debug("Entering %s", __FUNCTION__);
	Status st = ST_OK;

	if (!mStarted)
		return st;


	mThread.Stop();

	ps_log_debug("Camera stopped");

	mStarted = 0;

	return st;
}

void Camera::OnThread()
{
	++mTickCounter;

	if (!(mTickCounter % 10)/*1 sec period*/)
	{
		FrameContext frContext;
		frContext.Timestamp = mTickCounter;
		frContext.pData = FRAME_DATA;
		frContext.DataLength = strlen(FRAME_DATA) + 1 /*+ NULL termination*/;
		frContext.Resolution = mCaptureParam.Resolution;

		mCaptureParam.Callback.Callback(mCaptureParam.Callback.Context, &frContext);
	}

	usleep(100000/*100ms*/);

}

void Camera::GetCaps(CameraCaps& caps)
{
	caps.mOverheatTime = mOverheatTime;
}
