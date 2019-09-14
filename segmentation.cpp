#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

char str[]="2.jpg";      //---------------------image address---------------------

const int max_value_H = 360/2;
const int max_value = 255;  Mat frame_HSV, frame_threshold,src; 
Mat frame=imread(str); 

RNG rng(12345);

void Callbackfunc(int Event, int y, int x, int flags, void* userdata);      // Mouse callback function

char window_detection_name[]=" segmentation"; 

Mat clr=imread(str);

int low_H = 0, low_S = 0, low_V = 0,low_B = 0,low_G = 0,low_R = 0,b,g,r,area;
int high_H = max_value_H, high_S = max_value, high_V = max_value, high_B = max_value, high_G = max_value, high_R = max_value;
int hl,sl,vl,hh,sh,vh,colour;

int b1[]={255,0,0,0,0,255,255,180,0,255};         //indicator colour blue value array
int g1[]={0,255,0,255,128,255,0,0,0,255};			//indicator colour green value array
int r1[]={0,0,255,255,255,0,255,255,0,255};			//indicator colour red value array

int ctr=0;		

int click=1;
Mat m=clr.clone();							//counter variable to for index of indicator colour array

double areapercentage;
Mat frame1=clr.clone();


Mat colour_img();              // function to colour image
void check_in_Range();          // function to check image in range


//-------------------------------------------------------Trackbar call back function----------------------------------------------------


void on_low_B_thresh_trackbar(int, void *)
{
   low_B = min(high_B-1, low_B); clr=frame1.clone();
   colour_img();

  
}

void on_high_B_thresh_trackbar(int, void *)
{
   high_B = max(high_B, low_B+1); clr=frame1.clone();colour_img();
 
   
}

void on_low_G_thresh_trackbar(int, void *)
{
   low_G = min(high_G-1, low_G); clr=frame1.clone();colour_img();

    
}

void on_high_G_thresh_trackbar(int, void *)
{ 
    high_G = max(high_G, low_G+1); clr=frame1.clone();colour_img();

   
}

void on_low_R_thresh_trackbar(int, void *)
{
    low_R = min(high_R-1, low_R); clr=frame1.clone();colour_img();


}

void on_high_R_thresh_trackbar(int, void *)
{
   high_R = max(high_R, low_R+1); clr=frame1.clone();colour_img();

   
}



// ------------------------------------------------------ Main Function------------------------------------------------------



int main(int argc, char* argv[])
{
    

        namedWindow(window_detection_name, CV_WINDOW_NORMAL);resizeWindow(" segmentation",500,500);
		namedWindow("select plz :p", CV_WINDOW_NORMAL);resizeWindow("select plz :p",500,500);
		namedWindow("final output x) ", CV_WINDOW_NORMAL);resizeWindow("final output x) ",500,500);        	
	

		imshow("select plz :p",frame);  

    // Trackbars to set thresholds for BGR values

		createTrackbar("Low B", window_detection_name, &low_B, max_value, on_low_B_thresh_trackbar);
    	createTrackbar("High B", window_detection_name, &high_B, max_value, on_high_B_thresh_trackbar);
    	createTrackbar("Low G", window_detection_name, &low_G, max_value, on_low_G_thresh_trackbar);
    	createTrackbar("High G", window_detection_name, &high_G, max_value, on_high_G_thresh_trackbar);
    	createTrackbar("Low R", window_detection_name, &low_R, max_value, on_low_R_thresh_trackbar);
    	createTrackbar("High R", window_detection_name, &high_R, max_value, on_high_R_thresh_trackbar);
	
	//--mouse callback function

	setMouseCallback("select plz :p", Callbackfunc, NULL);
    
	while (true) 
	{
        
        if(frame.empty())
        {
            break;
        }
        
        // Convert from BGR to HSV colorspace and check if inRange
			
        	check_in_Range();
        	
		
        
        	  

			imshow("select plz :p",clr);    // window to select color range  

            char key = (char) waitKey(30);
        
        if (key == 'q' || key == 27)
        {
            break;
        }
    
    }
    waitKey(0);

    return 0;

}

Mat white(frame.rows,frame.cols, CV_8UC(1), Scalar::all(255));
int totalarea=countNonZero(white);

//-------------------------------------------Mouse call back function------------------------------------------------------------


void Callbackfunc(int Event, int y, int x, int flags, void* userdata)
{
	cvtColor(frame, src, COLOR_BGR2HSV);

	if (Event == EVENT_LBUTTONDOWN)
	{



		b=frame.at<Vec3b>(x, y)[0];
		g=frame.at<Vec3b>(x, y)[1];
		r=frame.at<Vec3b>(x, y)[2];
		

	  	setTrackbarPos("Low B", window_detection_name,b);
		setTrackbarPos("Low G", window_detection_name,g);
   		setTrackbarPos("Low R", window_detection_name,r); 
		setTrackbarPos("High B", window_detection_name,b);
		setTrackbarPos("High G", window_detection_name,g);	 
		setTrackbarPos("High R", window_detection_name,r);
		
		
				
		frame1=clr.clone();
				
		ctr++; 

		check_in_Range();

		
		
		m=colour_img();
	



	}



	


}

	






//--------------------------------------------Function To Colour Image----------------------------------------------------------


Mat colour_img()
{
  if(ctr>0)
  {

	for(int i=0;i<frame_threshold.rows;i++)
	{
		for(int j=0;j<frame_threshold.cols;j++)
		{

			colour =frame_threshold.at<uchar>(i,j);

			
			if(colour==255 ||(low_R==0 && low_G==0 && low_B==0 && high_R==255 && high_G==255 && high_B==255))
			{

			clr.at<Vec3b>(i, j)[0]=b1[ctr];
			clr.at<Vec3b>(i, j)[1]=g1[ctr];
			clr.at<Vec3b>(i, j)[2]=r1[ctr];
			

			}
		}
	}
	
	/*if(low_R==0 && low_G==0 && high_R==255 && high_G==255 && high_B==255)
	{
		clr=frame.clone();
	}*/

	
	

	area=countNonZero(frame_threshold); areapercentage=(double)area*100/(double)totalarea;
	cout<<"area percentage of phase "<< ctr <<" is : "<< areapercentage <<" %"<<endl<<endl;
	
	imshow("final output x) ",clr);    					 // output window
	return(clr);
}
}


//------------------------------------------------Function to check in Range------------------------------------------------------


void check_in_Range()
{

		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

    	Mat M(1,1, CV_8UC3, Scalar(low_B,low_G,low_R)),M_HSV;
  
 		cvtColor(M, M_HSV, COLOR_BGR2HSV);
 		hl=M_HSV.at<Vec3b>(0,0 )[0];
		sl=M_HSV.at<Vec3b>(0,0 )[1];
		vl=M_HSV.at<Vec3b>(0,0 )[2];


    	Mat M1(1,1, CV_8UC3, Scalar(high_B,high_G,high_R)),M1_HSV; 

    	cvtColor(M1, M1_HSV, COLOR_BGR2HSV);
		hh=M1_HSV.at<Vec3b>(0,0 )[0];
		sh=M1_HSV.at<Vec3b>(0,0 )[1];
		vh=M1_HSV.at<Vec3b>(0,0 )[2];




		low_H=hl-10;
		low_S=sl-30;
		low_V=vl-30;
		
		high_H=hh+10;
		high_S=sh+20;
		high_V= vh+20; 

		//cout<<low_H<<endl<<low_S<<endl<<low_V<<endl<<high_H<<endl<<high_S<<endl<<high_V<<endl;
		//cout<<hl<<" "<<sl<<" "<<vl<<endl;cout<<hh<<" "<<sh<<" "<<vh<<endl;


        // Detect the object based on HSV Range Values
        	inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);  


  

        	imshow(" segmentation" , frame_threshold);      //window for trackbar


}






















