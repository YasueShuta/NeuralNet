#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Gnuplot {

	class Handle
	{
	private:
		FILE* fp;

		void send();
		void setId();
	public:
		static int NEXTID;
		static Handle* HEAD;

		int id;
		Handle* nextHandle;
		//const char* GNUPLOT_EXE = "C:/gnuplot/bin/gnuplot.exe";
		const char* GNUPLOT_EXE = "gnuplot ";
		std::stringstream buf;

		Handle(std::string options_ = "");
		~Handle();
		void write();
		void write(std::string s);
		void dispBuf();
		void resetBuf();

		static Handle* findHandle(int id_);
	};

	class GP
	{
	private:
		Handle* h;
		std::string termSetting = "";
		bool isHold = false;

	public:
		void hwrite() { h->write(); };
		void hwrite(std::string s) { h->write(s); }
		int getId() { return h->id; }
		void holdOn() {	isHold = true;}
		void holdOff() { isHold = false; }

		//void plotFunc(std::string arg);
		void plotFunc(std::string arg, int linewidth, int linecolor, std::string title_ = "");
		void plotFunc(std::string arg, int linewidth = 3, std::string linecolor = "blue", std::string title_ = "");
		//		void plotVec1();
		void plotVec2Raw(std::vector<double> xdata_, std::vector<double> ydata_, std::string option_);
		void plotVec2(std::vector<double> xdata_, std::vector<double> ydata_, std::string title_, int linewidth_, std::string linecolor_);
		void plotVec2(std::vector<double> xdata_, std::vector<double> ydata_, std::string title_ = "data", int linewidth_ = 3, int linecolor_ = 1);
		void plotVec2Multi(std::vector<double> xdata_, std::vector<std::vector<double> > ydataarray_, std::vector<std::string> optionarray_ = std::vector<std::string>(0));
		void plotVec2Multi(std::vector<double> xdata_,
			std::vector<std::vector<double> > ydataarray_,
			int linewidth_ = 1,
			std::vector<std::string> linecolors_ = std::vector<std::string>(0),
			std::vector<std::string> titles_ = std::vector<std::string>(0));
		void plotVec2(double* xdata_, double* ydata_, int len_, std::string option);
		void plotVec2(double* xdata_, double* ydata_, int len_, int linewidth_, std::string linecolor_);
		void plotVec2(double* xdata_, double* ydata_, int len_, int linewidth_ = 3, int linecolor_ = 1);
		void plotVec2Multi(double* xdata_, int len_, double* ydataarray_[], int ynum, std::string optionarray_[]);
		void plotVec2Multi(double* xdata_, int len_, double* ydataarray_[], int ynum,
			int linewidth_ = 1,
			std::vector<std::string> linecolors_ = std::vector<std::string>(0),
			std::vector<std::string> titles_ = std::vector<std::string>(0));

		void inputVec2(std::vector<double> xdata_, std::vector<double> ydata_);
		void inputVec2(double* xdata_, double* ydata_, int len_);
		void inputVec2Multi(std::vector<double> xdata_, std::vector<std::vector<double> > ydataarray_);
		void inputVec2Multi(double* xdata_, int len_, double* ydataarray_[], int ynum_);

		void plotFile(std::string filename_,
			      std::string mode_,
			      std::string using_,
			      std::string option_,
			      std::string title_=""); 

		void save(std::string filename, std::string option="");
		void replot();

		static void replotAll();

		GP();
		GP(std::string option_);
		GP(int id);
		GP(Handle* h_);
		GP(bool isPOption_,
		   std::string terminal_ = "wxt", //"windows",
		   std::string title_ = "Figure",
		   std::string font_ = "Times New Roman",
		   double fontsize_ = 18,
		   int wsize_x_ = 640,
		   int wsize_y_ = 330,
		   int woffset_x_ = 30,
		   int woffset_y_ = 30
		   );
		~GP();
	};
}


