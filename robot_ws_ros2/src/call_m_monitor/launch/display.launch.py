import launch
from launch.substitutions import Command, LaunchConfiguration
import launch_ros
import os
from launch.actions import DeclareLaunchArgument, OpaqueFunction

pkg_share = launch_ros.substitutions.FindPackageShare(package='call_m_monitor').find('call_m_monitor')
default_rviz_config_path = os.path.join(pkg_share, 'config/rviz/urdf_config_dynamic_namespace.rviz')

def generate_launch_description():

    # Declare the launch arguments
    declare_param_namespace = DeclareLaunchArgument(
        'namespace',
        default_value='callm_no_id',
        description='namespace of robot'
    )

    # Declare the launch arguments
    declare_param_rvizconfig = DeclareLaunchArgument(
        'rvizconfig',
        default_value='none',
        description='Absolute path to rviz config file'
    )

    # Use OpaqueFunction to call the function that uses the parameter
    processes_to_launch = OpaqueFunction(function=run_launch_with_parameters)

    return launch.LaunchDescription([
        declare_param_namespace,
        declare_param_rvizconfig,
        processes_to_launch
    ])

def replace_namespace_in_rviz_config(template_path, namespace, out_path):
    with open(template_path, 'r') as file:
        config_content = file.read()

    config_content = config_content.replace('{{namespace}}', namespace)

    with open(out_path, 'w') as file:
        file.write(config_content)

    return out_path

def run_launch_with_parameters(context):
    #we get parameters values
    namespace_value = context.launch_configurations['namespace']
    rvizconfig_value = context.launch_configurations['rvizconfig']
    new_file_path = replace_namespace_in_rviz_config(default_rviz_config_path,namespace_value,os.path.join(pkg_share,'config/rviz/generated_file.rviz'))
    if(rvizconfig_value != 'none'):
        new_file_path = replace_namespace_in_rviz_config(rvizconfig_value,namespace_value,os.path.join(pkg_share,'config/rviz/generated_file.rviz'))
        

    #node to launch Rviz with
    rviz_node = launch_ros.actions.Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', new_file_path],
        namespace=namespace_value,
    )

    return [rviz_node]