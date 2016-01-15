#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "../eigen/Eigen/Core"
#include "../gnuplot/Gnuplot.h"

//#define _USE_MATH_DEFINES
#include <math.h>

using namespace Eigen;
using namespace Gnuplot;
using namespace std;

typedef Matrix<double, -1, -1, RowMajor> MyMat;


double sigmoid(double x);
VectorXd sigmoid(VectorXd x);
double d_sigmoid(double x);
VectorXd d_sigmoid(VectorXd x);



int main(int argc, char* argv[]){
  /*  parameter  */
  static int SIZE_X = 10;
  static int SIZE_Y = 10;
  static int Image_Num = 5;
  static int Image_Sample_Num = 10;
  
  static int Neutral_Num = 30;
  
  static int Train_Num = 2000;
  static double converge = 0.015;

  static double eta = 0.1;
  static double trainNoise = 0.0;
  static double NoiseRateMax = 0.25;

  bool isConvert = true;

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
	cout << "-i  set image num" << endl;
	cout << "-s  set image sample num" << endl;
	cout << "-v  no need to convert image" << endl;
	cout << "-r  set max noise rate" << endl;
	return 2;
      case 'e':
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
	break;      
      case 's':
	s = argv[++i];
	Image_Sample_Num = stod(s);
	break;
      case 'i':
	s = argv[++i];
        Image_Num = stod(s);
        break;
      case 'v':
	isConvert = false;
	break;
      case 'r':
	s = argv[++i];
	NoiseRateMax = stod(s);
	break;
      default:
	break;
      }
      i++;
    }
  }
  
  /*  Create Image Data  */
  vector<MyMat> image_data = vector<MyMat>(Image_Num*Image_Sample_Num);
  {  
    IplImage *src=0, *dst=0;
    string str_dir, str_file;
    int index;

    for(int i = 0;i<Image_Num;i++){
      for(int j = 0;j<Image_Sample_Num;j++){
	if(isConvert){
	  str_dir = "../image/im" + to_string(i+1);
	  str_file = "sam" + to_string(j+1);
	  src = cvLoadImage((str_dir + "/" + str_file + ".png").c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	  
	  if(src == NULL) return -1;
	  
	  dst = cvCreateImage(cvSize(SIZE_X, SIZE_Y), src->depth, src->nChannels);
	  cvResize(src, dst, CV_INTER_NN);	
	  
	  str_dir = "../image_resize/im" + to_string(i+1);
	  cvSaveImage((str_dir + "/" + str_file + ".bmp").c_str(), dst);
	} else {
	  str_dir = "../image_resize/im" + to_string(i+1);
	  str_file = "sam" + to_string(j+1);
	  dst = cvLoadImage((str_dir + "/" + str_file + ".bmp").c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	  
	  if(dst == NULL) return -1;
	}

	index = j*Image_Num + i;
	image_data.at(index) = MyMat(SIZE_Y, SIZE_X);
	for(int h=0; h < dst->height; h++){
	  for(int w=0; w < dst->width; w++){
	    image_data.at(index)(h, w) = (unsigned char)dst->imageData[h*dst->widthStep + w] / 256.0;
	  }
	}
      }
    }
  }
  
  //Create network
  VectorXd x1, x2, x3;
  VectorXd r1, r2, r3;
  MyMat w12, w23;
  int output;
  VectorXd ts, err;
  {
    x1 = VectorXd::Zero(SIZE_X*SIZE_Y);
    r1 = x1;
    x2 = VectorXd::Zero(Neutral_Num);
    r2 = x2;
    x3 = VectorXd::Zero(Image_Num);
    r3 = x3;
    
    w12 = MyMat::Random(x2.size(), x1.size());
    w23 = MyMat::Random(x3.size(), x2.size());
    
    output = 0;
    
    ts = VectorXd::Zero(Image_Num);
  }

  //Random Device
  random_device rd;
  mt19937 mt(rd());
  uniform_real_distribution<double> d(0.0,1.0);

  //Record
  ofstream rec_main("../record/rec_main.txt");
  rec_main << "Assignment 2." << endl;
  rec_main << "Add Noise 0 - " << (int)round(100*NoiseRateMax) << " %" << endl;
  rec_main << "No. Neutral Neuron: " << Neutral_Num << endl;
  rec_main << "Learning Rate:      " << eta << endl;
  rec_main << "Convergence       : " << converge << endl;
  rec_main << "Limit of Training : " << Train_Num << endl;
  rec_main << endl;
  rec_main << "Image Size        : " << SIZE_X << ", " << SIZE_Y << endl;
  rec_main << "Image Num         : " << Image_Num << endl;
  rec_main << "Sample Num        : " << Image_Sample_Num << endl;
  rec_main << endl;
  ofstream ofs, ofs_train;
  cout << "fig" << endl;
  GP *rec_fig = new GP(false, "wxt", "Record", "Times New Roman", 12);
  rec_fig->setLog("gnuplot_log.plt");
  
  //Training
  rec_main << "[Training]" << endl;
  double E_RMS=1;
  VectorXd delta2 = x2;
  VectorXd delta3 = x3;
  {
    ofs_train.open("../record/rec_train.dat");

    int count=0;
    int image_id;
    int sample_id;
    int print_every=10;
    double conv_d = 1.0;
    double conv = 1.0;
    bool isRecMode = false;
    string rec_file;
    char buf[30];
    double p;

    while(count < Train_Num && E_RMS > converge){
      if(E_RMS <= conv) {
	while(E_RMS <= conv){
	  if(conv <= 2*conv_d){
	    conv_d *= 0.1;
	  }
	  conv -= conv_d;
	}
	isRecMode = true; p = 0;
	cout << "[Rec Mode] " << E_RMS << "|" << conv+conv_d;
      }
    
      E_RMS = 0;
      for(sample_id=0; sample_id < Image_Sample_Num; sample_id++){
	for(image_id=0; image_id < Image_Num; image_id++){
	  //(1)initialize x1, ts
	  for(int i=0; i<x1.size(); i++){
	    //Add Noise
	    {
	      if(trainNoise && d(mt) <= trainNoise){
		x1(i) = d(mt);
	      } else {
		x1(i) = image_data.at(sample_id * Image_Num + image_id)(i);
	      }
	    }
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
	  //(2')record output
	  if(isRecMode){
	    if(output == image_id) p+=1;
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
      }
      E_RMS = sqrt(E_RMS) / (double)(Image_Num*Image_Num*Image_Sample_Num);

      if(isRecMode){
	p = 100*p/(double)(Image_Num*Image_Sample_Num);
	cout << "\tP: " << p << " %" << endl;
	p = 0;
	isRecMode = false;
      }

      //(5)output E_RMS and count
      if(count % print_every == 0){
	cout << "(i: " << count << ") E_RMS: " << E_RMS << endl;
	ofs_train << count << " " << E_RMS << endl;
      }
      count++;
    }
   
    //(6)record results
    cout << "(i: " << count << ") E_RMS: " << E_RMS << "\t(Training Finished)" << endl;
    ofs_train << count << " " << E_RMS << endl;
    ofs_train.close();

    rec_fig->hwrite("set xlabel 'count of training' font 'Times New Roman,20'");
    rec_fig->hwrite("set ylabel 'E_RMS' font 'Times New Roman,20'");
    rec_fig->plotFile("../record/rec_train.dat", "",  "lines", "lw 3 lc 'blue'");
    rec_fig->save("../record/rec_train.png", "png");
  }

  //Test
  std::vector<double> result(Image_Num, 0);
  std::vector<double> totals;
  rec_main << "[Test]" << endl;
  {
    ofstream ofs_test;
    string rec_file;
    char buf[30];
    double testNoise = 0.0;
    for(testNoise = 0; testNoise <= NoiseRateMax; testNoise += 0.05){
      cout << "Noise Rate: " << 100*testNoise << " %" << endl;
      rec_main << "Noise Rate: " << 100*testNoise << " %" << endl;

      E_RMS = 0;
      for(int i=0;i<result.size();i++){
	result.at(i) = 0;
      }
      for(int sample_id=0; sample_id<Image_Sample_Num; sample_id++){
	for(int image_id=0; image_id<Image_Num; image_id++) {

	  //(1)initialize x1, ts
	  for(int i=0; i<x1.size(); i++){
	    //Add Noise
	    {
	      if(testNoise && d(mt) <= testNoise) {
		x1(i) = d(mt);
	      } else {
		x1(i) = image_data.at(sample_id * Image_Num + image_id)(i);
	      }
	    }
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
	  
	  //(3)judge output
	  for(int i=0;i<r3.rows();i++){
	    if(r3(output) < r3(i))
	      output = i;
	  }
	  if(output == image_id) result.at(image_id)+=1;

	  //(4)calculate error evaluator
	  err = r3 - ts;
	  E_RMS += (err.transpose()*err)(0);
	}
      }
    
      //(5)record result
      E_RMS = sqrt(E_RMS)/(double)(Image_Num*Image_Num);
      cout << "E_RMS: " << E_RMS << endl;
      rec_main << "E_RMS: " << E_RMS << endl;

      cout << "Correct :" << endl;
      rec_main << "Correct :" << endl;

      sprintf(buf,  "../record/rec_test%02d", (int)(100*testNoise));
      rec_file = buf;
      ofs_test.open(rec_file + ".dat");

      double p;
      int total = 0;
      for(int i=0; i<Image_Num; i++){
	p = round(100*result.at(i)/(double)Image_Sample_Num);
	cout << i << ":" << p << endl; 
	rec_main << i << ":" << p << endl;      
	ofs_test << "Image" << i << " " << p << endl;
	total += result.at(i);
      }

      totals.push_back(round(100 * total/(double)(Image_Sample_Num * Image_Num)));

      cout << "Total: " << totals.at(totals.size()-1) << " %" << endl;
      rec_main << "Total: " << totals.at(totals.size()-1) << " %" << endl;
      ofs_test << "- 0" << endl << "Total " << totals.at(totals.size()-1) << endl;
      ofs_test.close();

      //draw graph
      rec_fig->hwrite("set xlabel");
      rec_fig->hwrite("set ylabel 'Percent of Correct [%]'");
      sprintf(buf, "set title 'Test: %02d%% Noise'", (int)round(100*testNoise));
      rec_fig->hwrite(buf);
      rec_fig->hwrite("set yrange [0:100]");
      rec_fig->hwrite("set style fill solid border lc rgb 'black'");
      rec_fig->plotFile(rec_file + ".dat", "0:2:xtic(1)", "boxes", "lc rgb 'cyan'", "");
      rec_fig->save(rec_file + ".png", "png");   
    }
    //Record Result
    rec_file = "../record/rec_test-total";
    ofs.open(rec_file + ".dat");
    for(int i=0;i<totals.size();i++){
      ofs << i*5 << " " << totals.at(i) << endl;
    }
    ofs.close();

    rec_fig->hwrite("set xlabel 'Noise Rate [%]'");
    rec_fig->hwrite("set ylabel 'Percent of Correct [%}'");
    rec_fig->hwrite("set title 'Noise Test'");
    rec_fig->plotFile(rec_file + ".dat", "", "lines", "lw 3 lc 'red'", "");
    rec_fig->save(rec_file + ".png", "png");
  }
  rec_main.close();

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
