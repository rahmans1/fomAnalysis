import sys
import os
import subprocess
import time

if not sys.argv[1]:
        jsub = "/w/halla-scifs17exp/moller12gev/rahmans/analysis/fomStudy/jsub"
else:
        jsub = sys.argv[1]
if not sys.argv[2]:   
	scratch = "/volatile/halla/moller12gev/rahmans/scratch/fomStudy"
else:
	scratch = sys.argv[2]
if not sys.argv[3]:
        root = "/volatile/halla/moller12gev/rahmans/root/fomStudy"
else:
        root = sys.argv[3]
if not sys.argv[4]:
	batch= "101"
else:
	batch= sys.argv[4]
if not sys.argv[5]:
	generator="elastic"
else:
	generator=sys.argv[5]
if not sys.argv[6]:
        aluminum=""
else:
        aluminum=sys.argv[6]

runrange= range(int(sys.argv[7]),int(sys.argv[8]))

jsub=jsub+"/"+generator
scratch=scratch+"/"+generator
root=root+"/"+ generator
if not os.path.exists(jsub):
  os.system("mkdir "+jsub)
if not os.path.exists(root):
  os.system("mkdir "+root)


if (generator=="inelasticAl" or generator=="elasticAl" or generator=="quasielasticAl" or generator=="pion"):
  jsub=jsub+"/"+aluminum
  scratch=scratch+"/"+aluminum
  root=root+"/"+ aluminum
  if not os.path.exists(jsub):
    os.system("mkdir "+jsub)
  if not os.path.exists(root):
    os.system("mkdir "+root)


jsub=jsub+"/"+batch
scratch=scratch+"/"+batch
root=root+"/"+batch
if not os.path.exists(jsub):
  os.system("mkdir "+jsub)
if not os.path.exists(root):
  os.system("mkdir "+root)

home = "/w/halla-scifs17exp/moller12gev/rahmans/analysis/fomStudy"

for i in runrange:
  for j in range(5,6):
   for k in ["Quartz"]:
     filename=scratch+"/"+generator+"_"+str(i)+".root"
     outfile=root+"/"+generator+"_"+str(j)+"_"+k+"_"+str(i)+".root"
     if os.path.exists(filename):
       jsubf=open(jsub+"/"+generator+"_"+str(j)+"_"+k+"_"+str(i)+".sh", "w")
       jsubf.write("#!/bin/bash\n")
       jsubf.write("#SBATCH --account=halla\n")
       jsubf.write("#SBATCH --partition=production\n")
       jsubf.write("#SBATCH --job-name=remollAna\n")
       jsubf.write("#SBATCH --time=00:05:00\n")
       jsubf.write("#SBATCH --nodes=1\n")
       jsubf.write("#SBATCH --ntasks=1\n")
       jsubf.write("#SBATCH --cpus-per-task=1\n")
       jsubf.write("#SBATCH --mem=2G\n")
       jsubf.write("tcsh -c \"source /apps/root/6.18.00/setroot_CUE\"\n")
       jsubf.write("#SBATCH --output=/volatile/halla/moller12gev/rahmans/tmp/tmp.out\n")
       jsubf.write("cd "+home+"\n")
       jsubf.write("echo \"Current working directory is `pwd`\"\n")	
       jsubf.write("./reroot -q -b analyse.C'(\""+filename+"\", \""+outfile+"\", \""+generator+"\", "+str(j)+", \""+k+"\")'\n")
       print("sbatch "+jsub+"/"+generator+"_"+str(j)+"_"+k+"_"+str(i)+".sh") 
       		
    
		
	
	
