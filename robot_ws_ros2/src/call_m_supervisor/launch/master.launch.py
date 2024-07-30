import launch
import launch_ros
import os
import xacro
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration
import re
from launch.actions import DeclareLaunchArgument, OpaqueFunction


def remove_comments(text):
    pattern = r'<!--(.*?)-->'
    return re.sub(pattern, '', text, flags=re.DOTALL)

def generate_launch_description():

    # Declare the launch arguments
    declare_param_namespace = DeclareLaunchArgument(
        'namespace',
        default_value='callm_no_id',
        description='namespace of robot'
    )

    # Use OpaqueFunction to call the function that uses the parameter
    processes_to_launch = OpaqueFunction(function=run_launch_with_parameters)

    return launch.LaunchDescription([
        launch.actions.DeclareLaunchArgument(name='use_sim_time', default_value="False",description='Flag to enable use_sim_time'),
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
    pkg_share = launch_ros.substitutions.FindPackageShare(package='call_m_supervisor').find('call_m_supervisor') 
    bot_model_subpath = 'description/bot/bot_description.urdf.xacro'
    # Use xacro to process the file
    xacro_file = os.path.join(pkg_share,bot_model_subpath)
    robot_description_raw = remove_comments(xacro.process_file(xacro_file).toxml())

    #we get parameters values
    namespace_value = context.launch_configurations['namespace']

    command_master_node = launch_ros.actions.Node(
       package='call_m_supervisor',
       executable='command_master_node',
       name='command_master_node',
       output='screen',
       namespace=namespace_value,
    )
    #command_master_node =  launch.actions.ExecuteProcess(cmd=cmd_debug + ['call_m_supervisor', 'command_master_node'], output='screen')

    # node to publish TFs of robot model
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{'robot_description': robot_description_raw,
        'use_sim_time': LaunchConfiguration('use_sim_time')}], # add other parameters here if required
        namespace=namespace_value
    )

    laserscan_toolbox_params_file = replace_namespace_in_file(os.path.join(pkg_share,"config/laserscan_toolbox_params.yaml"),namespace_value,os.path.join(pkg_share,"config/generated_laserscan_toolbox_params.yaml"))
    laserscan_toolbox=Node(
        parameters=[
          laserscan_toolbox_params_file,
          {'use_sim_time': LaunchConfiguration('use_sim_time')}
        ],
        package = 'multi-laserscan-toolbox-ros2',
        name = 'laserscan_toolbox_node',
        executable = 'laserscan_toolbox_node',
        output='screen',
        namespace=namespace_value
    )

    # node to publish /clock used to ensure that robot's computer are synced even wihout internet connection (not needed if using chrony)
    clock_sync_publisher = Node(
        package='call_m_supervisor',
        executable='clock_sync_node',
        output='screen',
        parameters=[{'sim_time': LaunchConfiguration('use_sim_time')}],
        namespace=namespace_value # add other parameters here if required
    )

    #joint states published by Gazebo for the simulation and by Hardware launch if hardware

    #Depth to scan converters
    #ros2 launch depth-filter-scan-converter depth_filter_scan_converter.launch.py use_sim_time:=true
    depth_converter_params_file1 = replace_namespace_in_file(os.path.join(pkg_share,"config/depth_filter_scan_converter_params1.yaml"),namespace_value,os.path.join(pkg_share,"config/generated_depth_filter_scan_converter_params1.yaml"))
    depth_converter1 = Node(
        parameters=[
          depth_converter_params_file1,
          {'use_sim_time': LaunchConfiguration('use_sim_time')}
        ],
        package = 'depth-filter-scan-converter',
        name = 'depth_filter_scan_converter_node1',
        executable = 'depth_filter_scan_converter_node',
        output='screen',
        namespace=namespace_value
    )

    depth_converter_params_file2 = replace_namespace_in_file(os.path.join(pkg_share,"config/depth_filter_scan_converter_params2.yaml"),namespace_value,os.path.join(pkg_share,"config/generated_depth_filter_scan_converter_params2.yaml"))
    depth_converter2 = Node(
        parameters=[
          depth_converter_params_file2,
          {'use_sim_time': LaunchConfiguration('use_sim_time')}
        ],
        package = 'depth-filter-scan-converter',
        name = 'depth_filter_scan_converter_node2',
        executable = 'depth_filter_scan_converter_node',
        output='screen',
        namespace=namespace_value
    )

    return [
        depth_converter1,
        depth_converter2,
        laserscan_toolbox,
        command_master_node,
        node_robot_state_publisher,
        clock_sync_publisher
        ]