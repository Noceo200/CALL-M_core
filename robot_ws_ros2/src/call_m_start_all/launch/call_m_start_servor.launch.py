import launch
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    """
    DIRECTORIES
    """
    dir_nav2 = get_package_share_directory('call_m_nav2')
    dir_slam = get_package_share_directory('call_m_supervisor')

    """
    PARAMETERS
    """
    nav_type = "localize" #'none', 'on_fly' or 'localize'
    nav_mode = os.path.join(dir_nav2, 'config', 'nav2_params_omni.yaml') #'diff' or 'omni', path to config file
    nav_tree = os.path.join(dir_nav2, 'behavior_trees', 'navigate_w_replanning_and_recovery.xml') #behavior tree to use for navigation
    map_loc = os.path.join(dir_nav2, 'maps', 'fujyfhtf.yaml') #path to the map if using localization
    slam_param = os.path.join(dir_slam, 'config/mapper_params_online_async.yaml') #path to params for slam

    """
    LAUNCHES
    """
    #Command to launch xterm and execute ROS 2 launch commands
    cmd_debug = ['xterm', '-fn', 'xft:fixed:size=12', '-geometry', '60x20', '-e', 'ros2', 'launch']
    cmd = ['ros2', 'launch']
    suffix = ['use_sim_time:=false']

    # Construct the absolute paths to the launch files
    master_launch =  launch.actions.ExecuteProcess(cmd=cmd + ['call_m_supervisor', 'master.launch.py'] + suffix, output='screen')
    slam_launch = launch.actions.ExecuteProcess(cmd=cmd + ['slam_toolbox', 'online_async_launch.py', 'params_file:='+slam_param]+ suffix, output='screen')
    hardware_launch = launch.actions.ExecuteProcess(cmd=cmd + ['call_m_hardware', 'bot.launch.py'], output='screen')
    display_launch = launch.actions.ExecuteProcess(cmd=cmd + ['call_m_monitor', 'display.launch.py'], output='screen')
    nav2_launch = launch.actions.ExecuteProcess(cmd=cmd_debug + ['call_m_nav2', 'navigation_launch.py','params_file:='+nav_mode, 'default_bt_xml_filename:='+nav_tree] + suffix, output='screen')
    nav2_launch_loc1 = launch.actions.ExecuteProcess(cmd=cmd_debug + ['call_m_nav2', 'localization_launch.py','map:='+map_loc,'params_file:='+nav_mode, 'default_bt_xml_filename:='+nav_tree] + suffix, output='screen')
    nav2_launch_loc2 = launch.actions.ExecuteProcess(cmd=cmd_debug + ['call_m_nav2', 'navigation_launch.py','params_file:='+nav_mode,'map_subscribe_transient_local:=true', 'default_bt_xml_filename:='+nav_tree] + suffix, output='screen')
    teleop_launch =  launch.actions.ExecuteProcess(cmd=cmd + ['call_m_teleoperation', 'teleop.launch.py'], output='screen')
    if nav_type == "on_fly":
        return LaunchDescription([master_launch,slam_launch,hardware_launch,display_launch,nav2_launch])
        #return LaunchDescription([master_launch,slam_launch,hardware_launch])
    elif nav_type == "localize":
        return LaunchDescription([master_launch,hardware_launch,display_launch,nav2_launch_loc1,nav2_launch_loc2])
        #return LaunchDescription([master_launch,hardware_launch])
    else:
        return LaunchDescription([master_launch,slam_launch,hardware_launch,display_launch])
        #return LaunchDescription([master_launch,slam_launch,hardware_launch])
