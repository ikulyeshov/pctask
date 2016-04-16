/*
 * ClientServerDefines.hpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_CLIENTSERVERDEFINES_HPP_
#define INCLUDES_CLIENTSERVERDEFINES_HPP_


namespace infra
{

const char MSG_START[] = 		"Start %i";
const char MSG_STOP[] = 		"Stop %i";
const char MSG_QOS[] = 			"Qos %i"; //bitrate as argument
const char MSG_CAPS[] = 		"Caps %i(%i) %ix%i"; //<camera> <row num> <resx x resy>
const char MSG_CAPS_REQ[] = 	"Capsreq";
const char MSG_OVERHEAT[] = 	"Overheat %i"; //camera

} //namespace infra

#endif /* INCLUDES_CLIENTSERVERDEFINES_HPP_ */
