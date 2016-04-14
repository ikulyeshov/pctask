/*
 * Encoder.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_ENCODER_HPP_
#define INCLUDES_ENCODER_HPP_


#include "IEncoder.hpp"

namespace infra
{

class Encoder: public IEncoder
{
public:
	Encoder();
	virtual ~Encoder();

private:
	//not copyable
	Encoder(const Encoder& rop);
	const Encoder& operator=(const Encoder& rop);

};

} //namespace infra




#endif /* INCLUDES_ENCODER_HPP_ */