#include "Gnuplot.h"

using namespace Gnuplot;

#ifndef _GNUPLOT_CPP
#define _GNUPLOT_CPP
int Handle::NEXTID = 0;
Handle* Handle::HEAD = NULL;
#endif

Handle::Handle(std::string options_) {
	std::stringstream ss;
	ss << GNUPLOT_EXE;
	if (options_.length()) {
		ss << " " << options_;
	}
	fp = popen(ss.str().c_str(), "w");
	setId();
}
void Handle::setId() {
	id = NEXTID;
	NEXTID += 1;

	nextHandle = HEAD;
	HEAD = this;
}
Handle::~Handle()
{
	Handle* curr = HEAD;
	if (curr == this) {
		HEAD = this->nextHandle;
	}
	while (curr->nextHandle != NULL) {
		if (curr->nextHandle == this) {
			curr->nextHandle = this->nextHandle;
			break;
		}
		curr = curr->nextHandle;
	}
	pclose(fp);
}

void Handle::write()
{
	buf << std::endl;
	send();
}
void Handle::write(std::string s)
{
	buf << s << std::endl;
	send();
}

void Handle::send()
{
	fputs(buf.str().c_str(), fp);
	fflush(fp);
	resetBuf();
}

void Handle::dispBuf()
{
	std::cout << buf.str() << std::endl;
}

void Handle::resetBuf()
{
	buf.str("");
}

Handle* Handle::findHandle(int id_) {
	Handle* curr = HEAD;
	while (curr != NULL) {
		if (curr->id == id_) break;
		curr = curr->nextHandle;
	}
	return curr;
}

GP::GP()
{
	h = new Handle();
}
GP::GP(std::string option_) {
	h = new Handle(option_);
}
GP::GP(int id) {
	h = Handle::findHandle(id);
}
GP::GP(Handle* h_) {
	h = h_;
}
GP::GP(bool isPOption_,
	std::string terminal_,
	std::string title_,
	std::string font_,
	double fontsize_,
	int wsize_x_,
	int wsize_y_,
	int woffset_x,
	int woffset_y
	)
{
	std::string ss = "";
	if (isPOption_) {
		ss  += " --persist";
	}
	h = new Handle(ss.c_str());

	ss = "";
	
	ss = "set terminal " + terminal_;
	if (title_.length() > 0)
		ss += " title \"" + title_ + "\"";
	if (font_.length() > 0)
		termSetting += " font '" + font_ + "," + std::to_string(fontsize_) + "'";
	termSetting += " size " + std::to_string(wsize_x_) + "," + std::to_string(wsize_y_);

	ss += termSetting;

	h->write(ss.c_str());
}
GP::~GP()
{
}
/*
void GP::plotFunc(std::string arg) {
	if (isHold) h->buf << "replot " << arg;
	else h->buf << "plot " << arg;
	h->write();
}
*/
void GP::plotFunc(std::string arg, int linewidth_, int linecolor_, std::string title_) {
	if (isHold) h->buf << "replot " << arg;
	else h->buf << "plot " << arg;	h->buf << " linewidth " << linewidth_;
	h->buf << " linecolor " << linecolor_;
	if (title_.length() > 0) h->buf << " title '" << title_ << "'";
	h->write();
}
void GP::plotFunc(std::string arg, int linewidth_, std::string linecolor_, std::string title_) {
	if (isHold) h->buf << "replot " << arg;
	else h->buf << "plot " << arg;
	h->buf << " linewidth " << linewidth_;
	h->buf << " linecolor '" << linecolor_ << "'";
	if (title_.length() > 0) h->buf << " title '" << title_ << "'";
	h->write();
}

void GP::plotVec2Raw(std::vector<double> xdata_, std::vector<double> ydata_, std::string option_) {
	if (xdata_.size() != ydata_.size()) return;
	h->buf << "plot '-' " << option_;
	h->write();
	inputVec2(xdata_, ydata_);
}
void GP::plotVec2(std::vector<double> xdata_, std::vector<double> ydata_, std::string title_, int linewidth_, std::string linecolor_) {
	if (xdata_.size() != ydata_.size()) return;
	h->buf << "plot '-' " << "with lines title '" << title_ << "' linewidth " << linewidth_ << " linecolor '" << linecolor_ << "'";
	h->write();
	inputVec2(xdata_, ydata_);
}
void GP::plotVec2(std::vector<double> xdata_, std::vector<double> ydata_, std::string title_, int linewidth_, int linecolor_) {
	if (xdata_.size() != ydata_.size()) return;
	h->buf << "plot '-' " << "with lines title '" << title_ << "' linewidth " << linewidth_ << " linecolor " << linecolor_;
	h->write();
	inputVec2(xdata_, ydata_);
}
void GP::plotVec2Multi(std::vector<double> xdata_, std::vector<std::vector<double> > ydataarray_, std::vector<std::string> optionarray_) {
	int ynum = ydataarray_.size();
	h->buf << "plot ";
	for (int i = 0;i < ynum;i++) {
		h->buf << "'-'";
		if (optionarray_.size() > 0)
			h->buf << " " << optionarray_.at(i);
		if (i < ynum - 1) h->buf << ", ";
	}
	h->write();
	inputVec2Multi(xdata_, ydataarray_);
}
void GP::plotVec2Multi(std::vector<double> xdata_, std::vector<std::vector<double> > ydataarray_, int linewidth_,
	std::vector<std::string> linecolors_, std::vector<std::string> titles_) {
	int ynum = ydataarray_.size();
	h->buf << "plot ";
	for (int i = 0;i < ynum;i++) {
		h->buf << "'-' with lines linewidth " << linewidth_;
		if (linecolors_.size() >= i && linecolors_.at(i).length() > 0)
			h->buf << " linecolor '" << linecolors_.at(i) << "'";
		if (titles_.size() >= i && titles_.at(i).length() > 0)
			h->buf << " title '" << titles_.at(i) << "'";
		if (i < ynum - 1) h->buf << ", ";
	}
	h->write();
	inputVec2Multi(xdata_, ydataarray_);
}

void GP::plotVec2(double* xdata_, double* ydata_, int num, std::string option_) {
	h->buf << "plot '-' " << option_;
	h->write();
	inputVec2(xdata_, ydata_, num);
}
void GP::plotVec2(double* xdata_, double* ydata_, int num, int linewidth_, std::string linecolor_) {
	h->buf << "plot '-' " << "with lines linewidth " << linewidth_ << " linecolor '" << linecolor_ << "'";
	h->write();
	inputVec2(xdata_, ydata_, num);
}
void GP::plotVec2(double* xdata_, double* ydata_, int num, int linewidth_, int linecolor_) {
	h->buf << "plot '-' " << "with lines linewidth " << linewidth_ << " linecolor " << linecolor_;
	h->write();
	inputVec2(xdata_, ydata_, num);
}
void GP::plotVec2Multi(double* xdata_, int len_, double* ydataarray_[], int ynum_, std::string optionarray_[]) {
	h->buf << "plot";
	for (int i = 0;i < ynum_;i++) {
		h->buf << " '-'";
		if (optionarray_ != NULL)
			h->buf << " " << optionarray_[i];
		if (i < ynum_ - 1) h->buf << ", ";
	}
	h->write();
	inputVec2Multi(xdata_, len_, ydataarray_, ynum_);
}
void GP::plotVec2Multi(double* xdata_, int len_, double* ydataarray_[], int ynum_, int linewidth_,
	std::vector<std::string> linecolors_, std::vector<std::string> titles_) {
	h->buf << "plot ";
	for (int i = 0;i < ynum_;i++) {
		h->buf << "'-' with lines linewidth " << linewidth_;
		if (linecolors_.size() >= i && linecolors_.at(i).length() > 0)
			h->buf << " linecolor '" << linecolors_.at(i) << "'";
		if (titles_.size() >= i && titles_.at(i).length() > 0)
			h->buf << " title '" << titles_.at(i) << "'";
		if (i < ynum_ - 1) h->buf << ", ";
	}
	h->write();
	inputVec2Multi(xdata_, len_, ydataarray_, ynum_);
}


void GP::inputVec2(std::vector<double> xdata_, std::vector<double> ydata_) {
	for (int i = 0;i < xdata_.size(); i++) {
		h->buf << xdata_.at(i) << " " << ydata_.at(i);
		h->write();
	}
	h->write("e");
}
void GP::inputVec2(double* xdata_, double* ydata_, int len_) {
	for (int i = 0;i < len_;i++) {
		h->buf << xdata_[i] << " " << ydata_[i];
		h->write();
	}
	h->write("e");
}
void GP::inputVec2Multi(std::vector<double> xdata_, std::vector<std::vector<double> > ydataarray_){
	int len = xdata_.size();
	int ynum = ydataarray_.size();
	for (int i = 0;i < ynum; i++) {
		for (int j = 0;j < len; j++) {
			h->buf << xdata_.at(j) << " " << ydataarray_.at(i).at(j);
			h->write();
		}
		h->write("e");
	}
}
void GP::inputVec2Multi(double* xdata_, int len_, double* ydataarray_[], int ynum_) {
	for (int i = 0;i < ynum_; i++) {
		for (int j = 0;j < len_; j++) {
			h->buf << xdata_[j]	<< " " << ydataarray_[i][j];
			h->write();
		}
		h->write("e");
	}
}

void GP::plotFile(std::string filename_,
		  std::string using_,
		  std::string mode_,
		  std::string option_,
		  std::string title_){
  std::string str;
  str = "plot '" + filename_ + "'";
  if(using_.length()){ str += " using " + using_; }
  if(mode_ == ""){ mode_ = "lines";}
  str += " with " + mode_;
  if(title_ == ""){ str += " notitle"; }
  else{ str += " title '" + title_ + "'"; }
  if(option_.length()){
    str += " " + option_;
  }
  h->write(str.c_str()); 
}

void GP::save(std::string filename_, std::string option_) {
	std::string arg;
	arg = "set terminal ";
	if (option_ == "") option_ = "emf";
	arg += "'" + option_ + "' " + termSetting;
	h->write(arg);
	arg = "set output '" + filename_ + "'";
	h->write(arg);
	replot();
	h->write("set terminal wxt");
	h->write("set output");
}
void GP::replot() {
	h->write("replot");
}
void GP::replotAll() {
	Gnuplot::GP* tmp;
	for (int i = 0;i < Handle::NEXTID; i++) {
		tmp = new Gnuplot::GP(i);
		if (tmp != NULL) tmp->hwrite("replot");
	}
}
