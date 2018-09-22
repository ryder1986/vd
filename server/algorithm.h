﻿#ifndef __ALG_H__
#define __ALG_H__
#include "darknet.h"
#include <stdbool.h>
#include<sys/time.h>
#ifndef __cplusplus
#ifdef OPENCV
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/core/version.hpp"
#endif
#endif

typedef  unsigned int 	Uint32;
typedef  int 			Int32;
typedef  unsigned short Uint16;
typedef  short 			Int16;
typedef  int             BOOL;
typedef  unsigned char	Uint8;


#define MAX_DETECTOR_TYPES		2			//最大支持两种检测器
#define MAX_DETECTOR_ONETYPE	8			//每种检测器最大支持8个
#define MAX_LANE		8	//	8			//每个检测器最大支持8个车道

#define FULL_COLS  					(1920)
#define FULL_ROWS  					(1080)

#define  CALIBRATION_POINT_NUM   8  //标定点数2015102


#define DETECTRECT_WIDTH_MAX		1920//500
#define DETECTRECT_HEIGHT_MAX		1080//240

#define	MAX_SPEEDDETECTOR_DOTS	768*576
#define MaxDotsInDetect 768*576

#define DETECTRECT_WIDTH_DEFAULT	160
#define DETECTRECT_HEIGHT_DEFAULT	40
#define LANE_AMOUNT_DEFAULT			3
#define DETECT_WIDTH			200
#define DETECT_HEIGHT			120
#define DETECT_ALGORITHM			3


#define TRUE 1
#define FALSE 0
#define	VISIB_LENGTH		10//txl,20160105
#define MAX_CLASSES 80
#define MAX_TARGET_NUM 100
#define MAX_DIRECTION_NUM 2
#define FRAME_FPS  10


typedef struct{
	int x;
	int y;
	int width;
	int height;
}CRect1;

typedef struct{
	int x;
	int y;
}CPoint;

typedef struct{
	CRect1 box;
	int class_id;
	int prob;
	char names[50];
}CDetBox;


typedef struct{
	int class_id;
	float prob[100];
	CRect1 box[100];
	char names[50];
	int classes_num;
	int lane_id[100];//框所在的车道
}CDetClass;

typedef struct{
	CRect1 box;	
	float prob;
	int class_id;
	char names[50];
	bool detected;
	int  lost_detected;
	int target_id;
	CPoint trajectory[3000];
	int trajectory_num;
	int vx;//速度
	int vy;
	int continue_num;
	float start_time;
	float end_time;//用于计算车速
	int lane_id;
	bool cal_speed;
	bool cal_flow;
	int direction;//目标方向
}CTarget;

///////////////////////////////////////////////////////////////////////////////////////////检测参数
typedef struct
{
	Uint16			LaneNo; //车道号
	//detect region
	CPoint          FarArea[4];//远线圈
	CPoint          NearArea[4];//流量线圈
	CPoint          LaneArea[4];//车道线圈

}LANECFG;

typedef struct
{
	Uint16			uLaneTotalNum;//车道总数
	LANECFG 		EachLaneCfg[MAX_LANE];
	CPoint			BasicCoil[4];//标定点	
	CPoint          BaseLine[2];//基准线上的点
	float           base_length;//基准线长
	float           near_point_length;//进距点
	float           far_point_length;//远距点
}FVDDETECTCFG;

typedef struct
{
	CPoint		    ptDetectLine[2];//检测线
	CRect1          detectROI;//检测区域
}PVDDETECTCFG;

typedef struct
{
	FVDDETECTCFG    FvdDetectCfg;
	PVDDETECTCFG    PvdDetectCfg;

}DETECTCFGSEG;

//////////////////////////////////////////////////检测结果输出

typedef struct
{
	Uint16		   LaneNo;	           //车道ID
	BOOL		   bInfoValid;				//检测器结果有效
	Uint16	       bVehicleSta;		    //车入车出状态
	Uint32         uVehicleFlow;          //车流量
	Uint16         uRegionVehicleNumber;  //区域车辆数
	Uint16	       calarflag;	           //车占有线圈状态
	Uint16         uVehicleDirection;     //车辆运行方向
	Uint16	       uVehicleSpeed;		   //车速
	Uint16	       uVehicleType;		   //车类型
	Uint16         uVehicleQueueLength;   //排队长度
	CPoint         QueLine[2];            //排队长度线
	Uint16         uLastVehicleLength;    //最后一辆车的位置
	CPoint	       LastVehiclePos[2];     //最后一辆车的位置
	Uint16	       uVehicleHeadtime;	  //车头时距
	bool           IsCarInTailFlag;      //尾部区域占有标志
	Uint16	       uReserved[20];			//预留		
}LANERESULTDATA;

typedef struct
{
	Uint16 		    LaneSum;                      
	bool            VideoException;                     //能见度状态
	bool            Visbility;                          //视频状态
	CDetBox         udetPersonBox[100];               //行人检测框
	Uint16          udetPersonToalNum;                  //检测框数
	CDetBox         udetVehicleBox[100];              //车检测框
	Uint16          udetVehicleTotalNum;               //检测框数
	Uint16          udetCarTotalNum;                  //car
	Uint16          udetBusTotalNum;                  //bus
	Uint16          udetTruckTotalNum;                //truck
	LANERESULTDATA	uEachLaneData[MAX_LANE]; //包含8个车道所有的检测信息
} FVDRESULTINFO;

typedef struct
{
    Uint16          uPersonNum[MAX_DIRECTION_NUM];      //分方向的行人总数
	Uint16          uBicycleNum[MAX_DIRECTION_NUM];     //分方向的自行车总数
	Uint16          uMotorbikeNum[MAX_DIRECTION_NUM];   //分方向的摩托车总数
	CDetBox         udetBox[100];                       //行人检测框
	Uint16          udetPersonTotalNum;                 //行人检测框数
	Uint16          udetBicycleTotalNum;                //自行车检测框数
	Uint16          udetMotorbikeTotalNum;              //自行车检测框数
} PVDRESULTINFO;

typedef struct
{
	FVDRESULTINFO   FvdResultInfo;
	PVDRESULTINFO   PvdResultInfo;
}RESULTMSG;
////////////////////////////////////////////////////////////////////////////////////////

typedef struct{
	Uint16	       LaneNo;                //车道号
	Uint32         uVehicleFlow;          //车流量
	Uint16         uRegionVehicleNumber;  //区域车辆数
	Uint16	       bFlowRegionState;	   //车入车出状态
	Uint16         uVehicleDirection;     //车辆运行方向
	Uint16	       uVehicleSpeed;		   //车速
	Uint16	       uVehicleType;		   //车类型
    Uint16         uVehicleQueueLength;   //排队长度
    CPoint         QueLine[2];            //排队长度线
	Uint16	       uVehicleHeadtime;	   //车头时距
	Uint16         uLastVehicleLength;    //最后一辆车的位置
	CPoint	       LastVehiclePos[2];
	bool           IsCarInTailFlag;      //尾部区域占有标志
	Uint16	       uReserved[20];			  //预留
}LANEDETECTRESULT;


typedef struct{
	LANEDETECTRESULT uEachLaneData[MAX_LANE];//每个车道数据
	Uint16           uLaneNum;               //车道数量
	bool             VideoException;                //视频状态
	bool             Visbility;               //能见度
	Uint16           uActualDetectLength[MAX_LANE];//流量区域的实际长度
	Uint16           uActualTailLength[MAX_LANE];  //占有区域的实际长度
	Uint16           uDegreePoint[20][2];   //刻度点
	CDetBox          detObj[100];//	检测框
	Uint16           uObjNum;//检测框个数
	Uint16          udetVehicleTotalNum;               //检测框数
	Uint16          udetCarTotalNum;                  //car
	Uint16          udetBusTotalNum;                  //bus
	Uint16          udetTruckTotalNum;                //truck
}FVDOUTBUF;

typedef struct{
	Uint32       uPersonSum[MAX_DIRECTION_NUM];//各个方向的行人数
	Uint32       uBicycleSum[MAX_DIRECTION_NUM];//各个方向的自行车数
	Uint32       uMotorbikeSum[MAX_DIRECTION_NUM];//各个方向的摩托车数
	CDetBox      detObj[100];//检测行人框
	Uint16       uObjNum;//检测框个数
	Uint32       uPersonTotalSum;//当前帧人总数
	Uint32       uBicycleTotalSum;//当前自行车总数
	Uint32       uMotorbikeTotalSum;//当前摩托车总数
}PVDOUTBUF;

typedef struct{
	PVDOUTBUF    PVDoutbuf;
	FVDOUTBUF    FVDoutbuf;
}OUTBUF;

typedef struct cSpeedDetectStruct
{
	Uint16		DetectId;								
	Uint16	    QueDetectDots;
	Uint16		QueDetectLines;
	Uint16		QueDetectColumns;
	Uint32		*CoordinatePointerQ;

	Uint16		*CurrQueueImage;
	Uint16		*PreQueueImage; 
	Uint16		*BackQueueImage;
	Uint16      *PrePreQueueImage;
	Uint16      *PrePrePreQueueImage;

	Uint16		uReserve[37];
}SPEED_DETECT_STRUCT;		

struct	cSpeedStructIndex
{
	Uint16	   SpeedId;
	struct cSpeedDetectStruct	*ExtStorageAddr;
};

typedef	struct	tagCameraParamaters
{
	struct	cSpeedStructIndex		dSpeedIdIndex[8];							                                    	  

	//extended by david 20130422:
	Uint8		*CameraCfgEntry;
	Uint8		*ImageStorageEntry;
}CAMERA_PARAMETERS;

typedef struct tagCfgs
{
	CAMERA_PARAMETERS		CameraLocalPara;
	RESULTMSG				ResultMsg;			
	DETECTCFGSEG	        DownDetectCfg;		

	//yolo检测参数
	network* net;
	char** names;
	float thresh;

	//mobileNet检测参数

	//行人检测参数
	CRect1  detectROI;//行人检测区域
	CPoint detLine[2];//检测线
	int detLeft;//根据检测线计算出来的跟踪区域
	int detRight;
	int detTop;
	int detBottom;
	float k;
	float b;//检测线的斜率和截距
	int line_vertical;
	CTarget objPerson[MAX_TARGET_NUM];//目标
	int person_id;
	int objPerson_size;
	CDetClass detPerson[3];//行人检测框
	Uint16 uPersonSum[MAX_DIRECTION_NUM];//各个方向上通过检测线的行人总数
	Uint16 uBicycleSum[MAX_DIRECTION_NUM];//各个方向上通过检测线的自行车总数
	Uint16 uMotorbikeSum[MAX_DIRECTION_NUM];//各个方向上通过检测线的摩托车总数

	//车辆检测参数
	int LaneAmount;//车道数
	CTarget targets[MAX_TARGET_NUM];//目标
	int target_id;
	int targets_size;
	Uint16 current_target_id[MAX_LANE];
	Uint16 headtime[MAX_LANE];//用于计算车头时距
	Uint16 jgtime[MAX_LANE];
	Uint16 Headposition[MAX_LANE];
	Uint16 Tailposition[MAX_LANE];
	CRect1 detBoxes[MAX_LANE][20];
	Uint16 detNum[MAX_LANE];
	Uint16	m_iWidth, m_iHeight;
	Uint16	team_width;
	Uint16  team_height;
	bool    IsCarInTail[MAX_LANE];
	//混合检测
	CDetClass detClasses[MAX_CLASSES];//检测类别

	//计算车速参数
	double currIime;//用于计算车辆速度
	struct timeval time_start;
	struct timeval time_end;

	Uint32 	gThisFrameTime;


	Uint32	uDetectInSum[MAX_LANE];		//车入数
	Uint32	uDetectOutSum[MAX_LANE];	//车出数
	Uint32  uDetectVehicleSum[MAX_LANE];//车道内的车辆总数
	Uint16  uStatQuePos[MAX_LANE][6];//统计排队长度
	Uint16 uVehicleQueueLength[MAX_LANE];//各车道排队长度值

	//对图像进行标定
	int calibration_point[4][2];//标定区域四个点
	int base_line[2][2];//基准线
	float base_length;//基准线长
	float near_point_length;//最近点的距离
	float actual_distance[8][FULL_ROWS];//图像到实际的映射
	Uint16 uActualDetectLength[8];//流量区域的实际长度
	Uint16 uActualTailLength[8];//车尾占有区域的实际长度
	float actual_degree_length[FULL_ROWS];//刻度线上的实际映射
	Uint16 degreepoint[20][2];//20个标定点

	Uint16 fuzzydegree;
	bool Visbility;//能见度
	int up_visib_value;//统计能见度
	Uint16 visib_value[VISIB_LENGTH];
	bool VideoException;//视频异常
	Uint32  abnormal_time;

}ALGCFGS;

typedef struct tagParams
{

	Uint8	*CurrQueueImage;  //车道区域内图像
	Uint8	*PreQueueImage;
	Uint8	*BackQueueImage;
	Uint8   *PrePreQueueImage;
	Uint8   *PrePrePreQueueImage;

}ALGPARAMS;


typedef struct args{
	ALGCFGS    *pCfgs;
	ALGPARAMS  *pParams;
	OUTBUF     *p_outbuf;
	DETECTCFGSEG    *pDetectCfg;

}m_args;


#ifdef __cplusplus
extern "C"{
#endif
	m_args* alg_mem_malloc();
	int alg_mem_free(m_args *arg_arg);
	bool CfgStructParse(FVDDETECTCFG *pDownDetectCfg, ALGCFGS *pCfgs, ALGPARAMS *pParams);//配置参数
	bool FvdArithInit(FVDDETECTCFG *pDownDetectCfg, ALGCFGS *pCfgs, ALGPARAMS *pParams);//算法初始化
	bool FvdRestParams(FVDDETECTCFG *pDownDetectCfg, ALGCFGS *pCfgs, ALGPARAMS *pParams);//重置参数
	void FvdProcessBox(ALGCFGS *pCfgs, int laneNum);//对检测框进行处理
	Uint16 FvdArithDetect(IplImage* img, ALGCFGS *pCfgs);//yolo检测
	bool CorrectImage(IplImage * img, ALGCFGS *pCfgs);//根据车道线对车道图像进行校正
    Uint16 FvdArithProc(IplImage* img, FVDOUTBUF* outBuf, ALGCFGS *pCfgs, ALGPARAMS	*pParams);//FVD
	void QueLengthCaculate(Uint16 LaneID, ALGCFGS *pCfgs, ALGPARAMS	*pParams, CPoint m_ptend[]);//排队长度
	void iSubStractImage(Uint8 *puSourceImage,Uint8 *puTargetImage, Uint32 nThreshold, Int16 nFromLine, Int16 nToLine, Int16 width, Int16 height);//帧差
	CPoint ptGetDot(CPoint* ptUpLeft, CPoint* ptUpRight, CPoint* ptDownRight, CPoint* ptDownLeft, Int16 width, Int16 height, Int16 nColNum, Uint32 * ptStorePlace);//图像框矫正
	static void camera_calibration(int base_line[][2], float base_length, int calibration_point[][2], float near_point_length, int laneNum, ALGCFGS *pCfgs);//标定
	float fuzzy(unsigned char* puNewImage, int width, int height);//能见度计算
	bool visible_judge(Uint16 *a, int visib_length, int threshold);//统计能见度
	bool Color_deviate(unsigned char* uImage, unsigned char* vImage, int width, int height);//视频图像状态计算

	int SetLine(ALGCFGS* pCfgs, CPoint* ptDetLine);//设置行人检测线
	//bool PvdArithInit(ALGCFGS *pCfgs, CPoint* ptDetLine, CRect1 detROI);//行人检测初始化
	bool PvdArithInit(ALGCFGS *pCfgs, CPoint* ptDetLine);//行人检测初始化
	bool PvdRestParams(ALGCFGS *pCfgs, CPoint* ptDetLine);//重置参数
	Uint16 PvdArithDetect(IplImage* img, ALGCFGS* pCfgs);//行人检测
	void PvdProcessBox(ALGCFGS* pCfgs);//对检测框进行处理
	Uint16 PvdArithProc(IplImage* img, ALGCFGS *pCfgs, PVDOUTBUF* outbuf);

	bool ArithInit(ALGCFGS *pCfgs, CPoint* ptDetLine, FVDDETECTCFG *pDownDetectCfg, ALGPARAMS *pParams);//算法初始化
	bool RestParams(ALGCFGS *pCfgs, CPoint* ptDetLine, FVDDETECTCFG *pDownDetectCfg, ALGPARAMS *pParams);//重置参数
	Uint16 ArithDetect(IplImage* img, ALGCFGS *pCfgs);
	Uint16 ArithProc(IplImage* img, OUTBUF* outBuf, ALGCFGS *pCfgs, ALGPARAMS	*pParams);
	Uint16 ArithProc_whole(IplImage* img, CRect1 detROI, OUTBUF* outBuf, ALGCFGS *pCfgs, ALGPARAMS	*pParams);//算法执行

#ifdef __cplusplus
}
#endif
//extern network *set_yolo_params(char *cfgfile, char *weightfile,int gpu_idex);
//extern char **load_class_names(char *datacfg);
//extern int yolo_detector(network *net, IplImage *img_cv, float thresh, float hier_thresh,float *result);
//extern void free_yolo_params(network *net, char **names);

#endif