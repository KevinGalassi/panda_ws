// Generated by gencpp from file schunk_pg70/get_errorRequest.msg
// DO NOT EDIT!


#ifndef SCHUNK_PG70_MESSAGE_GET_ERRORREQUEST_H
#define SCHUNK_PG70_MESSAGE_GET_ERRORREQUEST_H


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
struct get_errorRequest_
{
  typedef get_errorRequest_<ContainerAllocator> Type;

  get_errorRequest_()
    {
    }
  get_errorRequest_(const ContainerAllocator& _alloc)
    {
  (void)_alloc;
    }







  typedef boost::shared_ptr< ::schunk_pg70::get_errorRequest_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::schunk_pg70::get_errorRequest_<ContainerAllocator> const> ConstPtr;

}; // struct get_errorRequest_

typedef ::schunk_pg70::get_errorRequest_<std::allocator<void> > get_errorRequest;

typedef boost::shared_ptr< ::schunk_pg70::get_errorRequest > get_errorRequestPtr;
typedef boost::shared_ptr< ::schunk_pg70::get_errorRequest const> get_errorRequestConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::schunk_pg70::get_errorRequest_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >::stream(s, "", v);
return s;
}


} // namespace schunk_pg70

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::schunk_pg70::get_errorRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::schunk_pg70::get_errorRequest_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::schunk_pg70::get_errorRequest_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const ::schunk_pg70::get_errorRequest_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "schunk_pg70/get_errorRequest";
  }

  static const char* value(const ::schunk_pg70::get_errorRequest_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
{
  static const char* value()
  {
    return "\n"
;
  }

  static const char* value(const ::schunk_pg70::get_errorRequest_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream&, T)
    {}

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct get_errorRequest_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::schunk_pg70::get_errorRequest_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream&, const std::string&, const ::schunk_pg70::get_errorRequest_<ContainerAllocator>&)
  {}
};

} // namespace message_operations
} // namespace ros

#endif // SCHUNK_PG70_MESSAGE_GET_ERRORREQUEST_H
