#include "toolbox.hh"

class ZVertex : ToolboxPlugin
{
  public:
    ZVertex(void);
    ~ZVertex(void) {}

    std::string getDescription(void);
    bool processEvent(T4Event* event);
    void endOfEvents(void);
    void beginOfEvents(void);

  private:
    // istogrammi per le coordinate
    TH1D* Vertex_z;
    TH2D* Vertex_xy;
    // istogramma che plotta il particleId delle particelle uscenti dal vertice primario
    TH1D* Particles;
    // istogramma che assegna un numero da 0 a 11 alle particelle uscenti e le plotta in ordine. Non saprei come mettere sotto ogni colonna il nome della particella corrispondente, in ogni caso l'ordine in cui sono plottate è quello indicato più sotto (lo segnalerò con commento)
    TH1D* Particles_per_type;
    // istogramma per la z dei protoni con momento >175
    TH1D* ProtonVertex_z;
    // istogramma che mostra elettroni e positroni dei delta rays
    TH1D* Delta_rays;
};

static ZVertex* zVertex = new ZVertex();

ZVertex::ZVertex(void)
{
  myName = "ZVertex";
  pluginList::getInstance()->activated_classes.push_back(this);
}

std::string ZVertex::getDescription(void)
{
  std::string description = "Plotting of various characteristics of primary vertex.\n";
  description += "Generator: GEANT";
  return description;
}

void ZVertex::beginOfEvents(void)
{
  // inizializzo gli istogrammi
  Vertex_z = new TH1D("positionVertex_z", "vertex z position (cm)", 1000, -1000, 4000);
  Vertex_z->GetXaxis()->SetTitle("z position [cm]");
  Vertex_xy = new TH2D("positionVertex_xy", "vertex xy position (cm)", 12, -6, 6, 12, -6, 6);
  Vertex_xy->GetXaxis()->SetTitle("x position [cm]");
  Vertex_xy->GetYaxis()->SetTitle("y position [cm]");

  Particles = new TH1D("particles_from_vertex", "particles from primary vertex", 6000, -3000, 3000);
  Particles->GetXaxis()->SetTitle("particle Id");
  Particles_per_type = new TH1D("particles_per_type", "particles from vertex", 12, 0, 12);

  ProtonVertex_z = new TH1D("proton_vertex_z", "vertex z position (cm)", 1000, -1000, 4000);

  Delta_rays = new TH1D("delta_rays", "delta rays particles", 40, -20, 20);
  Delta_rays->GetXaxis()->SetTitle("particle Id");
}

//definisco e inizializzo i counter che mi serviranno per stampare il numero di particelle uscenti e inserirle nell'istogramma

int antip, p, mum, mup, pim, pip, pi0, km, kp, k0, n, others = 0;

bool ZVertex::processEvent(T4Event* event)
{ 
  for (unsigned int i = 0; i < event->beamData.trajectories.size(); i++) {
 
  T4Trajectory* trajectory = &event->beamData.trajectories.at(i);

      if (trajectory->parentId == 1) {
    
         if (abs(trajectory->particleId)!=11) { 

            //riempio gli istogrammi di posizione e ID delle particelle
            Vertex_z->Fill(trajectory->position[2]/10);
            Vertex_xy->Fill(trajectory->position[0]/10, trajectory->position[1]/10);
            Particles->Fill(trajectory->particleId);

            // qui il codice non è molto "compatto", ma, dopo vari fallimenti, è il modo più sicuro che sono riuscito a trovare per far funzionare tutto. Qui sotto aumento i counters di tutte le particelle
            
            if (trajectory->particleId==-2212) {antip++;}

            if (trajectory->particleId==2212) {p++;}

            if (trajectory->particleId==13) {mum++;}

            if (trajectory->particleId==-13) {mup++;}

            if (trajectory->particleId==-211) {pim++;}

            if (trajectory->particleId==211) {pip++;}

            if (trajectory->particleId==111) {pi0++;}

            if (trajectory->particleId==-321) {km++;}

            if (trajectory->particleId==321) {kp++;}

            if (trajectory->particleId==311) {k0++;}

            if (trajectory->particleId==2112) {n++;}

            //se l'ID della particella non corrisponde a nessuno di quelli sopra, aumento il counter dedicato alle altre particelle (che sono più di 100, forse vale la pena vedere quali sono?
            if (abs(trajectory->particleId)!=2212 && abs(trajectory->particleId)!=13 && abs(trajectory->particleId)!=211 && abs(trajectory->particleId)!=321 && trajectory->particleId!=111 && trajectory->particleId!=311 && trajectory->particleId!=2112) {others++;}
        
            //istogramma protoni con momento >175
            if (trajectory->particleId==2212 && trajectory->momentum[2]>175) {
               ProtonVertex_z->Fill(trajectory->position[2]/10);       
               }
           break;
         }
         else {
              //istogramma per i delta. Non sono sicuro se mettere o no il break anche qui
              Delta_rays->Fill(trajectory->particleId);
        }
      }
  }
return true;
}

void ZVertex::endOfEvents(void)
{
  //stampo il numero delle particelle uscenti. Era più che altro un controllo per me, per verificare che l'istogramma venisse giusto, ora si potrebbe anche rimuovere penso
  cout << endl;
  cout << "Particelle uscenti" << endl << endl;
  cout << "Antiprotoni: " << antip << endl;
  cout << "Protoni: " << p << endl;
  cout << "Muoni: " << mum << endl;
  cout << "Antimuoni: " << mup << endl;
  cout << "Pioni-: " << pim << endl;
  cout << "Pioni+: " << pip << endl;
  cout << "Pioni0: " << pi0 << endl;
  cout << "Kaoni-: " << km << endl;
  cout << "Kaoni+: " << kp << endl;
  cout << "Kaoni0: " << k0 << endl;
  cout << "Neutroni: " << n << endl;
  cout << "Altre: " << others << endl << endl;

  //riempio l'istogramma in cui le particelle sono tutte affiancate. L'ordine delle particelle nell'istogramma è lo stesso in cui riempio le colonne qui

  for (int i=0; i<antip; i++) {Particles_per_type->Fill(0);}
  for (int i=0; i<p; i++) {Particles_per_type->Fill(1);}
  for (int i=0; i<mum; i++) {Particles_per_type->Fill(2);}
  for (int i=0; i<mup; i++) {Particles_per_type->Fill(3);}
  for (int i=0; i<pim; i++) {Particles_per_type->Fill(4);}
  for (int i=0; i<pip; i++) {Particles_per_type->Fill(5);}
  for (int i=0; i<pi0; i++) {Particles_per_type->Fill(6);}
  for (int i=0; i<km; i++) {Particles_per_type->Fill(7);}
  for (int i=0; i<kp; i++) {Particles_per_type->Fill(8);}
  for (int i=0; i<k0; i++) {Particles_per_type->Fill(9);}
  for (int i=0; i<n; i++) {Particles_per_type->Fill(10);}
  for (int i=0; i<others; i++) {Particles_per_type->Fill(11);}

  Particles_per_type->Write();
  Vertex_z->Write();
  Vertex_xy->Write();
  Particles->Write();
  ProtonVertex_z->Write();
  Delta_rays->Write();
}
