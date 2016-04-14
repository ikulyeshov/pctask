/*
 * Camera.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_CAMERA_HPP_
#define INCLUDES_CAMERA_HPP_

#include "ICamera.hpp"

namespace infra
{

class Camera: public ICamera
{
public:
	Camera();
	virtual ~Camera();

private:
	//not copyable
	Camera(const Camera& rop);
	const Camera& operator=(const Camera& rop);

};

} //namespace infra


#endif /* INCLUDES_CAMERA_HPP_ */
