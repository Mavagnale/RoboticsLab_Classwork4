#include "ros/ros.h"
#include "classwork4_pkg/my_transformation.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
#include <tf/transform_listener.h>

 
using namespace std;

bool service_callback( classwork4_pkg::my_transformation::Request &req, classwork4_pkg::my_transformation::Response &res) {
	cout << "Ricevuta richiesta, frame a : " << req.frame_a.data << endl;
	cout << "Ricevuta richiesta, frame b : " << req.frame_b.data << endl;

    tf::TransformListener listener;
    tf::StampedTransform transform;
    try {
        listener.waitForTransform(req.frame_a.data, req.frame_b.data, ros::Time(0), ros::Duration(3.0));
        listener.lookupTransform(req.frame_a.data, req.frame_b.data, ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
        ROS_ERROR("%s",ex.what());
        ros::Duration(1.0).sleep();
    }

    ROS_INFO_STREAM(" Transform: " << 
        transform.getOrigin().x() << ", " << 
        transform.getOrigin().y() << ", " <<
        transform.getOrigin().z() << ", " << 
        transform.getRotation().x() << ", " << 
        transform.getRotation().y() << ", " << 
        transform.getRotation().z()
    );


    res.pose_a_b.position.x = transform.getOrigin().x();
    res.pose_a_b.position.y = transform.getOrigin().y();
    res.pose_a_b.position.z = transform.getOrigin().z();
    res.pose_a_b.orientation.x = transform.getRotation().getX();
    res.pose_a_b.orientation.y = transform.getRotation().getY();
    res.pose_a_b.orientation.z = transform.getRotation().getZ();
    res.pose_a_b.orientation.w = transform.getRotation().getW();

	return true;
}


int main(int argc, char **argv) {

	ros::init(argc, argv, "my_transformation_server");
	ros::NodeHandle n;

	//Initialize the service object: name of the service and callback function
	//	Like subscribers, also tje callback function can be declared as a class function
	ros::ServiceServer service = n.advertiseService("my_transformation", service_callback);

	//Call the spin function to maintain the node alive
	ros::spin();

	return 0;
}
