''' Gear System Generator
    This program contains the main following functions:
    1.  the detailed modelling of a single gear
    2.  create a series of identical gears with setting chosen by user  
    3.  create random gears with randomly chosen settings
    4.  disperse gears in a circle area with loosely chosen radius
    5.  addition of materials from created library
    
'''    
    
import maya.mel as mm
import random
import maya.cmds as cmds
import math

def basicGear(teethShape, option, teethNumber, gearHeight, gearRadius, gearThickness, radiusPart, radiusToy, thicknessToy, spacePatternToy, depthPatternToy, spacePatternToy1, depthPatternToy1, teethThicknessBridge, teethSpaceBridge, ringNumber=[], edge=[], edge1=[], depth=[], edge2=[], edge3=[], depth1=[], depthTab=[], spaceTab=[], startTab=[]):
    '''create one single gear and set specific parameters
    
    teethShape              : the shape of the gear teeth
    option                  : the option for the gear pattern: 'ringOneSide' - creates ring pattern only on one side of a gear, 'ringBothSides' - creates ring pattern on both sides of a gear, 'tabOneSide' - creates tab pattern only on one side of a gear, 'tabBothSides', creates tab pattern on one side of a gear, 'ringAndTab' - creates ring pattern on one side and tab pattern on another side
    teethNumber             : the number of the gear teeth 
    gearHeight              : the height of the gear
    gearRadius              : the radius of the gear              
    gearThickness           : the thickness of the gear
    radiusPart              : the part of the radius used for teeth creation
    radiusToy               : the radius of the toy (pipe) included inside of the gear
    thicknessToy            : the thickness of the toy
    spacePatternToy         : the spaces between toy tabs (tab is the only pattern for toy) on the first side
    depthPatternToy         : the depth of the toy pattern on the first side 
    spacePatternToy1        : the spaces between toy's tabs (tab is the only pattern for toy) on the second side
    depthPatternToy1        : the depth of the toy's pattern on the second side
    teethThicknessBridge    : the thickness of the bridge counted as the internal faces of the toy/gear
    teethSpaceBridge        : the spaces between the bridges counted as the internal faces of the toy/gear
    ringNumber              : the number of the rings on each side, first number from the list is the number of the rings on first side, the second one is the number of the rings on the another side
    edge                    : the position of the first edge of each ring on the first side of the gear, the sequent elements on the list are the distance from the edge which ends the previous ring (the percentage from 0 to 1) to the opposite border edge , first edge on this list is counted from the border edge in the center of the gear 
    edge1                   : the position of the second edge of each ring on the first side of the gear, the sequent elements on the list are the distance from the first edge of the ring to the opposite border edge of the gear (the percentage from 0 to 1) 
    depth                   : the depth of the subsequent rings on the first side  
    edge2                   : the position of the first edge of each ring on the second side of the gear, the sequent elements on the list are the distance from the edge which ends the previous ring (the percentage from 0 to 1) to the opposite border edge , first edge on this list is counted from the border edge in the center of the gear, 
    edge3                   : the position of the second edge of each ring on the second side of the gear, the sequent elements on the list are the distance from the first edge of the ring to the opposite border edge of the gear (the percentage from 0 to 1) 
    depth1                  : the depth of the subsequent rings on the second side
    depthTab                : the depth of the tab pattern, first element from the list is used for the first side of the gear, the second one is used for the another side of the gear
    spaceTab                : the spaces between gear's tabs, first element from the list is used for the first side, the second one is used for the another side of the gear 
    startTab                : the option for the tab pattern: 'below' - creates the tab pattern below the gear teeth, 'next' - creates the tab pattern next to the gear teeth
    '''   
       
    
    # create the basic gear shape
    gear = cmds.polyPipe(n='gear', sa= teethNumber*2, h=gearHeight, r=gearRadius, t=gearThickness)    
    cmds.select(clear=True)
        
    #  the first and the last face in a range which includes the the faces for teeth extrusion 
    firstFace = (teethNumber *2 * 2)
    lastFace = (teethNumber *2 * 3 -1)
	
    for i in range (firstFace, lastFace, 2):
        cmds.select(gear[0] +".f[%d]" %i, add=True) 
    
    # create two different kinds of teeth    
    if teethShape == 'rectangular':
        teethLength = radiusPart*gearRadius
        cmds.polyExtrudeFacet(ltz=teethLength)
        cmds.polyExtrudeFacet(ltz=teethLength*0.1,lsx=0.9, lsy=0.9, lsz=0.9)
    if teethShape == 'triangular':
        teethLength = radiusPart*gearRadius
        cmds.polyExtrudeFacet(ltz=teethLength, lsx=0.5, lsy=0.5, lsz=0.5)
        cmds.polyExtrudeFacet(ltz=teethLength*0.1, lsx=0.9, lsy=0.9, lsz=0.9)
    
    cmds.select(clear=True)
   
   	# the loop creates the ring pattern on the first side of the gear
   	# options which require the creations of the rings on first side of the gear         
    if option == 'ringOneSide' or option == 'ringBothSides' or option =='ringAndTab':
        cmds.select(clear=True)
        
        # choosing the position for the first edge
        for i in range(1, ringNumber[0]+1):
            if i == 1:
                firstEdge = (teethNumber *10)
                lastEdge = (teethNumber *12)
                for j in range (firstEdge, lastEdge):
                    cmds.select(gear[0] +".e[%d]" %j, add=True)
                cmds.polySplitRing(sma = 180, wt=edge[0])
                cmds.select(clear=True)
                
            else:
                firstEdge = (teethNumber*(34+(16*(i-2))))
                lastEdge = (teethNumber *(38+(16*(i-2))))
                cmds.select(gear[0] +".e[%d]" %firstEdge, add=True)
                for j in range (firstEdge+1, lastEdge-1, 2):
                    cmds.select(gear[0] +".e[%d]" %j, add=True)
                cmds.polySplitRing(sma = 180, wt=edge.pop(1))
                cmds.select(clear=True)                   
            
            # choosing the position for the second edge
            firstEdge = (teethNumber*(32+(16*(i-1))))
            lastEdge = (teethNumber*(36+(16*(i-1))))
            cmds.select(gear[0] +".e[%d]" %firstEdge, add=True)
            for j in range (firstEdge+1, lastEdge-1, 2):
                cmds.select(gear[0] +".e[%d]" %j, add=True)
            cmds.polySplitRing(sma = 180, wt=edge1.pop(0))
            cmds.select(clear=True)     
                
            # the extrusion of the ring   
            firstFace = (teethNumber*(16+(8*(i-1))))
            lastFace = (teethNumber*(18+(8*(i-1))))
            for j in range(firstFace, lastFace):
                cmds.select(gear[0] +".f[%d]" %j, add=True)
            cmds.polyExtrudeFacet(ltz=depth.pop(0))     
                    
    # the loop creates the ring pattern on the second side of the gear 
    # option which requires the creation of the rings on the second side of the gear             
    if option == 'ringBothSides':
        cmds.select(clear=True)
    	    
	    #the loop creates the ring pattern on the second side of the gear
        for i in range(1, ringNumber[1]+1):
            cmds.select(clear=True)
            
		    # choosing the position for the first edge
            firstEdge = (teethNumber *14)
            lastEdge = (teethNumber *16)
            for j in range (firstEdge, lastEdge):
                cmds.select(gear[0] +".e[%d]" %j, add=True)
            cmds.polySplitRing(sma = 180, wt=edge2.pop(0))
            cmds.select(clear=True)
               
            #choosing the position for the second edge
            firstEdge = (teethNumber *14)
            lastEdge = (teethNumber *16)
            for j in range (firstEdge, lastEdge):
                cmds.select(gear[0] +".e[%d]" %j, add=True)
            cmds.polySplitRing(sma = 180, wt=edge3.pop(0))
            cmds.select(clear=True)
               
            # the extrusion of the ring 
            firstFace = (teethNumber *(26+(8*(ringNumber[0]-1)))) + (teethNumber *(8*(i-1)))
            lastFace = (teethNumber *(28+(8*(ringNumber[0]-1)))) + (teethNumber *(8*(i-1)))
            for j in range(firstFace, lastFace):
                cmds.select(gear[0] +".f[%d]" %j, add=True)
            cmds.polyExtrudeFacet(ltz=depth1.pop(0))
                                   
    #the creation of the tab pattern on the first side of the gear
    #options which require the creation of the tab pattern on first side
    if option == 'tabOneSide' or option == 'tabBothSides':
        cmds.select(clear=True)
		
        firstFace = (teethNumber *2)
        lastFace = (teethNumber *4)
	    
        if startTab[0] == 'below':
            for i in range(firstFace, lastFace, spaceTab[0]):
                cmds.select(gear[0] +".f[%d]" %i, add=True)
	   	    
        if startTab[0] == 'next':
            for i in range(firstFace+1, lastFace, spaceTab[0]):
                cmds.select(gear[0] +".f[%d]" %i, add=True)       
                             
        cmds.polyExtrudeFacet(ltz=depthTab[0]) 
                       
    #the creation of the tab pattern on the second side of the gear
    #options which require the creation of the tab pattern of the second side    
  
    if option == 'tabBothSides' or 'ringAndTab': 
        cmds.select(clear=True)
        firstFace = (teethNumber *6)
        lastFace = (teethNumber *8)
    
        if startTab[1] == 'below':
            for i in range (firstFace, lastFace, spaceTab[1]):
                cmds.select(gear[0] +".f[%d]" %i, add=True)
            
        if startTab[1] == 'next':
            for i in range (firstFace+1, lastFace, spaceTab[1]):
                cmds.select(gear[0] +".f[%d]" %i, add=True)
              
        cmds.polyExtrudeFacet(ltz=depthTab[1])
        cmds.select(clear=True)

        
    #the bridge creation                   
    gearFaceNumber = cmds.polyEvaluate(gear[0], f=True)    
    
    # create the toy and the pattern on the first side
    toy = cmds.polyPipe(n='toy', h=gearHeight, r=radiusToy, t=thicknessToy, sa=teethNumber*2)
    cmds.select(clear=True)
    firstToyFace = teethNumber*2*3
    lastToyFace = teethNumber*2*4
    for i in range(firstToyFace, lastToyFace, spacePatternToy):
        cmds.select(toy[0] +".f[%d]" %i, add=True)
    cmds.polyExtrudeFacet(ltz=depthPatternToy)
    cmds.select(clear=True)
        
    # create the toy pattern on the second side 
    firstToyFace = teethNumber*2*1
    lastToyFace = teethNumber*2*2
    for i in range(firstToyFace, lastToyFace, spacePatternToy1):
        cmds.select(toy[0] +".f[%d]" %i, add=True)
    cmds.polyExtrudeFacet(ltz=depthPatternToy1)
    cmds.select(clear=True)
                
    # count the toy faces for the bridge   
    firstToyFace = teethNumber*2*2
    lastToyFace = teethNumber*2*3
    
    # combine two objects    
    gear = cmds.polyUnite(gear[0], toy[0], n=gear[0], ch=False, o=True)
    cmds.select(clear=True)
                                                                       
    # select faces on the toy for first bridge
    newFirstToyFace = firstToyFace + gearFaceNumber
    newLastToyFace = lastToyFace + gearFaceNumber
    for i in range(teethThicknessBridge):
        k = newFirstToyFace + i            
        cmds.select(gear[0] +".f[%d]" %k, add=True)
        
    # select internal faces of the gear for the first bridge                        
    firstGearFace = teethNumber*0 
    lastGearFace = teethNumber*2 
    for i in range(teethThicknessBridge):
        k = firstGearFace + i
        cmds.select(gear[0] +".f[%d]" %k, add=True)
                                                                    
    # create first bridge        
    mm.eval("polyBridgeFaces;")      
    cmds.select(clear=True)
        
    # the loop for the next bridges                                                     
    for i in range(1, teethNumber*2/(teethThicknessBridge+teethSpaceBridge)):                                                                                           
        for j in range(teethThicknessBridge):
             
            k = (newFirstToyFace - teethThicknessBridge + teethSpaceBridge + j)-((i-1)*(teethThicknessBridge-teethSpaceBridge))                                                   
            cmds.select(gear[0] +".f[%d]" %k, add = True)
                
            firstGearFace = teethNumber*0 +teethSpaceBridge+ j +((i-1)*teethSpaceBridge)
            k = firstGearFace 
            cmds.select(gear[0] +".f[%d]" %k, add=True)
                                
        mm.eval("polyBridgeFaces;")
        cmds.select(clear=True)        
        cmds.select(gear[0])
        cmds.delete(ch=True)
        cmds.select(clear=True)
                            
    cmds.select(gear[0])
    
    # the addition of the attributes
    cmds.addAttr(longName ='gearRadius', shortName ='gr', attributeType = 'float')
    cmds.setAttr(gear[0] + '.gr', gearRadius)
    cmds.select(clear = True)
    cmds.select(gear[0])
    cmds.addAttr(longName='teethLength', shortName='tl', attributeType = 'float')
    cmds.setAttr(gear[0] + '.tl', teethLength)    
    cmds.addAttr(longName='teethNumber', shortName='teeth', attributeType="byte")
    cmds.setAttr(gear[0] + '.teeth', teethNumber)
                       
def randomGear(gearsNumber, *pArgs):
    ''' create a series of random gears
    gearNumber : the number of gears to be created 
    '''        
    for i in range(gearsNumber):            
        random.seed()

        # the conditions for 50% for both shapes:    
        teethShapeProb = random.randint(0,10)    
        if teethShapeProb < 5:
            teethShape = 'rectangular'
        if teethShapeProb >= 5:
            teethShape = 'triangular'
            
       # the conditions for 33% for each option:           
        optionProb = random.randint(0,9)
        if optionProb < 3:
            option = 'ringBothSides'
        if optionProb >= 3 and optionProb < 6:
            option = 'tabBothSides'
        if optionProb >= 6:
            option = 'ringAndTab'
            
        # the dependences between the parameters of the function: 'basicGear'             
        teethNumber = random.randint(8, 20)
        gearRadius = random.uniform(0.5, 5)        
        gearHeight = random.uniform(gearRadius*0.1, gearRadius*0.4)
        radiusPart =random.uniform(0.1, 0.3)
        gearThickness = random.uniform(gearRadius*0.1, gearRadius*0.5) 
        radiusToy = random.uniform(gearRadius*0.1, gearRadius*0.5)
        thicknessToy =random.uniform(radiusToy*0.1, radiusToy)
        spacePatternToy = random.randint(2, 6)
        spacePatternToy1 = spacePatternToy            
        depthPatternToy = random.uniform(gearHeight*0.05, gearHeight*0.5)         
        depthPatternToy1 = depthPatternToy             
        teethThicknessBridge = random.randint(2,4)                                 
        teethSpaceBridge = teethThicknessBridge 

        basicGear(teethShape, option, teethNumber, gearHeight, gearRadius, gearThickness, radiusPart, radiusToy, thicknessToy, spacePatternToy, depthPatternToy, spacePatternToy1, depthPatternToy1, teethThicknessBridge, teethSpaceBridge, [2,2], [random.uniform(0.05, 0.3), random.uniform(0.2, 0.4)], [random.uniform(0.2, 0.4), random.uniform(0.2, 0.4)], [random.uniform(-gearHeight*0.2, gearHeight*0.2), random.uniform(-gearHeight*0.2, gearHeight*0.2)], [random.uniform(0.95, 0.7), random.uniform(0.8, 0.6)], [random.uniform(0.8, 0.6), random.uniform(0.8, 0.6)], [random.uniform(-gearHeight*0.2, gearHeight*0.4),random.uniform(-gearHeight*0.2, gearHeight*0.4)], [random.uniform(gearHeight*0.05, gearHeight*0.2), random.uniform(gearHeight*0.05, gearHeight*0.2)], [random.randint(1, 4), random.randint(1, 4)],['next', 'below']) 
 
def disperseGears(radiusOfSpace, *Args):
    ''' disperse all the gears in a circle area
    radiusOfSPace : a radius of space in which the gears will be dispersed
    '''          
    gears = cmds.ls('gear*', transforms=True)
    positionList =[]
    count = 0        
    for i in gears:
        iteration = 0
        newPosition = False
        radius = cmds.getAttr(i +'.gr') + cmds.getAttr(i + '.tl')
        while (not newPosition and iteration < 100):
            newPosition = True
            # the draw of the radius which is smaller than the border radius and included in the border radius, it is for finding the positions of the coordinates which are located on this smaller circle 
            smallRadius = random.uniform(0, radiusOfSpace)
            x = random.uniform(-smallRadius, smallRadius)
            formula = random.randint(1,2)
            if formula == 1:
                z = math.sqrt(smallRadius**2 - x**2)
            else:
                z = -math.sqrt(smallRadius**2 - x**2)                
            for j in range(count):
                if (math.sqrt((x-positionList[j][0])*(x-positionList[j][0])+(z-positionList[j][1])*(z-positionList[j][1])))<radius+positionList[j][2]:
                    newPosition = False
                    break
            iteration += 1
        if iteration < 100:
            positionList.append((x, z, radius))
            count += 1
            cmds.xform(i, t=[x, 0, z])
        else:
            print ("impossible to find in this area")
        return
            
            
def createUI(*args):
    '''create the user interface for modelling the gear, the random creation of the gear, disperse'''

    #clean the whole scene 
    gearGeneratorWindow = cmds.window(title ="GearGenerator", widthHeight=(280,340))    
    cmds.showWindow(gearGeneratorWindow)
    scrollLayout = cmds.scrollLayout(horizontalScrollBarThickness=16, verticalScrollBarThickness=16)      
    gearGeneratorWindow = 'GearGenerator'
    if cmds.window(gearGeneratorWindow, exists=True):
        cmds.deleteUI(windowID)
    
    
    #first section: model individual gear    
    cmds.columnLayout(adjustableColumn=True)
    cmds.separator( height=1, style='in' )
    cmds.text(label = 'Create your own gear:')
    cmds.separator( height=1, style='in' )            
    shapeControl=cmds.optionMenu( label='Shape of teeth:')        
    cmds.menuItem( label='rectangular')
    cmds.menuItem( label='triangular')        
    numTeethGearControl=cmds.intSliderGrp( field=True, label='Number of teeth', minValue=5, maxValue=50, value=10, step=1)
    
    cmds.text(label='What part of the radius teeth should be?', al="center")
    radiusPartControl=cmds.floatSliderGrp( field=True, label='Radius part:', minValue=0.05, maxValue=0.5, value=0.3, step=0.01)        
    heightGearControl=cmds.floatSliderGrp( field=True, label='Height of the gear:', minValue=0.1, maxValue=10, value=1, step=0.1)
    radiusGearControl=cmds.floatSliderGrp( field=True, label='Radius of the gear:', minValue=0.1, maxValue=20, value=5, step=0.1) 
    thicknessGearControl=cmds.floatSliderGrp( field=True, label='Thickness of the gear:', minValue=0.1, maxValue=20, value=1, step=0.1)               
    cmds.separator( height=1, style='in' )
    cmds.text(label = 'Choose patterns for your gear:')
        
    patternControl=cmds.optionMenu(
 label='Which pattern would you like?')        
    ringOneSideControl=cmds.menuItem( label='ringOneSide')
    ringBothSidesControl=cmds.menuItem( label='ringBothSides')
    tabOneSideControl=cmds.menuItem( label='tabOneSide')
    tabBothSidesControl=cmds.menuItem( label='tabBothSides')
    ringAndTabSidesControl=cmds.menuItem( label='ringAndTab')
    cmds.separator( height=1, style='in' )            
    # second section: UI for ring pattern
    cmds.frameLayout( label='Settings for ring pattern', collapsable=True, cl=True )
    cmds.text(label = 'How many rings would you like?')
    cmds.separator( height=1, style='in' )
    ringNumberFirstSideControl=cmds.intSliderGrp( field=True, label='First side:', minValue=1, maxValue=50, value=1, step=1)
    cmds.separator( height=1, style='in' )
    ringNumberSecondSideControl=cmds.intSliderGrp( field=True, label='Second side:', minValue=1, maxValue=50, value=1, step=1)
    form = cmds.formLayout()
    
    # this two lines are inspired from 'http://download.autodesk.com/us/maya/2011help/CommandsPython/show.html?separator.html&cat=Windows'
    tabs = cmds.tabLayout(innerMarginWidth=5, innerMarginHeight=5)
    cmds.formLayout( form, edit=True, attachForm=((tabs, 'top', 0), (tabs, 'left', 0), (tabs, 'bottom', 0), (tabs, 'right', 0)) )   

 
    # third section: first ring
    oneRing = cmds.columnLayout(adjustableColumn=True)
    cmds.text(label = 'Choose the parameters for the first ring:')
    
    cmds.text(label = 'First side of the gear:')
    edge0Control=cmds.floatSliderGrp( field=True, label='Position of first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge10Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depthFirstRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )
    
    cmds.text(label = 'Second side of the gear:')    
    edge20Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge30Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depth1FirstRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )       
    cmds.setParent( '..' )
                             
    # fouth section:  second ring
    twoRings = cmds.columnLayout( adjustableColumn=True )
    cmds.text(label = 'Choose the parameters for the second ring:')
    
    cmds.text(label = 'First side of the gear:')
    edge1Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge11Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depthSeconRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )    
    
    cmds.text(label = 'Second side of the gear:')    
    edge21Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge31Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depth1SecondRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )
    
    cmds.setParent( '..' )
                    
    # fifth section: third ring
    threeRings = cmds.columnLayout( adjustableColumn=True )
    cmds.text(label = 'Choose the parameters for the third ring:')
    
    cmds.text(label = 'First side of the gear:')
    edge2Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge12Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depthThirdRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )        
    
    cmds.text(label = 'Second side of the gear:')                
    edge22Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge32Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depth1ThirdRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )
    
    cmds.setParent( '..' )
                            
    # sixth section: fourth ring
    fourRings = cmds.columnLayout( adjustableColumn=True )
    cmds.text(label = 'Choose the parameters for the fourth ring:')
    
    cmds.text(label = 'First side of the gear:')
    edge3Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge13Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depthFourthRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )     
    
    cmds.text(label = 'Second side of the gear:')   
    edge23Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge33control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depth1FourthRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )
    
    cmds.setParent( '..' )
                                            
    # seventh section: fifth ring
    fiveRings= cmds.columnLayout( adjustableColumn=True ) 
    cmds.text(label = 'Choose the parameters for the fifth ring:')
    
    cmds.text(label = 'First side of the gear:')
    edge4Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge14Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depthFifthRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)
    cmds.separator( height=1, style='in' )
    
    cmds.text(label = 'Second side of the gear:')           
    edge24Control=cmds.floatSliderGrp( field=True, label='Position of the first edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    edge34Control=cmds.floatSliderGrp( field=True, label='Position of the second edge:', minValue=0.1, maxValue=0.9, value=0.5, step=0.01)
    depth1FifthRingControl=cmds.floatSliderGrp( field=True, label='Depth of the ring:', minValue=-4, maxValue=2, value=0.5, step=0.01)                
    cmds.separator( height=1, style='in' )
    
    cmds.setParent( '..' )
    cmds.tabLayout( tabs, edit=True, tabLabel=((oneRing, 'One ring'), (twoRings, 'Two rings'), (threeRings, 'Three rings'), (fourRings, 'Four rings'), (fiveRings, 'Five rings')))
    cmds.setParent( '..' )
    cmds.setParent( '..' )
    cmds.setParent( '..' )
    
    # eighth section: UI for tab pattern
    cmds.frameLayout( label='Settings for tab pattern', collapsable=True, cl=True)
    form1 = cmds.formLayout()
    cmds.columnLayout(adjustableColumn=True)
    cmds.text(label = "Warning! If you have chosen option: 'ringAndTab' you need to fill the fields for the second side of the gear.",  backgroundColor=(1, 0.6, 0.2))
    
    cmds.text(label = 'First side:')            
    startTabMenu=cmds.optionMenu( label='Choose if you would like tabs next or below the teeth:')
    startTabControl=cmds.menuItem( label='below')
    startTab1Control=cmds.menuItem( label='next')     
    depth0TabControl=cmds.floatSliderGrp( field=True, label='Depth of the tab:', minValue=0.1, maxValue=5, value=0.5, step=0.01)
    space0TabControl=cmds.intSliderGrp( field=True, label='Space between the tabs:', minValue=1, maxValue=7, value=1, step=1)
    cmds.separator( height=1, style='in' )
    
    cmds.text(label = 'Second side:')         
    startTabMenu1=cmds.optionMenu( label='Choose if you would like tabs next or below the teeth:')
    cmds.menuItem( label='below')
    cmds.menuItem( label='next')     
    depth1TabControl=cmds.floatSliderGrp( field=True, label='Depth of the tab:', minValue=0.1, maxValue=5, value=0.5, step=0.01)
    space1TabControl=cmds.intSliderGrp( field=True, label='Space between the tabs:', minValue=1, maxValue=7, value=1, step=1)
    cmds.separator( height=1, style='in' )
    cmds.setParent( '..' )
    cmds.setParent( '..' )
    cmds.setParent( '..' )
                        
    # ninth section: UI for the toy
    cmds.frameLayout( label='Toy inside', collapsable=True, cl=True )
    radiusToyControl=cmds.floatSliderGrp( field=True, label='Radius of the toy:', minValue=0.1, maxValue=5, value=0.5, step=0.01)
    thicknessToyControl=cmds.floatSliderGrp( field=True, label='Thickness of the toy:', minValue=0.1, maxValue=5, value=0.1, step=0.01)
    cmds.separator( height=1, style='in' )          
    cmds.text(label = 'Toy pattern:')
    cmds.separator( height=1, style='in' )
    cmds.text(label = 'First side:')
    cmds.separator(
 height=1, style='in' )
    depthToyPatternControl=cmds.floatSliderGrp( field=True, label='Depth of the tab:', minValue=0.05, maxValue=5, value=0.5, step=0.01)
    spacePatternToyControl=cmds.intSliderGrp( field=True, label='Space between the tabs', minValue=1, maxValue=7, value=1, step=1)
    
    cmds.text(label = 'Second side:')  
    cmds.separator( height=1, style='in' )
    depth1PatternToyControl=cmds.floatSliderGrp( field=True, label='Depth of the tab:', minValue=0.05, maxValue=5, value=0.5, step=0.01)
    space1PatternToyControl=cmds.intSliderGrp( field=True, label='Space between the tabs:', minValue=1, maxValue=7, value=1, step=1)
    cmds.separator( height=5, style='in' )
    cmds.setParent( '..' )
     
    # tenth section: UI for the bridge
    cmds.frameLayout( label='Bridge', collapsable=True, cl=True )     
    thicknessBridgeControl=cmds.intSliderGrp( field=True, label='Thickness of the bridge:', minValue=1, maxValue=7, value=3, step=1)
    spaceBridgeControl=cmds.intSliderGrp( field=True, label='Space between the bridges:', minValue=1, maxValue=7, value=3, step=1)     
    cmds.setParent( '..' )
    cmds.separator( height=5, style='in' ) 
    # eleventh section: button 'create gear'
    createGearControl=cmds.button(label='Create gear', command = lambda *args: basicGear(cmds.optionMenu(shapeControl, query=True, value=True), cmds.optionMenu(patternControl, query=True, value=True), cmds.intSliderGrp(numTeethGearControl, query=True, value=True), cmds.floatSliderGrp(heightGearControl, query=True, value=True), cmds.floatSliderGrp(radiusGearControl, query=True, value=True), cmds.floatSliderGrp(thicknessGearControl, query=True, value=True), cmds.floatSliderGrp(radiusPartControl, query=True, value=True), cmds.floatSliderGrp(radiusToyControl, query=True, value=True), cmds.floatSliderGrp(thicknessToyControl, query=True, value=True), cmds.intSliderGrp(spacePatternToyControl, query=True, value=True),  cmds.floatSliderGrp(depthToyPatternControl, query=True, value=True), cmds.intSliderGrp(space1PatternToyControl, query=True, value=True), cmds.floatSliderGrp(depth1PatternToyControl, query=True, value=True), cmds.intSliderGrp(thicknessBridgeControl, query=True, value=True), cmds.intSliderGrp(spaceBridgeControl, query=True, value=True), [cmds.intSliderGrp(ringNumberFirstSideControl, query=True, value=True), cmds.intSliderGrp(ringNumberSecondSideControl, query=True, value=True)], [cmds.floatSliderGrp(edge0Control, query=True, value=True), cmds.floatSliderGrp(edge1Control, query=True, value=True), cmds.floatSliderGrp(edge2Control, query=True, value=True), cmds.floatSliderGrp(edge3Control, query=True, value=True), cmds.floatSliderGrp(edge4Control, query=True, value=True)], [cmds.floatSliderGrp(edge10Control, query=True, value=True), cmds.floatSliderGrp(edge11Control, query=True, value=True), cmds.floatSliderGrp(edge12Control, query=True, value=True), cmds.floatSliderGrp(edge13Control, query=True, value=True), cmds.floatSliderGrp(edge14Control, query=True, value=True)],[cmds.floatSliderGrp(depthFirstRingControl, query=True, value=True), cmds.floatSliderGrp(depthSeconRingControl, query=True, value=True), cmds.floatSliderGrp(depthThirdRingControl, query=True, value=True), cmds.floatSliderGrp(depthFourthRingControl, query=True, value=True), cmds.floatSliderGrp(depthFifthRingControl, query=True, value=True)],  [cmds.floatSliderGrp(edge20Control, query=True, value=True), cmds.floatSliderGrp(edge21Control, query=True, value=True), cmds.floatSliderGrp(edge22Control, query=True, value=True), cmds.floatSliderGrp(edge23Control, query=True, value=True), cmds.floatSliderGrp(edge24Control, query=True, value=True)], [cmds.floatSliderGrp(edge30Control, query=True, value=True), cmds.floatSliderGrp(edge31Control, query=True, value=True), cmds.floatSliderGrp(edge32Control, query=True, value=True), cmds.floatSliderGrp(edge33control, query=True, value=True), cmds.floatSliderGrp(edge34Control, query=True, value=True)], [cmds.floatSliderGrp(depth1FirstRingControl, query=True, value=True), cmds.floatSliderGrp(depth1SecondRingControl, query=True, value=True), cmds.floatSliderGrp(depth1ThirdRingControl, query=True, value=True), cmds.floatSliderGrp(depth1FourthRingControl, query=True, value=True), cmds.floatSliderGrp(depth1FifthRingControl, query=True, value=True)], [cmds.floatSliderGrp(depth0TabControl, query=True, value=True), cmds.floatSliderGrp(depth1TabControl, query=True, value=True)], [cmds.intSliderGrp(space0TabControl, query=True, value=True), cmds.intSliderGrp(space1TabControl, query=True, value=True)], [cmds.optionMenu(startTabMenu, query=True, value=True), cmds.optionMenu(startTabMenu1, query=True, value=True)]))          
    cmds.frameLayout( label='Random gears', collapsable=True )
    cmds.separator( height=5, style='in' ) 
    
    # twelfth section: UI for random gears
    numberRandomGearsControl=cmds.intSliderGrp( field=True, label='Number of random gears:', minValue=1, maxValue=50, value=1, step=1)
    createRandomGearsControl=cmds.button(label='Create random gears', command= lambda *args: randomGear(cmds.intSliderGrp(numberRandomGearsControl, query=True, value=True)))     
    cmds.separator( height=5, style='in' )
    cmds.setParent( '..' )
    
    # thirteenth UI for disperse the gears
    cmds.frameLayout( label='Disperse gears in circle area', collapsable=True )
    radiusdisperseControl=cmds.floatSliderGrp( field=True, label='Radius of a circle:', minValue=5, maxValue=100, value=30, step=0.01)     
    disperseControl=cmds.button(label='Disperse', command= lambda *args: disperseGears(cmds.floatSliderGrp(radiusdisperseControl, query=True, value=True)))
    cmds.separator( height=5, style='in' )
    
if __name__ == "__main__":
    createUI()
    
