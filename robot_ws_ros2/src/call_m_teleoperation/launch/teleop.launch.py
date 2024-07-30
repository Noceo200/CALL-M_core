import os
import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
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
    pkg_dir = get_package_share_directory('call_m_teleoperation')
    
    # Get the namespace parameter
    namespace_value = context.launch_configurations['namespace']
    joystick_param_file = replace_namespace_in_file(os.path.join(pkg_dir, 'config', 'joystick_configuration.yaml'),namespace_value,os.path.join(pkg_dir, 'config', 'generated_joystick_configuration.yaml'))

    # ExecuteProcess actions with the namespace parameter
    keyboard_control_node = ExecuteProcess(
        cmd=['xterm', '-fn', 'xft:fixed:size=12', '-geometry', '60x20', '-e', 'ros2', 'run', 'call_m_teleoperation', 'keyboard_control_node', '--ros-args', '-r', '__ns:=/' + namespace_value],
        output='screen',
    )

    joystick_control_node = ExecuteProcess(
        cmd=['xterm', '-fn', 'xft:fixed:size=12', '-geometry', '70x40', '-e', 'ros2', 'run', 'call_m_teleoperation', 'joystick_control_node', '--ros-args', '--params-file', joystick_param_file, '-r', '__ns:=/' + namespace_value],
        output='screen',
    )

    return [keyboard_control_node,joystick_control_node]