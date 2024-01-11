#include <chrono>
#include <memory>
#include<unistd.h>
#include<math.h>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"

#include "servo3moog.h"
#include "kinema.h"

using std::placeholders::_1;

double constrain(double value, double min, double MAX)
{
    if (MAX >= value && value >= min) {
	return value;
    } else if (value > MAX) {
	return MAX;
    } else if (value < min) {
	return min;
    }
    return min;
}

void rotate_vect(double &vx, double &vy, double alpha){
    double temp_vx = vx;
    double temp_vy = vy;
    vx = cos(alpha)*temp_vx+sin(alpha)*temp_vy;
    vy = -sin(alpha)*temp_vx+cos(alpha)*temp_vy;
    //RCLCPP_INFO(this->get_logger(),"temp: vx=%.2f, vy = %.2f",temp_vx,temp_vy);
    //RCLCPP_INFO(this->get_logger(),"set: vx=%.2f, vy = %.2f",vx,vy);
}

double val_sign(double val){
    if(val<0.0){return -1.0;}
    else{return 1.0;}
}

double error_cmd(double prev_val,double wanted_val, double max_acc, double max_dcc,double dt){
    double error = wanted_val-prev_val;
    bool acceleration = (val_sign(wanted_val)==val_sign(prev_val) && abs(wanted_val)>abs(prev_val));
    if(acceleration){
        error = val_sign(error)*std::min(abs(error),dt*max_acc);
    }
    else{
        error = val_sign(error)*std::min(abs(error),dt*max_dcc);
    }
    return error;
}

class Bot_control_driver : public rclcpp::Node
{
  public:
    Bot_control_driver(): Node("bot_control_driver_node")
    {
        initialize_params();
        refresh_params();
        RCLCPP_INFO(this->get_logger(), "Sarted, connecting to motors...");
        rotor_rad_p_sec[0] = 0.0;
        rotor_rad_p_sec[1] = 0.0;
        rotor_rad_p_sec[2] = 0.0;
        while(servomotor_setup(device_name.c_str()) <0){
            //RCLCPP_ERROR(this->get_logger(), "Port tested: "+device_name);
            RCLCPP_ERROR(this->get_logger(), "Please plug servomotors and restart..");
            sleep(1);
        }
        RCLCPP_INFO(this->get_logger(), "Set up finished.");
        subscription= this->create_subscription<geometry_msgs::msg::Twist>("cmd_vel_apply", 10, std::bind(&Bot_control_driver::topic_callback, this, _1));
        timer_ = create_wall_timer(std::chrono::milliseconds(10), std::bind(&Bot_control_driver::timeout_secu, this));
        RCLCPP_INFO(this->get_logger(), "\033[%dm\033[2J\033[1;1f",0);
        RCLCPP_INFO(this->get_logger(), "ROBOT DRIVER:");
        RCLCPP_INFO(this->get_logger(), "Waiting commands...");
        
        clock = this->get_clock();
        t0 = clock->now();

        // Register the shutdown callback
        /*rclcpp::on_shutdown([this]() {
            customShutdownHandler();
        });*/

        // Initialize publisher for odometry feedback
        odometry_publisher_ = this->create_publisher<nav_msgs::msg::Odometry>("/odom_simu", 10);
    }

  private:
    //functions
    void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        callback_active = 0; //update the security variable
        //MAIN CODE to get commands
        //RCLCPP_INFO(this->get_logger(), "new commands: vx:'%f' vy:'%f' w:'%f' ", msg->vx, msg->vy, msg->w);
        this->vx = msg->linear.x;
        this->vy = msg->linear.y;
        this->w = msg->angular.z;
        apply_commands(true);
    }

    void timeout_secu(){
        callback_active ++;
        if(callback_active >= iteration_to_reset){
            apply_commands(false);
            callback_active = 0;//just to avoid to hit int values limit on computer, we restart the count so the bot is reset continiously each amount of time
        }
    }

    void apply_commands(bool armed)
    {
        float des_velx = 0.0;
        float des_vely = 0.0;
        float des_velw = 0.0;

        RCLCPP_INFO(this->get_logger(), "\033[%dm\033[2J\033[1;1f",0);
        RCLCPP_INFO(this->get_logger(), "ROBOT DRIVER:");

        bool apply_cmd = callback_active < iteration_to_reset;

        //received commands should be percentage (0->1)
        if(apply_cmd && armed)
        {
            des_velx = this->vx*MAX_VX;
            des_vely = this->vy*MAX_VY;
            des_velw = this->w*MAX_W;
            RCLCPP_INFO(this->get_logger(), "Command applied");
        }
        
        //security
        des_velx = constrain(des_velx, -MAX_VX, MAX_VX);
        des_vely = constrain(des_vely, -MAX_VY, MAX_VY);
        des_velw = constrain(des_velw, -MAX_W, MAX_W);

        if(!armed){
            RCLCPP_INFO(this->get_logger(), "Unarmed, Waiting commands...");
        }

        RCLCPP_INFO(this->get_logger(), "Goal: Vx:%.2f Vy:%.2f Vw:%.2f ",des_velx,des_vely,des_velw);

        //we rotate the speed vector to match wanted frame for the robot
        double new_des_velx = -des_velx; //we flip x axis
        double new_des_vely = des_vely;
        double alpha = M_PI/3;//5*M_PI/6;
        rotate_vect(new_des_velx, new_des_vely, alpha);

        //RCLCPP_INFO(this->get_logger(), "Vx:%.2f Vy:%.2f Vw:%.2f ",new_des_velx,new_des_vely,des_velw);
        
        ///////////////////////////
        //Accelerations mangement//
        ///////////////////////////
        double wanted_vals[3] = {new_des_velx, new_des_vely, des_velw};
        tf = clock->now();
        double dt = (tf-t0).seconds();
        t0 = clock->now();
        double errors_vals[3];

        //max acc and decc are converted to percentage equivalence
        errors_vals[0] = error_cmd(former_vals[0],wanted_vals[0],MAX_AX/MAX_VX,MAX_DX/MAX_VX,dt);
        errors_vals[1] = error_cmd(former_vals[1],wanted_vals[1],MAX_AY/MAX_VY,MAX_DY/MAX_VY,dt);
        errors_vals[2] = error_cmd(former_vals[2],wanted_vals[2],MAX_AW/MAX_W,MAX_DW/MAX_W,dt);

        //RCLCPP_INFO(this->get_logger(), "dt: %.2f ",dt);
        //RCLCPP_INFO(this->get_logger(), "errors_vals: Vx:%.2f Vy:%.2f Vw:%.2f ",errors_vals[0], errors_vals[1], errors_vals[2]);

        //////////////////
        //Apply commands//
        //////////////////
        
        new_des_velx = former_vals[0]+errors_vals[0];
        new_des_vely = former_vals[1]+errors_vals[1];
        des_velw = former_vals[2]+errors_vals[2];

        //we publish odometry feedback, but we need to have them in global frame again
        double global_vx = new_des_velx;
        double global_vy = new_des_vely;
        rotate_vect(global_vx, global_vy, -alpha); //we unrotate
        publishOdometry(-global_vx, global_vy, des_velw); //we unflip and publish
 
        // convert vehicle vel. -> rotor vel.
        if(former_vals[0] != new_des_velx || former_vals[1] != new_des_vely || former_vals[2] != des_velw){
            RCLCPP_INFO(this->get_logger(), "Actual(bot frame): Vx:%.2f Vy:%.2f Vw:%.2f ",new_des_velx, new_des_vely, des_velw);
            vel2rotor(this->rotor_rad_p_sec, new_des_velx, new_des_vely, des_velw);
            // move motor
            this->rotor_rad_per_sec_gain = servo3motor_loop (this->rotor_rad_p_sec, this->target_motor_rpm, this->real_motor_rpm);
            former_vals[0] = new_des_velx;
            former_vals[1] = new_des_vely;
            former_vals[2] = des_velw;
        }
        else{
            RCLCPP_INFO(this->get_logger(), "Done.");
        }
    }

    void initialize_params(){
      this->declare_parameter("device_name"); 
    }

    void refresh_params(){
        this->get_parameter_or<std::string>("device_name",device_name,"/dev/ttyUSB0");
    }

    void publishOdometry(double vx, double vy, double w)
    {
        // populate the Odometry message
        estimated_odom.header.stamp = clock->now();
        estimated_odom.header.frame_id = "odom";
        estimated_odom.child_frame_id = "base_link";

        // Set twist (linear and angular velocities)
        double kx = 1.0;
        double ky = 1.0;
        double kw = 0.86;
        estimated_odom.twist.twist.linear.x = vx*kx;
        estimated_odom.twist.twist.linear.y = vy*ky;
        estimated_odom.twist.twist.angular.z = w*kw;

        // Set pose information
        // no pose information needed, so we don't update

        // Set the covariance matrix values
        // Assuming we want to set the same covariance for all elements
        double covariance_value = 0.0;
        for (size_t i = 0; i < 36; ++i) {
            estimated_odom.pose.covariance[i] = covariance_value;
            estimated_odom.twist.covariance[i] = covariance_value;
        }

        RCLCPP_INFO(this->get_logger(), "Actual(world): Vx:%.2f Vy:%.2f Vw:%.2f ",estimated_odom.twist.twist.linear.x,estimated_odom.twist.twist.linear.y,estimated_odom.twist.twist.angular.z);

        // Publish the Odometry message
        odometry_publisher_->publish(estimated_odom);
    }


    //global variables
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription; 
    float rotor_rad_p_sec[3] = {0.0, 0.0, 0.0}; // rev. per sec
    float vx = 0;
    float vy = 0;
    int iteration_to_reset = 100;
    float w = 0;
    int callback_active = 0;
    float rotor_rad_per_sec_gain = 1.0;
    float target_motor_rpm[6];
    float real_motor_rpm[6];
    rclcpp::TimerBase::SharedPtr timer_;
    std::string device_name;

    double former_vals[3] = {0.0, 0.0, 0.0};
    double MAX_AX= 0.3*MAX_VX; //should be >0 m.s-2 Acceleration
    double MAX_AY= 0.3*MAX_VY; //should be >0 m.s-2
    double MAX_AW= 0.5*MAX_W; //should be >0 rad.s-2
    double MAX_DX= 1*MAX_VX; //should be >0 m.s-2  Deceleration
    double MAX_DY= 1*MAX_VY; //should be >0 m.s-2
    double MAX_DW= 1*MAX_W; //should be >0 rad.s-2
    rclcpp::Clock::SharedPtr clock;
    rclcpp::Time t0;
    rclcpp::Time tf;

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odometry_publisher_;
    nav_msgs::msg::Odometry estimated_odom;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Bot_control_driver>());
  rclcpp::shutdown();
  return 0;
}