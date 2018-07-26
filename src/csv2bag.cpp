#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Vector3.h>
//#include <vb_util_lib/am_geometry.h> // quanternion to RPY
#include <frame_names.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <mocap_csv2bag/StampedInterruptTime.h>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

int main(int argc, char **argv)
{
	geometry_msgs::PoseWithCovarianceStamped optitrack_pose;
	geometry_msgs::Vector3 rpy_msg;
	rosbag::Bag bag_read;
	rosbag::Bag bag;
	std::string motive_csv_file;
	std::string motive_bag_file;
	std::string mocap_bag_file_read;
	std::string synchronizer_topic;
	double interrupt_time = 0.0;

	ros::init(argc, argv, "csv2bag");
	ROS_INFO("TRANSLATE CSV FILE FROM MOTIVE TO ROS BAG FILE.");
	ros::start();

	ros::param::param<std::string>("~mocap_bag_file_read", mocap_bag_file_read, "/home/wt70707/testdata/LG-0001-2018-04-13-21-43-27.bag");
	ros::param::param<std::string>("~synchronizer", synchronizer_topic, "/interrupt_time");
	ros::param::param<std::string>("~motive_csv_file", motive_csv_file, "/home/wt70707/testdata/test.csv");
	ros::param::param<std::string>("~motive_bag_file", motive_bag_file, "/home/wt70707/testdata/test.bag");

	// read LG bag file to get the /interrupt_time
	ROS_INFO_STREAM("Read bag: " << mocap_bag_file_read);
	bag_read.open(mocap_bag_file_read, rosbag::bagmode::Read);

	std::vector<std::string> topics;
	topics.push_back(synchronizer_topic);

	rosbag::View view(bag_read, rosbag::TopicQuery(topics));

	foreach(rosbag::MessageInstance const m, view)
	{
		mocap_csv2bag::StampedInterruptTime::ConstPtr interrupt_time_t = m.instantiate<mocap_csv2bag::StampedInterruptTime>();
		if (interrupt_time_t != NULL)
		{
	        interrupt_time = interrupt_time_t->data;

	        ROS_INFO_STREAM("Interrupt time: " << std::fixed << std::setprecision(9) << interrupt_time);
	        ROS_INFO_STREAM("Time stamp of interrupt time: " << interrupt_time_t->header.stamp);
		}
	}

	bag_read.close();

	// translate csv file tot bag file
	ROS_INFO_STREAM(motive_csv_file);
	std::ifstream ifile(motive_csv_file);
	ROS_INFO_STREAM(motive_bag_file);
	bag.open(motive_bag_file, rosbag::bagmode::Write);

    std::string line; // we read the full line here
    std::getline(ifile, line); //read the first line
//    std::istringstream iss{line}; // construct a string stream from line
//	// read the tokens from current line separated by comma
//	std::vector<std::string> tokens; // here we store the tokens
//	std::string token; // current token
//	for (int i = 0; i < 10; i++)
//	{
//		std::getline(iss, token, ',');
//		tokens.push_back(token); // add the token to the vector
//	}
//	std::string start_time_str = tokens.at(9); // if system time is required, adjust data time with this time; it is only with precision of seconds.
//	std::cout << "The start time of logging data on Motive: " << start_time_str << std::endl;
//	std::tm t = {};
//	std::time_t start_time_epoch;
//	if (start_time_str.substr(20, 2) == "PM")
//	{
//		start_time_str = start_time_str.substr(0,11) + std::to_string(std::stoi(start_time_str.substr(11, 2)) + 12) + start_time_str.substr(13, 6);
//		//std::cout << start_time_str << "\n";
//	}
//	else if (start_time_str.substr(20, 2) != "AM")
//	{
//		std::cout << "Wrong locale" << "\n";
//		return -1;
//	}
//	std::istringstream ss(start_time_str);
//
//	if (ss >> std::get_time(&t, "%Y-%m-%d %H.%M.%S"))
//	{
//	   //std::cout << std::put_time(&t, "%c") << "\n" << std::mktime(&t) << "\n";
//	   start_time_epoch = std::mktime(&t);
//	   std::cout << "Start time in epoch: " << start_time_epoch << "\n";
//	}
//	else
//	{
//		std::cout << "Parse failed\n";
//	}

    int i = 0;
    for (i = 1; i < 7; i++) std::getline(ifile, line); // seek over the left header

    while (std::getline(ifile, line)) // read the current line
    {
        std::istringstream iss{line}; // construct a string stream from line
        // read the tokens from current line separated by comma
        std::vector<std::string> tokens; // here we store the tokens
        std::string token; // current token
        for (i = 0; i < 10; i++)
        {
			std::getline(iss, token, ',');
			if (token.empty()) break;
            tokens.push_back(token); // add the token to the vector
        }

        if (i < 10) break;

        optitrack_pose.header.seq = std::stoi(tokens[0]); // frame index in Motive log file
        //optitrack_pose.header.stamp = ros::Time(std::stod(tokens[1])); // frame time in Motive log file
        optitrack_pose.header.stamp = ros::Time(interrupt_time + std::stod(tokens[1]));
        //optitrack_pose.header.stamp = ros::Time(start_time_epoch + std::stod(tokens[1])); // frame time in Motive log file
        optitrack_pose.header.frame_id = Asset_Frame; // coordinate frame
        optitrack_pose.pose.pose.orientation.x = std::stod(tokens[2]); // orientation x
        //optitrack_pose.pose.pose.orientation.y = std::stod(tokens[3]); // orientation y
        optitrack_pose.pose.pose.orientation.y = -std::stod(tokens[4]); // orientation y
        //optitrack_pose.pose.pose.orientation.z = std::stod(tokens[4]); // orientation z
        optitrack_pose.pose.pose.orientation.z = std::stod(tokens[3]); // orientation z
        optitrack_pose.pose.pose.orientation.w = std::stod(tokens[5]); // orientation w
        optitrack_pose.pose.pose.position.x = std::stod(tokens[6]); // position x
        //optitrack_pose.pose.pose.position.y = std::stod(tokens[7]); // position y
        optitrack_pose.pose.pose.position.y = -std::stod(tokens[8]); // position y
        //optitrack_pose.pose.pose.position.z = std::stod(tokens[8]); // position z
        optitrack_pose.pose.pose.position.z = std::stod(tokens[7]); // position z
        optitrack_pose.pose.covariance[0] = std::stod(tokens[9]); // error per marker

        //Eigen::Quaterniond orientation;
        //orientation = Eigen::Quaterniond(optitrack_pose.pose.pose.orientation.w,
        //                                 optitrack_pose.pose.pose.orientation.x,
        //                                 optitrack_pose.pose.pose.orientation.y,
        //                                 optitrack_pose.pose.pose.orientation.z);
        //Eigen::Vector3d rpy = AM::quaternion_to_RPY(orientation);
        //rpy_msg.x = rpy[0];
        //rpy_msg.y = rpy[1];
        //rpy_msg.z = rpy[2];

        // we can now process the tokens, first display them
        //std::cout << "Tokenized line: ";
        //for (const auto& elem : tokens)
        //    std::cout << "[" << elem << "]";
        //std::cout << std::endl;
        //ROS_INFO_STREAM(start_time_epoch << " + " << std::stod(tokens[1]) << " = " << std::fixed << start_time_epoch + std::stod(tokens[1]) << "\n");

		bag.write("optitrack_pose", ros::Time(interrupt_time + std::stod(tokens[1])), optitrack_pose);
        //bag.write("optitrack_pose", ros::Time(interrupt_time + std::stod(tokens[1])), optitrack_pose);
    }

	bag.close();

    ros::shutdown();
}
