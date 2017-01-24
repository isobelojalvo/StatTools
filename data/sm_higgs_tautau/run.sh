cd diTauDatacards-mvis/; 
root -l -b -q copyFiles.C;  
cd /data/ojalvo/Htt_80X/2016-sync/;  
python compareDatacards.py -f /data/ojalvo/Htt_80X/plotting/CMSSW_7_4_7/src/UWAnalysis/StatTools/data/sm_higgs_tautau/diTauDatacards-mvis/htt_tt.inputs-sm-13TeV.root /afs/cern.ch/user/t/truggles/public/forIsobel/SM-HTT_dataCards/htt_tt.inputs-sm-13TeV_MVIS_2D_NoUnroll_20161206.root -t Prince,UW; 
cp tt_0jet-datacard-sync.pdf ~/.; 
cp tt_boosted-datacard-sync.pdf ~/.; 
cp tt_VBF-datacard-sync.pdf ~/.;
cd /data/ojalvo/Htt_80X/plotting/CMSSW_7_4_7/src/UWAnalysis/StatTools/data/sm_higgs_tautau;