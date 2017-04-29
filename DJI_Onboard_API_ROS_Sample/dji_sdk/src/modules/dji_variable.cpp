#include <dji_sdk/dji_variable.h>
#include <dji_sdk/attitude_quad.h>
#include <dji_sdk/velocity.h>

#define C_EARTH (double) 6378137.0
#define C_PI    (double) 3.141592653589793

namespace dji_variable
{
    dji_sdk::local_position local_position_ref;
    dji_sdk::global_position global_position_ref;
    bool localposbase_use_height = true;
    dji_sdk::attitude_quad attitude_quad;
    dji_sdk::velocity velocity;
    dji_sdk::acc acc;
    dji_sdk::rc_channels rc_channels;
    dji_sdk::global_position global_position;
    dji_sdk::global_position global_position_degree;
    dji_sdk::local_position local_position;
    nav_msgs::Odometry odem;
    uint8_t flight_status;
    uint8_t ctrl_device;
    float battery = 0;
    bool opened = false;
    bool activated = false;

    void gps_convert_ned(float &ned_x, float &ned_y,
                         double gps_t_lon,
                         double gps_t_lat,
                         double gps_r_lon,
                         double gps_r_lat
    )
    {

        //TODO :
        //fix bug with ellipsoid

        double d_lon = gps_t_lon - gps_r_lon;
        double d_lat = gps_t_lat - gps_r_lat;

        ned_x = d_lat * C_EARTH;
        ned_y = d_lon * C_EARTH * cos((gps_r_lat + gps_t_lat) / 2 * M_PI / 180.0f);

        return;
    }

    dji_sdk::local_position gps_convert_ned(dji_sdk::global_position loc)
    {
        dji_sdk::local_position local;
        gps_convert_ned(
          local.x,
          local.y,
          loc.lon,
          loc.lat,
          global_position_ref.lon,
          global_position_ref.lat
        );
        local.height = global_position.height;
        return local;
    }
};
