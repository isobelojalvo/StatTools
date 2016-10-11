#include "TCanvas.h"
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1.h"
#include "TF1.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h" 
#include <math.h>
#include <boost/algorithm/string.hpp>
#include "HiggsAnalysis/CombinedLimit/interface/th1fmorph.h"
#include "HiggsAnalysis/CombinedLimit/interface/TH1Keys.h"
#include <TEfficiency.h>
#include <TMath.h>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
//#include "FineBins.h"

using std::cout;
using std::string;
using std::vector;
using std::endl;
using std::pair;
using std::make_pair;

struct BkgOutput {
	float DATA;
	float W, dW;
	float QCD, dQCD;
	float ZLFT, dZLFT;
	float ZJFT, dZJFT;
	float TOP, dTOP;
	float VV, dVV;
	float ZTT, dZTT;

	BkgOutput(){};

	BkgOutput(float i) {
		// This assumes that a-d are all of the same type and all in order
		std::fill(&DATA, &dZTT+1, i);
	}

};



class DataCardCreatorHThTh_2016 {
	public:

		DataCardCreatorHThTh_2016(optutl::CommandLineParser parser) {
			channel_ = parser.stringValue("channel");
			shifts_  = parser.stringVector("shifts");
			energy_ = parser.stringValue("energy");
			samesign_ = parser.doubleValue("samesign");


                        string name_=channel_;
			//if (channel_=="tauTau") 
			filelabel_="tt";


			//create the name you need to add to the histograms 
			//in the root file 
			for(unsigned int i=0;i<shifts_.size();++i) {
				string shiftL = shifts_.at(i);
				shiftL.resize(1);
				boost::to_lower(shiftL);
				cout<<"shiftL: "<<shiftL<<endl;
				string chan = "mutau";
				//Change Me
				if(channel_=="eleTau") chan = "etau";
				shiftsPostFix_.push_back("CMS_scale_"+shiftL+"_"+chan+"_"+energy_);
			}

			//read input files
			zttFile_  = parser.stringValue("zttFile");
			zllFile_  = parser.stringValue("zllFile");
			wFile_    = parser.stringValue("wFile");
			vvFile_   = parser.stringValue("vvFile");
			topFile_  = parser.stringValue("topFile");
			qcdFile_  = parser.stringValue("qcdFile");
			dataFile_  = parser.stringValue("dataFile");

			//read control and signal regions
			preSelection_ = parser.stringValue("preselection");

			signalSelection_      = parser.stringValue("signalselection");
			wSelection_           = parser.stringValue("wselection");
			qcdSelection_         = parser.stringValue("qcdSelection");
			relaxedSelection_     = parser.stringValue("relaxedselection");
			//bSelection_           = parser.stringValue("bselection");
			//antibSelection_       = parser.stringValue("antibSelection");
			//btagRelaxedSelection_ = parser.stringValue("btagRelaxedSelection");
			trigSelection_        = parser.stringValue("trigSelection");
			trigSelectionData_    = parser.stringValue("trigSelectionData");
			blinding_             = parser.stringValue("blinding");
			charge_               = parser.stringValue("charge");
                        if (samesign_) charge_="abs(charge)==2";
   
                  
			ZTT_genTauSel_        = "gen_match_1==5&&gen_match_2==5"; //Zttyield
			ZLFT_genLSel_         = "(gen_match_1==6||gen_match_2==6)";
			ZJFT_genLReject_      = "(gen_match_1==5&&gen_match_2<5||gen_match_1<5&&gen_match_2==5)";
			ZLL_genLLSel_        = "(gen_match_1==6&&gen_match_2==6)"; 

			if(samesign_>0)
			  osSignalSelection_    = signalSelection_+"&&abs(charge)==2";
			else
			  osSignalSelection_    = signalSelection_+"&&charge==0";

			ssSignalSelection_    = signalSelection_+"&&abs(charge)==2";
			if(samesign_>0)
			  osWSelection_         = wSelection_+"&&abs(charge)==2";
			else
			  osWSelection_         = wSelection_+"&&charge==0";

			ssWSelection_         = wSelection_+"&&abs(charge)==2";
			verbose_              = parser.integerValue("verbose");

			//read systematic uncertainties 
			luminosity_    = parser.doubleValue("luminosity");
			luminosityErr_ = parser.doubleValue("luminosityErr");
			//muID_          = parser.doubleValue("muID");
			//muIDErr_       = parser.doubleValue("muIDErr");
			bID_           = parser.doubleValue("bID");
			bIDErr_        = parser.doubleValue("bIDErr");
			bMisID_        = parser.doubleValue("bMisID");
			bMisIDErr_     = parser.doubleValue("bMisIDErr");
			//eleID_         = parser.doubleValue("eleID");
			//eleIDErr_      = parser.doubleValue("eleIDErr");
			tauID_         = parser.doubleValue("tauID");
			tauIDHigh_     = parser.doubleValue("tauIDHigh");
			tauIDErr_      = parser.doubleValue("tauIDErr");
			zttScale_      = parser.doubleValue("zttScale");
			zttScaleErr_   = parser.doubleValue("zttScaleErr");

			//read the basic varibale you will put in the histogram
			variable_      = parser.stringValue("variable");

			//read the event weight for MC and embedded
			weight_        = parser.stringValue("weight");
			Zweight_        = parser.stringValue("Zweight");
			TTweight_        = parser.stringValue("TTweight");
			embWeight_     = parser.stringValue("embWeight");

			//define the histogram binning
			bins_ = parser.integerValue("bins");
			min_  = parser.doubleValue("min");
			max_  = parser.doubleValue("max");

			//Define background uncertainty Errors
			topErr_     = parser.doubleValue("topErr");
			qcdErr_     = parser.doubleValue("qcdErr");
			vvErr_      = parser.doubleValue("vvErr");
			zlftErr_    = parser.doubleValue("zlftErr");
			zlftFactor_ = parser.doubleValue("zlftFactor");

			zjftErr_      = parser.doubleValue("zjftErr");
			wFactorErr_   = parser.doubleValue("wFactorErr");
			qcdFactor_    = parser.doubleValue("qcdFactor");
			qcdFactorErr_ = parser.doubleValue("qcdFactorErr");
			bFactorZ_     = parser.doubleValue("bFactorZ");
			bFactorZErr_  = parser.doubleValue("bFactorZErr");
			bFactorW_     = parser.doubleValue("bFactorW");
			bFactorWErr_  = parser.doubleValue("bFactorWErr");

			dir_ = parser.stringValue("dir");

			scaleUp_ = parser.doubleValue("scaleUp");

			fout_ = new TFile(parser.stringValue("outputfile").c_str(),"RECREATE");

		}


		void makeHiggsShape(string preselection, string categoryselection, string prefix) {

			pair<float,float> tmp;
			float legCorr=1.0;

			//if(muID_!=0&&eleID_!=0) {legCorr*=muID_*eleID_;}
			//if(muID_!=0&&eleID_==0) {legCorr*=muID_*tauID_;}
			//if(muID_==0&&eleID_!=0) {legCorr*=eleID_*tauID_;}
                        cout<<"Make Higgs Shape"<<endl;
			//tmp= createHistogramAndShifts(dir_+"ggH120.root","ggH120",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"ggH125.root","ggH125",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"ggH130.root","ggH130",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"vbfH120.root","qqH120",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"vbfH125.root","qqH125",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"vbfH130.root","qqH130",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
                        cout<<"susyggH"<<endl;

			//tmp= createHistogramAndShifts(dir_+"susyggH_80.root","ggH80",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_90.root","ggH90",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_100.root","ggH100",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_110.root","ggH110",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_120.root","ggH120",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_140.root","ggH140",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_160.root","ggH160",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_180.root","ggH180",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_200.root","ggH200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_250.root","ggH250",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_400.root","ggH400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_450.root","ggH450",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_500.root","ggH500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_600.root","ggH600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_700.root","ggH700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_800.root","ggH800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_900.root","ggH900",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_1000.root","ggH1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_1200.root","ggH1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_1400.root","ggH1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_1600.root","ggH1600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_1800.root","ggH1800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_2000.root","ggH2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_2300.root","ggH2300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_2600.root","ggH2600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_2900.root","ggH2900",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susyggH_3200.root","ggH3200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);


/*
                        cout<<"susybbH"<<endl;
			tmp= createHistogramAndShifts(dir_+"susybbH_80.root","bbH80",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_90.root","bbH90",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_100.root","bbH100",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_110.root","bbH110",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_120.root","bbH120",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_140.root","bbH140",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_160.root","bbH160",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_180.root","bbH180",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_200.root","bbH200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_250.root","bbH250",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_300.root","bbH300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_350.root","bbH350",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_400.root","bbH400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_450.root","bbH450",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_500.root","bbH500",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_600.root","bbH600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_700.root","bbH700",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_800.root","bbH800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_900.root","bbH900",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_1000.root","bbH1000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_1200.root","bbH1200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			//tmp= createHistogramAndShifts(dir_+"susybbH_1400.root","bbH1400",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_1600.root","bbH1600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_1800.root","bbH1800",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_2000.root","bbH2000",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_2300.root","bbH2300",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_2600.root","bbH2600",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_2900.root","bbH2900",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);
			tmp= createHistogramAndShifts(dir_+"susybbH_3200.root","bbH3200",("("+preselection+"&&"+categoryselection+"&&"+trigSelection_+"&&"+osSignalSelection_+")*"+weight_),luminosity_*legCorr,prefix);


*/
		}


		/*-------------Create Histograms------------------*/

		BkgOutput runOSLSMT(string preSelection,string prefix,string zShape, float topExtrap) {
                        cout<<"runOSLSMT"<<endl;

			BkgOutput output(0);    

			float leg1Corr=tauID_;
			float tauIDCorr=tauID_*tauID_;
			printf("Tau ID Scale Factor is %.3f \n",tauID_);

			/*
			  Input QCD selection 
			  then the estimate takes the yield and shape from the Relaxed Selection in OS
			  Relaxed Selection is one Tau Tight one Tau Anti-Tight
			  then multiply by an Loose to Tight SF 
			  QCD = OS Loose * (SS tight / SS Loose)
			  ->The regions for the scale factor should be calculated in a distribution without tails (i.e. tau eta)
			  ->The Yields in these regions should subtract the MC estimates
			  Needed: 1.) Yield estimator (no histograms)  2.) division and multiplication error propogator
			 */
			cout<<"Create QCD"<<endl;
			if(!runQCD(preSelection, prefix, zShape, topExtrap, output, "pt_1>-100", qcdSelection_)){ //pt_1>-100 is the category Selection
				cout<<"Error Creating QCD"<<endl;
				return output;
			}

                        cout<<"Create Data"<<endl;
			cout<<"      Data Selection: "<<preSelection<<"&&"<<osSignalSelection_<<endl;
			string fullSelection = preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_;
			string fullSelectionData = preSelection+"&&"+trigSelectionData_+"&&"+osSignalSelection_;

			pair<float,float> dataY     = createHistogramAndShifts(dataFile_,"data_obs","("+fullSelectionData+"&&"+blinding_+")",scaleUp_,prefix);
			output.DATA = dataY.first;

                        cout<<"Create Top"<<endl;
			//Create ttbar

			pair<float,float> topYield   = createHistogramAndShifts(topFile_,"TT",("("+fullSelection+")*"+weight_+"*"+TTweight_),luminosity_*tauIDCorr*topExtrap,prefix);
			pair<float,float> topInflYield  = inflateError(topYield,topErr_);

			output.TOP  = topInflYield.first;
			output.dTOP = topInflYield.second;

                        cout<<"Create VV"<<endl;
			//Create Diboson
			pair<float,float> vvYield      = createHistogramAndShifts(vvFile_,"VV",("("+fullSelection+")*"+weight_),luminosity_*tauIDCorr,prefix);
			pair<float,float> vvInflYield  = inflateError(vvYield,vvErr_);
			output.VV  = vvInflYield.first;
			output.dVV = vvInflYield.second;


                        cout<<"Create ZLFT"<<endl;
                        cout<<"      Factor ZLFT: "<<zlftFactor_<<endl;
			//Create ZL and ZJ

			pair<float,float> zlftYield   = createHistogramAndShifts(zllFile_,"ZL",("("+fullSelection+"&&"+ZLFT_genLSel_+")*"+weight_),luminosity_*leg1Corr*zlftFactor_*zttScale_,prefix,false);
			pair<float,float> zlftInflYield  = inflateError(zlftYield,zlftErr_);


			output.ZLFT  = zlftInflYield.first;
			output.dZLFT = zlftInflYield.second;

                        cout<<"Create ZJFT"<<endl;
			pair<float,float> zjftYield      = createHistogramAndShifts(zllFile_,"ZJ",("("+fullSelection+"&&"+ZJFT_genLReject_+")*"+weight_),luminosity_*leg1Corr*zttScale_,prefix);    
			pair<float,float> zjftInflYield  = inflateError(zjftYield,zjftErr_);

			output.ZJFT  = zjftInflYield.first;
			output.dZJFT = zjftInflYield.second;

                        cout<<"Create ZTT"<<endl;
			pair<float,float> ZTT    = createHistogramAndShifts(zttFile_,"ZTT",("("+fullSelection+")*"+weight_),luminosity_*tauIDCorr*zttScale_,prefix);    
			output.ZTT = ZTT.first;
			output.dZTT = ZTT.second;

			//Create W 
			//In principle osSignalSelection should work as a dummy variable
			cout<<"Create W"<<endl;
			pair<float,float> W    = createHistogramAndShifts(wFile_,"W",("("+fullSelection+")*"+weight_),luminosity_*leg1Corr,prefix);
			output.W = W.first;
			output.dW = W.second;
			
                        cout<<"Create QCD"<<endl;
			//Create QCD
			printf("      TTbar events in signal region = %f + %f \n",topInflYield.first,topInflYield.second);
			printf("      Diboson events before inflation = %f + %f \n",vvYield.first,vvYield.second);
			printf("      Diboson events in signal region = %f + %f \n",vvInflYield.first,vvInflYield.second);
			printf("      Z (l->tau) in signal region = %f + %f \n",zlftInflYield.first,zlftInflYield.second);
			printf("      Z (j->tau) in signal region = %f + %f \n",zjftInflYield.first,zjftInflYield.second);

			cout<<"=============Data Yields============="<<endl;
			cout<<"DATA: "<< output.DATA<<"==="<<endl;
			cout<<"BKGD Yields ==="<<endl;
			cout<<"QCD: "<< output.QCD<<endl;
			cout<<"W: "<< output.W<<endl;
			cout<<"TOP: "<< output.TOP<<endl;
			cout<<"VV: "<< output.VV<<endl;
			cout<<"ZLFT: "<< output.ZLFT<<endl;
			cout<<"ZJFT: "<< output.ZJFT<<endl;
			cout<<"ZTT: "<< output.ZTT<<endl;

			//TODO: Check that this outputs the correct values
			float background    = output.QCD  + output.W  + output.TOP  + output.VV  + output.ZLFT  + output.ZJFT  + output.ZTT;
			float backgroundErr = sqrt( pow(output.dQCD,2) + pow(output.dW,2) + pow(output.dTOP,2) + pow(output.dVV,2) + pow(output.dZLFT,2) + pow(output.dZJFT,2) + pow(output.dZTT,2));

			printf("BACKGROUND=%f +-%f \n",background,backgroundErr);


			float fullBackgroundErr = sqrt(pow(quadrature(output.VV,output.dVV,zttScaleErr_,tauIDErr_),2)
					+pow(quadrature(output.TOP,output.dTOP,tauIDErr_),2)
					+pow(quadrature(output.ZJFT,output.dZJFT,zttScaleErr_),2)
					+pow(quadrature(output.ZLFT,output.dZLFT,zttScaleErr_),2)
					+pow(output.dQCD,2)
					+pow(output.dW,2)
					+pow(quadrature(output.ZTT,output.dZTT,zttScaleErr_,tauIDErr_),2));

			printf("Total Background & %.2f $\\pm$ %.2f & - & - & - \\\\ \n",background,sqrt(pow(quadrature(output.VV,    output.dVV,   zttScaleErr_,tauIDErr_),2)
						+pow(quadrature(output.TOP,  output.dTOP,  tauIDErr_),2)
						+pow(quadrature(output.ZJFT, output.dZJFT, zttScaleErr_),2)
						+pow(quadrature(output.ZLFT, output.dZLFT, zttScaleErr_),2)
						+pow(output.dQCD,2)
						+pow(output.dW,2)
						+pow(quadrature(output.ZTT,output.dZTT,zttScaleErr_,tauIDErr_),2)));



			//create a histogram with the error for plotting reasons and only
			TH1F *err = new TH1F("BKGErr","",1,0,1);
			err->SetBinContent(1,fullBackgroundErr/background);
			fout_->cd((filelabel_+prefix).c_str());
			err->Write();

			return output;      
		}

		/*
		 * Run the full extrapolation in the Btag category
		 * Needed: BtagSF, categorySelection
		 *
		 */

		BkgOutput runFullExtrapBtag(string relaxedSelection, string wSel, string preSelection, string categorySelection_, string prefix, string zShape, float topExtrap, float zExtrap, float zExtrapErr, string bTagSF) {

			cout<<"======================"<<endl;
                        cout<<"Run Full Extrapolation"<<endl;
			cout<<"categorySelection: "<<categorySelection_<<endl;

			string categorySelection = categorySelection_;
			string dummySelection_   = osSignalSelection_;
			string fullSelection = preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_;
			
			BkgOutput output(0);    

			float leg1Corr=tauID_;
			float tauIDCorr=tauID_*tauID_;
			printf("Tau ID Scale Factor is %.3f \n",tauID_);

			/*
			  Input QCD selection 
			  then the estimate takes the yield and shape from the Relaxed Selection in OS
			  Relaxed Selection is one Tau Tight one Tau Anti-Tight
			  then multiply by an Loose to Tight SF 
			  QCD = OS Loose * (SS tight / SS Loose)
			  ->The regions for the scale factor should be calculated in a distribution without tails (i.e. tau eta)
			  ->The Yields in these regions should subtract the MC estimates
			  Needed: 1.) Yield estimator (no histograms)  2.) division and multiplication error propogator
			 */
			cout<<"Create QCD"<<endl;
			if(!runQCD(preSelection, prefix, zShape, topExtrap, output, "pt_1>-100", qcdSelection_)){ //pt_1>-100 is the category Selection
				cout<<"Error Creating QCD"<<endl;
				return output;
			}

                        cout<<"Create Data"<<endl;
			cout<<"      Data Selection: "<<preSelection<<"&&"<<osSignalSelection_<<endl;
			string fullSelection = preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_;
			string fullSelectionData = preSelection+"&&"+trigSelectionData_+"&&"+osSignalSelection_;

			pair<float,float> dataY     = createHistogramAndShifts(dataFile_,"data_obs","("+fullSelectionData+"&&"+blinding_+")",scaleUp_,prefix);
			output.DATA = dataY.first;

                        cout<<"Create Top"<<endl;
			//Create ttbar

			pair<float,float> topYield   = createHistogramAndShifts(topFile_,"TT",("("+fullSelection+")*"+weight_+"*"+TTweight_),luminosity_*tauIDCorr*topExtrap,prefix);
			pair<float,float> topInflYield  = inflateError(topYield,topErr_);

			output.TOP  = topInflYield.first;
			output.dTOP = topInflYield.second;

                        cout<<"Create VV"<<endl;
			//Create Diboson
			pair<float,float> vvYield      = createHistogramAndShifts(vvFile_,"VV",("("+fullSelection+")*"+weight_),luminosity_*tauIDCorr,prefix);
			pair<float,float> vvInflYield  = inflateError(vvYield,vvErr_);
			output.VV  = vvInflYield.first;
			output.dVV = vvInflYield.second;


                        cout<<"Create ZLFT"<<endl;
                        cout<<"      Factor ZLFT: "<<zlftFactor_<<endl;
			//Create ZL and ZJ

			pair<float,float> zlftYield   = createHistogramAndShifts(zllFile_,"ZL",("("+fullSelection+"&&"+ZLFT_genLSel_+")*"+weight_),luminosity_*leg1Corr*zlftFactor_*zttScale_,prefix,false);
			pair<float,float> zlftInflYield  = inflateError(zlftYield,zlftErr_);


			output.ZLFT  = zlftInflYield.first;
			output.dZLFT = zlftInflYield.second;

                        cout<<"Create ZJFT"<<endl;
			pair<float,float> zjftYield      = createHistogramAndShifts(zllFile_,"ZJ",("("+fullSelection+"&&"+ZJFT_genLReject_+")*"+weight_),luminosity_*leg1Corr*zttScale_,prefix);    
			pair<float,float> zjftInflYield  = inflateError(zjftYield,zjftErr_);

			output.ZJFT  = zjftInflYield.first;
			output.dZJFT = zjftInflYield.second;

                        cout<<"Create ZTT"<<endl;
			pair<float,float> ZTT    = createHistogramAndShifts(zttFile_,"ZTT",("("+fullSelection+")*"+weight_),luminosity_*tauIDCorr*zttScale_,prefix);    
			output.ZTT = ZTT.first;
			output.dZTT = ZTT.second;

			//Create W 
			//In principle osSignalSelection should work as a dummy variable
			cout<<"Create W"<<endl;
			pair<float,float> W    = createHistogramAndShifts(wFile_,"W",("("+fullSelection+")*"+weight_),luminosity_*leg1Corr,prefix);
			output.W = W.first;
			output.dW = W.second;
			
                        cout<<"Create QCD"<<endl;
			//Create QCD
			printf("      TTbar events in signal region = %f + %f \n",topInflYield.first,topInflYield.second);
			printf("      Diboson events before inflation = %f + %f \n",vvYield.first,vvYield.second);
			printf("      Diboson events in signal region = %f + %f \n",vvInflYield.first,vvInflYield.second);
			printf("      Z (l->tau) in signal region = %f + %f \n",zlftInflYield.first,zlftInflYield.second);
			printf("      Z (j->tau) in signal region = %f + %f \n",zjftInflYield.first,zjftInflYield.second);

			cout<<"=============Data Yields============="<<endl;
			cout<<"DATA: "<< output.DATA<<"==="<<endl;
			cout<<"BKGD Yields ==="<<endl;
			cout<<"QCD: "<< output.QCD<<endl;
			cout<<"W: "<< output.W<<endl;
			cout<<"TOP: "<< output.TOP<<endl;
			cout<<"VV: "<< output.VV<<endl;
			cout<<"ZLFT: "<< output.ZLFT<<endl;
			cout<<"ZJFT: "<< output.ZJFT<<endl;
			cout<<"ZTT: "<< output.ZTT<<endl;

			//TODO: Check that this outputs the correct values
			float background    = output.QCD  + output.W  + output.TOP  + output.VV  + output.ZLFT  + output.ZJFT  + output.ZTT;
			float backgroundErr = sqrt( pow(output.dQCD,2) + pow(output.dW,2) + pow(output.dTOP,2) + pow(output.dVV,2) + pow(output.dZLFT,2) + pow(output.dZJFT,2) + pow(output.dZTT,2));

			printf("BACKGROUND=%f +-%f \n",background,backgroundErr);


			float fullBackgroundErr = sqrt(pow(quadrature(output.VV,output.dVV,zttScaleErr_,tauIDErr_),2)
					+pow(quadrature(output.TOP,output.dTOP,tauIDErr_),2)
					+pow(quadrature(output.ZJFT,output.dZJFT,zttScaleErr_),2)
					+pow(quadrature(output.ZLFT,output.dZLFT,zttScaleErr_),2)
					+pow(output.dQCD,2)
					+pow(output.dW,2)
					+pow(quadrature(output.ZTT,output.dZTT,zttScaleErr_,tauIDErr_),2));

			printf("Total Background & %.2f $\\pm$ %.2f & - & - & - \\\\ \n",background,sqrt(pow(quadrature(output.VV,    output.dVV,   zttScaleErr_,tauIDErr_),2)
						+pow(quadrature(output.TOP,  output.dTOP,  tauIDErr_),2)
						+pow(quadrature(output.ZJFT, output.dZJFT, zttScaleErr_),2)
						+pow(quadrature(output.ZLFT, output.dZLFT, zttScaleErr_),2)
						+pow(output.dQCD,2)
						+pow(output.dW,2)
						+pow(quadrature(output.ZTT,output.dZTT,zttScaleErr_,tauIDErr_),2)));



			//create a histogram with the error for plotting reasons and only
			TH1F *err = new TH1F("BKGErr","",1,0,1);
			err->SetBinContent(1,fullBackgroundErr/background);
			fout_->cd((filelabel_+prefix).c_str());
			err->Write();

			return output;      

			/*
			//no b-tagging run yet
			//weight_ = weight_+"*"+bTagSF;

			BkgOutput output;    

			float leg1Corr=1.0;

			printf("Tau ID Scale Factor is %.3f \n",tauID_);

			cout<<"Select DATA"<<endl;
			pair<float,float> dataY         = createHistogramAndShifts(dataFile_,"data_obs","("+fullSelection+"&&"+categorySelection+")",scaleUp_,prefix);
			output.DATA = dataY.first;
			cout<<"      DATA Yield: "<< output.DATA<<endl;
			cout<<"      DATA Selection: "<<preSelection<<"&&"<<trigSelection_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<endl; 

                        cout<<"Create DiBoson"<<endl;
			//Create Diboson
			pair<float,float> vvYield      = createHistogramAndShifts(vvFile_,"VV",("("+fullSelection+"&&"+categorySelection+")*"+weight_),luminosity_*leg1Corr*tauID_,prefix);

			pair<float,float> vvInflYield  = inflateError(vvYield,vvErr_);
			output.VV  = vvInflYield.first;
			output.dVV = vvInflYield.second;

			//Create ZL and ZJ

                        cout<<"Create ZLFT"<<endl;
			//ZL Yield
			pair<float,float> zlftYield = createHistogramAndShifts(zllFile_,"ZLTmp",("("+fullSelection+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_),luminosity_*leg1Corr*zlftFactor_*zttScale_,prefix,false);
			//ZLShape
			pair<float,float> zlftShape = createHistogramAndShifts(zllFile_,   "ZL",("("+fullSelection+"&&"+categorySelection+"&&"+ZLFT_genLSel_+")*"+weight_+"*"+Zweight_),luminosity_*leg1Corr*zlftFactor_*zttScale_,prefix,false);
			pair<float,float> zlftInflYield  = inflateError(zlftYield,zlftErr_);

			renormalizeHistogram(filelabel_+prefix,"ZL",zlftInflYield.first);
			output.ZLFT  = zlftInflYield.first;
			output.dZLFT  = zlftInflYield.second;

                        cout<<"Create ZJFT"<<endl;
			//ZJ Yield
			pair<float,float> zjftYield     = createHistogramAndShifts(zllFile_,"ZJTmp",("("+fullSelection+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_),luminosity_*leg1Corr*zttScale_,prefix);

			//ZJ Shape
			pair<float,float> zjftShape     = createHistogramAndShifts(zllFile_,   "ZJ",("("+fullSelection+"&&"+categorySelection+"&&"+ZJFT_genLReject_+")*"+weight_+"*"+Zweight_),luminosity_*leg1Corr*zttScale_,prefix);

			pair<float,float> zjftInflYield  = inflateError(zjftYield,zjftErr_);
			renormalizeHistogram(filelabel_+prefix,"ZJ",zjftInflYield.first);
			output.ZJFT  = zjftInflYield.first;
			output.dZJFT = zjftInflYield.second;

                        cout<<"Create ZTT"<<endl;
			//TODO: Check me, previous Btag ZTT shape correction had a special normalization method
			pair<float,float> ztt  = createHistogramAndShifts(zttFile_,"ZTT",("("+fullSelection+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_+"*"+Zweight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);

			if(!runZTT(preSelection, prefix, zShape, topExtrap, output, categorySelection)){
				cout<<"Error Creating Ztt"<<endl;
				return output;
			}


                        cout<<"Create TOP"<<endl;
			//Create ttbar
			//Last argument is a dummy argument
			pair<float,float> topShape      = createHistogramAndShifts(topFile_,"TT",("("+fullSelection+"&&"+categorySelection+")*"+weight_+"*"+TTweight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);

			if(!runTOP(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection)){
				cout<<"Error Creating TOP"<<endl;
				return output;
			}


                        cout<<"Create W"<<endl;
			pair<float,float> wShape         = createHistogramAndShifts(wFile_,"W",("("+fullSelection+"&&"+categorySelection+")*"+weight_),luminosity_,prefix,false);
			//cout<<"Relaxed Selection: "<<relaxedSelection<<endl;
			//pair<float,float> wYield         = createHistogramAndShifts(wFile_,"WYield",("("+fullSelection+"&&"+categorySelection+")*"+weight_),luminosity_,prefix,false);
			output.W  = wShape.first;
			output.dW = wShape.second;

			if(!runW(preSelection, prefix, zShape, topExtrap, output, categorySelection, relaxedSelection, wSel)){
				cout<<"Error Creating W"<<endl;
				return output;
				}


                        cout<<"Create QCD"<<endl;
			//Create QCD
			if(!runQCD(preSelection, prefix, zShape, topExtrap, output, categorySelection, qcdSelection_)){
				cout<<"Error Creating QCD"<<endl;
				return output;
			}



			cout<<endl;
			cout<<"=============Data Yields============="<<endl;
			cout<<"DATA: "<< output.DATA<<endl;
			cout<<endl;
	
			cout<<"      VV before error inflation: "<<vvYield.first<<endl;
			printf("      Diboson events in signal region = %f + %f \n",vvInflYield.first,vvInflYield.second);
			cout<<"      VV after error inflation: "<<vvInflYield.first<<endl;
			printf("      Z (l->tau) in signal region = %f + %f \n",zlftInflYield.first,zlftInflYield.second);
			printf("      Z (j->tau) in signal region = %f + %f \n",zjftInflYield.first,zjftInflYield.second);

			cout<<endl;
			cout<<"BKGD Yields "<<endl;
			cout<<"QCD: "<< output.QCD<<endl;
			cout<<"W: "<< output.W<<endl;
			cout<<"TOP: "<< output.TOP<<endl;
			cout<<"VV: "<< output.VV<<endl;
			cout<<"ZLFT: "<< output.ZLFT<<endl;
			cout<<"ZJFT: "<< output.ZJFT<<endl;
			cout<<"ZTT: "<< output.ZTT<<endl;


			float background    = output.QCD  + output.W  + output.TOP  + output.VV  + output.ZLFT  + output.ZJFT  + output.ZTT;
			float backgroundErr = sqrt( pow(output.dQCD,2) + pow(output.dW,2) + pow(output.dTOP,2) + pow(output.dVV,2) + pow(output.dZLFT,2) + pow(output.dZJFT,2) + pow(output.dZTT,2));
			printf("BACKGROUND=%f +-%f \n",background,backgroundErr);

			///LATEX->Here since we want it for the note add all errors , even those that will go separate in the datacard

			//printf("LATEX ------------------------------------\n");
			//printf("Total & %.2f & %.2f & %.2f & %.2f \\\\ \n", dataYield.first, dataYieldSdb.first, dataSSYield.first, dataSSYieldSdb.first);
			//printf("Di-Boson & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & - & - \\\\ \n", vvInflYield.first, quadrature(vvInflYield.first,vvInflYield.second,zttScaleErr_,tauIDErr_), vvInflYieldSdb.first, quadrature(vvInflYieldSdb.first,vvInflYieldSdb.second,tauIDErr_));
			//printf("$t\\bar{t}$ & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & - & - \\\\ \n", topInflYield.first,quadrature(topInflYield.first,topInflYield.second,tauIDErr_), topInflYieldSdb.first, quadrature(topInflYieldSdb.first,topInflYield.second,tauIDErr_));
			//printf("$Z^{l+jet}$ & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", zjftInflYield.first, quadrature(zjftInflYield.first,zjftInflYield.second,zttScaleErr_), zjftInflSSYield.first,quadrature(zjftInflSSYield.first,zjftInflSSYield.second,zttScaleErr_));
			//printf("$Z^{ll}$ & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", zlftInflYield.first, quadrature(zlftInflYield.first,zlftInflYield.second,zttScaleErr_),zlftInflSSYield.first,quadrature(zlftInflSSYield.first,zlftInflSSYield.second,zttScaleErr_));
			//printf("$W+jets$ & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f  \\\\ \n", osWLow.first, osWLow.second, osWHigh.first, osWHigh.second, ssWLow.first, ssWLow.second, dataSSYieldSdb.first, dataSSYieldSdb.second);
			//printf("QCD & %.2f $\\pm$ %.2f & - & %.2f $\\pm$ %.2f & - \\\\ \n", osQCD.first, osQCD.second, ssQCD.first, ssQCD.second);
			//printf("$Z\\rightarrow\\tau\\tau$ & %.2f $\\pm$ %.2f & - & - & - \\\\ \n", zttYield.first,quadrature(zttYield.first,zttYield.second,zttScaleErr_,tauIDErr_));


			float fullBackgroundErr = sqrt(pow(quadrature(output.VV,output.dVV,zttScaleErr_,tauIDErr_),2)
					+pow(quadrature(output.TOP,output.dTOP,tauIDErr_),2)
					+pow(quadrature(output.ZJFT,output.dZJFT,zttScaleErr_),2)
					+pow(quadrature(output.ZLFT,output.dZLFT,zttScaleErr_),2)
					+pow(output.dQCD,2)
					+pow(output.dW,2)
					+pow(quadrature(output.ZTT,output.dZTT,zttScaleErr_,tauIDErr_),2));

			printf("Total Background & %.2f $\\pm$ %.2f & - & - & - \\\\ \n",background,sqrt(pow(quadrature(output.VV,    output.dVV,   zttScaleErr_,tauIDErr_),2)
						+pow(quadrature(output.TOP,  output.dTOP,  tauIDErr_),2)
						+pow(quadrature(output.ZJFT, output.dZJFT, zttScaleErr_),2)
						+pow(quadrature(output.ZLFT, output.dZLFT, zttScaleErr_),2)
						+pow(output.dQCD,2)
						+pow(output.dW,2)
						+pow(quadrature(output.ZTT,output.dZTT,zttScaleErr_,tauIDErr_),2)));



			//create a histogram with the error for plotting reasons and only
			TH1F *err = new TH1F("BKGErr","",1,0,1);
			err->SetBinContent(1,fullBackgroundErr/background);
			fout_->cd((filelabel_+prefix).c_str());
			err->Write();
*/
			return output;

		}


		/*
		 * For Inclusive take the Ztt normalization directly from MC (zttFile_)
		 * For a category the Ztt normalization is as follows,
		 * ZttCategoryYield = ZttMCInclusiveYield * (ZttEmbedded_Category) / *Ztt(
		 */

		bool runZTT(string preSelection,string prefix,string zShape, float topExtrap, BkgOutput &output,
				string categorySelection = "pt_1>-100", string relaxedSelection = "pt_1>-100"){

			float leg1Corr=1.0;

			string fullSelection = preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_;

			pair<float,float> ztt  = createHistogramAndShifts(zttFile_,"ZTT",("("+fullSelection+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_+"*"+Zweight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);
			pair<float,float> zttDyUp  = createHistogramAndShifts(zttFile_,"ZTT_CMS_htt_dyShape_13TeVUp",("("+fullSelection+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_+"*"+Zweight_+"*"+Zweight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);
			pair<float,float> zttDyDown  = createHistogramAndShifts(zttFile_,"ZTT_CMS_htt_dyShape_13TeVDown",("("+fullSelection+"&&"+categorySelection+"&&"+ZTT_genTauSel_+")*"+weight_),luminosity_*zttScale_*leg1Corr*tauID_,prefix);

			cout<<"      ZTT Selection: "<<preSelection<<"&&"<<trigSelection_<<"&&"<<osSignalSelection_<<"&&"<<categorySelection<<endl;
			output.ZTT  = ztt.first;
			output.dZTT = ztt.second;

			printf("      Selection ZTT events in signal region = %f + %f \n",ztt.first,ztt.second);
			return true;
		}


		bool runTOP( string preSelection, string prefix, string zShape, float topExtrap, BkgOutput &output, 
				string categorySelection, string relaxedSelection ) {


			float leg1Corr=1.0;
			string fullSelection = preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_;

			pair<float,float> topShape      = createHistogramAndShifts(topFile_,"TT",("("+fullSelection+"&&"+categorySelection+")*"+weight_+"*"+TTweight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);
			pair<float,float> topShapeUp      = createHistogramAndShifts(topFile_,"TT_CMS_htt_ttbarShape_13TeVUp",("("+fullSelection+"&&"+categorySelection+")*"+weight_+"*"+TTweight_+"*"+TTweight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);
			pair<float,float> topShapeDown      = createHistogramAndShifts(topFile_,"TT_CMS_htt_ttbarShape_13TeVDown",("("+fullSelection+"&&"+categorySelection+")*"+weight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);

			pair<float,float> topYield      = createHistogramAndShifts(topFile_,"TTYield",("("+fullSelection+"&&"+categorySelection+")*"+weight_+"*"+TTweight_), luminosity_*leg1Corr*tauID_*topExtrap, prefix);


			pair<float,float> topInflYield  = inflateError(topYield,topErr_);

			output.TOP  = topInflYield.first;
			output.dTOP = topInflYield.second;

			//Subtract the ztt ttbar overlap of 1.5% from the ttbar yield
			//not used for 2014 MSSM
			//output.TOP = output.TOP-(output.ZTT*0.015);   //only for  
			renormalizeHistogram(filelabel_+prefix,"TT",output.TOP);

			printf("      TTbar events in signal region = %f + %f \n",topInflYield.first,topInflYield.second);

			return true;}
		



		bool runQCD(string preSelection, string prefix, string zShape, float topExtrap, BkgOutput &output, string categorySelection, string qcdSelection) 
		{
			float leg1Corr=1.0;
			
			cout<<"QCD Selection= "<<"("<<qcdSelection_<<"&&"<<trigSelection_<<"&&"<<ssSignalSelection_<<"&&"<<categorySelection<<")"<<endl;
			std::string fullRelaxedSelectionDataSS = qcdSelection+"&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection;
			std::string fullRelaxedSelectionSS     = qcdSelection+"&&"+trigSelection_    +"&&"+ssSignalSelection_+"&&"+categorySelection;
			
			std::string fullRelaxedSelectionDataOS = qcdSelection+"&&"+trigSelectionData_+"&&"+osSignalSelection_+"&&"+categorySelection;
			std::string fullRelaxedSelectionOS     = qcdSelection+"&&"+trigSelection_    +"&&"+osSignalSelection_+"&&"+categorySelection;

			std::string fullSelectionDataSS        = preSelection+    "&&"+trigSelectionData_+"&&"+ssSignalSelection_+"&&"+categorySelection;
			std::string fullSelectionSS            = preSelection+    "&&"+trigSelection_    +"&&"+ssSignalSelection_+"&&"+categorySelection;

			std::string fullSelectionDataOS        = preSelection+    "&&"+trigSelectionData_+"&&"+osSignalSelection_+"&&"+categorySelection;
			std::string fullSelectionOS            = preSelection+    "&&"+trigSelection_    +"&&"+osSignalSelection_+"&&"+categorySelection;

			//estimate Yield in OS Loose
			//estimate Yield in SS Loose
			//estimate Yield in SS Signal
			std::cout<<"weight applied "<<weight_<<std::endl;
			pair<float,float> dataQCD = createHistogramAndShifts(dataFile_,   "QCD","("+fullRelaxedSelectionDataSS+")",scaleUp_,prefix); 
			pair<float,float> ZQCD    = createHistogramAndShifts(zttFile_,   "ZQCD","("+fullRelaxedSelectionSS+")*"+weight_+"*"+Zweight_,luminosity_,prefix); 
			pair<float,float> TopQCD  = createHistogramAndShifts(topFile_, "TopQCD","("+fullRelaxedSelectionSS+")*"+weight_+"*"+TTweight_,luminosity_,prefix); 
			pair<float,float> VVQCD   = createHistogramAndShifts(vvFile_,   "VVQCD","("+fullRelaxedSelectionSS+")*"+weight_,luminosity_,prefix,false);
			pair<float,float> WQCD    = createHistogramAndShifts(wFile_,     "WQCD","("+fullRelaxedSelectionSS+")*"+weight_,luminosity_,prefix,false);

			subtractHistogram(filelabel_+prefix,"QCD","ZQCD");
			subtractHistogram(filelabel_+prefix,"QCD","TopQCD");
			subtractHistogram(filelabel_+prefix,"QCD","VVQCD");
			subtractHistogram(filelabel_+prefix,"QCD","WQCD");

			//pair<float,float>YieldSSRelaxed = estimateQCDYield();
			pair<float,float>YieldSSLoose = estimateQCDYield("SS Loose Region", fullRelaxedSelectionDataSS, fullRelaxedSelectionSS);
			cout<<"     Data Events in SS Signal QCD loose sideband region = "<<YieldSSLoose.first<<" +/- " <<YieldSSLoose.second<<endl;
			pair<float,float>YieldOSLoose = estimateQCDYield("OS Loose Region", fullRelaxedSelectionDataOS, fullRelaxedSelectionOS);
			cout<<"     Data Events in OS Signal QCD loose sideband region = "<<YieldOSLoose.first<<" +/- " <<YieldOSLoose.second<<endl;
			pair<float,float>YieldSSTight = estimateQCDYield("SS Tight Region", fullSelectionDataSS,        fullSelectionSS);
			cout<<"     Data Events in SS Signal QCD tight sideband region = "<<YieldSSTight.first<<" +/- " <<YieldSSTight.second<<endl;

			float LooseToTightSF    = YieldSSTight.first/YieldSSLoose.first; 
			//Calculate SSTight/SSLoose
			float LooseToTightSFErr = errorDiv(YieldSSTight,YieldSSLoose);
			std::cout<<"Loose to Tight SF: "<<LooseToTightSF<<" +/- "<<LooseToTightSFErr<<std::endl;
			//Multiply OSLoose by SSTight/SSLoose
			float finalYield = YieldOSLoose.first*LooseToTightSF*qcdFactor_;
			float finalErr   = errorMult(YieldOSLoose,make_pair(LooseToTightSF,LooseToTightSFErr));
			std::cout<<"QCD Final Yield: "<<finalYield<<" +/- "<<finalErr<<std::endl;
			renormalizeHistogram(filelabel_+prefix,"QCD",finalYield);

			output.QCD = finalYield;
			output.dQCD = finalErr;
			if(shifts_.size()>0){
				qcdSyst(filelabel_, prefix, "QCD", 1.1, 0.1);
			}

			return true;


		}


		/* 
		 * W = R_W * SS_W * highMtlowMt 
		 * R_W = OS/SS (no Mt cut)
		 * SS_W MT>70 = [OS - OS_MCBKG - R_QCD*(SS - SS_MCBKG) ]
		 *         _______________________________________
		 * 			(R_W - R_QCD) 
		 *
		 * highMtlowMt per category
		 */

		bool runW(string preSelection,string prefix,string zShape, float topExtrap, BkgOutput &output, string categorySelection, string relaxedSelection, string wSel) {

		        string fullSelection = preSelection+"&&"+trigSelection_+"&&"+osSignalSelection_;
			pair<float,float> Rw = extractRFactor(wFile_,preSelection+"&&"+categorySelection,prefix);

			printf("          R_W = %f +- %f\n",Rw.first,Rw.second);
			//WShape is relaxed
			pair<float,float> wShape         = createHistogramAndShifts(wFile_,"W",("("+fullSelection+"&&"+relaxedSelection+")*"+weight_),luminosity_,prefix,false);
			cout<<"Relaxed Selection: "<<relaxedSelection<<endl;
			pair<float,float> wYield         = createHistogramAndShifts(wFile_,"WYield",("("+fullSelection+"&&"+categorySelection+")*"+weight_),luminosity_,prefix,false);
			cout<<"Straight W MC Yield: "<<wYield.first<<" stat error:" <<wYield.second<<endl;
			cout<<"Straight W MC Yield: "<<preSelection+"&&"<<trigSelection_<<"&&"<<categorySelection<<"&&"<<osSignalSelection_<<endl; 


			pair<float,float> dataSdb     = createHistogramAndShifts(dataFile_,"data_os_sdb","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+charge_+"&&"+wSel+")",scaleUp_,prefix);
			printf("      1. Select OS high MT W sideband in data: %f +- %f \n",dataSdb.first,dataSdb.second);
			pair<float,float> topSdb      = createHistogramAndShifts(topFile_,"TT_SDB",("("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+charge_+"&&"+wSel+")*"+weight_+"*"+TTweight_),luminosity_*tauID_*topExtrap,prefix);
			pair<float,float> vvSdb      = createHistogramAndShifts(vvFile_,"VV_SDB",("("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+charge_+"&&"+wSel+")*"+weight_),luminosity_*tauID_,prefix);
			pair<float,float> zSdb      = createHistogramAndShifts(zttFile_,"Z_SDB",("("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+charge_+"&&"+wSel+")*"+weight_+"*"+Zweight_),luminosity_*tauID_,prefix);

			printf("      2. Find QCD in SS sideband \n");
			pair<float,float> dataSSQCDSdb     = createHistogramAndShifts(dataFile_,"data_ss_sdb","("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+"charge!=0"+"&&"+wSel+")",scaleUp_,prefix);
			pair<float,float> topSSQCDSdb      = createHistogramAndShifts(topFile_,"TT_SS_SDB",("("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+"charge!=0"+"&&"+wSel+")*"+weight_+"*"+TTweight_),luminosity_*tauID_*topExtrap,prefix);
			pair<float,float> vvSSQCDSdb       = createHistogramAndShifts(vvFile_,"VV_SS_SDB",("("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+"charge!=0"+"&&"+wSel+")*"+weight_),luminosity_*tauID_,prefix);
			pair<float,float> zSSQCDSdb      = createHistogramAndShifts(zttFile_,"Z_SS_SDB",("("+preSelection+"&&"+trigSelection_+"&&"+categorySelection+"&&"+"charge!=0"+"&&"+wSel+")*"+weight_+"*"+Zweight_),luminosity_*tauID_,prefix);
			pair<float,float> wQCDSSQCDSdb		= make_pair(TMath::Nint(dataSSQCDSdb.first-topSSQCDSdb.first-vvSSQCDSdb.first-zSSQCDSdb.first),
					sqrt(dataSSQCDSdb.second*dataSSQCDSdb.second+topSSQCDSdb.second*topSSQCDSdb.second+vvSSQCDSdb.second*vvSSQCDSdb.second+zSSQCDSdb.second*zSSQCDSdb.second));
			pair<float,float> wQCDOSSdb = make_pair(wQCDSSQCDSdb.first*qcdFactor_,wQCDSSQCDSdb.second*qcdFactor_);


			printf("          qcd in W Sdb = %f +- %f\n",wQCDOSSdb.first,wQCDOSSdb.second);


			printf("      3. Subtract TTbar and diboson and ZJets and (datadriven) QCD from sideband \n");
			pair<float,float> osWHigh = make_pair(TMath::Nint(dataSdb.first-topSdb.first-vvSdb.first-zSdb.first-wQCDOSSdb.first),
					sqrt(dataSdb.second*dataSdb.second+topSdb.second*topSdb.second+vvSdb.second*vvSdb.second+zSdb.second*zSdb.second+wQCDOSSdb.second*wQCDOSSdb.second));

			pair<float,float> ssWHigh = make_pair(osWHigh.first/(Rw.first-qcdFactor_),osWHigh.second*Rw.second);

			printf("          dataWSdb = %f +- %f\n",dataSdb.first,dataSdb.second);
			printf("        - topWSdb = %f +- %f\n",topSdb.first,topSdb.second);
			printf("        - vvWSdb = %f +- %f\n",vvSdb.first,vvSdb.second);
			printf("        - zSdb = %f +- %f\n",zSdb.first,zSdb.second);
			printf("        - wQCDSdb = %f +- %f\n",wQCDOSSdb.first,wQCDOSSdb.second);
			printf("        = osWHigh = %f \n",osWHigh.first);
			printf("          Same Sign ssWHigh = %f = %f /( %f - %f) \n",ssWHigh.first,osWHigh.first,Rw.first,qcdFactor_);


			printf("      4. Extrapolate W in the low MT region\n");
			pair<float,float> wFactor = extractWFactor(wFile_,preSelection+"&&"+trigSelection_+"&&"+categorySelection,prefix,charge_+"&&"+wSel,osSignalSelection_);
			//pair<float,float> wFactor = extractWFactorFinal(wFile_,preSelection+"&&"+trigSelection_+"&&"+categorySelection,prefix);
			printf("          W extrapolation factor as measured in category MC = %f +- %f\n",wFactor.first,wFactor.second);

			pair<float,float> osWLow = make_pair(Rw.first*ssWHigh.first*wFactor.first,
					sqrt(Rw.first*Rw.first+ssWHigh.first*ssWHigh.first*wFactor.second*wFactor.second+ssWHigh.second*ssWHigh.second*wFactor.first*wFactor.first));

			printf("          OS W  in core  =%f *%f*%f  = %f +- %f \n",ssWHigh.first,Rw.first, wFactor.first,osWLow.first,osWLow.second);
			printf("          OS W  in MC = %f \n",wYield.first);


			renormalizeHistogram(filelabel_+prefix,"W",osWLow.first);
			output.W  = osWLow.first;
			output.dW  = osWLow.second;

			return true;
		}

		pair<float,float> estimateQCDYield(string text, string dataSelection, string mcSelection){
		  cout<<endl;
		  cout<<"Estimating the QCD Yield for :::: "<<text<< " ::::" <<endl;
		  //Data
		  pair<float,float> dataYield = getYield(dataFile_,dataSelection);
		  cout<<"Initial data Yield = "<<dataYield.first<<endl;
		  //Z
		  pair<float,float> ZYield    = getYield(zttFile_, "("+mcSelection+")*"+weight_,luminosity_*tauID_);
		  cout<<"           Z Yield    - "<<ZYield.first<<endl;
		  //W
		  pair<float,float> WYield    = getYield(wFile_, "("+mcSelection+")*"+weight_,luminosity_*tauID_);
		  cout<<"           W Yield    - "<<WYield.first<<endl;
		  //TOP
		  pair<float,float> TopYield  = getYield(topFile_, "("+mcSelection+")*"+weight_,luminosity_*tauID_);
		  cout<<"         Top Yield    - "<<TopYield.first<<endl;
		  //VV
		  pair<float,float> VVYield   = getYield(vvFile_,  "("+mcSelection+")*"+weight_,luminosity_*tauID_);
		  cout<<"         VV Yield    - "<<TopYield.first<<endl;
		  cout<<"                     -----------------"<<endl;
		  cout<<"                     "<<
		    dataYield.first - ZYield.first - WYield.first - TopYield.first - VVYield.first
		      <<" +/- "<<quadrature(dataYield.second,ZYield.second,WYield.second,TopYield.second,VVYield.second)
		      << endl;		  
		  float finalYield    = dataYield.first - ZYield.first - WYield.first - TopYield.first - VVYield.first;
		  float finalYieldErr = quadrature(dataYield.second,ZYield.second,WYield.second,TopYield.second,VVYield.second);

		  return make_pair(finalYield,finalYieldErr);
		}
		
		//-------------Helper Functions------------------

		//provide two numbers with errors that are to be divided, return division error
		float errorDiv(pair<float,float>numerator, pair<float,float>denominator){
		  float error = 0;
		  error = sqrt( (numerator.second/numerator.first)*(numerator.second/numerator.first) +  (denominator.second/denominator.first)*(denominator.second/denominator.first) ) * numerator.first/denominator.first;
		    return error;
		}
		//provide two numbers with errors that are to be multipled, return multiplication error
		float errorMult(pair<float,float>number1, pair<float,float>number2){
		  float error = 0;
		  error = sqrt( (number1.second/number1.first)*(number1.second/number1.first) +  (number2.second/number2.first)*(number2.second/number2.first) ) * number1.first * number2.first;
		  return error;
		}

		void addHistogramZTT(string folder, string histo1, string histo2)
		{
			cout<<"In AddHistogramZTT"<<endl;
			addHistogram(folder,histo1,histo2);
			for(unsigned int i=0;i<shifts_.size();++i) {
				TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Up").c_str());
				if(h1!=0) 
				{
					addHistogram(folder,(histo1+"_"+shiftsPostFix_[i]+"Up").c_str(),histo2);
				}
				else
				{
					printf("Shift not found = %s\n",(folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Up").c_str());
				}
				TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Down").c_str());
				if(h2!=0)
				{
					addHistogram(folder,(histo1+"_"+shiftsPostFix_[i]+"Down").c_str(),histo2);
				}		
				else
				{
					printf("Shift not found = %s\n",(folder+"/"+histo1+"_"+shiftsPostFix_[i]+"Down").c_str());
				}
			}//end for


		}//end addHistogram ZTT

		pair<float,float> createHistogramAndShifts(string file,string name, string cut,float scaleFactor = 1, string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
			TFile *f  = new TFile(file.c_str());
			if(f==0) printf("Not file Found\n");
			//get the nominal tree first
			TTree *t= (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
			if(t==0) printf("Not Tree Found in file %s\n",file.c_str());
			pair<float,float> yield;

			if(!keys)
				yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
			else
				yield =makeTHKeys(t,filelabel_+postfix,name,cut,scaleFactor);

			//now the shifts
			pair<float,float> tmpYield;
			for(unsigned int i=0;i<shifts_.size();++i) {

				TTree *ts= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Up/eventTree").c_str());
				if(ts!=0) {
					if(!keys)
						tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);
					else
						tmpYield = makeTHKeys(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

					if(!normUC)
						scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

				}
				TTree *td= (TTree*)f->Get((channel_+"EventTree"+shifts_[i]+"Down/eventTree").c_str());
				if(td!=0) {
					if(!keys)
						tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);
					else
						tmpYield = makeTHKeys(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

					if(!normUC)
						scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);

				}
			}
			f->Close();
			return yield;
		}


		pair<float,float> createHistogramAndShiftsFinal(string file,string name, string cut,float scaleFactor = 1, string postfix = "",bool normUC  = true, bool keys=false,bool ShapeUncertainty=true) {
			TFile *f  = new TFile(file.c_str());
			if(f==0) printf("Not file Found\n");
			//get the nominal tree first
			TTree *t= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());
			if(t==0) printf("Not Tree Found in file %s\n",file.c_str());

			pair<float,float> yield;

			if(!keys)
				yield =makeHistogram(t,filelabel_+postfix,name,cut,scaleFactor);
			else
				yield =makeTHKeys(t,filelabel_+postfix,name,cut,scaleFactor);

			//now the shifts
			pair<float,float> tmpYield;
			for(unsigned int i=0;i<shifts_.size();++i) {

				TTree *ts= (TTree*)f->Get((channel_+"EventTreeFinal"+shifts_[i]+"Up/eventTree").c_str());
				if(ts!=0) {
					if(!keys)
						tmpYield = makeHistogram(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);
					else
						tmpYield = makeTHKeys(ts,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",cut,scaleFactor);

					if(!normUC)
						scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Up",yield.first/tmpYield.first);

				}
				TTree *td= (TTree*)f->Get((channel_+"EventTreeFinal"+shifts_[i]+"Down/eventTree").c_str());
				if(td!=0) {
					if(!keys)
						tmpYield = makeHistogram(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);
					else
						tmpYield = makeTHKeys(td,filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",cut,scaleFactor);

					if(!normUC)
						scaleHistogram(filelabel_+postfix,name+"_"+shiftsPostFix_[i]+"Down",yield.first/tmpYield.first);

				}
			}
			f->Close();
			return yield;
		}


		void setBinning(const vector<double>& binning) {
			binning_ = binning;
		}

		pair<float,float> inflateError(pair<float,float> input,float error, float error2 =0.0, float error3 = 0.0 ) {
			float value = input.first;
			float err = sqrt(input.second*input.second+(input.first*error)*(input.first*error)+(input.first*error2)*(input.first*error2)+(input.first*error3)*(input.first*error3));

			if(err>value) {
				err=0.99*value;
				printf("Found error above value -> Setting error to value\n");
			}

			return make_pair(value,err);
		}

		void renormalizeHistogram(string folder, string histo, float yield)
		{

			TH1F * h =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
			double scaleFactor = yield/h->Integral();


			h->Scale(scaleFactor);
			fout_->cd(folder.c_str());
			h->Write(h->GetName(),TObject::kOverwrite);

			for(unsigned int i=0;i<shifts_.size();++i) {
				TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
				if(hh!=0) {
					hh->Scale(scaleFactor);
					fout_->cd(folder.c_str());
					hh->Write(hh->GetName(),TObject::kOverwrite);
				}
				else
				{
					printf("Shift not found = %s\n",(folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
				}


				TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
				if(hhh!=0) {
					hhh->Scale(scaleFactor);
					fout_->cd(folder.c_str());
					hhh->Write(hhh->GetName(),TObject::kOverwrite);
				}
				else
				{
					printf("Shift not found\n");
				}
			}
		}

		float quadrature(float value, float error, float e0 = 0.0, float e1 = 0.0 ,float e2 = 0.0, float e3 = 0.0 ,float e4 = 0.0, float e5 = 0.0 ){
			float returnValue = sqrt(error*error+(e0*e0+e1*e1+e2*e2+e3*e3+e4*e4+e5*e5)*value*value);
			return returnValue;
		}

		pair<float,float> makeHistogram(TTree* tree,string folder,string name,string cut,float scaleFactor = 1.) {

			if(fout_->Get(folder.c_str())==0)
				fout_->mkdir(folder.c_str());
			TH1F *h=0;

			TString variableOld_=variable_;
			if((name=="data_obs"||name=="data_obs_ss"||name=="data_obs_sdb"||name=="data_obs_ss_sdb"||name=="QCD")&&variable_=="pt2ES"){
				variable_="pt_2";
			}


			if(binning_.size()==0)
				h= new TH1F(name.c_str(),name.c_str(),bins_,min_,max_);
			else 
				h = new TH1F(name.c_str(),name.c_str(),binning_.size()-1,&binning_[0]);
			h->Sumw2();

			tree->Draw((variable_+">>"+name).c_str(),cut.c_str());

			variable_=variableOld_;//

			h->Scale(scaleFactor);

			if(verbose_>0)
				cout<< " " <<name<<": "<<h->Integral()<<endl;

			//     printf("Created Histogram %s with entries=%f\n",name.c_str(),h->Integral());
			fout_->cd(folder.c_str());

			Double_t error=0.0;
			//LD
			float yield = h->IntegralAndError(1,h->GetNbinsX(),error,"");
			//float yield = h->IntegralAndError(0,h->GetNbinsX()+1,error,"");
			if(yield == 0){
				h->SetBinContent(1,0.00001);
			}
			h->Write(h->GetName(),TObject::kOverwrite);

			return make_pair(yield,error);
		}

		pair<float,float> getYield(string file,string cut,float scaleFactor = 1.) {

			TFile *f  = new TFile(file.c_str());
			if(f==0) printf("Not file Found\n");

			//get the nominal tree first
			TTree *tree= (TTree*)f->Get("diTauEventTree/eventTree");
			if(tree==0) printf("Not Tree Found in file %s\n",file.c_str());

			TH1F *h=0;

			h= new TH1F("tmp","tmp",30,-5,5);
			h->Sumw2();

			tree->Draw("eta_1>>tmp",cut.c_str());

			h->Scale(scaleFactor);

			Double_t error=0.0;
			float yield = h->IntegralAndError(1,h->GetNbinsX(),error,"");

			return make_pair(yield,error);
		}


		void close() {
			fout_->Close();
		}


		pair<float,float> makeTHKeys(TTree* tree,string folder,string name,string cut,float scaleFactor = 1.) {

			if(fout_->Get(folder.c_str())==0)
				fout_->mkdir(folder.c_str());

			TH1Keys *h=0;

			if(binning_.size()==0)
				h= new TH1Keys("tmp","tmp",bins_,min_,max_,"a",0.5);
			else 
				h = new TH1Keys("tmp","tmp",binning_.size()-1,&binning_[0],"a",0.5);
			tree->Draw((variable_+">>tmp").c_str(),cut.c_str());

			if(h->Integral()==0.000) {
				printf("Keys for %s failed, retrying\n",name.c_str());
				delete h;
				if(binning_.size()==0)
					h= new TH1Keys("tmp","tmp",bins_,min_,max_,"a",0.5);
				else 
					h = new TH1Keys("tmp","tmp",binning_.size()-1,&binning_[0],"a",0.5);
				tree->Draw((variable_+">>tmp").c_str(),cut.c_str());

				if(h->Integral()==0.0) {
					printf("KEYS FAILED AGAIN \n");

				}
			}

			//now get the yield
			TH1F *hh=0;
			if(binning_.size()==0)
				hh= new TH1F("tmp2",name.c_str(),bins_,min_,max_);
			else 
				hh = new TH1F("tmp2",name.c_str(),binning_.size()-1,&binning_[0]);
			tree->Draw((variable_+">>tmp2").c_str(),cut.c_str());
			hh->Scale(scaleFactor);
			Double_t error=0.0;
			float yield = hh->IntegralAndError(1,hh->GetNbinsX(),error,"");

			fout_->cd(folder.c_str());
			TH1F * histo =(TH1F*) h->GetHisto();
			histo->Scale(yield/histo->Integral());
			histo->SetName(name.c_str());
			histo->Write();


			delete h;
			delete hh;

			return make_pair(yield,error);
		}

		pair<float,float> createHistogramShifted(string file,string name, string cut,string shift,float scaleFactor = 1, string postfix = "") {
			TFile *f  = new TFile(file.c_str());
			if(f==0) printf("Not file Found\n");
			//get the nominal tree first
			TTree *t= (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());
			if(t==0) printf("Not Tree Found in file %s\n",file.c_str());

			pair<float,float> yield;

			yield =makeHistogramShifted(t,filelabel_+postfix,name,cut,shift,scaleFactor);

			f->Close();
			return yield;
		}

		pair<float,float> makeHistogramShifted(TTree* tree,string folder,string name,string cut, string shift,float scaleFactor = 1.) {

			if(fout_->Get(folder.c_str())==0)
				fout_->mkdir(folder.c_str());
			TH1F *h=0;

			if(binning_.size()==0)
				h= new TH1F(name.c_str(),name.c_str(),bins_,min_,max_);
			else 
				h = new TH1F(name.c_str(),name.c_str(),binning_.size()-1,&binning_[0]);
			h->Sumw2();


			tree->Draw((variable_+"*"+shift+">>"+name).c_str(),cut.c_str());

			h->Scale(scaleFactor);
			//     printf("Created Histogram %s with entries=%f\n",name.c_str(),h->Integral());
			fout_->cd(folder.c_str());

			Double_t error=0.0;
			float yield = h->IntegralAndError(1,h->GetNbinsX(),error,"");
			if(yield == 0){
				h->SetBinContent(1,0.00001);
			}
			h->Write(h->GetName(),TObject::kOverwrite);

			return make_pair(yield,error);
		}

		void scaleHistogram(string folder, string histo, float scaleFactor)
		{
			TH1F * h =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
			h->Scale(scaleFactor);
			fout_->cd(folder.c_str());
			h->Write(h->GetName(),TObject::kOverwrite);
			fout_->cd();

			for(unsigned int i=0;i<shifts_.size();++i) {
				TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
				if(hh!=0) {
					hh->Scale(scaleFactor);
					fout_->cd(folder.c_str());
					hh->Write(hh->GetName(),TObject::kOverwrite);
					fout_->cd();
				}

				TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
				if(hhh!=0) {
					hhh->Scale(scaleFactor);
					fout_->cd(folder.c_str());
					hhh->Write(hhh->GetName(),TObject::kOverwrite);
					fout_->cd();
				}

			}
		}


		pair<float,float> extractRFactor(string file,string preselection,string postfix) {
			TFile *f  = new TFile (file.c_str());
			TTree *t = (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());

			pair<float,float> ss;
			pair<float,float> os;
			ss = makeHistogram(t,filelabel_+postfix,"W_SS",("("+preselection+"&&charge!=0)*"+weight_).c_str());
			os = makeHistogram(t,filelabel_+postfix,"W_OS",("("+preselection+"&&charge==0)*"+weight_).c_str());

			cout<<"W Rfactor Selection: ("<<preselection+"&&(charge requirement) )*"<<weight_<<endl;
			cout<<"W + Jets SS: "<< ss.first<<" OS: "<< os.first<<endl;

			float factor = os.first/ss.first;
			float factorerrStat = sqrt(os.second*os.second + ss.second*ss.second);
			//float factorerrSyst = factor*wFactorErr_;
			//float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);
			float factorErr = factorerrStat; 

			if(ss.first==0.){ 
				factor=0.;
				factorErr=0.;
			}
			return make_pair(factor,factorErr);   

		}





		pair<float,float> extractWFactor(string file,string preselection,string postfix, string Wweight = "1") {
			TFile *f  = new TFile (file.c_str());
			TTree *t = (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());

			pair<float,float> high;
			pair<float,float> low;
			high = makeHistogram(t,filelabel_+postfix,"W_High",("("+preselection+"&&"+wSelection_+")*"+weight_+"*"+Wweight).c_str());
			low = makeHistogram(t,filelabel_+postfix,"W_Low",("("+preselection+"&&"+signalSelection_+")*"+weight_+"*"+Wweight).c_str());

			cout<<"W + Jets High: "<< high.first<<" Low: "<< low.first<<endl;

			float factor = low.first/high.first;
			float factorerrStat = sqrt(low.second*low.second + high.second*high.second);
			float factorerrSyst = factor*wFactorErr_;
			float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);

			if(high.first==0.){ 
				factor=0.;
				factorErr=0.;
			}
			return make_pair(factor,factorErr);   

		}

		pair<float,float> extractWFactorFinal(string file,string preselection,string postfix, string Wweight = "1") {
			TFile *f  = new TFile (file.c_str());
			TTree *t = (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());

			pair<float,float> high;
			pair<float,float> low;

			high = makeHistogram(t,filelabel_+postfix,"W_High",("("+preselection+"&&"+wSelection_+")*"+weight_+"*"+Wweight).c_str());
			low = makeHistogram(t,filelabel_+postfix,"W_Low",("("+preselection+"&&"+signalSelection_+")*"+weight_+"*"+Wweight).c_str());

			cout<<"W + Jets High: "<< high.first<<" Low: "<< low.first<<endl;

			float factor = low.first/high.first;
			float factorerrStat = sqrt(low.second*low.second + high.second*high.second);
			float factorerrSyst = factor*wFactorErr_;
			float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);
			if(high.first==0.){
				factor=0.;
				factorErr=0.;
			}
			return make_pair(factor,factorErr);   

		}

		pair<float,float> extractWFactor(string file, string preselection, string postfix, string wSel, string sigSel) {
			TFile *f  = new TFile (file.c_str());
			TTree *t = (TTree*)f->Get((channel_+"EventTree/eventTree").c_str());
			//TTree *t = (TTree*)f->Get((channel_+"EventTreeFinal/eventTree").c_str());

			pair<float,float> high;
			pair<float,float> low;


			high = makeHistogram(t,filelabel_+postfix,"W_High",("("+preselection+"&&"+wSel+")*"+weight_).c_str(),luminosity_);
			low = makeHistogram(t,filelabel_+postfix,"W_Low",("("+preselection+"&&"+sigSel+")*"+weight_).c_str(),luminosity_);

			cout<<"      Cuts in extract WFactor "<<"      OS W Mt>70: "<<high.first<<"      OS W Mt Low: "<<low.first<<endl;
			cout<<"      Extract WFactor Second"<<"      OS W Mt>70: "<<high.second<<"      OS W Mt Low: "<<low.second<<endl;
			float factor = low.first/high.first;
			float factorerrStat = sqrt(low.second*low.second + high.second*high.second);
			float factorerrSyst = factor*wFactorErr_;
			float factorErr = sqrt(factorerrStat*factorerrStat+factorerrSyst*factorerrSyst);


			cout<<"      WFactor Stat Error "<<"      factor: "<<factor<<"      StatError: "<<factorerrStat<<endl;
			cout<<"      WFactor Syst Error "<<"      factor*wFactorErr_: "<<factor<<"*"<<wFactorErr_<<" = "<<factorerrSyst<<endl; 
			cout<<"      WFactor Error "<<"      factorError = sqrt(Syst Error^2+Stat Error^2)= "<<factorErr<<endl; 
			if(high.first==0.){
				factor=0.;
				factorErr=0.;
			}
			return make_pair(factor,factorErr);   

		}

		void mergeHistogram(string folder, string histo1, string histo2, string newName)
		{
			TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
			TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
			TH1F *hNew =(TH1F*) h1->Clone(TString(newName));
			hNew->Add(h2,1.);

			for(int i=1;i<h1->GetNbinsX();++i){
				if(h1->GetBinContent(i)<0){
					h1->SetBinContent(i,0.00001);
					h1->SetBinError(i,1.0);
				}
			}

			fout_->cd(folder.c_str());
			hNew->Write(TString(newName),TObject::kOverwrite);
		}

		pair<float,float> convertToPoisson(pair<float,float> measurement) {
			float yield = measurement.first;
			float CLHi = TMath::ChisquareQuantile(1-0.32/2,2*yield+2)/2.;
			float CLLo = TMath::ChisquareQuantile(0.32/2,2*yield)/2.;
			printf("Yield =%f Lo=%f Hi=%f\n",measurement.first,CLLo,CLHi);
			return make_pair(measurement.first,(CLHi-CLLo)/2.);

		}

		void subtractHistogram(string folder, string histo1, string histo2)
		{
			TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
			TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
			h1->Add(h2,-1.);

			for(int i=1;i<h1->GetNbinsX();++i){
				if(h1->GetBinContent(i)<0){
					h1->SetBinContent(i,0.00001);
					h1->SetBinError(i,1.0);
				}
			}

			fout_->cd(folder.c_str());
			h1->Write(h1->GetName(),TObject::kOverwrite);
		}

		void addHistogram(string folder, string histo1, string histo2)
		{
			TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
			TH1F * h2 =(TH1F*) fout_->Get((folder+"/"+histo2).c_str());
			h1->Add(h2,1.);

			fout_->cd(folder.c_str());
			h1->Write(h1->GetName(),TObject::kOverwrite);
		}

		void renameHist(string folder, string name ,string histo1)///folder, new name, old name
		{
			TH1F * h1 =(TH1F*) fout_->Get((folder+"/"+histo1).c_str());
			TH1F * h2 = (TH1F*) h1->Clone(TString(name));

			h2->Write(h2->GetName(),TObject::kOverwrite);
		}


		void FixNegativeBins(string folder, string histo, bool is_qcd, bool verbose=false) {

			TH1F * hist =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
			unsigned bins = hist->GetNbinsX();
			for (unsigned i = 1; i <= bins; ++i) {
				if (hist->GetBinContent(i) < 0.){
					hist->SetBinContent(i,0);
					if(verbose)
						cout<<"Bin "<< i << " is negative, setting to 0"<<endl;
				}
			}
			hist->Write(hist->GetName(),TObject::kOverwrite);
		}

		void FixNegativeBinsMC(string folder, string histo, bool is_qcd, bool verbose=false){

			FixNegativeBins(folder, histo, is_qcd, verbose);

			for(unsigned int i=0;i<shifts_.size();++i) {
				TH1F * hh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Up").c_str());
				if(hh!=0) {
					FixNegativeBins(folder, (histo+"_"+shiftsPostFix_[i]+"Up").c_str(), is_qcd, verbose);
				}

				TH1F * hhh =(TH1F*) fout_->Get((folder+"/"+histo+"_"+shiftsPostFix_[i]+"Down").c_str());
				if(hhh!=0) {
					FixNegativeBins(folder, (histo+"_"+shiftsPostFix_[i]+"Down").c_str(), is_qcd, verbose);
				}

			}
		}


		void FixEmptyBins(string folder, string histo, bool is_qcd, bool verbose=false) {

			TH1F * hist =(TH1F*) fout_->Get((folder+"/"+histo).c_str());
			unsigned bins = hist->GetNbinsX();
			unsigned first_populated = 0;
			unsigned last_populated = 0;
			for (unsigned i = 1; i <= bins; ++i) {
				if (hist->GetBinContent(i) > 0. && first_populated == 0) first_populated = i;
				if (hist->GetBinContent(bins-(i-1)) > 0. && last_populated == 0) last_populated = bins-(i-1);
			}
			if (last_populated <= first_populated) {
				if (verbose) cout << "Cannot correct this distribution!" << endl;
				return;
			}
			if (verbose) cout << "First populated bin: " << first_populated << endl;
			if (verbose) cout << "Last populated bin: " << last_populated << endl;
			double av_weight = ( hist->Integral() / double(hist->GetEntries()));
			if (!is_qcd) {
				if (verbose) cout << "Integral: " << hist->Integral() << endl;
				if (verbose) cout << "Entries: " << hist->GetEntries() << endl;
				if (verbose) cout << "Av. Weight: " << av_weight << endl;
			}
			for (unsigned i = first_populated+1; i < last_populated; ++i) {
				if (hist->GetBinContent(i) == 0.) {
					if (verbose) cout << "Bin " << i << " is empty!" << endl;
					if (is_qcd) {
						hist->SetBinError(i, 1.0);
					} else {
						if (verbose) cout << "Set weight to 1.0 * av_weight = " << (1.0 * av_weight) << endl;
						hist->SetBinError(i, av_weight);    
					}
				}
			}
			hist->Write(hist->GetName(),TObject::kOverwrite);
		}


		bool qcdSyst(string channel, string prefix, string histo1, float corr, float unc)
		{

			string chan = "mt";
			if(channel == "eleTau")
				chan = "et";

			TH1F * h1 =(TH1F*) fout_->Get((channel+prefix+"/"+histo1).c_str());
			TH1F * h2 = (TH1F*)h1->Clone(TString(histo1+string("_CMS_htt_QCDShape_"+chan+prefix+"_"+energy_+"Up")));
			TH1F * h3 = (TH1F*)h1->Clone(TString(histo1+string("_CMS_htt_QCDShape_"+chan+prefix+"_"+energy_+"Down")));
			cout << "Channel label is " << chan << endl;
			float x,y;
			float cUp = corr+unc;
			float cDown = corr-unc;
			//printf("Creating QCD Shape Systematic \n");
			float scale = h1->Integral();
			for(int i=1;i<h1->GetNbinsX();++i){
				x = h1->GetXaxis()->GetBinCenter(i);
				if(x<50){
					y = h1->GetBinContent(i);
					h1->SetBinContent(i,y*corr);
					h2->SetBinContent(i,y*cUp);
					h3->SetBinContent(i,y*cDown);
				}

			}
			h1->Scale(scale/h1->Integral());
			h2->Scale(scale/h2->Integral());
			h3->Scale(scale/h3->Integral());
			h1->Write(h1->GetName(),TObject::kOverwrite);
			h2->Write(h2->GetName(),TObject::kOverwrite);
			h3->Write(h3->GetName(),TObject::kOverwrite);        

			return true;
		}



	private:
		string channel_;
		string filelabel_;
		vector<string> shifts_;
		vector<string> shiftsPostFix_;
		string energy_;
		int samesign_;

		//files
		TFile *fout_;
		int verbose_;
		string zttFile_;
		string zllFile_;
		string wFile_;
		string vvFile_;
		string topFile_;
		string qcdFile_;
		string dataFile_;
		///////////////////////

		//selections of regions
		string preSelection_;
		string osSignalSelection_;
		string ssSignalSelection_;
		string signalSelection_;
		string osWSelection_;
		string ssWSelection_;
		string wSelection_;
		string qcdSelection_;
		//string bSelection_;
		//string antibSelection_;
		string relaxedSelection_;
		//string btagRelaxedSelection_;
		string trigSelection_;
		string trigSelectionData_;
		string blinding_;
		string charge_;
		string ZLL_genLLSel_;
		string ZTT_genTauSel_;
		string ZJFT_genLReject_;
		string ZLFT_genLSel_;
		string dataSelection_;

		//Luminosity and efficiency corrections
		float luminosity_;
		float luminosityErr_;
		//float  muID_   ;      
		//float muIDErr_;      
		//float eleID_ ;       
		//float eleIDErr_;     
		float tauID_  ;
		float tauIDHigh_;      
		float tauIDErr_;     
		float zttScale_;     
		float zttScaleErr_;  

		float scaleUp_;

		//histogram options
		string variable_;
		int bins_;
		float min_;
		float max_;
		vector<double> binning_;
		string weight_;
		string Zweight_;
		string TTweight_;
		string embWeight_;

		//external parameters
		float topErr_;
		float qcdErr_;//tmp
		float vvErr_;
		float zlftErr_;
		float zlftFactor_;
		float zjftErr_;
		float wFactorErr_;
		float qcdFactor_;
		float qcdFactorErr_;


		float bFactorZ_;
		float bFactorZErr_;
		float bFactorW_;
		float bFactorWErr_;

		float vbfFactorZ_;
		float vbfFactorZErr_;
		float vbfFactorW_;
		float vbfFactorWErr_;


		float boostFactorZ_;
		float boostFactorZErr_;
		float boostFactorW_;
		float boostFactorWErr_;

		float vhFactorZ_;
		float vhFactorZErr_;
		float vhFactorW_;
		float vhFactorWErr_;


		float bID_;
		float bIDErr_;
		float bMisID_;
		float bMisIDErr_;

		vector<string> mssmMasses_;
		vector<string> smMasses_;
		vector<string> smMassesDC_;

		vector<float> mssmBBFraction_;
		vector<float> smSigma_;
		vector<float> vbfSigma_;
		vector<float> vhSigma_;

		string dir_;


};
