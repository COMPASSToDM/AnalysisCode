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
    // the place to declare some histograms
    TH1D* Vertex_z;
    TH2D* Vertex_xy;
};

static ZVertex* zVertex = new ZVertex();

ZVertex::ZVertex(void)
{
  myName = "ZVertex";
  pluginList::getInstance()->activated_classes.push_back(this);
}

std::string ZVertex::getDescription(void)
{
  std::string description = "Plotting of primary vertex x, y and z coordinates.\n";
  description += "Generator: GEANT";
  return description;
}

void ZVertex::beginOfEvents(void)
{
  // place to initialize the histograms
  Vertex_z = new TH1D("positionVertex_z", "vertex z position (cm)", 300, -500, 400);
  Vertex_z->GetXaxis()->SetTitle("z position [cm]");
  Vertex_xy = new TH2D("positionVertex_xy", "vertex xy position (cm)", 12, -6, 6, 12, -6, 6);
  Vertex_xy->GetXaxis()->SetTitle("x position [cm]");
  Vertex_xy->GetYaxis()->SetTitle("y position [cm]");
}

bool ZVertex::processEvent(T4Event* event)
{
  // ho dei dubbi sulla correttezza di questo ciclo

  for (unsigned int i = 0; i < event->beamData.trajectories.size(); i++) {
 
  T4Trajectory* trajectory = &event->beamData.trajectories.at(i);

  if (trajectory->parentId == 1) { 

     Vertex_z->Fill(trajectory->position[2]/10);
     Vertex_xy->Fill(trajectory->position[0]/10, trajectory->position[1]/10);

     }

  }

return true;

}

void ZVertex::endOfEvents(void)
{
  Vertex_z->Write();
  Vertex_xy->Write();
}
