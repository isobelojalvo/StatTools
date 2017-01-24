cd diTauDatacards-2D/; 
root -l -b -q copyFiles.C;  
python unroll.py
root -l -b -q copy0Jet.C
root -l -b -q copyDataCards0Jet.C
root -l -b -q copyDataCardsBoosted.C
root -l -b -q copyDataCardsVBF.C
cp htt_tt.inputs-sm-13TeV-1D-final.root ~/public/dataCards/htt_tt.inputs-sm-13TeV-1D-Dec5.root
cd /data/ojalvo/Htt_80X/2016-sync/;  
python compareDatacards.py -f /data/ojalvo/Htt_80X/plotting/CMSSW_7_4_7/src/UWAnalysis/StatTools/data/sm_higgs_tautau/diTauDatacards-2D/htt_tt.inputs-sm-13TeV-1D.root ~/Htt/shapes/USCMS/htt_tt.inputs-sm-13TeV-svfitmass2D-20161125.root -t Prince,UW; 
cp tt_0jet-datacard-sync.pdf ~/.; 
cp tt_boosted-datacard-sync.pdf ~/.; 
cp tt_VBF-datacard-sync.pdf ~/.;
cd /data/ojalvo/Htt_80X/plotting/CMSSW_7_4_7/src/UWAnalysis/StatTools/data/sm_higgs_tautau;