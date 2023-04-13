#include "ros/ros.h"
#include "classwork4_pkg/my_transformation.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
#include <tf/transform_broadcaster.h>

 
using namespace std;

int main(int argc, char **argv) {

	ros::init(argc, argv, "my_transformation_client");
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<classwork4_pkg::my_transformation>("my_transformation");
	
	classwork4_pkg::my_transformation srv;
//	srv.request.frame_a.data = "base_link";   
//	srv.request.frame_b.data = "end_effector";   

    if (argc != 1 && argc != 3){
        ROS_ERROR("Incorrect number of paramaters: please specify base frame and dynamic frame");
        return -1;
    }
    else if (argc == 1){             // di default usa la trasformazione base -> end effector
	    srv.request.frame_a.data = "base_link";   
	    srv.request.frame_b.data = "end_effector";  
    }
    else if(argc == 3){
        srv.request.frame_a.data = argv[1];   
        srv.request.frame_b.data = argv[2];   
    }
        ROS_INFO("Waiting for the client server");
        client.waitForExistence();
        ROS_INFO("Client server up now");

        tf::TransformBroadcaster br;
        tf::Transform transform;

        ros::Rate r(10);
        while (ros::ok()) {
            if (!client.call(srv)) {
                ROS_ERROR("Error calling the service");
                return 1;
            }
            cout << "Service output: " << srv.response.pose_a_b << endl;
            
            transform.setOrigin(tf::Vector3(srv.response.pose_a_b.position.x, srv.response.pose_a_b.position.y, srv.response.pose_a_b.position.z));
            tf::Quaternion q(srv.response.pose_a_b.orientation.x,srv.response.pose_a_b.orientation.y,srv.response.pose_a_b.orientation.z,srv.response.pose_a_b.orientation.w);
            transform.setRotation(q);
            br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), srv.request.frame_a.data, "dynamic_tf"));
            r.sleep();
        }
	return 0;
}
