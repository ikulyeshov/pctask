/*
 * TextMessageDefines.hpp
 *
 *  Created on: Apr 15, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_TEXTMESSAGEDEFINES_HPP_
#define INCLUDES_TEXTMESSAGEDEFINES_HPP_


namespace infra
{
namespace msgserver
{

const int TEXT_MESSAGE_MAX_MESSAGE_LEN =		100;

enum Status
{
	ST_OK,
	ST_ERROR,
	ST_SELECT_ERROR,
	ST_BINDING_ERROR,
	ST_ACCEPT_ERROR,
	ST_OPEN_ERROR,
	ST_WRITE_ERROR,
	ST_PARAM_ERROR,
	ST_CONNECT_ERROR
};

} //namespace msgserver
} //namespace infra


#endif /* INCLUDES_TEXTMESSAGEDEFINES_HPP_ */
