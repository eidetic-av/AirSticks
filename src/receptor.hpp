#include <thread>

#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscPacketListener.h"
#include "../lib/oscpack/osc/OscReceivedElements.h"

#include "plugin.hpp"

#include <iterator>

namespace osc {
class AirSticksListener : public OscPacketListener {
	protected:
		virtual void ProcessMessage(const ReceivedMessage& m,
				const IpEndpointName& remoteEndpoint) ;
};
}

template <typename Out>
void split(const std::string& s, char delim, Out result) {
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}


