



void gps_buffer_helper(ROS_IO_Manager* pRIM,CallbackBufferBlock<sensor_msgs::NavSatFix>& nav_buffer,
		const boost::shared_ptr<sensor_msgs::NavSatFix const>& nav_msg)
{
    bool locked = pRIM->slam_buf_mutex.try_lock();
    if(!locked)
    {
        return;
    }
    cout<<"GPS message received!"<<endl;
    nav_msg->header.stamp = ros::Time::now();

    nav_buffer.onCallbackBlock(*nav_msg);
    pRIM->_gps_pos_update = true;
}

void slam_buffer_helper(ROS_IO_Manager* pRIM,CallbackBufferBlock<geometry_msgs::PoseStamped>& slam_buffer,
		const boost::shared_ptr<geometry_msgs::PoseStamped const>& slam_msg)
{
    if (pRIM->getGraph() == nullptr)
    {
        cout<<"waiting for gog init.pass this msg."<<endl;
        return;
    }
    cout<<"SLAM message received!"<<endl;
    bool state = ( pRIM->getGraph()->getStatus() & 1 );
    cout<<"Is running with gps:"<<state<<endl;
    slam_buffer.onCallbackBlock(*slam_msg);
    visualization_msgs::Marker slam_marker;
    slam_marker.header.frame_id = "world";
    slam_marker.header.stamp = ros::Time::now();
    slam_marker.type = visualization_msgs::Marker::ARROW;
    slam_marker.action = visualization_msgs::Marker::ADD;
    auto q_ = slam_msg->pose.orientation;
    
    slam_marker.pose.orientation.x = q_.x;
    slam_marker.pose.orientation.y = q_.y;
    slam_marker.pose.orientation.z = q_.z;
    slam_marker.pose.orientation.w = q_.w;
    
    slam_marker.scale.x = 4;
    slam_marker.scale.y = 2;
    slam_marker.scale.z = 1;
    slam_marker.color.r = 0;
    slam_marker.color.g = 0;
    slam_marker.color.b = 1;
    slam_marker.color.a = 1;
    pRIM->publish_slam_marker(slam_marker);
    pRIM->_slam_msg_update = true;
}

void velocity_buffer_helper(ROS_IO_Manager* pRIM, CallbackBufferBlock<geometry_msgs::TwistStamped>& velocity_buffer,const boost::shared_ptr<geometry_msgs::TwistStamped const>& velocity_msg)
{
    cout<<"Velocity msgs received!"<<endl;
    velocity_buffer.onCallbackBlock(*velocity_msg);
    pRIM->_gps_vel_update = true;
}


void ahrs_buffer_helper(ROS_IO_Manager* pRIM, CallbackBufferBlock<nav_msgs::Odometry>& ahrs_buffer,
		const boost::shared_ptr<nav_msgs::Odometry const>& ahrs_msg)
{
    cout<<"AHRS message received!"<<endl;
    ahrs_buffer.onCallbackBlock(*ahrs_msg);
    visualization_msgs::Marker ahrs_marker;
    ahrs_marker.header.frame_id = "world";
    ahrs_marker.header.stamp = ros::Time::now();
    ahrs_marker.type = visualization_msgs::Marker::ARROW;
    ahrs_marker.action = visualization_msgs::Marker::ADD;
    auto  q_ = ahrs_msg->pose.pose.orientation;
    ahrs_marker.pose.orientation.x = q_.x;
    ahrs_marker.pose.orientation.y = q_.y;
    ahrs_marker.pose.orientation.z = q_.z;
    ahrs_marker.pose.orientation.w = q_.w;
    
    ahrs_marker.scale.x = 4;
    ahrs_marker.scale.y = 2;
    ahrs_marker.scale.z = 1;
    ahrs_marker.color.r = 0;
    ahrs_marker.color.g = 1;
    ahrs_marker.color.b = 0;
    ahrs_marker.color.a = 1;
    pRIM->publish_ahrs_marker(ahrs_marker);
}



