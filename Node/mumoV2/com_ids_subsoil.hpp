#ifndef HEADER_device_subsoil_hpp_ALREADY_INCLUDED
#define HEADER_device_subsoil_hpp_ALREADY_INCLUDED

#include "com_Ids.hpp"

namespace com { namespace ids { 

    //The ids and keys of the subsoil seeeduino with reset button
    inline Ids subsoil()
    {
        Ids ids;

        ids.device_EUI = "00BEA0888A7FE90A";
        ids.application_EUI = "70B3D57ED003BB73";
        ids.app_key = "800317A9D6AAD3C045DFFC285592481F";

        ids.device_address = "26013242";
        ids.network_session_key = "2D3311932EA0879115C6B4F7D8B73A8E";
        ids.app_session_key = "456DED8EE204D36B72CF00297CBFA484";

        return ids;
    }

} } 

#endif
