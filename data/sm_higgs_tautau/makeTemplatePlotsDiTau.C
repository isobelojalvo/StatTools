{
   gROOT->ProcessLine(".L ../../ROOT/plotMacros/makePlotHThTh.C");

   //makeLTauStack(TString name,TString file,TString dir,int s,TString labelX,TString units = "GeV",bool left=false,TString channel = "",TString year = "2012",bool log = false,bool dndm=false,bool doRatio = false,true)

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mvis","tauTauPlots/tauTau_mvis.root","_inclusive",3,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_njetspt20","tauTauPlots/tauTau_njetspt20.root","_inclusive",3,"# Jets p_{T}>20 GeV","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
}
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mt_1","tauTauPlots/tauTau_mt_1.root","_inclusive",3,"m_{T}(#tau_{1},MET)","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mt_2","tauTauPlots/tauTau_mt_2.root","_inclusive",3,"m_{T}(#tau_{2},MET)","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_mvis","tauTauPlotsVBF/tauTau_mvis.root","_inclusive",3,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_mvis","tauTauPlotsMed/tauTau_mvis.root","_inclusive",3,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   //makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mt","tauTauPlots/tauTau_mt.root","_inclusive",3,"m_{T}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_eta_1","tauTauPlots/tauTau_eta_1.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_eta_2","tauTauPlots/tauTau_eta_2.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_pt_1","tauTauPlots/tauTau_pt_1.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_pt_2","tauTauPlots/tauTau_pt_2.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mJJ","tauTauPlots/tauTau_mJJ.root","_inclusive",3,"m_{JJ}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_ptJJ","tauTauPlots/tauTau_ptJJ.root","_inclusive",3,"p_{T}(JJ)","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_met","tauTauPlots/tauTau_met.root","_inclusive",3,"met","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_fullPt","tauTauPlots/tauTau_fullPt.root","_inclusive",3,"p_{T}({#tau#tau})","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_isoMVARaw_1","tauTauPlots/tauTau_isoMVARaw_1.root","_inclusive",3,"iso MVA Raw #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_isoMVARaw_2","tauTauPlots/tauTau_isoMVARaw_2.root","_inclusive",3,"iso MVA Raw #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_isoDBRaw_1","tauTauPlots/tauTau_isoDBRaw_1.root","_inclusive",3,"#Delta#beta iso #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_isoDBRaw_2","tauTauPlots/tauTau_isoDBRaw_2.root","_inclusive",3,"#Delta#beta iso #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   //boosted
   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_mvis","tauTauPlotsMed/tauTau_mvis.root","_inclusive",3,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   
   //makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_mt","tauTauPlotsMed/tauTau_mt.root","_inclusive",3,"m_{T}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_eta_1","tauTauPlotsMed/tauTau_eta_1.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_eta_2","tauTauPlotsMed/tauTau_eta_2.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_pt_1","tauTauPlotsMed/tauTau_pt_1.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_pt_2","tauTauPlotsMed/tauTau_pt_2.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_mJJ","tauTauPlotsMed/tauTau_mJJ.root","_inclusive",3,"m_{JJ}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_ptJJ","tauTauPlotsMed/tauTau_ptJJ.root","_inclusive",3,"p_{T}(JJ)","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_met","tauTauPlotsMed/tauTau_met.root","_inclusive",3,"met","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_fullPt","tauTauPlotsMed/tauTau_fullPt.root","_inclusive",3,"p_{T}({#tau#tau})","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_isoMVARaw_1","tauTauPlotsMed/tauTau_isoMVARaw_1.root","_inclusive",3,"iso MVA Raw #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_isoMVARaw_2","tauTauPlotsMed/tauTau_isoMVARaw_2.root","_inclusive",3,"iso MVA Raw #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_isoDBRaw_1","tauTauPlotsMed/tauTau_isoDBRaw_1.root","_inclusive",3,"#Delta#beta iso #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/medium/diTau_isoDBRaw_2","tauTauPlotsMed/tauTau_isoDBRaw_2.root","_inclusive",3,"#Delta#beta iso #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   //boosted High
   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_mvis","tauTauPlotsHigh/tauTau_mvis.root","_inclusive",3,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   
   //makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_mt","tauTauPlotsHigh/tauTau_mt.root","_inclusive",3,"m_{T}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_eta_1","tauTauPlotsHigh/tauTau_eta_1.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_eta_2","tauTauPlotsHigh/tauTau_eta_2.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_pt_1","tauTauPlotsHigh/tauTau_pt_1.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_pt_2","tauTauPlotsHigh/tauTau_pt_2.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_mJJ","tauTauPlotsHigh/tauTau_mJJ.root","_inclusive",3,"m_{JJ}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_ptJJ","tauTauPlotsHigh/tauTau_ptJJ.root","_inclusive",3,"p_{T}(JJ)","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_met","tauTauPlotsHigh/tauTau_met.root","_inclusive",3,"met","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_fullPt","tauTauPlotsHigh/tauTau_fullPt.root","_inclusive",3,"p_{T}({#tau#tau})","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_isoMVARaw_1","tauTauPlotsHigh/tauTau_isoMVARaw_1.root","_inclusive",3,"iso MVA Raw #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_isoMVARaw_2","tauTauPlotsHigh/tauTau_isoMVARaw_2.root","_inclusive",3,"iso MVA Raw #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_isoDBRaw_1","tauTauPlotsHigh/tauTau_isoDBRaw_1.root","_inclusive",3,"#Delta#beta iso #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/high/diTau_isoDBRaw_2","tauTauPlotsHigh/tauTau_isoDBRaw_2.root","_inclusive",3,"#Delta#beta iso #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   //boosted High
   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_mvis","tauTauPlotsVBF/tauTau_mvis.root","_inclusive",3,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   
   //makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_mt","tauTauPlotsHigh/tauTau_mt.root","_inclusive",3,"m_{T}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_eta_1","tauTauPlotsVBF/tauTau_eta_1.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_eta_2","tauTauPlotsVBF/tauTau_eta_2.root","_inclusive",3,"#eta #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_pt_1","tauTauPlotsVBF/tauTau_pt_1.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_pt_2","tauTauPlotsVBF/tauTau_pt_2.root","_inclusive",3,"p_{T} #tau_{1}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_mJJ","tauTauPlotsVBF/tauTau_mJJ.root","_inclusive",3,"m_{JJ}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_ptJJ","tauTauPlotsVBF/tauTau_ptJJ.root","_inclusive",3,"p_{T}(JJ)","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_met","tauTauPlotsVBF/tauTau_met.root","_inclusive",3,"met","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_fullPt","tauTauPlotsVBF/tauTau_fullPt.root","_inclusive",3,"p_{T}({#tau#tau})","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_isoMVARaw_1","tauTauPlotsVBF/tauTau_isoMVARaw_1.root","_inclusive",3,"iso MVA Raw #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_isoMVARaw_2","tauTauPlotsVBF/tauTau_isoMVARaw_2.root","_inclusive",3,"iso MVA Raw #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);

   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_isoDBRaw_1","tauTauPlotsVBF/tauTau_isoDBRaw_1.root","_inclusive",3,"#Delta#beta iso #tau_{1}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   makeDiTauStack("~/Dropbox/ztt/diTau/vbf/diTau_isoDBRaw_2","tauTauPlotsVBF/tauTau_isoDBRaw_2.root","_inclusive",3,"#Delta#beta iso #tau_{2}","",false,"#tau_{h}#tau_{h}","Golden",false,false,true);


   //makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mvisPtHigh","tauTauPlotsHighBlinded/tauTau_mvisPtHigh.root","_inclusive",0,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   //makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mvisPtMed","tauTauPlotsHighBlinded/tauTau_mvisPtMed.root","_inclusive",0,"m_{vis}","GeV",false,"#tau_{h}#tau_{h}","Golden",false,false,true);
   //makeDiTauStack("~/Dropbox/ztt/diTau/startup/diTau_mt","tauTauPlotsVBF/tauTau_mt.root","_inclusive",0,"m_{T}","GeV",true,"#tau_{h}#tau_{h}","Golden",false,false,true);
}

   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusivemt40",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_OldMVAIso","muTauPlots/muTau_OldMVAIso.root","mt_inclusivemt40",3,"Raw MVAIsolation","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   //makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_m_vis","datacards/htt_mt.inputs-sm-13TeV.root","mt_inclusivemt40",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_mt_1","muTauPlots/muTau_mt_1.root","mt_inclusivemt40",3,"m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_mt_2","muTauPlots/muTau_mt_2.root","mt_inclusivemt40",3,"#mu +MET m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_mt","muTauPlots/muTau_mt.root","mt_inclusivemt40",3,"Total m_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusivemt40",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_m_vis_nocut","muTauPlots/muTau_m_vis_nocut.root","mt_inclusivemt40",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_iso_1","muTauPlots/muTau_iso_1.root","mt_inclusivemt40",3,"Isolation #mu DR=0.3","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_tauIso","muTauPlots/muTau_tauIso.root","mt_inclusivemt40",3,"Raw Tau Isolation","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_met","muTauPlots/muTau_met.root","mt_inclusivemt40",3,"ME_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_pt1","muTauPlots/muTau_pt_1.root","mt_inclusivemt40",3,"#mu P_{T} ","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_genVisPt1","muTauPlots/muTau_genVisPt1.root","mt_inclusivemt40",3,"Gen Visible #mu P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_pt_2","muTauPlots/muTau_pt_2.root","mt_inclusivemt40",3,"#tau P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_genVisPt2","muTauPlots/muTau_genVisPt2.root","mt_inclusivemt40",3,"Gen Visible #tau P_{T}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_eta_1","muTauPlots/muTau_eta_1.root","mt_inclusivemt40",3,"#eta_{#mu}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_eta_2","muTauPlots/muTau_eta_2.root","mt_inclusivemt40",3,"#eta_{#tau}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_m_vis","muTauPlots/muTau_m_vis.root","mt_inclusivemt40",3,"visible mass","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_pt","muTauPlots/muTau_pt.root","mt_inclusivemt40",3,"di-tau Pt","GeV",false,"#tau_{#mu}#tau_{h}","Golden",true,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_q_1","muTauPlots/muTau_q_1.root","mt_inclusivemt40",3,"q_{#mu}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_q_2","muTauPlots/muTau_q_2.root","mt_inclusivemt40",3,"q_{#tau}","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_npv","muTauPlots/muTau_npv.root","mt_inclusivemt40",3,"npv","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_tauDecayMode","muTauPlots/muTau_tauDecayMode.root","mt_inclusivemt40",3,"#tau decay mode","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_m_2","muTauPlots/muTau_m_2.root","mt_inclusivemt40",3,"M_{#tau}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_mjj","muTauPlots/muTau_mjj.root","mt_inclusivemt40",3,"M_{JJ}","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_njetspt20","muTauPlots/muTau_njetspt20.root","mt_inclusivemt40",3,"Number Jets Pt>20","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_njets","muTauPlots/muTau_njets.root","mt_inclusivemt40",3,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_nbtag","muTauPlots/muTau_nbtag.root","mt_inclusivemt40",3,"Number Jets Pt>30","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_jeta_1","muTauPlots/muTau_jeta_1.root","mt_inclusivemt40",3,"Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_jeta_2","muTauPlots/muTau_jeta_2.root","mt_inclusivemt40",3,"Sub-Leading jet #eta","",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_tauNIsoTracks","muTauPlots/muTau_tauNIsoTracks.root","mt_inclusivemt40",3,"Number Isolated Tracks","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_jpt_1","muTauPlots/muTau_jpt_1.root","mt_inclusivemt40",3,"Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);
   makeLTauStack("~/www/ztt/MuTau/25ns_FullSelection/muTau_jpt_2","muTauPlots/muTau_jpt_2.root","mt_inclusivemt40",3,"Sub-Leading jet Pt","GeV",false,"#tau_{#mu}#tau_{h}","Golden",false,false,true);

}
