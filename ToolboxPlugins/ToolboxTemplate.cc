#include "toolbox.hh"

/*! \class ToolboxTemplate
 *  \brief A template for your own implementation to analyze TGeant output w/o CORAL production.
 *  You can just create a copy of this file, replace 'ToolboxTemplate' with your own classname and start programming!
 *  Few more notes on toolbox: run it like
 *  $TGEANT/bin/toolbox -u NameOfYourPlugin.cc -h FileForResult.root yourTGEANTfile.tgeant
 *  The example reported hereafter is just taking into account histograms but you can also use trees and other root structures (including the
 *  corresponding libraries on top of this class. You can see what are the classes available for toolbox analysis in TGEANT/TGEANT/libEvent/include/T4Event.hh
 *  Interesting classes are: T4BeamData, T4BeamParticle, T4Trajectory, T4HitData, T4RichData
 */
class ToolboxTemplate : ToolboxPlugin
{
  public:
    ToolboxTemplate(void);
    ~ToolboxTemplate(void) {}

    std::string getDescription(void);
    bool processEvent(T4Event* event);
    void endOfEvents(void);
    void beginOfEvents(void);

  private:
    // place to declare some histograms
    // For instance, if you want to declare a TH1D to store z vertex position
    TH1D* zVtx;
    //Initialization of the pointers to the histograms should not be performed here, but later on in beginOfEvents method.
};

/*Here pay attention if you use "replace" command to change all the ToolboxTemplate in the name of your analysis plugin.
the pointer should not be named exactly as the class. It is a case sensitive assignment of the memory, so if you change a capital letter into
a not-capital one is enough*/
static ToolboxTemplate* toolboxTemplate = new ToolboxTemplate();

/*Here it is just for architecture of the software. Replacing ToolboxExample with your analysis plugin name is enough*/
ToolboxTemplate::ToolboxTemplate(void)
{
  myName = "ToolboxTemplate";
  pluginList::getInstance()->activated_classes.push_back(this);
}

/*Here you may replace the description between " " with the real description of your own analysis class (e.g. plotting of primary vertex position)*/
std::string ToolboxTemplate::getDescription(void)
{
  std::string description = "Toolbox template to add own user functions.";
  return description;
}

void ToolboxTemplate::beginOfEvents(void)
{
  // This method is being executed at the beginning, before looping on the events record.
  //  So it is the place to initialize the histograms
  // Following up the example of the z-vertex position
  /* WARNING: TGeant units are : mm for position and MeV for energy and momenta.
   * So pay attention on the declaration of your histograms! You can always convert the unit, as in this example
   */
  zVtx = new TH1D("positionVertex_z", "vertex z position (cm)", 300, -500, 400);

}

bool ToolboxTemplate::processEvent(T4Event* event)
{
  // this function is called for each event
  // return true if this event should be saved, false if not
  // (only if option -o is activated)
  // (-o saves the stream of the whole event, so it should be used just if you're modyifing your TGeant output, not for analysis).
  // Let's go on with our example: plotting z vertex position
    // We get the vertex position directly from beamData:
     zVtx->Fill(event->beamData.vertexPosition[2]*10); //z position, converted into cm

  return true;
}

void ToolboxTemplate::endOfEvents(void)
{
  // Here you put all the actions you would execute after the analysis before closing your plugin
  // For instance it is the right place to write the histograms!
    zVtx->Write();
  // The histogram will be saved in the target file you are pointing in your -h option while running toolbox

}

