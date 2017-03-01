cd diTauDatacards-mvis-moriond/; 
root -l -b -q copyFiles.C;  
cd ../
cp -r diTauDatacards-mvis-moriond ~/public/syncntuple/.
cd /data/ojalvo/Htt_80X/moriond17Sync/2016-sync/
python compareDatacards.py ~/public/syncntuple/diTauDatacards-mvis-moriond/htt_tt.inputs-sm-13TeV.root  /afs/cern.ch/user/o/ojalvo/private/plotting/CMSSW_7_4_7/src/UWAnalysis/StatTools/data/sm_higgs_tautau/shapes/USCMS/htt_tt.inputs-sm-13TeV-2D-20170227.root  -t Prince,UW
#cp tt_0jet-datacard-sync.pdf ~/.; 
#cp tt_boosted-datacard-sync.pdf ~/.; 
#cp tt_VBF-datacard-sync.pdf ~/.;
cp tt_inclusive-datacard-sync.pdf ~/.
cd /data/ojalvo/Htt_80X/plotting/CMSSW_7_4_7/src/UWAnalysis/StatTools/data/sm_higgs_tautau;