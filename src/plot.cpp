#include <iostream>
#include <string>
#include <fstream>
#include "../gnuplot/Gnuplot.h"

using namespace std;
using namespace Gnuplot;

int main(int argc, char* argv[])
{
  if(argc < 2){
    cout << "Input data file name" << endl;
    return 1;
  }

  string fn, str;
  fn = argv[1];

  ifstream ifs(fn);
  if(ifs == NULL){
    cout << "File " << fn << " not exist" << endl;
    return 1;
  }
  ifs.close();

  
  GP* fig = new GP(false);

  if(argc < 3){
    cout << "Set title: " << endl;
    getline(cin, str);
  } else {
    str = argv[2];
  }
  fig->hwrite("set title '" + str + "' font 'Segoe UI,18'");

  if(argc < 4){
    cout << "Set xlabel: " << endl;
    getline(cin, str);
  } else {
    str = argv[3];
  }
  fig->hwrite("set xlabel '" + str + "' font 'Segoe UI,18'");

  if(argc < 5){
    cout << "Set ylabel: " << endl;
    cin >> str;
  } else {
    str = argv[4];
  }
  fig->hwrite("set ylabel '" + str + "' font 'Segoe UI,18'");

  str = "plot '" + fn + "'";

  if(argc >= 7) str += " using " + string(argv[6]);
  str += " with ";
  if(argc < 6) str += "lines";
  else str += argv[5];

  if(argc >= 8) str += " " + string(argv[7]);

  fig->hwrite(str);

  int it = fn.rfind(".");
  fn.resize(it+1);
  fn += "png";

  fig->save(fn, "png");

  return 0;
}





