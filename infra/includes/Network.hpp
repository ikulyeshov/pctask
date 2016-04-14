/*
 * Network.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_NETWORK_HPP_
#define INCLUDES_NETWORK_HPP_


#include "INetwork.hpp"

namespace infra
{

class Network: public INetwork
{
public:
	Network();
	virtual ~Network();

private:
	//not copyable
	Network(const Network& rop);
	const Network& operator=(const Network& rop);

};

} //namespace infra




#endif /* INCLUDES_NETWORK_HPP_ */
