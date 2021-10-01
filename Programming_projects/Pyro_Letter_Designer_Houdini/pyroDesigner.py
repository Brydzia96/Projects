import hou
from PySide2 import QtCore, QtUiTools, QtWidgets

# Get scene root node
sceneRoot = hou.node('/obj/')
#Create camera
cameraMain = sceneRoot.createNode('cam', 'cam1')
# Create empty "firstNode" geometry node in scene root
firstNode = sceneRoot.createNode('geo', 'firstNode', run_init_scripts=False)
# Create render node
sceneRoot2 = hou.node('/out/')
renderNode = sceneRoot2.createNode('ifd','fireLetterRender') 
#Visualisation Display Nodes
firstNode.setDisplayFlag(False)


def setParameter(objectName, parameterName, value):
    for x in range(len(parameterName)):
        objectName.parmTuple(parameterName[x]).set([value[x]])
    return 0
    
def connectNodes(topNodeName, lowNodeName, path):
    for x in range(len(topNodeName)):
        xform_A = hou.node('%(path)s%(topNodeName)s' %{'path': path,'topNodeName': topNodeName[x]}) 
        xform_B = hou.node('%(path)s%(lowNodeName)s' %{'path': path,'lowNodeName': lowNodeName[x]}) 
       # print (xform_A)
       # print (xform_B)
        xform_B.setInput(0,xform_A)

def createSourceNodes(fontParameter, textParameter, fontSizeParameter, thicknessParameter):        #fontParameter , selectCamera, picturePath):
 
    firstNode.setDisplayFlag(False)
    fonti = firstNode.createNode('font', 'fonti')
    fontiParameters = ['file','text','fontsize']
    fontiValues = [fontParameter ,textParameter ,fontSizeParameter]
    setParameter(fonti, fontiParameters, fontiValues)
    
    polyExtrude = firstNode.createNode('polyextrude::2.0', 'polyextrude1')
    polyExtrudeParameters = ['dist','outputback']
    polyExtrudeValues = [thicknessParameter,1]
    setParameter(polyExtrude, polyExtrudeParameters, polyExtrudeValues)
    
    convert = firstNode.createNode('convert', 'convert1')    
    fluidSource = firstNode.createNode('fluidsource', 'FUEL')
    delete_iso_fuel = firstNode.createNode('delete', 'delete_iso_fuel')
    nullSource = firstNode.createNode('null', 'OUT_FUEL')
    
    fluidSourceParameters = ['source_attribute','number_of_volumes','initialize','name1','name2','divsize','eloc','size','use_noise']
    fluidSourceValues = ['fuel',2,1,'fuel','temperature',0.02,0.005,0.01,1]
    setParameter(fluidSource, fluidSourceParameters, fluidSourceValues)
    
    delete_iso_fuelParameters = ['group','negate']
    delete_iso_fuelValues = ['@name=fuel',1]
    setParameter(delete_iso_fuel, delete_iso_fuelParameters, delete_iso_fuelValues)
    
    nodesTopSource = ['fonti','convert1', 'polyextrude1', 'FUEL', 'delete_iso_fuel']
    nodesLowSource = ['convert1', 'polyextrude1', 'FUEL', 'delete_iso_fuel', 'OUT_FUEL']
    path1 = '/obj/firstNode/'
    connectNodes(nodesTopSource, nodesLowSource, path1)
    
    #Creating BBOX 
    BBOX = firstNode.createNode('bound', 'bound1')
    timeShift = firstNode.createNode('timeshift', 'timeshift1')
    nullBBOX = firstNode.createNode('null', 'BBOX')


    BBOX.parmTuple('minpad').set([0.5, 0.5, 0.5])
    BBOX.parmTuple('maxpad').set([0.5, 0.5, 0.5])

    nodesFirstWingTopSource = ['delete_iso_fuel','bound1', 'timeshift1']
    nodesFirstWingLowSource = ['bound1','timeshift1', 'BBOX']
    connectNodes(nodesFirstWingTopSource, nodesFirstWingLowSource, path1)

    #Creating Tracking Object

    tracker = firstNode.createNode('bound', 'bound2')
    trackerNull = firstNode.createNode('null', 'TRACKER')

    nodesSecondWingTopSource = ['delete_iso_fuel','bound2'] 
    nodesSecondWingLowSource = ['bound2','TRACKER']
    connectNodes(nodesSecondWingTopSource, nodesSecondWingLowSource, path1)
    
def createDOPNet():
    DOP_NET = sceneRoot.createNode('dopnet', 'DOP_NET', run_init_scripts=False)

    smokeObject = DOP_NET.createNode('smokeobject', 'fire')
    gasResize = DOP_NET.createNode('gasresizefluiddynamic', 'gasresizefluiddynamic1')
    sourceVolume = DOP_NET.createNode('sourcevolume', 'SRC_FUEL')
    pyroSolver= DOP_NET.createNode('pyrosolver::2.0', 'pyrosolver1')
    outputDOP = DOP_NET.createNode('output', 'output')

    smokeObjectParameters = ['divsize','solvefirstframe']
    smokeObjectValues = [0.01*5, 1]
    setParameter(smokeObject, smokeObjectParameters, smokeObjectValues)

    def setExpressionTrackingObject(sizeParameter,dimensionParameter, value1, value2):
        for x in range(len(sizeParameter)):
            smokeObject.parm(sizeParameter[x]).setExpression('bbox("../../firstNode/BBOX",%(value1)s)' %{'value1':value1[x]})
            smokeObject.parm(dimensionParameter[x]).setExpression('centroid("../../firstNode/BBOX",%(value2)s)' %{'value2':value2[x]})
        return 0

    sizeParameters = ['sizex', 'sizey', 'sizez']
    dimensionParameters = ['tx', 'ty', 'tz']
    value1 = ['D_XSIZE', 'D_YSIZE', 'D_ZSIZE']
    value2 = [0,1,2]
    setExpressionTrackingObject(sizeParameters,dimensionParameters, value1, value2)

    gasResize
    gasResizeParameters = ['bound_padding','use_tracking_objects','tracking_path']
    gasResizeValues = [0.1, 1, 'obj/firstNode/TRACKER']
    gasResize.parmTuple('padding_min_scale').set([1, 0.05, 1])
    setParameter(gasResize, gasResizeParameters, gasResizeValues)

    #sourceFuel

    sourceVolumeParameters = ['initialize', 'source_path','scale_source', 'scale_temperature','velocity_merge', 'vel_mask', 'source_name','vol_sourcedest']
    sourceVolumeValues = [6, '/obj/firstNode/OUT_FUEL', 3, 3, 1, 'fuel', 'fuel', 'fuel']
    setParameter(sourceVolume, sourceVolumeParameters, sourceVolumeValues)
    
    #pyro Solver
    #Connecting Top Nodes with Pyrosolver
    xform_A = hou.node('/obj/DOP_NET/fire')
    xform_C = hou.node('/obj/DOP_NET/gasresizefluiddynamic1')
    xform_D = hou.node('/obj/DOP_NET/SRC_FUEL')
    xform_B = hou.node('/obj/DOP_NET/pyrosolver1')

    xform_B.setInput(0, xform_A)
    xform_B.setInput(2, xform_C)
    xform_B.setInput(4, xform_D)

    pyroSolver.parmTuple('ignitiontemp').set([-1])

    #Connecting output node
    topNode = ['pyrosolver1']
    lowNode = ['output']
    path2 = '/obj/DOP_NET/'
    connectNodes(topNode, lowNode, path2)
    
    outputDOP.setSelected(1,1)
    outputDOP.setDisplayFlag(True)
    

def exportPyro():
    #Create export pyro node
    EXPORT_PYRO = sceneRoot.createNode('geo', 'EXPORT_PYRO', run_init_scripts=False)
    dopImport = EXPORT_PYRO.createNode('dopimportfield', 'dopimportfield1')
    volumeViz = EXPORT_PYRO.createNode('volumevisualization', 'volumevisualization1')
    nullFireFoOut = EXPORT_PYRO.createNode('null', 'FONT_FIRE_OUT')

    exportPyroParameters = ['shop_materialpath', 'caching']
    exportPyroValues = ['/mat/fontfireshader1', 0]
    setParameter(EXPORT_PYRO, exportPyroParameters, exportPyroValues)

    #dopImport

    dopImportParameters = ['doppath', 'dopnode', 'fields', 'fieldname1', 'fieldname2', 'fieldname3', 'fieldname4', 'fieldname5', 'fieldname6', 'fieldname7', 'visible2','visible3','visible4','visible5','visible7']  
    dopImportValues = ['/obj/DOP_NET', '/obj/DOP_NET/fire', 7, 'density', 'vel', 'rest', 'rest2', 'temperature', 'heat', 'fuel', 3, 3, 3, 3, 3] 
    setParameter(dopImport, dopImportParameters, dopImportValues)

    #Creating Valume Viz

    volumeVizParameters = ['densityfield', 'cdfield', 'densityscale', 'emitscale', 'emitfield', 'emitcdfield', 'emitcdrangeoverride']  
    volumeVizValues = ['density', 'density', 100, 100, 'heat', 'temperature', 1] 
    setParameter(volumeViz, volumeVizParameters, volumeVizValues)

    volumeViz.parmTuple('emitcdpreset').set([4])
    volumeViz.parm('emitcdpreset').pressButton()
    volumeViz.parmTuple('emitcdrange').set([0,3])
    

    #Connect nodes in pyro export

    topNodesExport = ['dopimportfield1', 'volumevisualization1']
    lowNodesExport = ['volumevisualization1', 'FONT_FIRE_OUT']
    path3 = '/obj/EXPORT_PYRO/'
    connectNodes(topNodesExport,lowNodesExport,path3)
    
    
    volumeViz.setSelected(1,1)
    volumeViz.setDisplayFlag(True)

def defaultMaterial():
    #Create material
    sceneRoot1 = hou.node('/mat/')
    fireShader = sceneRoot1.createNode('pyroshader', 'fontfireshader1',)
    
    fireShaderParameters = ['s_densityscale', 'fi_int', 'fc_int', 'fc_colormode', 'fc_colorramp','fc_colorramp1pos','fc_colorramp1interp','fc_colorramp2pos','fc_colorramp2interp','fc_colorramp3pos','fc_colorramp3interp', 'fc_colorramp4pos', 'fc_colorramp4interp','fc_colorramp5pos','fc_colorramp5interp', 'fc_colorramp6pos','fc_colorramp6interp'] 
    fireShaderValues = [0.3, 5, 2, 1, 6, 0.186094, 2, 0.482618, 2, 0.642127, 2, 0.783231, 2, 0.885481, 2, 0.986066, 2]            
    setParameter(fireShader, fireShaderParameters, fireShaderValues)
        
    fireShader.parmTuple('s_color').set([0.4,0.4,0.4])
    
    fireShader.parmTuple('fc_colorramp2c').set([0.223438,0.499882,0.9])
    fireShader.parmTuple('fc_colorramp3c').set([1,0.432,0])
    fireShader.parmTuple('fc_colorramp4c').set([0.9,0.9,0])
    fireShader.parmTuple('fc_colorramp5c').set([1,1,1])
    fireShader.parmTuple('fc_colorramp6c').set([0,0.9,0.9])
    
    
def renderFunction():

    #Set up render node
    
    renderNodeParameters = ['vm_picture', 'vm_usemaxthreads', 'vobject']
    renderNodeValues = ['$HOME/Desktop/Innovations/geo/FireRender/$HIPNAME.$OS.$F4.exr', 2, 'EXPORT_PYRO' ]
    setParameter(renderNode, renderNodeParameters, renderNodeValues)
    renderNode.parmTuple('camera').set(['/obj/cam1'])
        
def renderToMPlay(cameraPath):
    renderNode.parmTuple('camera').set([cameraPath])
    renderNode.parmTuple('trange').set([0])
    renderNode.parm('renderpreview').pressButton()
  
    
    
def renderToDisk(renderPath, startFrame, endFrame):
    renderNode.parmTuple('trange').set([1])
    renderNode.parmTuple('vm_picture').set([renderPath])
    renderNode.parmTuple('f').deleteAllKeyframes()
    renderNode.parmTuple('f').set([startFrame, endFrame, 1])
    renderNode.parm('execute').pressButton()
    
    

#Set parameters for Source
float_parm = hou.FloatParmTemplate("new_float_parm", 
                                   "New Float Parm", 
                                   1)        

class UICreator(QtWidgets.QWidget):
    def __init__(self):
        super(UICreator,self).__init__()
        ui_file = 'D:\MASTER_PROJECTS\Innovations\InnovationsSub\Final3.ui'
        self.ui = QtUiTools.QUiLoader().load(ui_file, parentWidget=self)                                  
        self.setParent(hou.ui.mainQtWindow(), QtCore.Qt.Window)
        
        
        
        
        # Setup "Create Pyro Network" button
        self.ui.btn_create.clicked.connect(self.buttonClicked)
        
        # Setup "Render to MPlay" button        
        self.ui.btn_renderMPlay.clicked.connect(self.buttonMPlayClicked)
        
        # Setup 'Render to Disk above Sequence' button
        self.ui.btn_renderToDisk.clicked.connect(self.buttonRenderToDiskClicked)
        
                                   
        
              
    def buttonClicked(self):
        #Get names from user
        customFont = self.ui.comboBox_font.currentText() 
        customText = self.ui.lin_name.text()
        customFontSize = self.ui.lin_fontSize.text()
        customThickness =self.ui.lin_thickness.text()
        createSourceNodes(customFont, customText, customFontSize, customThickness)#customFont
        createDOPNet()
        exportPyro()
        defaultMaterial()
        renderFunction()
        
    def buttonMPlayClicked(self):
        #Get camera settings from user
         customCamera = self.ui.line_camera.text()
         renderToMPlay(customCamera)
         
        
    def buttonRenderToDiskClicked(self):
        #Get camera settings from user
        renderFunction()
        customRenderPath = self.ui.lineRender_path.text()
        customStartFrame = self.ui.spinBox_startFrame.text()
        customEndFrame = sel
        
        f.ui.spinBox_endFrame.text()
        renderToDisk(customRenderPath, customStartFrame, customEndFrame)
        
def run():        
    win = UICreator()
    win.show()
