// Generated by gencpp from file schunk_pg70/get_position.msg
// DO NOT EDIT!


#ifndef SCHUNK_PG70_MESSAGE_GET_POSITION_H
#define SCHUNK_PG70_MESSAGE_GET_POSITION_H

#include <ros/service_traits.h>


#include <schunk_pg70/get_positionRequest.h>
#include <schunk_pg70/get_positionResponse.h>


namespace schunk_pg70
{

struct get_position
{

typedef get_positionRequest Request;
typedef get_positionResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct get_position
} // namespace schunk_pg70


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::schunk_pg70::get_position > {
  static const char* value()
  {
    return "40db4b6d09ee0b910be5d9e61aacbb7d";
  }

  static const char* value(const ::schunk_pg70::get_position&) { return value(); }
};

template<>
struct DataType< ::schunk_pg70::get_position > {
  static const char* value()
  {
    return "schunk_pg70/get_position";
  }

  static const char* value(const ::schunk_pg70::get_position&) { return value(); }
};


// service_traits::MD5Sum< ::schunk_pg70::get_positionRequest> should match
// service_traits::MD5Sum< ::schunk_pg70::get_position >
template<>
struct MD5Sum< ::schunk_pg70::get_positionRequest>
{
  static const char* value()
  {
    return MD5Sum< ::schunk_pg70::get_position >::value();
  }
  static const char* value(const ::schunk_pg70::get_positionRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::schunk_pg70::get_positionRequest> should match
// service_traits::DataType< ::schunk_pg70::get_position >
template<>
struct DataType< ::schunk_pg70::get_positionRequest>
{
  static const char* value()
  {
    return DataType< ::schunk_pg70::get_position >::value();
  }
  static const char* value(const ::schunk_pg70::get_positionRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::schunk_pg70::get_positionResponse> should match
// service_traits::MD5Sum< ::schunk_pg70::get_position >
template<>
struct MD5Sum< ::schunk_pg70::get_positionResponse>
{
  static const char* value()
  {
    return MD5Sum< ::schunk_pg70::get_position >::value();
  }
  static const char* value(const ::schunk_pg70::get_positionResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::schunk_pg70::get_positionResponse> should match
// service_traits::DataType< ::schunk_pg70::get_position >
template<>
struct DataType< ::schunk_pg70::get_positionResponse>
{
  static const char* value()
  {
    return DataType< ::schunk_pg70::get_position >::value();
  }
  static const char* value(const ::schunk_pg70::get_positionResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // SCHUNK_PG70_MESSAGE_GET_POSITION_H
