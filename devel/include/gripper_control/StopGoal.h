// Generated by gencpp from file gripper_control/StopGoal.msg
// DO NOT EDIT!


#ifndef GRIPPER_CONTROL_MESSAGE_STOPGOAL_H
#define GRIPPER_CONTROL_MESSAGE_STOPGOAL_H


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
struct StopGoal_
{
  typedef StopGoal_<ContainerAllocator> Type;

  StopGoal_()
    {
    }
  StopGoal_(const ContainerAllocator& _alloc)
    {
  (void)_alloc;
    }







  typedef boost::shared_ptr< ::gripper_control::StopGoal_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::gripper_control::StopGoal_<ContainerAllocator> const> ConstPtr;

}; // struct StopGoal_

typedef ::gripper_control::StopGoal_<std::allocator<void> > StopGoal;

typedef boost::shared_ptr< ::gripper_control::StopGoal > StopGoalPtr;
typedef boost::shared_ptr< ::gripper_control::StopGoal const> StopGoalConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::gripper_control::StopGoal_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::gripper_control::StopGoal_<ContainerAllocator> >::stream(s, "", v);
return s;
}


} // namespace gripper_control

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::gripper_control::StopGoal_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gripper_control::StopGoal_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gripper_control::StopGoal_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gripper_control::StopGoal_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gripper_control::StopGoal_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gripper_control::StopGoal_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::gripper_control::StopGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const ::gripper_control::StopGoal_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::gripper_control::StopGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "gripper_control/StopGoal";
  }

  static const char* value(const ::gripper_control::StopGoal_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::gripper_control::StopGoal_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
;
  }

  static const char* value(const ::gripper_control::StopGoal_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::gripper_control::StopGoal_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream&, T)
    {}

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct StopGoal_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::gripper_control::StopGoal_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream&, const std::string&, const ::gripper_control::StopGoal_<ContainerAllocator>&)
  {}
};

} // namespace message_operations
} // namespace ros

#endif // GRIPPER_CONTROL_MESSAGE_STOPGOAL_H