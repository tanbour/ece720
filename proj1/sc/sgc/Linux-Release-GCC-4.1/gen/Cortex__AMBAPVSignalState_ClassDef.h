/*
 * 
 *
 * This is an automatically generated file. Do not edit.
 *
 * Copyright 2005 ARM Limited.
 * Copyright 2011 ARM Limited.
 *
 * All rights reserved.
 */

/** \file
 * Class definition for protocol Cortex__AMBAPVSignalState.
 */

#ifndef Cortex__AMBAPVSignalState_ClassDef_h_
#define Cortex__AMBAPVSignalState_ClassDef_h_

#include <MxTypes.h>
#include <eslapi/eslapi_stdint.h>

#include <list>
#include "sg/SGPort.h"

#include "protocol_AMBAPVSignalState.h"

namespace Cortex_NMS {

class Cortex__AMBAPVSignalState;

// Abstract interface implementation for port
class protocol_AMBAPVSignalState__Implementation: public protocol_AMBAPVSignalState
{
public:
    protocol_AMBAPVSignalState__Implementation(): thisPort(0) {}

    void SetPort(Cortex__AMBAPVSignalState *port) { thisPort = port; }

    // Protocol members
	inline void set_state(int export_id, const bool& state);
	inline bool get_state(int export_id, tlm::tlm_tag<bool> * t);


private:
    // Parent port this interface belongs to.
    Cortex__AMBAPVSignalState *thisPort;
};


// Protocol (port) class definition
class Cortex__AMBAPVSignalState: public sg::Port
{
private:
    
public:
    // Resources


    // Connector instances
	sg::SlaveConnector< void (int,const bool&) > set_state;
	sg::SlaveConnector< bool (int,tlm::tlm_tag<bool> *) > get_state;


    Cortex__AMBAPVSignalState()
    {
        abstractInterface__.SetPort(this);
		define_behaviour(set_state);
		define_behaviour(get_state);


    }

    // Set name (currently ignored here)
    void SetName__(const char *) {}

    // Abstract port interface for non-LISA C++ components
    protocol_AMBAPVSignalState__Implementation abstractInterface__;

    // Get abstract interface for non-LISA C++ components
    protocol_AMBAPVSignalState *getAbstractInterface() { return &abstractInterface__; }
};


// Abstract interface implementations
inline void protocol_AMBAPVSignalState__Implementation::set_state(int export_id, const bool& state)
{
	return thisPort->set_state(export_id, state);
}

inline bool protocol_AMBAPVSignalState__Implementation::get_state(int export_id, tlm::tlm_tag<bool> * t)
{
	return thisPort->get_state(export_id, t);
}




// Mapping object
// (these ports are implemented inside Cortex__AMBAPVSignalState__AddressablePort)
class Cortex__AMBAPVSignalState__AddressablePort_OutPort: public Cortex__AMBAPVSignalState
{
public:
    // constructor
    Cortex__AMBAPVSignalState__AddressablePort_OutPort(MxU32 inFirst,
                                                 MxU32 inLast,
                                                 MxU32 outFirst,
                                                 MxU32 outLast):
    inFirst__(inFirst),
    inLast__(inLast),
    outFirst__(outFirst),
    outSize__(outLast - outFirst + 1)
    {
    }

    bool isCovered__(MxU32 address) const
    {
        return (address >= inFirst__) && (address <= inLast__);
    }

    void remapAddress__(MxU32 &address) const
    {
        if(outSize__)
            address = ((address - inFirst__) % outSize__) + outFirst__;
    }

private:
    MxU32 inFirst__;
    MxU32 inLast__;
    MxU32 outFirst__;
    MxU32 outSize__;
};


// Addressable protocol class definition
class Cortex__AMBAPVSignalState__AddressablePort: public Cortex__AMBAPVSignalState
{
public:
    declare_slave_behaviour(mapRange, sg::Port*(MxU64 inFirst, MxU64 inLast, MxU64 outFirst, MxU64 outLast));
    declare_slave_behaviour(mapDefault, sg::Port*());
    declare_slave_behaviour(unmapRange, void(sg::Port*));
    declare_slave_behaviour(isMapped, bool(MxU64));

    Cortex__AMBAPVSignalState__AddressablePort():
    defaultOutPort__(0),
    portName__("(unknown name)")
    {
        define_behaviour(mapRange);
        define_behaviour(mapDefault);
        define_behaviour(unmapRange);
        define_behaviour(isMapped);
        bind_method(*this, mapRange, &Cortex__AMBAPVSignalState__AddressablePort::mapRange__, this);
        bind_method(*this, mapDefault, &Cortex__AMBAPVSignalState__AddressablePort::mapDefault__, this);
        bind_method(*this, unmapRange, &Cortex__AMBAPVSignalState__AddressablePort::unmapRange__, this);
        bind_method(*this, isMapped, &Cortex__AMBAPVSignalState__AddressablePort::isMapped__, this);

    }

    ~Cortex__AMBAPVSignalState__AddressablePort()
    {
        OutPortList__::iterator it;
        for (it = outPortList__.begin(); it != outPortList__.end(); ++it)
            delete *it;
        delete defaultOutPort__;
    }

    void SetName__(const char *name) { portName__ = name; }

    Cortex__AMBAPVSignalState__AddressablePort_OutPort *locateOutPort__(MxU32 &addr)
    {
        OutPortList__::iterator it;
        for (it = outPortList__.begin(); it != outPortList__.end(); ++it)
        {
            if ((*it)->isCovered__(addr)) {
                (*it)->remapAddress__(addr);
                return *it;
            }
        }
        return defaultOutPort__;
    }

    sg::Port* mapRange__(MxU64 inFirst, MxU64 inLast, MxU64 outFirst, MxU64 outLast)
    {
        Cortex__AMBAPVSignalState__AddressablePort_OutPort *masterPort =
            new Cortex__AMBAPVSignalState__AddressablePort_OutPort(inFirst, inLast, outFirst, outLast);
		this->set_state.connectTo(&masterPort->set_state);
		this->get_state.connectTo(&masterPort->get_state);

        // push_front() because the first statement should have the lowest priority and the last statement should have the
        // highest priority, like PVBusDecoder
        outPortList__.push_front(masterPort);
        return masterPort;
    }

    // map default slave
    sg::Port* mapDefault__()
    {
        delete defaultOutPort__;
        defaultOutPort__ = new Cortex__AMBAPVSignalState__AddressablePort_OutPort(0, MxU32(0) - 1, 0, MxU32(0) - 1);
        return defaultOutPort__;
    }

    void unmapRange__(sg::Port* port)
    {
        outPortList__.remove((Cortex__AMBAPVSignalState__AddressablePort_OutPort*)port);
        if(defaultOutPort__ == port)
        {
            delete defaultOutPort__;
            defaultOutPort__ = 0;
        }
    }

    bool isMapped__(MxU64 addr)
    {
        OutPortList__::iterator it;
        for (it = outPortList__.begin(); it != outPortList__.end(); ++it)
        {
            if ((*it)->isCovered__(static_cast<MxU32>(addr)))
                return true;
        }
        return false;
    }

    // Wrapper functions for protocol behaviors


private:
    typedef std::list<Cortex__AMBAPVSignalState__AddressablePort_OutPort*> OutPortList__;
    OutPortList__ outPortList__;
    Cortex__AMBAPVSignalState__AddressablePort_OutPort *defaultOutPort__;
    const char *portName__;
};

} // namespace Cortex_NMS

#endif
