#ifndef HEADER_device_subsoil_hpp_ALREADY_INCLUDED
#define HEADER_device_subsoil_hpp_ALREADY_INCLUDED

#include "device_Info.hpp"

namespace device { 

    inline Info subsoil()
    {
        Info info;

        info.device_EUI = "00BEA0888A7FE90A";
        info.application_EUI = "70B3D57ED003BB73";
        info.app_key = "800317A9D6AAD3C045DFFC285592481F";

        info.device_address = "26013242";
        info.network_session_key = "2D3311932EA0879115C6B4F7D8B73A8E";
        info.app_session_key = "456DED8EE204D36B72CF00297CBFA484";

        return info;
    }

} 

#endif
