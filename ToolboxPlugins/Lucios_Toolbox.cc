#include "toolbox.hh"
#include <iostream>
#include <vector>
#include <string>

class Lucios_Toolbox : ToolboxPlugin
{
  public:
    Lucios_Toolbox(void);
    ~Lucios_Toolbox(void) {}

    std::string getDescription(void);
    bool processEvent(T4Event* event);
    void endOfEvents(void);
    void beginOfEvents(void);

  private:

    //PROVA AGGIUNTA TREE

    TH1D *molteplicita;
    TTree *tree;

    // dichiaro gli istogrammi (RICH)
    TH2D* moment_vs_cheren;
    TH2D* mvch_with_cut;
    vector < TH2D* > mvch_per_particle;
    // dichiaro gli istogrammi (analisi vertici PRIMARI)
    TH1D* Vertex_z_tot;
    TH2D* Vertex_xy_tot;
    TH2D* Vertex_yz_tot;
    TH1D* Vertex_z;
    TH2D* Vertex_xy;
    TH2D* Vertex_yz;
    TH1D* Particles_per_type;
    TH1D* ProtonVertex_z;
    TH1D* Delta_rays;
    TH1D* Particles_per_type_per_ev;
    TH1D* Particles_with_protons;
    TH1D* Moment;
    TH2D* Moment_vs_angle;
    TH2D* logmoment_vs_logangle;
    TH2D* pseudo_vs_momentum;
    TH1D* recprot_mom;
    TH1D* recprot_ang;
    vector < TH2D* > momvsangle_per_particle;
    vector < TH2D* > logmomvsangle_per_particle;
    vector < TH2D* > pseudovmom_per_particle;
    vector < TH1D* > Momenta_per_Id;
    //dichiaro gli istogrammi (analisi vertici SECONDARI)
    TH1D* secantip_mom;
    
    //definisco i counters
    int antip, p, mum, mup, pim, pip, pi0, km, kp, k0, k0l, k0s, n, others, em, ep, adron, lepton, ggamma, sec, secantip, recprot;
    int richproton, richpion, richkaon;    

};

static Lucios_Toolbox* lucios_toolbox = new Lucios_Toolbox();

Lucios_Toolbox::Lucios_Toolbox(void)
{
  myName = "Lucios_Toolbox";
  pluginList::getInstance()->activated_classes.push_back(this);
}

std::string Lucios_Toolbox::getDescription(void)
{
  std::string description = "Toolbox for Lucio's thesis.\n";
  description += "Generator: GEANT";
  return description;
}

void Lucios_Toolbox::beginOfEvents(void)
{ 
  //inizializzo i counters
  antip, p, mum, mup, pim, pip, pi0, km, kp, k0, k0l, k0s, n, others, em, ep, adron, lepton, ggamma, sec, secantip, recprot = 0;
  richproton, richpion, richkaon = 0;
  const double pi = 3.141592653589793238463;

  //PROVA AGGIUNTA TREE


  molteplicita = new TH1D("molteplicita", "Molteplicità Tracce", 400, 0, 40);
  tree = new TTree("T", "tree_prova");
  tree->Branch("molteplicita", "TH1D", &molteplicita);


  // Istogrammi del RICH

  moment_vs_cheren = new TH2D("RICH_angle_vs_p", "Cherenkov Angle vs Momentum", 400, 0, 60, 400, 0, 80);
  moment_vs_cheren->GetXaxis()->SetTitle("Momentum [GeV]");
  moment_vs_cheren->GetYaxis()->SetTitle("Cherenkov Angle [mrad]");
  mvch_with_cut = new TH2D("RICH_cut_angle_vs_p", "Cherenkov Angle vs Momentum", 400, 0, 60, 400, 0, 80);
  mvch_with_cut->GetXaxis()->SetTitle("Momentum [GeV]");
  mvch_with_cut->GetYaxis()->SetTitle("Cherenkov Angle [mrad]");
 
  Int_t r;
  const Int_t nr = 3;
  const char *richparticles[nr] = {"Protons", "Pions", "Kaons"};
  for (r = 0; r < nr; r++) {
      std::string richparticelle = richparticles[r];

      mvch_per_particle.push_back(new TH2D((richparticelle+"_p_vs_cheren").c_str(), (richparticelle+" Cherenkov Angle vs Momentum").c_str(), 400, 0, 60, 400, 0, 80));
      mvch_per_particle.at(r)->GetXaxis()->SetTitle("Momentum [GeV]");
      mvch_per_particle.at(r)->GetYaxis()->SetTitle("Cherenkov Angle [mrad]");}
      //aggiungere qui impostazioni sul colore
      mvch_per_particle.at(0)->SetMarkerColor(2);
      mvch_per_particle.at(1)->SetMarkerColor(3);
      mvch_per_particle.at(2)->SetMarkerColor(4);
//________________________________________________________________________________________________________________________________________


  //Inizializzo i vettori di istogrammi

  //Momenta_per_Id histogram
  Int_t i;
  const Int_t nx = 13;
  const char *particles[nx] = {"Antiprotons", "Protons", "Muons", "Antimuons", "Pions-", "Pions+", "Pions0", "Kaons-", "Kaons+", "Kaons0L", "Kaons0S", "Neutrons", "Others"};
  for (i = 0; i < nx; i++) {
      std::string particelle = particles[i];
      
      Momenta_per_Id.push_back(new TH1D((particelle+"_momentum").c_str(), (particelle+" momentum distribution").c_str(), 100, 0, 40));
      Momenta_per_Id.at(i)->GetXaxis()->SetTitle((particelle+" Momentum [GeV]").c_str());
      Momenta_per_Id.at(i)->SetFillColor(0);
      }
      Momenta_per_Id.at(0)->SetLineColor(2);
      Momenta_per_Id.at(1)->SetLineColor(3);
      Momenta_per_Id.at(2)->SetLineColor(38);
      Momenta_per_Id.at(3)->SetLineColor(44);
      Momenta_per_Id.at(4)->SetLineColor(6);
      Momenta_per_Id.at(5)->SetLineColor(7);
      Momenta_per_Id.at(6)->SetLineColor(8);
      Momenta_per_Id.at(7)->SetLineColor(9);
      Momenta_per_Id.at(8)->SetLineColor(12);
      Momenta_per_Id.at(9)->SetLineColor(4);
      Momenta_per_Id.at(10)->SetLineColor(5);
      Momenta_per_Id.at(11)->SetLineColor(1);
      Momenta_per_Id.at(12)->SetLineColor(46);
    
   //momentum vs forward angle per particle
   Int_t m;
   const Int_t nm = 6;
   const char *momentpartics[nm] = {"Antiprotons", "Protons", "Pions-", "Pions+", "Kaons-", "Kaons+"};
   for (m = 0; m < nm; m++) {
   std::string momentparticelle = momentpartics[m];
   momvsangle_per_particle.push_back(new TH2D((momentparticelle+"_p_vs_angle").c_str(), (momentparticelle+" Forward Angle vs Momentum").c_str(), 300, 0, 30, 20, 0, 2));
   logmomvsangle_per_particle.push_back(new TH2D((momentparticelle+"_log_p_vs_angle").c_str(), (momentparticelle+" ForwardAngle vs Momentum").c_str(), 40, -1, 3, 50, -2, 3));
   pseudovmom_per_particle.push_back(new TH2D((momentparticelle+"_pseudo_vs_mom").c_str(), (momentparticelle+" pseudorapidity vs Momentum").c_str(), 500, 0, 30, 190, -2, 8));
   pseudovmom_per_particle.at(m)->GetXaxis()->SetTitle("Momentum [GeV]");
   pseudovmom_per_particle.at(m)->GetYaxis()->SetTitle("Pseudorapidity");} 

   //Delta_rays histogram
   // Int_t n;
   const Int_t nd = 2;
   const char *deltas[nd] = {"Electrons", "Positrons"};

   //Particles_with_protons histogram
   // Int_t m;
   const Int_t np = 3;
   const char *withprotons[np] = {"Adroni", "Leptoni", "Fotoni"};


  // inizializzo gli istogrammi (VERTICI PRIMARI)

  //POSIZIONE (totale e con cut nel target)
  Vertex_z_tot = new TH1D("positionVertex_ztot", "Vertex z position (cm)", 300, -300, 300);
  Vertex_z_tot->GetXaxis()->SetTitle("z [cm]");
  Vertex_xy_tot = new TH2D("positionVertex_xytot", "Vertex xy position (cm)", 400, -3, 3, 180, -6, 6);
  Vertex_xy_tot->GetXaxis()->SetTitle("x [cm]");
  Vertex_xy_tot->GetYaxis()->SetTitle("y [cm]");
  Vertex_yz_tot = new TH2D("positionVertex_yztot", "Vertex yz position (cm)", 300, -300, 300, 1800, -6, 6);
  Vertex_yz_tot->GetXaxis()->SetTitle("z [cm]");
  Vertex_yz_tot->GetYaxis()->SetTitle("y [cm]");
  Vertex_z = new TH1D("positionVertex_z", "Vertex z position (cm)", 300, -300, 300);
  Vertex_z->GetXaxis()->SetTitle("z [cm]");
  Vertex_z->SetFillColor(2);
  Vertex_xy = new TH2D("positionVertex_xy", "Vertex xy position (cm)", 400, -3, 3, 180, -6, 6);
  Vertex_xy->GetXaxis()->SetTitle("x [cm]");
  Vertex_xy->GetYaxis()->SetTitle("y [cm]");
  Vertex_yz = new TH2D("positionVertex_yz", "Vertex yz position (cm)", 800, -300, 300, 300, -6, 6);
  Vertex_yz->GetXaxis()->SetTitle("z [cm]");
  Vertex_yz->GetYaxis()->SetTitle("y [cm]");

  Particles_per_type = new TH1D("particles_per_type", "Particles from primary vertex", 13, 0, 13);
  for (i=1; i<=nx; i++) Particles_per_type->GetXaxis()->SetBinLabel(i, particles[i-1]);
  Particles_per_type->SetLineColor(2);
  Particles_per_type->SetFillColor(38);
  Particles_per_type->GetXaxis()->SetLabelOffset(0.008);

  ProtonVertex_z = new TH1D("proton_vertex_z", "Vertex z position (cm) for protons with momentum>175", 300, -100, 0);
  ProtonVertex_z->GetXaxis()->SetTitle("z [cm]");

  Delta_rays = new TH1D("delta_rays", "Delta rays particles", 2, 0, 2);
  for (n=1; n<=nd; n++) Delta_rays->GetXaxis()->SetBinLabel(n, deltas[n-1]);
  Delta_rays->SetLineColor(2);
  Delta_rays->SetFillColor(38);

  Particles_with_protons = new TH1D("particles_with_protons", "Particles produced with protons", 3, 0, 3);
  for (m=1; m<=np; m++) Particles_with_protons->GetXaxis()->SetBinLabel(m, withprotons[m-1]);

  Moment = new TH1D("momentum", "Momentum Distribution", 100, 0, 40);
  Moment->GetXaxis()->SetTitle("Momentum [GeV]");
  Moment->SetLineColor(32);
  Moment->SetFillColor(31);

  Moment_vs_angle = new TH2D("moment_vs_angle", "Forward Angle vs Momentum", 300, 0, 30, 20, 0, 2);
  Moment_vs_angle->GetXaxis()->SetTitle("Momentum [GeV]");
  Moment_vs_angle->GetYaxis()->SetTitle("Forward Angle [deg]");

  logmoment_vs_logangle = new TH2D("log_moment_vs_angle", "Forward Angle vs Momentum (log)", 40, -1, 3, 50, -2, 3);
  logmoment_vs_logangle->GetXaxis()->SetTitle("Momentum [log10 GeV]");
  logmoment_vs_logangle->GetYaxis()->SetTitle("Forward angle [log10 deg]");

  pseudo_vs_momentum = new TH2D("pseudo_vs_momentum", "Pseudorapidity vs Momentum", 500, 0, 30, 190, -2, 8);
  pseudo_vs_momentum->GetXaxis()->SetTitle("Momentum [GeV]");
  pseudo_vs_momentum->GetYaxis()->SetTitle("Pseudorapidity");
  
  //recoil protons
  recprot_mom = new TH1D("recoil_momentum", "Recoil Protons Momentum", 150, 0, 15);
  recprot_mom->GetXaxis()->SetTitle("Momentum [GeV]");
  recprot_ang = new TH1D("recoil_angle", "Recoil Protons Angle", 400, 0, 4);
  recprot_ang->GetXaxis()->SetTitle("Forward Angle [rad]");
  //inizializzo gli istogrammi (VERTICI SECONDARI)
  secantip_mom = new TH1D("secantip_momentum", "Momentum of secondary antiprotons", 300, 0, 30);
  secantip_mom->GetXaxis()->SetTitle("Momentum [GeV]");

} //fine void beginOfEvents


bool Lucios_Toolbox::processEvent(T4Event* event)
{ //ANALISI DEL RICH

 //for (unsigned int j = 0; j < event->beamData.trajectories.size(); j++) {
 
 //T4Trajectory* trajectory = &event->beamData.trajectories.at(j); ->POI TOGLIERE QUESTO STESSO CICLO DA SOTTO

 //TAGLIO NEL TARGET (per ora commentato pr avere più dati)
 //if (trajectory->position[2]/10>=-68.4 && trajectory->position[2]/10<=-28.4) {


 for (unsigned int i = 0; i < event->rich.size(); i++) {
 
 T4RichData* richphoton = &event->rich.at(i);
 
 int richtrack = richphoton->parentTrackId;
 double richmoment = (sqrt(pow(richphoton->momentumMotherParticle[0], 2) + pow(richphoton->momentumMotherParticle[1], 2) + pow(richphoton->momentumMotherParticle[2], 2)))/1000;
 TVector3 richmomentvec(richphoton->momentumMotherParticle[0], richphoton->momentumMotherParticle[1], richphoton->momentumMotherParticle[2]);
 Double_t richforwang = richmomentvec.Theta();


 for (unsigned int j = 0; j < event->beamData.trajectories.size(); j++) {
 
 T4Trajectory* trajectory = &event->beamData.trajectories.at(j);

    //ciclo con break per prendere solo una volta tutto il cono cherenkov
    if (richforwang >= 0.018 && richforwang <= 0.180) {
    if (trajectory->parentId == richtrack) {             
    if (trajectory->particleId==2212) {
    richproton++;
    mvch_per_particle.at(0)->Fill(richmoment, richphoton->cerenkovAngle*1000);}
    if (abs(trajectory->particleId)==211 || trajectory->particleId==111) {
    richpion++;
    mvch_per_particle.at(1)->Fill(richmoment, richphoton->cerenkovAngle*1000);}
    if (abs(trajectory->particleId)==321 || trajectory->particleId==130 || trajectory->particleId==310) {
    richkaon++;
    mvch_per_particle.at(2)->Fill(richmoment, richphoton->cerenkovAngle*1000);}
    break;}}} 



    moment_vs_cheren->Fill(richmoment, richphoton->cerenkovAngle*1000);
    if (richforwang >= 0.018 && richforwang <= 0.180) {
    mvch_with_cut->Fill(richmoment, richphoton->cerenkovAngle*1000);}
    break;}  

//SECONDO CICLO FOR PER PRENDERE TUTTI I FOTONI DEL CONO

for (unsigned int i = 0; i < event->rich.size(); i++) {
 
 T4RichData* richphoton = &event->rich.at(i);
 
 int richtrack = richphoton->parentTrackId;
 double richmoment = (sqrt(pow(richphoton->momentumMotherParticle[0], 2) + pow(richphoton->momentumMotherParticle[1], 2) + pow(richphoton->momentumMotherParticle[2], 2)))/1000;
 TVector3 richmomentvec(richphoton->momentumMotherParticle[0], richphoton->momentumMotherParticle[1], richphoton->momentumMotherParticle[2]);
 Double_t richforwang = richmomentvec.Theta();


// for (unsigned int j = 0; j < event->beamData.trajectories.size(); j++) {
 
// T4Trajectory* trajectory = &event->beamData.trajectories.at(j);

// if (richforwang >= 0.018 && richforwang <= 0.180) {
// if (trajectory->parentId == richtrack) {
//    if (trajectory->particleId == 2212) {cout << "true" << endl;}}}}
}
  
//_____________________________________________________________________________________________________________________________________

  //ANALISI DEI VERTICI PRIMARI: posizione e particelle uscenti con relative distribuzioni in momento

  //primo ciclo for per le COORDINATE DEL VERTICE PRIMARIO DI TUTTI GLI EVENTI -> con break per prendere ogni vertice solo una volta
  for (unsigned int i = 0; i < event->beamData.trajectories.size(); i++) {
 
  T4Trajectory* trajectory = &event->beamData.trajectories.at(i);
  
      if (trajectory->parentId == 1) {
 
       if (abs(trajectory->particleId)!=11) {        
       //fillo gli istogrammi di posizione del vertice TOTALI (esterni al target)
       if (trajectory->position[2]/10<-68.4 || trajectory->position[2]/10>-28.4) {
          if ((sqrt(pow(trajectory->position[0], 2)+pow(trajectory->position[1], 2)))/10>1.6) {
       Vertex_z_tot->Fill(trajectory->position[2]/10);
       Vertex_xy_tot->Fill(trajectory->position[0]/10, trajectory->position[1]/10);
       Vertex_yz_tot->Fill(trajectory->position[2]/10, trajectory->position[1]/10);
       break;}}}}}
       
  //secondo ciclo for per le COORDINATE DEL VERTICE PRIMARIO NEL TARGET
  for (unsigned int i = 0; i < event->beamData.trajectories.size(); i++) {
 
  T4Trajectory* trajectory = &event->beamData.trajectories.at(i);
  const double pi = 3.141592653589793238463;
  double momentumtot = (sqrt(pow(trajectory->momentum[0], 2) + pow(trajectory->momentum[1], 2) + pow(trajectory->momentum[2], 2)))/1000;
  TVector3 momentvec(trajectory->momentum[0], trajectory->momentum[1], trajectory->momentum[2]);
  Double_t forwang = momentvec.Theta();
  
      if (trajectory->parentId == 1) {

       if (trajectory->position[2]/10>=-68.4 && trajectory->position[2]/10<=-28.4 && (sqrt(pow(trajectory->position[0], 2)+pow(trajectory->position[1], 2)))/10<=1.6){//verificare taglio in xy
         //fillo gli istogrammi di posizione del vertice NEL TARGET    
         if (abs(trajectory->particleId)!=11) { 
            //recoil proton
            //if (trajectory->particleId == 2212 && forwang >= (50/180)*pi && forwang <= pi) {recprot++;
                                                                                           // recprot_mom->Fill(momentumtot);
                                                                                           // recprot_ang->Fill(forwang);}
            //posizione vertici primari nel target
            Vertex_z->Fill(trajectory->position[2]/10);
            Vertex_xy->Fill(trajectory->position[0]/10, trajectory->position[1]/10);
            Vertex_yz->Fill(trajectory->position[2]/10, trajectory->position[1]/10);          
            break;}}}}

  //terzo ciclo for per l'IDENTIFICAZIONE DELLE PARTICELLE USCENTI -> senza break per prendere tutto quello che esce
               
  for (unsigned int i = 0; i < event->beamData.trajectories.size(); i++) {

  T4Trajectory* trajectory = & event->beamData.trajectories.at(i);
  double momentumtot = (sqrt(pow(trajectory->momentum[0], 2) + pow(trajectory->momentum[1], 2) + pow(trajectory->momentum[2], 2)))/1000;
  double logmoment = log10(momentumtot);
  TVector3 momentvec(trajectory->momentum[0], trajectory->momentum[1], trajectory->momentum[2]);
  Double_t forwang = momentvec.Theta();
  double tg_forwang = tan (forwang/2);
  double pseudorapidity = -log(tg_forwang);
  double logangle = log10(forwang);
  const double pi = 3.141592653589793238463;
      
      if (trajectory->parentId == 1) {

       if (trajectory->position[2]/10>=-68.4 && trajectory->position[2]/10<=-28.4 && (sqrt(pow(trajectory->position[0], 2)+pow(trajectory->position[1], 2)))/10<=1.6) { //verificare taglio in xy

            if (abs(trajectory->particleId)!=11) {
               Moment->Fill(momentumtot);
               Moment_vs_angle->Fill(momentumtot, forwang);
               logmoment_vs_logangle->Fill(logmoment, logangle);
               pseudo_vs_momentum->Fill(momentumtot, pseudorapidity);}

            if (trajectory->particleId==-2212) {antip++;
                                                momvsangle_per_particle.at(0)->Fill(momentumtot, forwang);
                                                logmomvsangle_per_particle.at(0)->Fill(logmoment, logangle);
                                                pseudovmom_per_particle.at(0)->Fill(momentumtot, pseudorapidity);
                                                Momenta_per_Id.at(0)->Fill(momentumtot);}
            if (trajectory->particleId==2212) {p++;
                                               //PROVA TREE
                                               molteplicita->Fill(momentumtot);
                                               momvsangle_per_particle.at(1)->Fill(momentumtot, forwang);
                                               logmomvsangle_per_particle.at(1)->Fill(logmoment, logangle);
                                               pseudovmom_per_particle.at(1)->Fill(momentumtot, pseudorapidity);
                                               Momenta_per_Id.at(1)->Fill(momentumtot);
                                               if (forwang >= (50./180.)*pi && forwang <= pi) {recprot++;
                                                                                              recprot_mom->Fill(momentumtot);
                                                                                              recprot_ang->Fill(forwang);}
                                               }
            if (trajectory->particleId==13) {mum++;
                                             Momenta_per_Id.at(2)->Fill(momentumtot);}
            if (trajectory->particleId==-13) {mup++;
                                              Momenta_per_Id.at(3)->Fill(momentumtot);}
            if (trajectory->particleId==-211) {pim++;
                                               momvsangle_per_particle.at(2)->Fill(momentumtot, forwang);
                                               logmomvsangle_per_particle.at(2)->Fill(logmoment, logangle);
                                               pseudovmom_per_particle.at(2)->Fill(momentumtot, pseudorapidity);
                                               Momenta_per_Id.at(4)->Fill(momentumtot);}
            if (trajectory->particleId==211) {pip++;
                                              momvsangle_per_particle.at(3)->Fill(momentumtot, forwang);
                                              logmomvsangle_per_particle.at(3)->Fill(logmoment, logangle);
                                              pseudovmom_per_particle.at(3)->Fill(momentumtot, pseudorapidity);
                                              Momenta_per_Id.at(5)->Fill(momentumtot);}
            if (trajectory->particleId==111) {pi0++;
                                              Momenta_per_Id.at(6)->Fill(momentumtot);}
            if (trajectory->particleId==-321) {km++;
                                               momvsangle_per_particle.at(4)->Fill(momentumtot, forwang);
                                               logmomvsangle_per_particle.at(4)->Fill(logmoment, logangle);
                                               pseudovmom_per_particle.at(4)->Fill(momentumtot, pseudorapidity);
                                               Momenta_per_Id.at(7)->Fill(momentumtot);}
            if (trajectory->particleId==321) {kp++;
                                              momvsangle_per_particle.at(5)->Fill(momentumtot, forwang);
                                              logmomvsangle_per_particle.at(5)->Fill(logmoment, logangle);
                                              pseudovmom_per_particle.at(5)->Fill(momentumtot, pseudorapidity);
                                              Momenta_per_Id.at(8)->Fill(momentumtot);}
            if (trajectory->particleId==130) {k0l++;
                                              Momenta_per_Id.at(9)->Fill(momentumtot);}
            if (trajectory->particleId==310) {k0s++;
                                              Momenta_per_Id.at(10)->Fill(momentumtot);}
            if (trajectory->particleId==2112) {n++;
                                               Momenta_per_Id.at(11)->Fill(momentumtot);}
            if (trajectory->particleId==11) {em++;}
            if (trajectory->particleId==-11) {ep++;}
            if (abs(trajectory->particleId)!=2212 && abs(trajectory->particleId)!=13 && abs(trajectory->particleId)!=211 && abs(trajectory->particleId)!=321 && trajectory->particleId!=111 && trajectory->particleId!=311 && trajectory->particleId!=2112 && abs(trajectory->particleId)!=11)
            {others++;
            // cout << endl << trajectory->particleId << endl;
             Momenta_per_Id.at(12)->Fill(momentumtot);}
        
            if (trajectory->particleId==2212 && trajectory->momentum[2]>175) {ProtonVertex_z->Fill(trajectory->position[2]/10);}
 
            //cerco le particelle che vengono prodotte insieme ai protoni

            if (trajectory->particleId==2212) {
               for (unsigned int t = 0; t < event->beamData.trajectories.size(); t++) {
                   T4Trajectory* othertrajectory = & event->beamData.trajectories.at(t);
                   if (othertrajectory->parentId==trajectory->parentId) {
                      if (abs(othertrajectory->parentId)==11 || abs(othertrajectory->parentId)==12 || abs(othertrajectory->parentId)==13 || abs(othertrajectory->parentId)==14 || abs(othertrajectory->parentId)==15 || abs(othertrajectory->parentId)==16) {lepton++;}
                      if (othertrajectory->particleId==22) {ggamma++;}
                      else adron++;}  //fine if othertrajectory->parentId==traectory->parentId
                 } //fine for per i protoni
               } //fine trajectory->particleId==2212
            } //fine taglio nel bersaglio
          
       } //fine if parentId==1


       //ANALISI VERTICI SECONDARI

       if (trajectory->parentId != 0 && trajectory->parentId != 1) {
          sec++;
          if (trajectory->particleId == -2212) {
          secantip++; 
          secantip_mom->Fill(momentumtot);} //fine if antiprotoni
          } //fine if vertici secondari

  } //fine for per le trajectories
tree->Fill();
return true;
} //fine bool

void Lucios_Toolbox::endOfEvents(void)
{
  //scrivo gli istogrammi del RICH
  moment_vs_cheren->Write();
  mvch_with_cut->Write();

  cout << endl << "Protoni nel RICH: " << richproton <<endl;
  cout << "Pioni nel RICH: " << richpion << endl;
  cout << "Kaoni nel RICH: " << richkaon << endl;

  for (int i = 0; i < mvch_per_particle.size(); i++) {mvch_per_particle.at(i)->Write();}


//__________________________________________________________________________________________________________________________________________

  //riempio l'istogramma di particelle per tipo e lo scrivo
  for (int i=0; i<antip; i++) {Particles_per_type->Fill(0);}
  for (int i=0; i<p; i++) {Particles_per_type->Fill(1);}
  for (int i=0; i<mum; i++) {Particles_per_type->Fill(2);}
  for (int i=0; i<mup; i++) {Particles_per_type->Fill(3);}
  for (int i=0; i<pim; i++) {Particles_per_type->Fill(4);}
  for (int i=0; i<pip; i++) {Particles_per_type->Fill(5);}
  for (int i=0; i<pi0; i++) {Particles_per_type->Fill(6);}
  for (int i=0; i<km; i++) {Particles_per_type->Fill(7);}
  for (int i=0; i<kp; i++) {Particles_per_type->Fill(8);}
  for (int i=0; i<k0l; i++) {Particles_per_type->Fill(9);}
  for (int i=0; i<k0s; i++) {Particles_per_type->Fill(10);}
  for (int i=0; i<n; i++) {Particles_per_type->Fill(11);}
  for (int i=0; i<others; i++) {Particles_per_type->Fill(12);}
  Particles_per_type->Write();
  //lo clono e lo rifaccio pesato per evento
  Particles_per_type_per_ev = (TH1D*)Particles_per_type->Clone();
  Particles_per_type_per_ev->Scale(1/Vertex_z->GetSumOfWeights());
  Particles_per_type_per_ev->Write("Particles_per_type_per_ev");
  //scrivo gli istogrammi di posizione e tempo del vertice primario
  Vertex_z_tot->Write();
  Vertex_xy_tot->Write();
  Vertex_yz_tot->Write();
  Vertex_z->Write();
  Vertex_xy->Write();
  Vertex_yz->Write();  
  //scrivo l'istogramma della z del vertice primario dei protoni con p>175GeV
  ProtonVertex_z->Write();
  //riempio l'istogramma dei delta rays e lo scrivo
  for (int i=0; i<em; i++) {Delta_rays->Fill(0);}
  for (int i=0; i<ep; i++) {Delta_rays->Fill(1);}
  Delta_rays->Write();
  //riempio l'istogramma delle particelle prodotte insieme ai protoni e lo scrivo
  for (int i=0; i<adron; i++) {Particles_with_protons->Fill(0);}
  for (int i=0; i<lepton; i++) {Particles_with_protons->Fill(1);}
  for (int i=0; i<ggamma; i++) {Particles_with_protons->Fill(2);}
  Particles_with_protons->Write();
  //scrivo gli istogrammi del momento contro l'angolo polare in scala normale e logaritmica
  Moment_vs_angle->Write();
  logmoment_vs_logangle->Write();
  //e gli stessi per particelle separate
  for (int i = 0; i < momvsangle_per_particle.size(); i++) {
  momvsangle_per_particle.at(i)->Write();
  logmomvsangle_per_particle.at(i)->Write();
  pseudovmom_per_particle.at(i)->Write();}


//DISEGNO NEI CANVAS____________________________________________________________________________________________________________

  //definisco un const char da trasformare poi in una stringa
  Int_t l;
  const Int_t nl = 13;
  const char *partics[nl] = {"Antiprotons", "Protons", "Muons", "Antimuons", "Pions-", "Pions+", "Pions0", "Kaons-", "Kaons+", "Kaons0L", "Kaons0S", "Neutrons", "Others"};
  //e un altro per la pseudorapidity
  Int_t k;
  const Int_t nk = 6;
  const char *pseudo[nk] = {"Antiprotons", "Protons", "Pions-", "Pions+", "Kaons-", "Kaons+"};

  //DISEGNO I PLOT DELLA POSIZIONE IN APPOSITO CANVAS
  TCanvas *cposition = new TCanvas("cposition", "canvas_position", 10, 10, 900, 500);
  TLegend *posleg = new TLegend(0.90, 0.65, 0.75, 0.9);
  //TEllipse *el1 = new TEllipse(0, 0, 1.6, 1.6);
  //el1->SetLineColor(2);
  //TLine *line = new TLine();
  //line->SetVertical(0);
  //line->SetLineColor(kBlue);
  cposition->cd();
  Vertex_z_tot->SetLineWidth(1);
  Vertex_z->SetLineWidth(1);
  Vertex_z->Draw();
  gStyle->SetOptStat(0);
  Vertex_z_tot->Draw("same");
  //line->Draw("same");
  cposition->Print("Vertex_z.pdf");
  Vertex_xy_tot->Draw();
  gStyle->SetOptStat(0); 
  Vertex_xy->Draw("same colz");
  cposition->Print("Vertex_xy.pdf");
  Vertex_yz_tot->Draw();
  gStyle->SetOptStat(0);
  Vertex_yz->Draw("same colz");
  cposition->Print("Vertex_yz.pdf");
  Particles_per_type_per_ev->Draw();
  cposition->Print("Particles.pdf");


  //DISEGNO I PLOT DEL MOMENTO IN APPOSITO CANVAS
  TCanvas *cmoment = new TCanvas("cmoment", "canvas_momenta", 10, 10, 900, 500);
  TLegend *momenleg1 = new TLegend(0.90, 0.65, 0.75, 0.90);
  TLegend *momenleg2 = new TLegend(0.90, 0.25, 0.75, 0.90);
  cmoment->cd();
  recprot_mom->Draw();
  cmoment->Print("Recoil_p.pdf");
  recprot_ang->Draw();
  cmoment->Print("Recprot_ang.pdf");
  pseudo_vs_momentum->Draw("colz");
  cmoment->Print("Pseudorapidity_tot.pdf");
  for (k = 0; k < nk; k++) {
      std::string pseudostring = pseudo[k];
      pseudovmom_per_particle.at(k)->Draw("colz");
      cmoment->Print((pseudostring+"_pseudovmom.pdf").c_str());}

   gPad->SetLogy();
   
  //disegno il grafico del momento di tutte le particelle e uno dopo l'altro quelli di tutte le particelle
   for (l = 0; l < nl; l++) {
       std::string partistring = partics[l]; 
       Moment->SetLineWidth(1);
       Momenta_per_Id.at(l)->SetLineWidth(1);
       Moment->Draw();
       gStyle->SetOptStat(0); 
       Momenta_per_Id.at(l)->Draw("same");
       momenleg1->Clear();
       momenleg1->AddEntry(Moment, "All Particles");
       momenleg1->AddEntry(Momenta_per_Id.at(l), (partistring).c_str());
       momenleg1->Draw();
       cmoment->Print((partistring+"_graphic.pdf").c_str());}
       secantip_mom->Draw();
       cmoment->Print("Secondary_antip.pdf");

   Moment->Draw();
   gStyle->SetOptStat(0);
   momenleg2->AddEntry(Moment, "All Particles");
   //disegno le distribuzioni di tutte le particelle insieme sulla distribuzione totale
   for (l=0; l < nl; l++) {
       std::string newpartistring = partics [l];
       Momenta_per_Id.at(l)->Draw("same");
       momenleg2->AddEntry(Momenta_per_Id.at(l), (newpartistring).c_str());}
       momenleg2->Draw();
       cmoment->Print("allparticles_graph.pdf");


  //DISEGNO I PLOT CHERENKOV IN APPOSITO CANVAS
  Int_t ch;
  const Int_t nch = 3;
  const char *cherenkov[nch] = {"Protons", "Pions", "Kaons"};

  TCanvas *ccheren = new TCanvas("ccheren", "canvas_cherenkov", 10, 10, 900, 500);
  TLegend *cherenleg = new TLegend(0.90, 0.65, 0.75, 0.90);
  ccheren->cd();
  mvch_with_cut->Draw("colz");
  ccheren->Print("Cherenkov_tot.pdf");
  gStyle->SetOptStat(0);
  mvch_per_particle.at(0)->Draw();
  cherenleg->AddEntry(mvch_per_particle.at(0), "Protons");
  for (ch = 1; ch < nch; ch++) {
  std::string cherenstring = cherenkov[ch];
  mvch_per_particle.at(ch)->Draw("same");
  cherenleg->AddEntry(mvch_per_particle.at(ch), (cherenstring).c_str());}
  cherenleg->Draw();
  ccheren->Print("Cherenkov.pdf");  







  
//Scrivo gli istogrammi di momento di anti-p e k- singoli
Momenta_per_Id.at(0)->Write();
Momenta_per_Id.at(7)->Write();

//istorgrammi VERTICI SECONDARI

secantip_mom->Write();
//cout << "Antiprotoni da secondari: " << secantip << endl;


recprot_mom->Write();
recprot_ang->Write();
pseudo_vs_momentum->Write();

cout << endl << "Particles from secondary verex: " <<  sec << endl;
cout << "Recoil protons: " << recprot << endl;

tree->Print();
} //fine void endOfEvents

//fine toolbox!
