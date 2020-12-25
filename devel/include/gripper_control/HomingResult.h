// Generated by gencpp from file gripper_control/HomingResult.msg
// DO NOT EDIT!


#ifndef GRIPPER_CONTROL_MESSAGE_HOMINGRESULT_H
#define GRIPPER_CONTROL_MESSAGE_HOMINGRESULT_H


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
struct HomingResult_
{
  typedef HomingResult_<ContainerAllocator> Type;

  HomingResult_()
    : success(false)
    , error()  {
    }
  HomingResult_(const ContainerAllocator& _alloc)
    : success(false)
    , error(_alloc)  {
  (void)_alloc;
    }



   typedef uint8_t _success_type;
  _success_type success;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _error_type;
  _error_type error;





  typedef boost::shared_ptr< ::gripper_control::HomingResult_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::gripper_control::HomingResult_<ContainerAllocator> const> ConstPtr;

}; // struct HomingResult_

typedef ::gripper_control::HomingResult_<std::allocator<void> > HomingResult;

typedef boost::shared_ptr< ::gripper_control::HomingResult > HomingResultPtr;
typedef boost::shared_ptr< ::gripper_control::HomingResult const> HomingResultConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::gripper_control::HomingResult_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::gripper_control::HomingResult_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::gripper_control::HomingResult_<ContainerAllocator1> & lhs, const ::gripper_control::HomingResult_<ContainerAllocator2> & rhs)
{
  return lhs.success == rhs.success &&
    lhs.error == rhs.error;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::gripper_control::HomingResult_<ContainerAllocator1> & lhs, const ::gripper_control::HomingResult_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace gripper_control

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::gripper_control::HomingResult_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gripper_control::HomingResult_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gripper_control::HomingResult_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gripper_control::HomingResult_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gripper_control::HomingResult_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gripper_control::HomingResult_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::gripper_control::HomingResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "45872d25d65c97743cc71afc6d4e884d";
  }

  static const char* value(const ::gripper_control::HomingResult_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x45872d25d65c9774ULL;
  static const uint64_t static_value2 = 0x3cc71afc6d4e884dULL;
};

template<class ContainerAllocator>
struct DataType< ::gripper_control::HomingResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "gripper_control/HomingResult";
  }

  static const char* value(const ::gripper_control::HomingResult_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::gripper_control::HomingResult_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n"
"bool success\n"
"string error\n"
;
  }

  static const char* value(const ::gripper_control::HomingResult_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::gripper_control::HomingResult_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.success);
      stream.next(m.error);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct HomingResult_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::gripper_control::HomingResult_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::gripper_control::HomingResult_<ContainerAllocator>& v)
  {
    s << indent << "success: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.success);
    s << indent << "error: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.error);
  }
};

} // namespace message_operations
} // namespace ros

#endif // GRIPPER_CONTROL_MESSAGE_HOMINGRESULT_H
