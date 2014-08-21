#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <sys/time.h>
#include <thread>

#include "rclcpp/Node.h"
#include "rclcpp/Publisher.h"

#include "simple_msgs/AllDynamicArrayTypes.h"
#include "simple_msgs/AllPrimitiveTypes.h"
#include "simple_msgs/AllStaticArrayTypes.h"
#include "simple_msgs/Nested.h"
#include "simple_msgs/Uint32.h"

#include "userland/command_line_arguments.h"


template<typename T>
int publish(rclcpp::Node* node, void (*set_data_func)(T&, uint32_t))
{
  rclcpp::Publisher<T>* p = node->create_publisher<T>("topic_name");
  T ros_msg;

  //int number_of_msgs = 1000000;

  auto start = std::chrono::steady_clock::now();
  uint32_t i = 1;
  while (true) {
    set_data_func(ros_msg, i);
    p->publish(ros_msg);
    // if (i % 100000 == 0) {
      std::cout << "published ros msg #" << i << std::endl;
    // }
    ++i;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  auto end = std::chrono::steady_clock::now();

  std::cout << (end - start).count() << std::endl;

  return 0;
}

void set_counter_data(simple_msgs::Uint32& ros_msg, uint32_t i)
{
  ros_msg.data = i;
}

void set_all_primitive_data(simple_msgs::AllPrimitiveTypes& ros_msg, uint32_t i)
{
  ros_msg.my_bool = i % 2;
  ros_msg.my_byte = i % 256;
  ros_msg.my_char = i % 256;
  ros_msg.my_float32 = 0.1f + i;
  ros_msg.my_float64 = 0.1 + i;
  ros_msg.my_int8 = i;
  ros_msg.my_uint8 = i;
  ros_msg.my_int16 = i;
  ros_msg.my_uint16 = i;
  ros_msg.my_int32 = i;
  ros_msg.my_uint32 = i;
  ros_msg.my_int64 = i;
  ros_msg.my_uint64 = i;
  ros_msg.my_string = "foo " + std::to_string(i);
}

void set_all_static_array(simple_msgs::AllStaticArrayTypes& ros_msg, uint32_t i)
{
  int start = i - 1; // get the zero
  int end = i + 5; // assuming that the array size is 6
  int iteration_step;
  for (int j = start; j < end; ++j){
    iteration_step = j - i + 1;
    ros_msg.my_bool[iteration_step] = j % 2;
    ros_msg.my_byte[iteration_step] = j % 256;
    ros_msg.my_char[iteration_step] = j % 256;
    ros_msg.my_float32[iteration_step] = 0.1f + j;
    ros_msg.my_float64[iteration_step] = 0.1 + j;
    ros_msg.my_int8[iteration_step] = j;
    ros_msg.my_uint8[iteration_step] = j;
    ros_msg.my_int16[iteration_step] = j;
    ros_msg.my_uint16[iteration_step] = j;
    ros_msg.my_int32[iteration_step] = j;
    ros_msg.my_uint32[iteration_step] = j;
    ros_msg.my_int64[iteration_step] = j;
    ros_msg.my_uint64[iteration_step] = j;
    ros_msg.my_string[iteration_step] = std::string("foo ") + std::to_string(j);
  }
}

void set_all_dynamic_array(simple_msgs::AllDynamicArrayTypes& ros_msg, uint32_t i)
{
  int array_size = i - 1;
  ros_msg.my_bool.resize(array_size);
  ros_msg.my_byte.resize(array_size);
  ros_msg.my_char.resize(array_size);
  ros_msg.my_float32.resize(array_size);
  ros_msg.my_float64.resize(array_size);
  ros_msg.my_int8.resize(array_size);
  ros_msg.my_uint8.resize(array_size);
  ros_msg.my_int16.resize(array_size);
  ros_msg.my_uint16.resize(array_size);
  ros_msg.my_int32.resize(array_size);
  ros_msg.my_uint32.resize(array_size);
  ros_msg.my_int64.resize(array_size);
  ros_msg.my_uint64.resize(array_size);
  ros_msg.my_string.resize(array_size);

  for (int j = array_size; j < 2 * array_size; ++j){
    ros_msg.my_bool[j - array_size] = j % 2;
    ros_msg.my_byte[j - array_size] = j % 256;
    ros_msg.my_char[j - array_size] = j % 256;
    ros_msg.my_float32[j - array_size] = 0.1f + j;
    ros_msg.my_float64[j - array_size] = 0.1 + j;
    ros_msg.my_int8[j - array_size] = j;
    ros_msg.my_uint8[j - array_size] = j;
    ros_msg.my_int16[j - array_size] = j;
    ros_msg.my_uint16[j - array_size] = j;
    ros_msg.my_int32[j - array_size] = j;
    ros_msg.my_uint32[j - array_size] = j;
    ros_msg.my_int64[j - array_size] = j;
    ros_msg.my_uint64[j - array_size] = j;
    ros_msg.my_string[j - array_size] = std::string("foo ") + std::to_string(j);
  }
}

void set_nested(simple_msgs::Nested& ros_msg, uint32_t i)
{
  ros_msg.submsg.data = i;
}

int main(int argc, char** argv)
{
  if (has_argument(argv, argv + argc, "--help")) {
    std::cout << "usage: " << argv[0] << std::endl;
    print_message_usage();
    return 0;
  }

  rclcpp::Node* node = rclcpp::create_node();

  const std::string msg_arg = get_named_argument(argv, argv + argc, "--msg", valid_message_args[0]);
  if (msg_arg == valid_message_args[0]) {
    return publish<simple_msgs::Uint32>(node, &set_counter_data);
  } else if (msg_arg == valid_message_args[1]) {
    return publish<simple_msgs::AllPrimitiveTypes>(node, &set_all_primitive_data);
  } else if (msg_arg == valid_message_args[2]) {
    return publish<simple_msgs::AllStaticArrayTypes>(node, &set_all_static_array);
  } else if (msg_arg == valid_message_args[3]) {
    return publish<simple_msgs::AllDynamicArrayTypes>(node, &set_all_dynamic_array);
  } else if (msg_arg == valid_message_args[4]) {
    return publish<simple_msgs::Nested>(node, &set_nested);
  }


  std::cerr << "unsupported '--msg' argument '" << msg_arg << "'" << std::endl;
  return 1;
}