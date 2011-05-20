#ifndef SR06_H
#define SR06_H

#include <ethercat_hardware/ethercat_device.h>
#include <sr_edc_ethercat_drivers/sr0x.h>
#include <realtime_tools/realtime_publisher.h>
#include <std_msgs/Int16.h>
#include <sr_edc_ethercat_drivers/SimpleMotorFlasher.h>
#include <pthread.h>
#include <bfd.h>
#include <boost/smart_ptr.hpp>

typedef unsigned char       int8u;
typedef   signed char       int8s;

typedef unsigned short      int16u;
typedef   signed short      int16s;

typedef unsigned int        int32u;
typedef   signed int        int32s;

extern "C" {
	#include "external/0220_palm_edc/0220_palm_edc_ethercat_protocol.h"
}

class SR06 : public SR0X
{
public:
    SR06();
    ~SR06();

    void construct(EtherCAT_SlaveHandler *sh, int &start_address);
    int  initialize(pr2_hardware_interface::HardwareInterface *hw, bool allow_unprogrammed=true);
    void diagnostics(diagnostic_updater::DiagnosticStatusWrapper &d, unsigned char *);

    bool SimpleMotorFlasher(sr_edc_ethercat_drivers::SimpleMotorFlasher::Request &req, sr_edc_ethercat_drivers::SimpleMotorFlasher::Response &res);
    void packCommand(unsigned char *buffer, bool halt, bool reset);
    bool unpackState(unsigned char *this_buffer, unsigned char *prev_buffer);
    bool can_data_is_ack(ETHERCAT_CAN_BRIDGE_DATA * packet);
    void erase_flash();
    bool read_flash(unsigned int offset, unsigned char baddrl, unsigned char baddrh, unsigned char baddru);
    void update_which_motors(ETHERCAT_DATA_STRUCTURE_0200_PALM_EDC_INCOMING   *command);

protected:
    int                                                                  counter_;
    ETHERCAT_DATA_STRUCTURE_0200_PALM_EDC_OUTGOING                       data_;
    ros::NodeHandle                                                      nodehandle_;
    std::vector< boost::shared_ptr<realtime_tools::RealtimePublisher<std_msgs::Int16> > >   realtime_pub_;

private:

    static const unsigned int        nb_sensors_const;
    static const unsigned int        max_retry;
    static const unsigned short int  max_iter_const;
    static const unsigned short int  ros_pub_freq_const;
    static const unsigned short int  device_pub_freq_const;
    static const unsigned char       nb_publish_by_unpack_const;
    std::string                      firmware_file_name;
    pthread_mutex_t                  producing;
    ros::ServiceServer               serviceServer;

    int                              which_motors;
    int                              which_data_from_motors;

    bool                             flashing;
    ETHERCAT_CAN_BRIDGE_DATA         can_message_;
    bool                             can_message_sent;
    bool                             can_packet_acked;
    bfd_byte                        *binary_content; // buffer containing the binary content to be flashed
    unsigned int                     pos; // position in binary_content buffer
    unsigned int                     motor_being_flashed;

    ///counter for the number of empty buffer we're reading.
    unsigned int                     zero_buffer_read;

    pthread_mutex_t mutex;
};


/* For the emacs weenies in the crowd.
Local Variables:
   c-basic-offset: 2
End:
*/


#endif /* SR06_H */
