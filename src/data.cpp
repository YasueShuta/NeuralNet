#include <fstream>
#include <iostream>

using namespace std;

int main()
{
  string loadname = "sample.dat";
  string savename = "rec.dat";

  ifstream ifs(loadname);
  if(ifs == NULL) return 1;
  ofstream ofs(savename);

  string line;
  int it;
  while(getline(ifs, line)){
    //TODO
    if( line.substr(0,5) == "Noise" ){
      it = line.find(':');
      line = line.substr(it+2);
      line.erase(line.length()-1);
      ofs << line;            //noiserate_
    } else if(line.substr(0,5) == "E_RMS" ){
      it = line.find(':');
      line = line.substr(it+2);
      ofs << line << " ";            //noiserate_Erms_
    } else if(line.substr(0,5) == "Total" ){
      it = line.find(':');
      line = line.substr(it+2);
      line.erase(line.length()-1);
      ofs << line << endl;
    }

    cout << line << endl;
  }
  return 0;
}
