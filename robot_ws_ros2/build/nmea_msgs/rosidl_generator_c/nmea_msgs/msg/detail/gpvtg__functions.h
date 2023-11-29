// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from nmea_msgs:msg/Gpvtg.idl
// generated code does not contain a copyright notice

#ifndef NMEA_MSGS__MSG__DETAIL__GPVTG__FUNCTIONS_H_
#define NMEA_MSGS__MSG__DETAIL__GPVTG__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "nmea_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "nmea_msgs/msg/detail/gpvtg__struct.h"

/// Initialize msg/Gpvtg message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * nmea_msgs__msg__Gpvtg
 * )) before or use
 * nmea_msgs__msg__Gpvtg__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
bool
nmea_msgs__msg__Gpvtg__init(nmea_msgs__msg__Gpvtg * msg);

/// Finalize msg/Gpvtg message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
void
nmea_msgs__msg__Gpvtg__fini(nmea_msgs__msg__Gpvtg * msg);

/// Create msg/Gpvtg message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * nmea_msgs__msg__Gpvtg__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
nmea_msgs__msg__Gpvtg *
nmea_msgs__msg__Gpvtg__create();

/// Destroy msg/Gpvtg message.
/**
 * It calls
 * nmea_msgs__msg__Gpvtg__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
void
nmea_msgs__msg__Gpvtg__destroy(nmea_msgs__msg__Gpvtg * msg);

/// Check for msg/Gpvtg message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
bool
nmea_msgs__msg__Gpvtg__are_equal(const nmea_msgs__msg__Gpvtg * lhs, const nmea_msgs__msg__Gpvtg * rhs);

/// Copy a msg/Gpvtg message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
bool
nmea_msgs__msg__Gpvtg__copy(
  const nmea_msgs__msg__Gpvtg * input,
  nmea_msgs__msg__Gpvtg * output);

/// Initialize array of msg/Gpvtg messages.
/**
 * It allocates the memory for the number of elements and calls
 * nmea_msgs__msg__Gpvtg__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
bool
nmea_msgs__msg__Gpvtg__Sequence__init(nmea_msgs__msg__Gpvtg__Sequence * array, size_t size);

/// Finalize array of msg/Gpvtg messages.
/**
 * It calls
 * nmea_msgs__msg__Gpvtg__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
void
nmea_msgs__msg__Gpvtg__Sequence__fini(nmea_msgs__msg__Gpvtg__Sequence * array);

/// Create array of msg/Gpvtg messages.
/**
 * It allocates the memory for the array and calls
 * nmea_msgs__msg__Gpvtg__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
nmea_msgs__msg__Gpvtg__Sequence *
nmea_msgs__msg__Gpvtg__Sequence__create(size_t size);

/// Destroy array of msg/Gpvtg messages.
/**
 * It calls
 * nmea_msgs__msg__Gpvtg__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
void
nmea_msgs__msg__Gpvtg__Sequence__destroy(nmea_msgs__msg__Gpvtg__Sequence * array);

/// Check for msg/Gpvtg message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
bool
nmea_msgs__msg__Gpvtg__Sequence__are_equal(const nmea_msgs__msg__Gpvtg__Sequence * lhs, const nmea_msgs__msg__Gpvtg__Sequence * rhs);

/// Copy an array of msg/Gpvtg messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_nmea_msgs
bool
nmea_msgs__msg__Gpvtg__Sequence__copy(
  const nmea_msgs__msg__Gpvtg__Sequence * input,
  nmea_msgs__msg__Gpvtg__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // NMEA_MSGS__MSG__DETAIL__GPVTG__FUNCTIONS_H_
