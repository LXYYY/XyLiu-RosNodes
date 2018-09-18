//
//  loop_closure.cpp
//  loop_closure
//
//  Created by Xiangyu Liu on 18/9/18.
//  Copyright © 2018 Xiangyu Liu. All rights reserved.
//

#include "loop_closure.hpp"
#include <iostream>


#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/String.h>
//#include <opencv-3.3.1-dev/opencv2/opencv.hpp>

#include <opencv2/opencv.hpp>
#include <DBoW3/DBoW3.h>

using namespace cv;
using namespace std;

////orb dbow initialization
vector<Mat> descriptors;
Ptr< Feature2D > detector = ORB::create();

int nImages=0;

void imageCallback(const sensor_msgs::ImageConstPtr &msg)
{
    cout<<"running"<<endl;
    try{
        Mat raw_image=cv_bridge::toCvShare(msg, "bgr8")->image;
        imshow("test",raw_image);
        
        waitKey(1);
        
        vector<KeyPoint> keypoints;
        Mat descriptor;
        detector->detectAndCompute( image, Mat(), keypoints, descriptor );
        descriptors.push_back( descriptor );
        
        
        
//        if(nImages<1000){
//            cout<<"extract total "<<descriptors.size()*500<<" features."<<endl;
//
//            cout<<"creating vocabulary, please wait ... "<<endl;
//            DBoW3::Vocabulary vocab;
//            vocab.create( descriptors );
//            cout<<"vocabulary info: "<<vocab<<endl;
//            vocab.save( "vocab_larger.yml.gz" );
//            cout<<"done"<<endl;
//        }
//        else {
//            vector<KeyPoint> keypoints;
//            Mat descriptor;
//            detector->detectAndCompute(raw_image, Mat(), keypoints, descriptor);
//            descriptors.push_back(descriptor);
//            cout<<"extracting features frome frame "<<nImages++<<endl;
//        }
    }
    catch(cv_bridge::Exception &e){
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.",msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"loop_closure");
    
    ros::NodeHandle n;
    
    cout<<"reading database"<<endl;
    DBoW3::Vocabulary vocab("./vocabulary.yml.gz");
    
    if ( vocab.empty() )
    {
        cerr<<"Vocabulary does not exist."<<endl;
        return 1;
    }
    
    namedWindow("test");
    startWindowThread();
    
    image_transport::ImageTransport it(n);
    
    image_transport::Subscriber sub = it.subscribe("/camera/left/image_raw", 1, imageCallback);
    
    ros::spin();
    
    destroyWindow("test");
    
    return 0;
}

