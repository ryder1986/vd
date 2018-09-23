#ifndef DETECTREGION_DATA_H
#define DETECTREGION_DATA_H
#include "tool.h"
#include "jsonpacket.h"
class DataEvent
{
public:
    enum Event{
        VerTriggered=1,
        VersTriggered
    };
    DataEvent() {}
public:
    virtual bool start_event(VdPoint pnt)=0;
    virtual bool process_event(VdPoint pnt)=0;
    void end_event()
    {
       triggered=false;
    }
public:
    int flag;
    bool triggered;
    VdPoint ori_pnt;
    int point_index;
};
class DetectRegionInputData:public JsonData,public DataEvent
{

public:
    string SelectedProcessor;
    vector <VdPoint>ExpectedAreaVers;
    JsonPacket ProcessorData;//TODO: dynamic binding
    DetectRegionInputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    DetectRegionInputData(JsonPacket pdata,string type,vector<VdPoint> vers)
        :ProcessorData(pdata),SelectedProcessor(type),ExpectedAreaVers(vers)
    {
        encode();
    }

    void set_point(VdPoint p,int index)
    {
        ExpectedAreaVers[index-1]=p;
        encode();
    }

    void set_points(vector <VdPoint> p)
    {
        ExpectedAreaVers=p;
        encode();
    }

    void set_processor_data(JsonPacket p)
    {
        ProcessorData=p;
        encode();
    }

    void set_processor(string p,JsonPacket pd)
    {
        SelectedProcessor=p;
        ProcessorData=pd;
        encode();
    }
    void operator =(DetectRegionInputData dt)
    {
        config=dt.config;
        decode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
        DECODE_PKT_MEM(ProcessorData);
        DECODE_STRING_MEM(SelectedProcessor);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
        ENCODE_STRING_MEM(SelectedProcessor);
        ENCODE_PKT_MEM(ProcessorData);
    }

    inline bool p_on_ver(VdPoint pnt)
    {
        point_index=2;
        return true;
    }

    inline bool p_on_line(VdPoint pnt)
    {
        return true;
    }

    virtual bool start_event(VdPoint pnt)
    {
        if(p_on_ver(pnt)){
            triggered=true;
            ori_pnt=pnt;
            flag=VerTriggered;
            return true;
        }
        if(p_on_line(pnt)){
            triggered=true;
            ori_pnt=pnt;
            flag=VersTriggered;
            return true;
        }
        return false;
    }
    virtual bool process_event(VdPoint pnt)
    {

        if(triggered){
            switch (flag) {
            case Event::VerTriggered:
                ExpectedAreaVers[point_index]=pnt;
                break;
            case Event::VersTriggered:
            {
                int offx=pnt.x-ori_pnt.x;
                int offy=pnt.y-ori_pnt.y;
                ori_pnt=pnt;
                int i=0;
                int sz=ExpectedAreaVers.size();
                for(i=0;i<sz;i++){
                    ExpectedAreaVers[i]=VdPoint(ExpectedAreaVers[i].x+offx,ExpectedAreaVers[i].y+offy);
                }
                break;
            }
            default:
                break;
            }
            encode();
        }
        return true;

    }

};
class ProcessorDataJsonDataRequest:public JsonData{
public:
    string SelectedProcessor;
    JsonPacket ProcessorData;
    ProcessorDataJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    ProcessorDataJsonDataRequest(string pro,JsonPacket d)
    {
        ProcessorData=d;
        SelectedProcessor=pro;
        encode();
    }
    void decode()
    {
        DECODE_STRING_MEM(SelectedProcessor);
        DECODE_PKT_MEM(ProcessorData);
    }
    void encode()
    {
        ENCODE_STRING_MEM(SelectedProcessor);
        ENCODE_PKT_MEM(ProcessorData);
    }
};
class AreaVersJsonDataRequest:public JsonData{
public:
    vector <VdPoint>ExpectedAreaVers;
    AreaVersJsonDataRequest(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }
    AreaVersJsonDataRequest(vector <VdPoint> vs)
    {
        ExpectedAreaVers.assign(vs.begin(),vs.end());
        encode();
    }
    void decode()
    {
        DECODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
    }
    void encode()
    {
        ENCODE_JSONDATA_ARRAY_MEM(ExpectedAreaVers);
    }
};
class DetectRegionOutputData:public JsonData
{
public:
    VdRect DetectionRect;
    JsonPacket Result;
    DetectRegionOutputData(JsonPacket pkt):JsonData(pkt)
    {
        decode();
    }

    DetectRegionOutputData(JsonPacket rst ,VdRect rct)
    {
        DetectionRect=rct;
        Result=rst;
        encode();
    }

    void decode()
    {
        DECODE_PKT_MEM(DetectionRect);
        DECODE_PKT_MEM(Result);
    }

    void encode()
    {
        ENCODE_JSONDATA_MEM(DetectionRect);
        ENCODE_PKT_MEM(Result);
    }

};


#endif // DETECTREGION_DATA_H
