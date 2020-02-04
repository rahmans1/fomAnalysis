import sys
import os
import subprocess


if not sys.argv[1]:
	home = "/w/halla-scifs17exp/moller12gev/rahmans/remoll-root/remoll"
else:
	home= sys.argv[1]
if not sys.argv[2]:
	macro= "/w/halla-scifs17exp/moller12gev/rahmans/analysis/fomStudy/jsub"
else:
	macro= sys.argv[2]
if not sys.argv[3]:
        jsub= "/w/halla-scifs17exp/moller12gev/rahmans/analysis/fomStudy/jsub"
else:
        jsub = sys.argv[3]
if not sys.argv[4]:
	field= "/w/halla-scifs17exp/moller12gev/rahmans/remoll-root/remoll/map_directory"
else:
	field= sys.argv[4]
if not sys.argv[5]:
	geometry = "/w/halla-scifs17exp/moller12gev/rahmans/remoll-root/remoll/geometry"
else:
	geometry= sys.argv[5]	
if not sys.argv[6]:   
	scratch= "/volatile/halla/moller12gev/rahmans/root/fomStudy"
else:
	scratch= sys.argv[6]
if not sys.argv[7]:
        tmp= "/volatile/halla/moller12gev/rahmans/tmp/fomStudy"
else:
        tmp= sys.argv[7]
if not sys.argv[8]:
	batch= "101_hybrid_kryp"
else:
	batch= sys.argv[8]
if not sys.argv[9]:
	generator="beam"
else:
	generator=sys.argv[9]
if not sys.argv[10]:
        aluminum="USAlTarg"
else:
        aluminum=sys.argv[10]
if not sys.argv[11]:
        ring=5
else:
        ring=sys.argv[11]
if not sys.argv[12]:
        mod="Quartz"
else:
        mod=sys.argv[12]
motherfile={}
motherfile["USAlTarg"]="mollerMother_merged_usAl.gdml"
motherfile["DSAlTarg"]="mollerMother_merged_dsAl.gdml"

detector = [28]
runrange= range(1,1001)
eventsperfile = 5000

jsub=jsub+"/"+generator
macro=macro+"/"+generator
scratch=scratch+"/"+generator
tmp=tmp+"/"+ generator



if (generator=="inelasticAl" or generator=="elasticAl" or generator=="quasielasticAl" or generator=="pion"):
  jsub=jsub+"/"+aluminum
  macro=macro+"/"+aluminum
  scratch=scratch+"/"+aluminum
  tmp=tmp+"/"+ aluminum


jsub=jsub+"/"+batch
macro=macro+"/"+batch 
scratch=scratch+"/"+batch
tmp=tmp+"/"+batch
subprocess.call("source /apps/root/6.18.00/setroot_CUE",shell=True)
count=0
for i in runrange:
  filename=scratch+"/"+generator+"_"+str(ring)+"_"+mod+"_"+str(i)+".root"
  	        
  if os.path.exists(filename):
    c=subprocess.call("root -q -b -l fix.C'(\""+filename+"\")'",shell=True)
    if (c>0):     
      subprocess.call("sbatch "+jsub+"/"+generator+"_"+str(ring)+"_"+mod+"_"+ str(i)+ ".sh",shell=True)
      count=count+1
  else:
    subprocess.call("sbatch "+jsub+"/"+generator+"_"+str(ring)+"_"+mod+"_"+ str(i)+ ".sh",shell=True)

print("Total number of corrupt files is"+str(count)+".\n")
		
		
		
	
	
