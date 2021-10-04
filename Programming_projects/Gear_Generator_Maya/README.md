# Gear Generator
## Description
Gear System Generator
This program contains the main following functions:
1.  the detailed modelling of a single gear
2.  create a series of identical gears with setting chosen by user  
3.  create random gears with randomly chosen settings
4.  disperse gears in a circle area with loosely chosen radius
5.  addition of materials from created library

### Functions:

#### basicGear(teethShape, option, teethNumber, gearHeight, gearRadius, gearThickness, radiusPart, radiusToy, thicknessToy, spacePatternToy, depthPatternToy, spacePatternToy1, depthPatternToy1, teethThicknessBridge, teethSpaceBridge, ringNumber=[], edge=[], edge1=[], depth=[], edge2=[], edge3=[], depth1=[], depthTab=[], spaceTab=[], startTab=[])
    create one single gear and set specific parameters
     
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

#### createUI(*args)
    create the user interface for modelling the gear, the random creation of the gear, disperse

#### disperseGears(radiusOfSpace, *Args)
    disperse all the gears in a circle area
    radiusOfSPace : a radius of space in which the gears will be dispersed

#### randomGear(gearsNumber, *pArgs)
    create a series of random gears
    gearNumber : the number of gears to be created

#### User Interface
<img src="https://github.com/Brydzia96/Projects/blob/main/images/Gear_Generator.png" width="50%" height="50%">

#### Example gear
<img src="https://github.com/Brydzia96/Projects/blob/main/images/random_gear.png" width="90%" height="90%">
