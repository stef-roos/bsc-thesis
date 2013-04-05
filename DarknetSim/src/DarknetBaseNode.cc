//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <omnetpp.h>
#include <IPAddressResolver.h>
#include "DarknetBaseNode.h"
#include <UDPPacket.h>
//#include "UDPControlInfo_m.h"

void DarknetBaseNode::initialize(int stage)
{
    nodeID = par("node_id").stdstringValue();
    localPort = par("local_port");
    bindToPort(localPort);
}

void DarknetBaseNode::sendPacket(DarknetMessage* dmsg, IPvXAddress& destAddr, int destPort) {

    UDPPacket *pkg = new UDPPacket("udp");

    pkg->setByteLength(8);
    pkg->encapsulate(dmsg);
    sendToUDP(pkg, localPort, destAddr, destPort);
}


void DarknetBaseNode::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        // maybe a timer / internal message handling?
        // or a packet that we want so send..
        //sendMessage(check_and_cast<DarknetMessage*>(msg));
        sendMessage(new DarknetMessage(this->nodeID, "host3", "trolo"));
    }
    else
    {
        if(msg->isPacket()) {
            printPacket((cPacket*)msg);
            cPacket *packet = ((cPacket*)msg)->getEncapsulatedPacket();
            if(packet!=NULL)
                processIncomingMessage(check_and_cast<DarknetMessage*>(packet));
            else EV << "no encapsulated packet contained..."<< endl;
        }else EV << "uh..dont know what to do here..";
//        }
    }
}


void DarknetBaseNode::processIncomingMessage(DarknetMessage *msg)
{
    EV << "received some DarknetMessage...";
    if (msg->destNodeID != nodeID) {
            forwardMessage(msg);
    }else {
        //distinguish MessageTypes
       EV << "received message for this node! YAY :D";
    }
}

void DarknetBaseNode::forwardMessage(DarknetMessage* msg) {
    if(msg->TTL > 0) {
        msg->TTL--;
        sendMessage((DarknetMessage*)msg->dup());
    }
    // TODO: inform simulator/user of droped message
    EV << "dropped message";
}
