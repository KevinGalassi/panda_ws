// Generated by gencpp from file gripper_control/GraspFeedback.msg
// DO NOT EDIT!


#ifndef GRIPPER_CONTROL_MESSAGE_GRASPFEEDBACK_H
#define GRIPPER_CONTROL_MESSAGE_GRASPFEEDBACK_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace gripper_control
{
template <class ContainerAllocator>
struct GraspFeedback_
{
  typedef GraspFeedback_<ContainerAllocator> Type;

  GraspFeedback_()
    {
    }
  GraspFeedback_(const ContainerAllocator& _alloc)
    {
  (void)_alloc;
    }







  typedef boost::shared_ptr< ::gripper_control::GraspFeedback_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::gripper_control::GraspFeedback_<ContainerAllocator> const> ConstPtr;

}; // struct GraspFeedback_

typedef ::gripper_control::GraspFeedback_<std::allocator<void> > GraspFeedback;

typedef boost::shared_ptr< ::gripper_control::GraspFeedback > GraspFeedbackPtr;
typedef boost::shared_ptr< ::gripper_control::GraspFeedback const> GraspFeedbackConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::gripper_control::GraspFeedback_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::gripper_control::GraspFeedback_<ContainerAllocator> >::stream(s, "", v);
return s;
}


} // namespace gripper_control

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::gripper_control::GraspFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gripper_control::GraspFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gripper_control::GraspFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gripper_control::GraspFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gripper_control::GraspFeedback_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gripper_control::GraspFeedback_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::gripper_control::GraspFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const ::gripper_control::GraspFeedback_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::gripper_control::GraspFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "gripper_control/GraspFeedback";
  }

  static const char* value(const ::gripper_control::GraspFeedback_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::gripper_control::GraspFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"\n"
;
  }

  static const char* value(const ::gripper_control::GraspFeedback_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::gripper_control::GraspFeedback_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream&, T)
    {}

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct GraspFeedback_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::gripper_control::GraspFeedback_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream&, const std::string&, const ::gripper_control::GraspFeedback_<ContainerAllocator>&)
  {}
};

} // namespace message_operations
} // namespace ros

#endif // GRIPPER_CONTROL_MESSAGE_GRASPFEEDBACK_H
