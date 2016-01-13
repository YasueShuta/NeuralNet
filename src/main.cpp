#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Eigen/Core>
//#define _USE_MATH_DEFINES
#include <math.h>

using namespace Eigen;
using namespace std;

typedef Matrix<double, -1, -1, RowMajor> Mat;


double sigmoid(double x);
VectorXd sigmoid(VectorXd x);
double d_sigmoid(double x);
VectorXd d_sigmoid(VectorXd x);



int main(int argc, char* argv[]){
  /*  parameter  */
  static int SIZE_X = 10;
  static int SIZE_Y = 10;
  static int Image_Num = 5;
  
  static int Neutral_Num = 20;
  
  static int Train_Num = 100;
  static double converge = 0.01;

  static double eta = 0.1;



  /*  argv dealing  */
  {
    string s;
    int i=1;
    while(i<argc){
      s = argv[i];
      switch(s.at(1)){
      case 'h':
	//help      
	cout << "-h  help" << endl;
	cout << "-e  set eta" << endl;
	cout << "-n  set neutral num" << endl;
	cout << "-x  set size_x" << endl;
	cout << "-y  set size_y" << endl;
	cout << "-t  set train num" << endl;
	cout << "-c  set converge threshold" << endl;
	return 2;
      case 'm':
	s = argv[++i];
	eta = stod(s);
	break;
      case 'n':
	s = argv[++i];
	Neutral_Num = stoi(s);
	break;
      case 'x':
	s = argv[++i];
	SIZE_X = stoi(s);
	break;
      case 'y':
	s = argv[++i];
	SIZE_Y = stoi(s);
	break;
      case 't':	
	s = argv[++i];
	Train_Num = stoi(s);
	break;
      case 'c':
	s = argv[++i];
	converge = stod(s);
      default:
	break;
      }
      i++;
    }
  }
  
  /*  Create Image Data  */
  vector<Mat> image_data = vector<Mat>(Image_Num);
  {  
    IplImage *src=0, *dst=0;
    string str;
    
    for(int i = 0;i<Image_Num;i++){
      str = "../image/sample" + std::to_string(i+1) + ".jpg";
      src = cvLoadImage(str.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
      
      if(src == NULL) return -1;
      
      dst = cvCreateImage(cvSize(SIZE_X, SIZE_Y), src->depth, src->nChannels);
      cvResize(src, dst, CV_INTER_NN);
      
      str = "../image_resize/sample" + to_string(i+1) + ".bmp";
      image_data.at(i) = Mat(SIZE_Y, SIZE_X);
      for(int h=0; h < dst->height; h++){
	for(int w=0; w < dst->width; w++){
	  image_data.at(i)(h, w) = (unsigned char)dst->imageData[h*dst->widthStep + w] / 256.0;
	}
      }
      cvSaveImage(str.c_str(), dst);
    }
  }
  
  //Create network
  VectorXd x1, x2, x3;
  VectorXd r1, r2, r3;
  Mat w12, w23;
  int output;
  VectorXd ts, err;
  {
    x1 = VectorXd::Zero(SIZE_X*SIZE_Y);
    r1 = x1;
    x2 = VectorXd::Zero(Neutral_Num);
    r2 = x2;
    x3 = VectorXd::Zero(Image_Num);
    r3 = x3;
    
    w12 = Mat::Random(x2.size(), x1.size());
    w23 = Mat::Random(x3.size(), x2.size());
    
    output = 0;
    
    ts = VectorXd::Zero(Image_Num);
  }

  //Record
  ofstream ofs("record.txt");
  ofs << "Assignment 1." << endl;
  ofs << "No. Neutral Neuron: " << Neutral_Num << endl;
  ofs << "Learning Rate:      " << eta << endl;
  ofs << "Convergence       : " << converge << endl;
  ofs << "Limit of Training : " << Train_Num << endl;
  ofs << endl;
  ofs << "Image Size        : " << SIZE_X << ", " << SIZE_Y << endl;
  ofs << "Image Num         : " << Image_Num << endl;
  ofs << endl;

  //Training
  ofs << "[Training]" << endl;
  double E_RMS;
  VectorXd delta2 = x2;
  VectorXd delta3 = x3;
  {
    int count=0;
    int image_id;
    while(count < Train_Num){
      E_RMS = 0;
      for(image_id=0; image_id < Image_Num; image_id++){
	//(1)initialize x1, ts
	for(int i=0; i<x1.size(); i++){
	  x1(i) = image_data.at(image_id)(i);
	}
	ts *= 0;
	ts(image_id) = 1;
	
	//(2)front propagation
	r1 = sigmoid(x1);
	x2 = w12 * r1;
	r2 = sigmoid(x2);
	x3 = w23 * r2;
	r3 = sigmoid(x3);
	output = 0;
	for(int i=0;i<r3.rows();i++){
	  if(r3(output) < r3(i))
	    output = i;
	}
	
	//(3)back propagation
	err = r3 - ts; 
	delta3.array() = err.array() * d_sigmoid(x3).array();
	w23 -= eta * delta3 * r2.transpose();
	delta2 = (w23.transpose() * delta3).array() * d_sigmoid(x2).array();
	w12 -= eta * delta2 * r1.transpose();
	
	//(4)calculate error evaluator
	E_RMS += (err.transpose()*err)(0);
      }
      E_RMS = sqrt(E_RMS) / (double)(Image_Num*Image_Num);
      cout << "(i: " << count << ") E_RMS: " << E_RMS << endl;
      ofs << "(i: " << count << ")\tE_RMS: " << E_RMS << endl;
      count++;
    }
  }

  //Test
  double result=0;
  ofs << "[Test]" << endl;
  {
    E_RMS = 0;
    for(int image_id=0; image_id<Image_Num; image_id++) {

	//(1)initialize x1, ts
	for(int i=0; i<x1.size(); i++){
	  x1(i) = image_data.at(image_id)(i);
	}
	ts *= 0;
	ts(image_id) = 1;
	
	//(2)front propagation
	r1 = sigmoid(x1);
	x2 = w12 * r1;
	r2 = sigmoid(x2);
	x3 = w23 * r2;
	r3 = sigmoid(x3);
	output = 0;

	cout << image_id << ": output: " << endl;
	cout << r3.transpose() << endl;
	ofs << image_id << ": Output: " << endl;
	ofs << r3.transpose() << endl;

	for(int i=0;i<r3.rows();i++){
	  if(r3(output) < r3(i))
	    output = i;
	}
	if(output == image_id) result+=1;
	ofs << "Judge: " << output << "(" << image_id << ")" << endl;

	//(3)calculate error evaluator
	E_RMS += (err.transpose()*err)(0);
    }
    result /= (double)Image_Num;
    E_RMS = sqrt(E_RMS)/(double)(Image_Num*Image_Num);
    cout << "E_RMS: " << E_RMS << endl;
    ofs << "E_RMS: " << E_RMS << endl;
    cout << "Collect " << round(100*result) << " %" << endl;
    ofs << "Collect " << round(100*result) << " %" << endl;
  }

  ofs.close();
  return 0;
}






double sigmoid(double x){
  return 1/(1 + exp(-x));
}
VectorXd sigmoid(VectorXd x){
  VectorXd ret = x;
  for(int i = 0; i<x.size(); i++){
    ret(i) = sigmoid(x(i));
  }
  return ret;
}
double d_sigmoid(double x){
  return (1-sigmoid(x))*sigmoid(x);
}
VectorXd d_sigmoid(VectorXd x){
  VectorXd ret = x;
  for(int i=0; i<x.size(); i++){
    ret(i) = (1-sigmoid(x(i)))*sigmoid(x(i));
  }
  return ret;
}
