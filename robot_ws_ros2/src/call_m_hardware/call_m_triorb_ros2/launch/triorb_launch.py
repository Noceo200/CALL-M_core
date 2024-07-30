#
#
import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import LaunchConfiguration

from launch.events import Shutdown
from launch_ros.actions import Node
import launch
from launch.actions import DeclareLaunchArgument, OpaqueFunction

def generate_launch_description():
    # Declare the launch arguments
    declare_param_namespace = DeclareLaunchArgument(
        'namespace',
        default_value='callm_no_id',
        description='namespace of robot'
    )

    # Use OpaqueFunction to call the function that uses the parameter
    processes_to_launch = OpaqueFunction(function=run_launch_with_parameters)

    return LaunchDescription([
        declare_param_namespace,
        processes_to_launch
        ])

def replace_namespace_in_file(template_path, namespace, out_path):
    with open(template_path, 'r') as file:
        config_content = file.read()

    config_content = config_content.replace('{{namespace}}', namespace)

    with open(out_path, 'w') as file:
        file.write(config_content)

    return out_path

def run_launch_with_parameters(context):
    namespace_value = context.launch_configurations['namespace']
    # triorb
    config_ = replace_namespace_in_file(os.path.join(get_package_share_directory('call_m_triorb_ros2'),'config', 'parameters.yaml'),namespace_value,os.path.join(get_package_share_directory('call_m_triorb_ros2'),'config', 'generated_parameters.yaml'))
    triorb_node = Node(package='call_m_triorb_ros2', executable='triorb',
                       name='triorb', output='screen',
                       parameters=[config_], 
                       namespace=namespace_value,
                       on_exit=Shutdown())
    return triorb_node