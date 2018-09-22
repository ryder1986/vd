﻿#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include "tool.h"
#ifdef IS_UNIX
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#endif
#ifdef IS_WIN
    #include <opencv2/opencv.hpp>
#endif
#if 1
#define USE_CVCAP 1
//#undef USE_CVCAP

#ifdef  USE_CVCAP
#else
#include "ffmpegvideocapture.h"
#endif
using namespace std;
using namespace cv;
class VideoSource
{
public:
    VideoSource(string path);
    VideoSource(string path,bool only_key_frame);
     ~VideoSource();
    inline string get_url()
    {
        return url;
    }
    bool get_size(int &w,int &h)
    {
        bool ret=false;
        if(vcap.isOpened()){
            ret=true;
            w=vcap.get(CV_CAP_PROP_FRAME_WIDTH);
            h=vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
        }
        return ret;
    }
    bool end_with_str(string str,string end_str)
    {
        bool ret=false;
        if(str.size()>end_str.size()){
            string st= str.substr(str.size()-end_str.size(),end_str.size());
            return st==end_str;
        }
        return ret;
    }
    bool get_frame(Mat &frame)
    {
        if(get_pic(frame))
            return true;
        int ret=false;
        lock.lock();
        if(frame_list.size()>0){
            frame_list.front().copyTo(frame);
            frame_list.erase(frame_list.begin());
            ret=true;
        }else{
            ret=false;
        }
        lock.unlock();
        return ret;
    }

    inline  bool get_pic(Mat &frame)
    {

        bool ret=false;
        if(is_pic)
        {
            png_frame.copyTo(frame);
            this_thread::sleep_for(chrono::milliseconds(100));
            if(png_frame.cols>0)
                ret=true;
        }
        return ret;

    }

    bool get_frame(Mat &frame, int &timestamp)
    {
        if(get_pic(frame))
            return true;


        bool ret=false;
        lock.lock();

        if(frame_list.size()>0){
            frame_list.front().copyTo(frame);


            //            int start_time=get_ms();
            //  prt(info,"%d,%d",frame.cols,frame.rows);
            //           resize(frame,frame,Size( 640,480),CV_INTER_LINEAR);
            //           int end_time=get_ms();
            //           prt(info,"%d",end_time-start_time);


            frame_list.erase(frame_list.begin());
            timestamp=cur_ms_list.front();
            cur_ms_list.erase(cur_ms_list.begin());
            ret=true;
        }else{
            ret=false;
        }
        lock.unlock();
        return ret;
    }
private:
    void run();
    void check_point()
    {
        if(vcap.isOpened()){
            double w= vcap.get(CV_CAP_PROP_POS_FRAMES);
        }
    }
private:
#ifdef USE_CVCAP
    VideoCapture  vcap;
#else
    FfmpegVideoCapture vcap;
#endif
    //  PdVideoCapture vcap;
    //  VideoCapture vcap;
    vector <Mat> frame_list;
    vector <int> cur_ms_list;

    int frame_wait_time;
    mutex lock;
    int frame_rate;
    string url;
    volatile bool quit_flg;
    Timer1 t1;

    thread *src_trd;
    Mat png_frame;
    bool is_pic;
    int try_times;

    bool only_key_frame;
};
#endif
#endif // VIDEOSOURCE_H
