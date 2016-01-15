#include <iostream>
#include <string>
#include <fstream>
#include "../gnuplot/Gnuplot.h"

using namespace std;
using namespace Gnuplot;

int main(int argc, char* argv[])
{
  int lw = 3;

  int filenum = (argc-1)/3;

  if(filenum == 0){
    cout << "Input data file name" << endl;
    return 1;
  }

  string fn, str;
  GP* fig = new GP(false);
  fig->setLog("gnuplot_log.plt");

  cout << "Set title: " << endl;
  getline(cin, str);
  fig->hwrite("set title '" + str + "' font 'Segoe UI,18'");

  cout << "Set xlabel: " << endl;
  getline(cin, str);
  fig->hwrite("set xlabel '" + str + "' font 'Segoe UI,18'");

  cout << "Set ylabel: " << endl;
  getline(cin, str);
  fig->hwrite("set ylabel '" + str + "' font 'Segoe UI,18'");

  cout << "Set xrange: " << endl;
  getline(cin, str);
  if(str.length())
    fig->hwrite("set xrange ["+str+"]");

  cout << "Set yrange: " << endl;
  getline(cin, str);
  if(str.length())
    fig->hwrite("set yrange ["+str+"]");

  fn = argv[1];
  str = "plot '" + fn + "' using " + string(argv[2]);
  str += " with lines lw " + to_string(lw);
  if(string(argv[3]) == "n"){
    str += " notitle";
  } else {
    str += " title '" + string(argv[3]) + "'";
  }

  for(int i=1; i<filenum; i++){
    str += ",\\";
    cout << "[Send] " << str << endl;
    fig->hwrite(str);

    fn = argv[3*i+1];
    str = "'" + fn + "' using " + string(argv[3*i+2]);
    str += " with lines lw " + to_string(lw);
    if(string(argv[3*i+3]) == "n"){
      str += " notitle";
    } else {
      str += " title '" + string(argv[3*i+3]) + "'";
    }
  }
  cout << "[Send] " << str << endl;
  fig->hwrite(str);

  cout << "Save graph to 'rec.png'" << endl;
  fig->save("rec.png", "png");
  cout << "Save graph to 'rec.emf'" << endl;
  fig->save("rec.emf", "emf");

  return 0;
}





