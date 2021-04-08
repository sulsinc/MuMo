#ifndef HEADER_device_Info_hpp_ALREADY_INCLUDED
#define HEADER_device_Info_hpp_ALREADY_INCLUDED

namespace device { 

    struct Info
    {
        const char * device_EUI = nullptr;
        const char * application_EUI = nullptr;
        const char * app_key = nullptr;

        const char * device_address = nullptr;
        const char * network_session_key = nullptr;
        const char * app_session_key = nullptr;

        bool valid() const
        {
            auto check_size = [](const char *cstr, unsigned int expected_size){
                if (!cstr) return false;
                if (strlen(cstr) != expected_size) return false;
                return true;
            };

            if (!check_size(device_EUI, 16)) return false;
            if (!check_size(application_EUI, 16)) return false;
            if (!check_size(app_key, 32)) return false;

            if (!check_size(device_address, 8)) return false;
            if (!check_size(network_session_key, 32)) return false;
            if (!check_size(app_session_key, 32)) return false;

            return true;
        }
    };

} 

#endif
