#ifndef MVDPROCESSOR_H
#define MVDPROCESSOR_H

#include "videoprocessor.h"
#include "jsonpacket.h"
#include "algorithm.h"
#define TEST_CLIENT
#undef TEST_CLIENT
//#define  SAVE_VIDEO
#ifdef SAVE_VIDEO
Mat imgSave;
#endif
class MvdProcessor : public VideoProcessor
{
    m_args *p_cfg;
public:
    MvdProcessor(JsonPacket pkt)
    {
        int i = 0, j = 0;

        p_cfg = alg_mem_malloc();
        MvdProcessorInputData da(pkt);

        //pvd
        CPoint DetLine[2];
        DetLine[0].x = da.DetectLine[0].x;
        DetLine[0].y = da.DetectLine[0].y;
        DetLine[1].x = da.DetectLine[1].x;
        DetLine[1].y = da.DetectLine[1].y;

        //fvd
        for(i = 0; i < da.BasicCoil.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].x = da.BasicCoil[i].x;
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].y = da.BasicCoil[i].y;
        }

        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].x = da.BaseLine.BeginPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].y = da.BaseLine.BeginPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].x = da.BaseLine.EndPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].y = da.BaseLine.EndPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.base_length = da.BaseLine.Length;

        p_cfg->pDetectCfg->FvdDetectCfg.near_point_length = da.NearPointDistance;
        p_cfg->pDetectCfg->FvdDetectCfg.far_point_length = da.FarPointDistance;

        p_cfg->pDetectCfg->FvdDetectCfg.uLaneTotalNum = da.LaneData.size();
        for(i = 0; i < da.LaneData.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneNo = da.LaneData[i].LaneNo;
            for(j = 0; j < da.LaneData[i].FarArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].x = da.LaneData[i].FarArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].y = da.LaneData[i].FarArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].NearArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].x = da.LaneData[i].NearArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].y = da.LaneData[i].NearArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].LaneArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].x = da.LaneData[i].LaneArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].y = da.LaneData[i].LaneArea[j].y;
            }

        }
#ifndef TEST_CLIENT
        ArithInit(p_cfg->pCfgs, DetLine, &p_cfg->pDetectCfg->FvdDetectCfg, p_cfg->pParams);
#endif
    }
    ~MvdProcessor()
    {
        prt(info,"############free start");
#ifndef TEST_CLIENT
        alg_mem_free(p_cfg);
#endif
        prt(info,"############free done");
    }

    virtual bool modify_processor(JsonPacket p)
    {
        int i,j;
        MvdProcessorInputData da(p);
        //pvd
        CPoint DetLine[2];
        DetLine[0].x = da.DetectLine[0].x;
        DetLine[0].y = da.DetectLine[0].y;
        DetLine[1].x = da.DetectLine[1].x;
        DetLine[1].y = da.DetectLine[1].y;

        //fvd
        for(i = 0; i < da.BasicCoil.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].x = da.BasicCoil[i].x;
            p_cfg->pDetectCfg->FvdDetectCfg.BasicCoil[i].y = da.BasicCoil[i].y;
        }

        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].x = da.BaseLine.BeginPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[0].y = da.BaseLine.BeginPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].x = da.BaseLine.EndPoint.x;
        p_cfg->pDetectCfg->FvdDetectCfg.BaseLine[1].y = da.BaseLine.EndPoint.y;
        p_cfg->pDetectCfg->FvdDetectCfg.base_length = da.BaseLine.Length;

        p_cfg->pDetectCfg->FvdDetectCfg.near_point_length = da.NearPointDistance;
        p_cfg->pDetectCfg->FvdDetectCfg.far_point_length = da.FarPointDistance;

        p_cfg->pDetectCfg->FvdDetectCfg.uLaneTotalNum = da.LaneData.size();
        for(i = 0; i < da.LaneData.size(); i++)
        {
            p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneNo = da.LaneData[i].LaneNo;
            for(j = 0; j < da.LaneData[i].FarArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].x = da.LaneData[i].FarArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[j].y = da.LaneData[i].FarArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].NearArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].x = da.LaneData[i].NearArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[j].y = da.LaneData[i].NearArea[j].y;
            }
            for(j = 0; j < da.LaneData[i].LaneArea.size(); j++)
            {
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].x = da.LaneData[i].LaneArea[j].x;
                p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[j].y = da.LaneData[i].LaneArea[j].y;
            }

        }
#ifndef TEST_CLIENT
        RestParams(p_cfg->pCfgs, DetLine, &p_cfg->pDetectCfg->FvdDetectCfg, p_cfg->pParams);//���ò���
#endif
        return true;
    }
    virtual bool process_whole_pic(Mat img_src,JsonPacket &pkt,Rect rct)
    {
#ifdef SAVE_VIDEO
        img_src.copyTo(imgSave);
#endif
        //imshow("1",img_src);
        // prt(info,"%d %d",img_src.rows,img_src.cols);
        int i = 0;
        int x = 0, y = 0, w = 0, h = 0, c;
        string names;
        IplImage img = IplImage(img_src);
        CRect1 r1;
        r1.x=rct.x;
        r1.y=rct.y;
        r1.width=rct.width;
        r1.height=rct.height;
#ifndef TEST_CLIENT
        ArithProc_whole(&img, r1, p_cfg->p_outbuf, p_cfg->pCfgs, p_cfg->pParams);
#endif
        //pvd
        int PersonFlow1 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[0];
        int PersonFlow2 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[1];
        int CurrentPersionCount = p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;

        int BicycleFlow1 = p_cfg->p_outbuf->PVDoutbuf.uBicycleSum[0];
        int BicycleFlow2 = p_cfg->p_outbuf->PVDoutbuf.uBicycleSum[1];
        int CurrentBicycleCount = p_cfg->p_outbuf->PVDoutbuf.uBicycleTotalSum;

        int MotorbikeFlow1 = p_cfg->p_outbuf->PVDoutbuf.uMotorbikeSum[0];
        int MotorbikeFlow2 = p_cfg->p_outbuf->PVDoutbuf.uMotorbikeSum[1];
        int CurrentMotorbikeCount = p_cfg->p_outbuf->PVDoutbuf.uBicycleTotalSum;
        //fvd
        vector <ObjectRect> MvdDetectedObjects;
        for( i = 0; i <p_cfg->p_outbuf->FVDoutbuf.uObjNum; i++)
        {
            x = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.x;
            y = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.y;
            w = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.width;
            h = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->FVDoutbuf.detObj[i].prob;
            char names_tmp[50];
            strcpy(names_tmp, p_cfg->p_outbuf->FVDoutbuf.detObj[i].names);
            names=string(names_tmp);
            ObjectRect  obj = ObjectRect(x, y, w, h, names, c);
            MvdDetectedObjects.push_back(obj);
        }
        int CurrentVehicleNumber = p_cfg->p_outbuf->FVDoutbuf.uObjNum;
        int Visibility = p_cfg->p_outbuf->FVDoutbuf.Visbility;
        int VideoState = p_cfg->p_outbuf->FVDoutbuf.VideoException;

        vector <LaneOutputJsonData> LaneOutputData;
        for( i = 0;  i <p_cfg->p_outbuf->FVDoutbuf.uLaneNum; i++)
        {
            int laneNo, queLen, vehiNum, flow, speed,Farcarexist, Nearcarexist;
            laneNo = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].LaneNo;
            queLen = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleQueueLength;
            vehiNum = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uRegionVehicleNumber;
            flow = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleFlow;
            speed = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleSpeed;
            VdPoint pt1( p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].y);
            VdPoint pt2(p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].y );
            Farcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].IsCarInTailFlag;
            Nearcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].bFlowRegionState;

            LaneOutputJsonData detLaneData = LaneOutputJsonData(laneNo, queLen, pt1, pt2, vehiNum, flow, speed
                                                                ,p_cfg->p_outbuf->FVDoutbuf.uActualDetectLength[i],
                                                                p_cfg->p_outbuf->FVDoutbuf.uActualTailLength[i],Farcarexist,Nearcarexist);
            LaneOutputData.push_back(detLaneData);
        }
        vector <DegreeJsonData> DegreeData; // on  lane points
        for( i = 0; i < 20; i++)
        {
            DegreeJsonData d(p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][0],p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][1]);
            DegreeData.push_back(d);

        }

        MvdProcessorOutputData out(MvdDetectedObjects,
                                   CurrentVehicleNumber,
                                   Visibility,
                                   VideoState,
                                   LaneOutputData,
                                   DegreeData,
                                   PersonFlow1,
                                   PersonFlow2,
                                   CurrentPersionCount);
        pkt=out.data();
        //   prt(info,"sending %s",pkt.str().data());
#ifdef SAVE_VIDEO
#ifndef TEST_CLIENT
        //�������
        rectangle(imgSave, cvRect(r1.x, r1.y, r1.width, r1.height), Scalar(0, 255 ,255), 2, 8, 0 );
        //��������
        for( i = 0; i < p_cfg->p_outbuf->FVDoutbuf.uObjNum; i++)
        {
            Rect objRect;
            int c;
            objRect.x = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.x;
            objRect.y = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.y;
            objRect.width = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.width;
            objRect.height = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->FVDoutbuf.detObj[i].prob;
            char str[30];
            sprintf(str, " %d", c);
            char str1[50];
            strcpy(str1, p_cfg->p_outbuf->FVDoutbuf.detObj[i].names);
            strcat(str1, str);
            putText(imgSave, str1, Point(objRect.x,objRect.y - 15),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),1,8);
            rectangle(imgSave, objRect, Scalar(0, 0 ,255), 2, 8, 0 );
        }
        for( i = 0; i < p_cfg->p_outbuf->PVDoutbuf.uObjNum; i++)//���ˡ����г���Ħ�г�����
        {
            Rect objRect;
            int c;
            objRect.x = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.x;
            objRect.y = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.y;
            objRect.width = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.width;
            objRect.height = p_cfg->p_outbuf->PVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->PVDoutbuf.detObj[i].prob;
            char str[30];
            sprintf(str, " %d", c);
            char str1[50];
            strcpy(str1, p_cfg->p_outbuf->PVDoutbuf.detObj[i].names);
            strcat(str1, str);
            putText(imgSave, str1, Point(objRect.x,objRect.y - 15),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,0,255),1,8);
            rectangle(imgSave, objRect, Scalar(0, 0 ,255), 2, 8, 0 );
        }
        int CurrentCarCount = p_cfg->pCfgs->ResultMsg.FvdResultInfo.udetCarTotalNum;
        int CurrentBusCount = p_cfg->pCfgs->ResultMsg.FvdResultInfo.udetBusTotalNum;
        int CurrentTruckCount = p_cfg->pCfgs->ResultMsg.FvdResultInfo.udetTruckTotalNum;
        //��ǰ�˳���
        char str2[50], str3[50],str4[50],str5[50];
        sprintf(str2, "Vehicle:%d", CurrentVehicleNumber);
        putText(imgSave, str2, Point(0,30),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        sprintf(str3, "Person:%d   ", CurrentPersionCount);
        sprintf(str4, "Bicycle:%d   ", CurrentBicycleCount);
        sprintf(str5, "Motorbike:%d", CurrentMotorbikeCount);
        strcat(str3,str4);
        strcat(str3,str5);
        putText(imgSave, str3, Point(0,80),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        char str6[50],str7[50],str8[50];
        sprintf(str6, "Car:%d   ", CurrentCarCount);
        sprintf(str7, "Bus:%d   ", CurrentBusCount);
        sprintf(str8, "Truck:%d", CurrentTruckCount);
        strcat(str6,str7);
        strcat(str6,str8);
        char str9[50],str10[50],str11[50],str12[50],str13[50],str14[50];
        sprintf(str9, "Person North: %d ", PersonFlow1);
        sprintf(str10, "South: %d", PersonFlow2);
        strcat(str9,str10);
        sprintf(str11, "Bicycle North: %d ", BicycleFlow1);
        sprintf(str12, "South: %d", BicycleFlow2);
        strcat(str11,str12);
        sprintf(str13, "Motorbike North: %d ", MotorbikeFlow1);
        sprintf(str14, "South: %d", MotorbikeFlow2);
        strcat(str13,str14);
        putText(imgSave, str9, Point(0,130),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        putText(imgSave, str11, Point(0,180),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        putText(imgSave, str13, Point(0,230),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
        //pvd�����
        CPoint detLline[2];
        detLline[0].x = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].x + +rct.x;
        detLline[0].y = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[0].y + rct.y;
        detLline[1].x = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].x + rct.x;
        detLline[1].y = p_cfg->pCfgs->DownDetectCfg.PvdDetectCfg.ptDetectLine[1].y + rct.y;
        line(imgSave, Point(detLline[0].x,detLline[0].y),Point(detLline[1].x,detLline[1].y), Scalar(255, 255 ,0), 3, 8, 0 );
        for(i = 0; i < p_cfg->pDetectCfg->FvdDetectCfg.uLaneTotalNum; i++)
        {
            CPoint m_ptend[12];
            /*m_ptend[0]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[0];
            m_ptend[1]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[1];
            m_ptend[2]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[3];
            m_ptend[3]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].LaneArea[2];
            m_ptend[4]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[0];
            m_ptend[5]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[1];
            m_ptend[6]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[3];
            m_ptend[7]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].FarArea[2];
            m_ptend[8]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[0];
            m_ptend[9]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[1];
            m_ptend[10]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[3];
            m_ptend[11]= p_cfg->pDetectCfg->FvdDetectCfg.EachLaneCfg[i].NearArea[2]; */
            m_ptend[0]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[0];
            m_ptend[1]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[1];
            m_ptend[2]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[3];
            m_ptend[3]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].LaneArea[2];
            m_ptend[4]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[0];
            m_ptend[5]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[1];
            m_ptend[6]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[3];
            m_ptend[7]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].FarArea[2];
            m_ptend[8]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[0];
            m_ptend[9]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[1];
            m_ptend[10]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[3];
            m_ptend[11]= p_cfg->pCfgs->DownDetectCfg.FvdDetectCfg.EachLaneCfg[i].NearArea[2];
            line(imgSave, Point(m_ptend[0].x + rct.x,m_ptend[0].y+ rct.y),Point(m_ptend[1].x+ rct.x,m_ptend[1].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            line(imgSave, Point(m_ptend[0].x + rct.x,m_ptend[0].y+ rct.y),Point(m_ptend[2].x+ rct.x,m_ptend[2].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            line(imgSave, Point(m_ptend[1].x+ rct.x,m_ptend[1].y+ rct.y),Point(m_ptend[3].x+ rct.x,m_ptend[3].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            line(imgSave, Point(m_ptend[2].x + rct.x,m_ptend[2].y+ rct.y),Point(m_ptend[3].x+ rct.x,m_ptend[3].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            if(p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].calarflag)
            {
                line(imgSave, Point(m_ptend[8].x+ rct.x,m_ptend[8].y+ rct.y),Point(m_ptend[9].x+ rct.x,m_ptend[9].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0 );
                line(imgSave, Point(m_ptend[10].x+ rct.x,m_ptend[10].y+ rct.y),Point(m_ptend[11].x+ rct.x,m_ptend[11].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0 );
            }
            else
            {
                line(imgSave, Point(m_ptend[8].x+ rct.x,m_ptend[8].y+ rct.y),Point(m_ptend[9].x+ rct.x,m_ptend[9].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
                line(imgSave, Point(m_ptend[10].x+ rct.x,m_ptend[10].y+ rct.y),Point(m_ptend[11].x+ rct.x,m_ptend[11].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0 );
            }
            line(imgSave, Point(p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[0].x,p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[0].y),Point(p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[1].x,p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].QueLine[1].y), Scalar(0, 255 ,0), 3, 8, 0 );
            if(p_cfg->pCfgs->IsCarInTail[i])
            {
                line(imgSave, Point(m_ptend[4].x+ rct.x,m_ptend[4].y+ rct.y),Point(m_ptend[5].x+ rct.x,m_ptend[5].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0);
                line(imgSave, Point(m_ptend[6].x+ rct.x,m_ptend[6].y+ rct.y),Point(m_ptend[7].x+ rct.x,m_ptend[7].y+ rct.y), Scalar(0, 0 ,255), 3, 8, 0);
            }
            else
            {
                line(imgSave, Point(m_ptend[4].x+ rct.x,m_ptend[4].y+ rct.y),Point(m_ptend[5].x+ rct.x,m_ptend[5].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0);
                line(imgSave, Point(m_ptend[6].x+ rct.x,m_ptend[6].y+ rct.y),Point(m_ptend[7].x+ rct.x,m_ptend[7].y+ rct.y), Scalar(255, 0 ,0), 3, 8, 0);
            }
            char str4[50], str5[50];
            sprintf(str4, "%d ", p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].uRegionVehicleNumber);//��������
            sprintf(str5, "%d ", p_cfg->pCfgs->ResultMsg.FvdResultInfo.uEachLaneData[i].uVehicleFlow);//������
            putText(imgSave, str4, Point(i * 80 ,280),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);
            putText(imgSave, str5, Point(i * 80 ,330),CV_FONT_HERSHEY_SIMPLEX,1,Scalar(0,255,255),3,8);

        }
        //    namedWindow("detect",CV_WINDOW_NORMAL);
        //     imshow("detect", imgSave);
        //    waitKey(1);
#endif
#endif
    }

    bool process(Mat img_src,JsonPacket &pkt)
    {
#ifdef SAVE_VIDEO
        img_src.copyTo(imgSave);
#endif
        //imshow("1",img_src);
        //prt(info,"%d %d",img_src.rows,img_src.cols);
        int i = 0;
        int x = 0, y = 0, w = 0, h = 0, c;
        string names;
        IplImage img = IplImage(img_src);
        ArithProc(&img, p_cfg->p_outbuf, p_cfg->pCfgs, p_cfg->pParams);

        //pvd
        int PersonFlow1 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[0];
        int PersonFlow2 = p_cfg->p_outbuf->PVDoutbuf.uPersonSum[1];
        int CurrentPersionCount = p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;
        //fvd
        vector <ObjectRect> MvdDetectedObjects;
        for( i = 0; i <p_cfg->p_outbuf->FVDoutbuf.uObjNum; i++)
        {
            x = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.x;
            y = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.y;
            w = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.width;
            h = p_cfg->p_outbuf->FVDoutbuf.detObj[i].box.height;
            c = p_cfg->p_outbuf->FVDoutbuf.detObj[i].prob;
            char names_tmp[50];
            strcpy(names_tmp, p_cfg->p_outbuf->FVDoutbuf.detObj[i].names);
            names=string(names_tmp);
            ObjectRect  obj = ObjectRect(x, y, w, h, names, c);
            MvdDetectedObjects.push_back(obj);
        }
        int CurrentVehicleNumber = p_cfg->p_outbuf->FVDoutbuf.uObjNum - p_cfg->p_outbuf->PVDoutbuf.uPersonTotalSum;
        int Visibility = p_cfg->p_outbuf->FVDoutbuf.Visbility;
        int VideoState = p_cfg->p_outbuf->FVDoutbuf.VideoException;

        vector <LaneOutputJsonData> LaneOutputData;
        for( i = 0;  i <p_cfg->p_outbuf->FVDoutbuf.uLaneNum; i++)
        {
            int laneNo, queLen, vehiNum, flow, speed, Farcarexist, Nearcarexist;
            laneNo = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].LaneNo;
            queLen = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleQueueLength;
            vehiNum = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uRegionVehicleNumber;
            flow = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleFlow;
            speed = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].uVehicleSpeed;
            VdPoint pt1( p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[0].y);
            VdPoint pt2(p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].x,
                    p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].QueLine[1].y );
            Farcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].IsCarInTailFlag;
            Nearcarexist = p_cfg->p_outbuf->FVDoutbuf.uEachLaneData[i].bFlowRegionState;

            LaneOutputJsonData detLaneData = LaneOutputJsonData(laneNo, queLen, pt1, pt2, vehiNum, flow, speed
                                                                ,p_cfg->p_outbuf->FVDoutbuf.uActualDetectLength[i],
                                                                p_cfg->p_outbuf->FVDoutbuf.uActualTailLength[i],Farcarexist,Nearcarexist);
            LaneOutputData.push_back(detLaneData);
        }
        vector <DegreeJsonData> DegreeData; // on  lane points
        for( i = 0; i < 20; i++)
        {
            DegreeJsonData d(p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][0],p_cfg->p_outbuf->FVDoutbuf.uDegreePoint[i][1]);
            DegreeData.push_back(d);

        }

        MvdProcessorOutputData out(MvdDetectedObjects,
                                   CurrentVehicleNumber,
                                   Visibility,
                                   VideoState,
                                   LaneOutputData,
                                   DegreeData,
                                   PersonFlow1,
                                   PersonFlow2,
                                   CurrentPersionCount);
        pkt=out.data();
    }
};


#endif // MVDPROCESSOR_H
