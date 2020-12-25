// Generated by gencpp from file read_sensor/proximity_sensor_data.msg
// DO NOT EDIT!


#ifndef READ_SENSOR_MESSAGE_PROXIMITY_SENSOR_DATA_H
#define READ_SENSOR_MESSAGE_PROXIMITY_SENSOR_DATA_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace read_sensor
{
template <class ContainerAllocator>
struct proximity_sensor_data_
{
  typedef proximity_sensor_data_<ContainerAllocator> Type;

  proximity_sensor_data_()
    : header()
    , proximity_sensor_data()  {
    }
  proximity_sensor_data_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , proximity_sensor_data(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _proximity_sensor_data_type;
  _proximity_sensor_data_type proximity_sensor_data;





  typedef boost::shared_ptr< ::read_sensor::proximity_sensor_data_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::read_sensor::proximity_sensor_data_<ContainerAllocator> const> ConstPtr;

}; // struct proximity_sensor_data_

typedef ::read_sensor::proximity_sensor_data_<std::allocator<void> > proximity_sensor_data;

typedef boost::shared_ptr< ::read_sensor::proximity_sensor_data > proximity_sensor_dataPtr;
typedef boost::shared_ptr< ::read_sensor::proximity_sensor_data const> proximity_sensor_dataConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::read_sensor::proximity_sensor_data_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::read_sensor::proximity_sensor_data_<ContainerAllocator1> & lhs, const ::read_sensor::proximity_sensor_data_<ContainerAllocator2> & rhs)
{
  return lhs.header == rhs.header &&
    lhs.proximity_sensor_data == rhs.proximity_sensor_data;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::read_sensor::proximity_sensor_data_<ContainerAllocator1> & lhs, const ::read_sensor::proximity_sensor_data_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace read_sensor

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::read_sensor::proximity_sensor_data_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::read_sensor::proximity_sensor_data_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::read_sensor::proximity_sensor_data_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
{
  static const char* value()
  {
    return "3a2387fb9f4f2007e1f416de15deda17";
  }

  static const char* value(const ::read_sensor::proximity_sensor_data_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x3a2387fb9f4f2007ULL;
  static const uint64_t static_value2 = 0xe1f416de15deda17ULL;
};

template<class ContainerAllocator>
struct DataType< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
{
  static const char* value()
  {
    return "read_sensor/proximity_sensor_data";
  }

  static const char* value(const ::read_sensor::proximity_sensor_data_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n"
"uint8[] proximity_sensor_data\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
;
  }

  static const char* value(const ::read_sensor::proximity_sensor_data_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.proximity_sensor_data);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct proximity_sensor_data_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::read_sensor::proximity_sensor_data_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::read_sensor::proximity_sensor_data_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "proximity_sensor_data[]" << std::endl;
    for (size_t i = 0; i < v.proximity_sensor_data.size(); ++i)
    {
      s << indent << "  proximity_sensor_data[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.proximity_sensor_data[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // READ_SENSOR_MESSAGE_PROXIMITY_SENSOR_DATA_H
