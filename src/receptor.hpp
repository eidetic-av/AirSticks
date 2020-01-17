#include <thread>

#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscPacketListener.h"
#include "../lib/oscpack/osc/OscReceivedElements.h"

#include "plugin.hpp"

namespace osc {
class AirSticksListener : public OscPacketListener {
	protected:
		virtual void ProcessMessage(const ReceivedMessage& m,
				const IpEndpointName& remoteEndpoint) ;
};
}
