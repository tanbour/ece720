/*****************************************************************************

The following code is derived, directly or indirectly, from the SystemC
source code Copyright (c) 1996-2008 by all Contributors.
All Rights reserved.

The contents of this file are subject to the restrictions and limitations
set forth in the SystemC Open Source License Version 3.0 (the "License");
You may not use this file except in compliance with such restrictions and
limitations. You may obtain instructions on how to receive a copy of the
License at http://www.systemc.org/. Software distributed by Contributors
under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
ANY KIND, either express or implied. See the License for the specific
language governing rights and limitations under the License.

*****************************************************************************/
//#include <systemc>
#ifndef __AHBLITE_H__
#define __AHBLITE_H__


#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

template <int NR_OF_INITIATORS, int NR_OF_TARGETS>
class AHBlite : public sc_core::sc_module
{
public:
  typedef tlm::tlm_generic_payload transaction_type;
  typedef tlm::tlm_phase phase_type;
  typedef tlm::tlm_sync_enum sync_enum_type;
  typedef tlm_utils::simple_target_socket_tagged<AHBlite,64,tlm::tlm_base_protocol_types> target_socket_type;
  typedef tlm_utils::simple_initiator_socket_tagged<AHBlite> initiator_socket_type;// jie s.slave or dw.target_socket

public:
  target_socket_type target_socket[NR_OF_INITIATORS];
  initiator_socket_type initiator_socket[NR_OF_TARGETS];

public:
  SC_HAS_PROCESS(AHBlite);
  AHBlite(sc_core::sc_module_name name) :
    sc_core::sc_module(name)
  {
    for (unsigned int i = 0; i < NR_OF_INITIATORS; ++i) {
      target_socket[i].register_b_transport(this, &AHBlite::initiatorBTransport, i);
      target_socket[i].register_transport_dbg(this, &AHBlite::transportDebug, i);
     // target_socket[i].register_get_direct_mem_ptr(this, &AHBlite::getDMIPointer, i);
    }
    for (unsigned int i = 0; i < NR_OF_TARGETS; ++i) {
     ;// initiator_socket[i].register_invalidate_direct_mem_ptr(this, &AHBlite::invalidateDMIPointers, i);
    }
  }

  //
  // Dummy decoder:
  // - address[31-28]: portId
  // - address[27-0]: masked address
  //

  unsigned int getPortId(const sc_dt::uint64& address)
  {
   // if (address == 0x40000000) { std::cout << "sucks1 " << address << std::endl;  }
    return (unsigned int)address >> 28;
    //if (address == 0x40000000) { std::cout << "sucks1" << std::endl;  }
  }

  sc_dt::uint64 getAddressOffset(unsigned int portId)
  {
    return portId << 28;
  }

  sc_dt::uint64 getAddressMask(unsigned int portId)
  {
    return 0xffffffff;
  }

  unsigned int decode(const sc_dt::uint64& address)
  {
    // decode address:
    // - return initiator socket id

    return getPortId(address);
  }

  //
  // interface methods
  //

  //
  // LT protocol
  // - forward each call to the target/initiator
  //
  void initiatorBTransport(int SocketId,
                           transaction_type& trans,
                           sc_core::sc_time& t)
  {
    initiator_socket_type* decodeSocket;
    unsigned int portId = 0;//decode(trans.get_address());
    //std::cout << "===1.In AHB portId is " << portId << " === addr is "<< std::hex <<trans.get_address()<< std::endl;
    assert(portId < NR_OF_TARGETS);
    decodeSocket = &initiator_socket[portId];
     if (trans.get_address()!=0x40000000) trans.set_address(trans.get_address() & getAddressMask(portId));
    //std::cout << "===In AHB portId is " << portId << " === addr is "<< std::hex <<trans.get_address()<< std::endl;
    (*decodeSocket)->b_transport(trans, t);
  }

  unsigned int transportDebug(int SocketId,
                              transaction_type& trans)
  {
    unsigned int portId = decode(trans.get_address());
    assert(portId < NR_OF_TARGETS);
    initiator_socket_type* decodeSocket = &initiator_socket[portId];
    trans.set_address( trans.get_address() & getAddressMask(portId) );
    
    return (*decodeSocket)->transport_dbg(trans);
  }

  bool limitRange(unsigned int portId, sc_dt::uint64& low, sc_dt::uint64& high)
  {
    sc_dt::uint64 addressOffset = getAddressOffset(portId);
    sc_dt::uint64 addressMask = getAddressMask(portId);

    if (low > addressMask) {
      // Range does not overlap with addressrange for this target
      return false;
    }

    low += addressOffset;
    if (high > addressMask) {
      high = addressOffset + addressMask;

    } else {
      high += addressOffset;
    }
    return true;
  }

  bool getDMIPointer(int SocketId,
                     transaction_type& trans,
                     tlm::tlm_dmi& dmi_data)
  {
    sc_dt::uint64 address = trans.get_address();

    unsigned int portId = decode(address);
    assert(portId < NR_OF_TARGETS);
    initiator_socket_type* decodeSocket = &initiator_socket[portId];
    sc_dt::uint64 maskedAddress = address & getAddressMask(portId);

    trans.set_address(maskedAddress);

    bool result =
      (*decodeSocket)->get_direct_mem_ptr(trans, dmi_data);
    
    if (result)
    {
      // Range must contain address
      assert(dmi_data.get_start_address() <= maskedAddress);
      assert(dmi_data.get_end_address() >= maskedAddress);
    }
    
    // Should always succeed
sc_dt::uint64 start, end;
start = dmi_data.get_start_address();
end = dmi_data.get_end_address();

limitRange(portId, start, end);

dmi_data.set_start_address(start);
dmi_data.set_end_address(end);

    return result;
  }

  void invalidateDMIPointers(int port_id,
                             sc_dt::uint64 start_range,
                             sc_dt::uint64 end_range)
  {
    // FIXME: probably faster to always invalidate everything?

    if (!limitRange(port_id, start_range, end_range)) {
      // Range does not fall into address range of target
      return;
    }
    
    for (unsigned int i = 0; i < NR_OF_INITIATORS; ++i) {
      (target_socket[i])->invalidate_direct_mem_ptr(start_range, end_range);
    }
  }

};

#endif
