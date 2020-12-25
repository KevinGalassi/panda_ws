// Generated by gencpp from file schunk_pg70/stopResponse.msg
// DO NOT EDIT!


#ifndef SCHUNK_PG70_MESSAGE_STOPRESPONSE_H
#define SCHUNK_PG70_MESSAGE_STOPRESPONSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace schunk_pg70
{
template <class ContainerAllocator>
struct stopResponse_
{
  typedef stopResponse_<ContainerAllocator> Type;

  stopResponse_()
    : stop_result(0.0)  {
    }
  stopResponse_(const ContainerAllocator& _alloc)
    : stop_result(0.0)  {
  (void)_alloc;
    }



   typedef float _stop_result_type;
  _stop_result_type stop_result;





  typedef boost::shared_ptr< ::schunk_pg70::stopResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::schunk_pg70::stopResponse_<ContainerAllocator> const> ConstPtr;

}; // struct stopResponse_

typedef ::schunk_pg70::stopResponse_<std::allocator<void> > stopResponse;

typedef boost::shared_ptr< ::schunk_pg70::stopResponse > stopResponsePtr;
typedef boost::shared_ptr< ::schunk_pg70::stopResponse const> stopResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::schunk_pg70::stopResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::schunk_pg70::stopResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::schunk_pg70::stopResponse_<ContainerAllocator1> & lhs, const ::schunk_pg70::stopResponse_<ContainerAllocator2> & rhs)
{
  return lhs.stop_result == rhs.stop_result;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::schunk_pg70::stopResponse_<ContainerAllocator1> & lhs, const ::schunk_pg70::stopResponse_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace schunk_pg70

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::schunk_pg70::stopResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::schunk_pg70::stopResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::schunk_pg70::stopResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::schunk_pg70::stopResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::schunk_pg70::stopResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::schunk_pg70::stopResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::schunk_pg70::stopResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "1dc3c304d8e1ef4b9d7103ce1e3c990a";
  }

  static const char* value(const ::schunk_pg70::stopResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x1dc3c304d8e1ef4bULL;
  static const uint64_t static_value2 = 0x9d7103ce1e3c990aULL;
};

template<class ContainerAllocator>
struct DataType< ::schunk_pg70::stopResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "schunk_pg70/stopResponse";
  }

  static const char* value(const ::schunk_pg70::stopResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::schunk_pg70::stopResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float32 stop_result\n"
"\n"
;
  }

  static const char* value(const ::schunk_pg70::stopResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::schunk_pg70::stopResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.stop_result);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct stopResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::schunk_pg70::stopResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::schunk_pg70::stopResponse_<ContainerAllocator>& v)
  {
    s << indent << "stop_result: ";
    Printer<float>::stream(s, indent + "  ", v.stop_result);
  }
};

} // namespace message_operations
} // namespace ros

#endif // SCHUNK_PG70_MESSAGE_STOPRESPONSE_H